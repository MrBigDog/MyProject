#ifndef WFSPipelineOptions_h__
#define WFSPipelineOptions_h__



#include <osgEarth/Common>
#include <osgEarth/URI>
#include <osgEarthFeatures/FeatureSource>

namespace gwPipeline
{
    using namespace osgEarth;
    using namespace osgEarth::Features;

    /**
     * Options for the WFS driver.
     */
    class WFSPipelineOptions : public FeatureSourceOptions // NO EXPORT; header only
    {
    public:
        /** Base URL of the WFS service (not including any WFS parameters) */
        optional<URI>& url() { return _url; }
        const optional<URI>& url() const { return _url; }
        
        /** deprecated */
        optional<Config>& geometryProfileOptions() { return _geometryProfileConf; }
        const optional<Config>& geometryProfileOptions() const { return _geometryProfileConf; }

        /** WFS typename parameter (see WFS spec) */
        optional<std::string>& typeName() { return _typename; }
        const optional<std::string>& typeName() const { return _typename; }

        /** Cap on the number of features that the WFS service will return on a single request */
        optional<unsigned int>& maxFeatures() { return _maxFeatures; }
        const optional<unsigned int>& maxFeatures() const { return _maxFeatures;}

        /** Response format to request (geojson, gml) */
        optional<std::string>& outputFormat() { return _outputFormat; }
        const optional<std::string>& outputFormat() const { return _outputFormat; }

        /** Explicity disable a "TFS" tiled feature source queries */
        optional<bool>& disableTiling() { return _disableTiling; }
        const optional<bool>& disableTiling() const { return _disableTiling;}

        /** The number of map units to buffer bounding box requests with 
         *  to ensure that enough data is returned.  This is useful when rendering buffered lines
         *  using the AGGLite driver.         
         */
        optional<double>& buffer() { return _buffer;}
        const optional<double>& buffer() const { return _buffer;}



    public:
		WFSPipelineOptions( const ConfigOptions& opt =ConfigOptions() ) :
          FeatureSourceOptions( opt ),
          _buffer( 0 )
        {
            setDriver( "gwpipeline_wfs" );
            fromConfig( _conf );            
        }

        virtual ~WFSPipelineOptions() { }

    public:
        Config getConfig() const 
		{
            Config conf = FeatureSourceOptions::getConfig();
            conf.updateIfSet( "url", _url ); 
            conf.updateIfSet( "geometry_profile", _geometryProfileConf );
            conf.updateIfSet( "typename", _typename );
            conf.updateIfSet( "outputformat", _outputFormat);
            conf.updateIfSet( "maxfeatures", _maxFeatures );
            conf.updateIfSet( "disable_tiling", _disableTiling );
            conf.updateIfSet( "request_buffer", _buffer);

            return conf;
        }

    protected:
        void mergeConfig( const Config& conf ) 
		{
            FeatureSourceOptions::mergeConfig( conf );
            fromConfig( conf );
        }

    private:
        void fromConfig( const Config& conf ) 
		{            
            conf.getIfSet( "url", _url );
            conf.getIfSet( "geometry_profile", _geometryProfileConf );
            conf.getIfSet( "typename", _typename);
            conf.getIfSet( "outputformat", _outputFormat );
            conf.getIfSet( "maxfeatures", _maxFeatures );
            conf.getIfSet( "disable_tiling", _disableTiling);
            conf.getIfSet( "request_buffer", _buffer);            
        }

        optional<URI>         _url;        
        optional<std::string> _typename;
        optional<Config>      _geometryProfileConf;
        optional<std::string> _outputFormat;
        optional<unsigned>    _maxFeatures;            
        optional<bool>    _disableTiling;            
        optional<double>  _buffer;            
    };

}

#endif // WFSPipelineOptions_h__
