#include <gwPipeline/PipelineSource.h>
#include <osgEarthFeatures/ResampleFilter>
#include <osgEarthFeatures/BufferFilter>
#include <osgEarthFeatures/ConvertTypeFilter>
#include <osgEarth/Registry>
#include <osg/Notify>
#include <osgDB/ReadFile>
#include <OpenThreads/ScopedLock>

#define LC "[PipelineSource] "

using namespace osgEarth::Features;
using namespace osgEarth::Symbology;
using namespace OpenThreads;

namespace gwPipeline
{
	PipelineSourceOptions::PipelineSourceOptions(const ConfigOptions& options) 
		: DriverConfigOptions( options )
	{
		fromConfig( _conf );
	}

	PipelineSourceOptions::~PipelineSourceOptions()
	{
		//nop
	}

	void PipelineSourceOptions::fromConfig(const Config& conf)
	{
		unsigned numResamples = 0;

		conf.getIfSet   ( "open_write",   _openWrite );
		conf.getIfSet   ( "name",         _name );
		conf.getObjIfSet( "profile",      _profile );
		conf.getObjIfSet( "cache_policy", _cachePolicy );
		conf.getIfSet   ( "geo_interpolation", "great_circle", _geoInterp, GEOINTERP_GREAT_CIRCLE );
		conf.getIfSet   ( "geo_interpolation", "rhumb_line",   _geoInterp, GEOINTERP_RHUMB_LINE );
		conf.getIfSet   ( "fid_attribute", _fidAttribute );

		// For backwards-compatibility (before adding the "filters" block)
		// TODO: Remove at some point in the distant future.
		const std::string bcstrings[3] = { "resample", "buffer", "convert" };
		for(unsigned i=0; i<3; ++i) 
		{
			if ( conf.hasChild(bcstrings[i]) )
			{
				_filterOptions.push_back( conf.child(bcstrings[i]) );
			}
		}

		const Config& filters = conf.child("filters");
		for(ConfigSet::const_iterator i = filters.children().begin(); i != filters.children().end(); ++i)
		{
			_filterOptions.push_back( *i );
		}

	}

	Config PipelineSourceOptions::getConfig() const
	{
		Config conf = DriverConfigOptions::getConfig();

		conf.updateIfSet   ( "open_write",   _openWrite );
		conf.updateIfSet   ( "name",         _name );
		conf.updateObjIfSet( "profile",      _profile );
		conf.updateObjIfSet( "cache_policy", _cachePolicy );
		conf.updateIfSet   ( "geo_interpolation", "great_circle", _geoInterp, GEOINTERP_GREAT_CIRCLE );
		conf.updateIfSet   ( "geo_interpolation", "rhumb_line",   _geoInterp, GEOINTERP_RHUMB_LINE );
		conf.updateIfSet   ( "fid_attribute", _fidAttribute );

		if ( !_filterOptions.empty() )
		{
			Config filters;
			for(unsigned i=0; i<_filterOptions.size(); ++i)
			{
				filters.add( _filterOptions[i].getConfig() );
			}
			conf.update( "filters", filters );
		}

		return conf;
	}

	//------------------------------------------------------------------------

	PipelineSource::PipelineSource(const ConfigOptions&  options, const osgDB::Options* readOptions)
		: _options( options )
	{    
		_readOptions  = readOptions;
		_uriContext  = URIContext( _readOptions.get() );
	}

	PipelineSource::~PipelineSource()
	{
		//nop
	}

	const Status& PipelineSource::open(const osgDB::Options* readOptions)
	{
		if ( readOptions )
			_readOptions = readOptions;

		// Create and initialize the filters.
		for(unsigned i=0; i<_options.filters().size(); ++i)
		{
			const ConfigOptions& conf = _options.filters().at(i);
			FeatureFilter* filter = FeatureFilterRegistry::instance()->create( conf.getConfig(), 0L );
			if ( filter )
			{
				_filters.push_back( filter );
				filter->initialize( readOptions );
			}
		}

		_status = initialize(readOptions);
		return _status;
	}

	void PipelineSource::setFeatureProfile(const FeatureProfile* fp)
	{
		_featureProfile = fp;
	}

	const FeatureFilterList& PipelineSource::getFilters() const
	{
		return _filters;
	}

	const FeatureSchema& PipelineSource::getSchema() const
	{
		static FeatureSchema s_emptySchema;
		return s_emptySchema;
	}

	void PipelineSource::addToBlacklist( FeatureID fid )
	{
		Threading::ScopedWriteLock exclusive( _blacklistMutex );
		_blacklist.insert( fid );
	}

	void PipelineSource::removeFromBlacklist( FeatureID fid )
	{
		Threading::ScopedWriteLock exclusive( _blacklistMutex );
		_blacklist.erase( fid );
	}

	void PipelineSource::clearBlacklist()
	{
		Threading::ScopedWriteLock exclusive( _blacklistMutex );
		_blacklist.clear();
	}

	bool PipelineSource::isBlacklisted( FeatureID fid ) const
	{
		Threading::ScopedReadLock shared( const_cast<PipelineSource*>(this)->_blacklistMutex );
		return _blacklist.find( fid ) != _blacklist.end();
	}

	void PipelineSource::applyFilters(FeatureList& features, const GeoExtent& extent) const
	{
		// apply filters before returning.
		if ( !getFilters().empty() )
		{
			FilterContext cx;
			cx.setProfile( getFeatureProfile() );
			cx.extent() = extent;
			for(FeatureFilterList::const_iterator filter = getFilters().begin(); filter != getFilters().end(); ++filter)
			{
				cx = filter->get()->push( features, cx );
			}
		}
	}

	//------------------------------------------------------------------------

#undef  LC
#define LC "[FeatureSourceFactory] "
#define FEATURE_SOURCE_OPTIONS_TAG "__osgEarth::PipelineSourceOptions"

	PipelineSource* FeatureSourceFactory::create( const PipelineSourceOptions& options )
	{
		PipelineSource* featureSource = 0L;

		if ( !options.getDriver().empty() )
		{
			std::string driverExt = std::string(".osgearth_feature_") + options.getDriver();

			osg::ref_ptr<osgDB::Options> rwopts = Registry::instance()->cloneOrCreateOptions();
			rwopts->setPluginData( FEATURE_SOURCE_OPTIONS_TAG, (void*)&options );

			featureSource = dynamic_cast<PipelineSource*>( osgDB::readObjectFile( driverExt, rwopts.get() ) );
			if ( featureSource )
			{
				if ( options.name().isSet() )
					featureSource->setName( *options.name() );
				else
					featureSource->setName( options.getDriver() );
			}
			else
			{
				OE_WARN << LC << "FAILED to load feature driver \"" << options.getDriver() << "\"" << std::endl;
			}
		}
		else
		{
			OE_WARN << LC << "ILLEGAL null feature driver name" << std::endl;
		}

		return featureSource;
	}

	//------------------------------------------------------------------------

	const PipelineSourceOptions& FeatureSourceDriver::getFeatureSourceOptions( const osgDB::ReaderWriter::Options* rwopt ) const
	{
		static PipelineSourceOptions s_default;
		const void* data = rwopt->getPluginData(FEATURE_SOURCE_OPTIONS_TAG);
		return data ? *static_cast<const PipelineSourceOptions*>(data) : s_default;
	}
}
