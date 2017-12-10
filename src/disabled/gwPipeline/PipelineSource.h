#ifndef PipelineSource_h__
#define PipelineSource_h__

#include <export.h>

#include <osgEarthFeatures/Common>
#include <osgEarthFeatures/Feature>
#include <osgEarthFeatures/FeatureCursor>
#include <osgEarthSymbology/Geometry>
#include <osgEarthSymbology/Query>
#include <osgEarthFeatures/Filter>
#include <osgEarthSymbology/Style>
#include <osgEarth/Profile>
#include <osgEarth/GeoData>
#include <osgEarth/Cache>
#include <osgEarth/CachePolicy>
#include <osgEarth/URI>
#include <osgEarth/Revisioning>

#include <osgDB/ReaderWriter>
#include <OpenThreads/Mutex>
#include <list>

namespace gwPipeline
{   
    using namespace osgEarth;
    using namespace osgEarth::Symbology;

    class GW_EXPORT PipelineSourceOptions : public DriverConfigOptions
    {
    public: // properties
        /** Name of this feature source. */
        optional<std::string>& name() { return _name; }
        const optional<std::string>& name() const { return _name; }

        /** Set of FeatureFilter options. */
        std::vector<ConfigOptions>& filters() { return _filterOptions; }
        const std::vector<ConfigOptions>& filters() const { return _filterOptions; }

        /** Opens the feature data for writing, if supported */
        optional<bool>& openWrite() { return _openWrite; }
        const optional<bool>& openWrite() const { return _openWrite; }

        /** Explicitly overrides profile information contained in the actual data source. */
        optional<ProfileOptions>& profile() { return _profile; }
        const optional<ProfileOptions>& profile() const { return _profile; }
        
        /** Interpolation type to use for geodetic points */
        optional<GeoInterpolation>& geoInterp() { return _geoInterp; }
        const optional<GeoInterpolation>& geoInterp() const { return _geoInterp; }

        /** The feature attribute to use as the fid for the features from this PipelineSource*/
        optional<std::string>& fidAttribute() { return _fidAttribute; }
        const optional<std::string>& fidAttribute() const { return _fidAttribute; }

    public:
        PipelineSourceOptions( const ConfigOptions& options =ConfigOptions() );
        virtual ~PipelineSourceOptions();
        virtual Config getConfig() const;

    protected:
        virtual void mergeConfig( const Config& conf )
		{
            DriverConfigOptions::mergeConfig( conf );
            fromConfig( conf );
        }

    private:
        void fromConfig( const Config& conf );

        std::vector<ConfigOptions> _filterOptions;
        optional<std::string>      _name;
        optional< bool >           _openWrite;
        optional<ProfileOptions>   _profile;
        optional<CachePolicy>      _cachePolicy;
        optional<GeoInterpolation> _geoInterp;
        optional<std::string>      _fidAttribute;
    };

    /**
     * A PipelineSource is a pluggable object that generates Features, and 
     * optionally, styling information to go along with them.
     */
    class GW_EXPORT PipelineSource : public osg::Object, public Revisioned
    {
    public:      
        PipelineSource( const ConfigOptions&  options =ConfigOptions(), const osgDB::Options* readOptions =0L );

        /**
         * Opens the feature source and returns an initial status.
         */
        const Status& open(const osgDB::Options* readOptions);
        const Status& open() { return open(0L); }

        /**
         * Status of the feature source
         */
        const Status& getStatus() const { return _status; }

        /**
         * Gets a reference to the metadata that describes features that you can
         * get from this PipelineSource. A valid feature profile indiciates that the
         * feature source successfully initialized.
         */
        const FeatureProfile* getFeatureProfile() const { return _featureProfile.get(); }

        /**
         * Gets the options that were passed into this object's CTOR.
         */
        const PipelineSourceOptions& getFeatureSourceOptions() const { return _options; }

        /**
         * Creates a cursor that iterates over all the features corresponding to the
         * specified query.
         *
         * Caller takes ownership of the returned object.
         */
        virtual FeatureCursor* createFeatureCursor(const Symbology::Query& query) =0;
        FeatureCursor* createFeatureCursor() { return createFeatureCursor(Symbology::Query()); }

        /**
         * Whether this PipelineSource supports inserting and deleting features
         */
        virtual bool isWritable() const { return false; }

        /**
         * Deletes the feature with the given FID
         * @return True on success; false on failure or if the source is read-only
         */
        virtual bool deleteFeature(FeatureID fid) { return false; }

        /**
         * Gets the number of features in this PipelineSource
         * @return
         *      The number of features or -1 if the number of features cannot be determined.
         */
        virtual int getFeatureCount() const { return -1; }

        /**
         * Whether the source can look up a Feature by its ID.
         * @return True or False
         */
        virtual bool supportsGetFeature() const { return false; }

        /**
         * Gets the Feature with the given FID
         * @return
         *     The Feature with the given FID or NULL if not found.
         */
        virtual Feature* getFeature( FeatureID fid ) { return 0L; }

        /**
         * Gets the FeatureSchema for this PipelineSource. If the schema doesn't
         * publish a source, this might be empty.
         */
        virtual const FeatureSchema& getSchema() const;

        /**
         * Inserts the given feature into the PipelineSource
         * @return
         *     True if the feature was inserted, false if not
         */
        virtual bool insertFeature(Feature* feature) { return false; }

        /**
         * Gets the Geometry type of the PipelineSource
         * @return
         *      The Geometry type of the PipelineSource
         */
        virtual Geometry::Type getGeometryType() const { return Geometry::TYPE_UNKNOWN; }


    public: // blacklisting.

        /**
         * Adds a feature ID to the blacklist.
         */
        void addToBlacklist( FeatureID fid );

        /**
         * Removes a feature from the blacklist.
         */
        void removeFromBlacklist( FeatureID fid );

        /**
         * Clears the blacklist.
         */
        void clearBlacklist();
        
        /**
         * Checks the blacklist for a feature ID.
         */
        bool isBlacklisted( FeatureID fid ) const; 

        /**
         * Sets the feature profile for this source.
         * This is required. Usually the subclass should call this from initialize().
         */
        void setFeatureProfile(const FeatureProfile* profile);


    public: // Styling

        /**
         * Returns true if this source creates features with embedded style information.
         * By default, this is false (features are not expected to carry their own
         * style definitions).
         */
        virtual bool hasEmbeddedStyles() const 
		{
            return false; 
		}

    public:

        /**
         * Accesses the list of feature filters that will transform features
         * before they are returned in a feature cursor.
         */
        const FeatureFilterList& getFilters() const;

    public: 

        // META_Object specialization:
        virtual osg::Object* cloneType() const { return 0; } // cloneType() not appropriate
        virtual osg::Object* clone(const osg::CopyOp&) const { return 0; } // clone() not appropriate
        virtual bool isSameKindAs(const osg::Object* obj) const { return dynamic_cast<const PipelineSource*>(obj)!=NULL; }
        virtual const char* className() const { return "PipelineSource"; }
        virtual const char* libraryName() const { return "osgEarthFeatures"; }

        /**
         * Dirties the feature profile
         */
        void dirtyFeatureProfile() { _featureProfile = 0;}

        
        void setReadOptions(const osgDB::Options* readOptions) { _readOptions = readOptions; }
        
    protected:
        
        /**
         * Initialize the PipelineSource.
         * If you override this function, please call this base implementation from within.
         */
        virtual Status initialize(const osgDB::Options* readOptions) =0;

        /**
         * Creates and returns a metadata structure describing the features in a named
         * feature class. This method is called by the public function getFeatureProfile()
         * in this same object to create the metadata structure.
         */
        //virtual const FeatureProfile* createFeatureProfile(int dummy) =0;

        /**
         * DTOR is protected to prevent this object from being allocated on the stack.
         */
        virtual ~PipelineSource();

        /** Access the raw DB options that came in */
        const osgDB::Options* getReadOptions() const { return _readOptions.get(); }

        /** The URI context (for resolving relative paths) */
        const URIContext& uriContext() const { return _uriContext; }

        /** Convenience function to apply the filters to a FeatureList */
        void applyFilters(FeatureList& features, const GeoExtent& extent) const;

        /** Subclass can call this if the status changes */
        void setStatus(const Status& value) { _status = value; }

    private:
        const PipelineSourceOptions         _options;
        osg::ref_ptr<const FeatureProfile> _featureProfile;
        Threading::Mutex                   _createMutex;

        osg::ref_ptr<const osgDB::Options> _readOptions;
        URIContext                         _uriContext;

        Threading::ReadWriteMutex          _blacklistMutex;
        std::set<FeatureID>                _blacklist;
        
        FeatureFilterList                  _filters;

        Status                             _status;

        friend class Map;
        friend class FeatureSourceFactory;
    };

    //--------------------------------------------------------------------

    class GW_EXPORT FeatureSourceDriver : public osgDB::ReaderWriter
    {
    public:
        virtual const PipelineSourceOptions& getFeatureSourceOptions( const osgDB::ReaderWriter::Options* rwopt ) const;
    };

    //--------------------------------------------------------------------

    /**
     * Factory class that will instantiate a PipelineSource corresponding to a driver name.
     */
    class GW_EXPORT FeatureSourceFactory
    {
	public:
        static PipelineSource* create( const PipelineSourceOptions& options );
    };

}

#endif // PipelineSource_h__
