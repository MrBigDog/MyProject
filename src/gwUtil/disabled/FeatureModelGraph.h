#ifndef GWUTIL_FeatureModelGraph1_H
#define GWUTIL_FeatureModelGraph1_H 1

#include <gwUtil/Export.h>
#include <osgEarthFeatures/Common>
#include <osgEarthFeatures/FeatureModelSource>
#include <osgEarthFeatures/Session>
#include <osgEarthSymbology/Style>
#include <osgEarth/OverlayNode>
#include <osgEarth/NodeUtils>
#include <osgEarth/ThreadingUtils>
#include <osgEarth/DepthOffset>
#include <osgDB/Callbacks>
#include <osg/Node>
#include <set>

namespace osgEarth { class ClampableNode; }


using namespace osgEarth::Features;

namespace gwUtil
{
	using namespace osgEarth;
	using namespace osgEarth::Symbology;

	class GW_UTIL_EXPORT FeatureModelGraph1 : public osg::Group
	{
	public:
		/**
		 * Constructs a new model graph.
		 *
		 * @param options
		 *      Model source options
		 * @param factory
		 *      Node factory that will be invoked to compile feature data into nodes
		 * @param session
		 *      Session under which to create elements in this graph
		 * @param preMergeOperations
		 *      Node operations that will execute in the pager thread after it finishes
		 *      building a node
		 * @param postMergeOperations
		 *      Node operations that will execute after merging a node into the graph
		 */
		FeatureModelGraph1(
			Session*                         session,
			const FeatureModelSourceOptions& options,
			FeatureNodeFactory*              factory,
			ModelSource*                     modelSource,
			RefNodeOperationVector*          preMergeOperations,
			RefNodeOperationVector*          postMergeOperations);

		/* @deprecated */
		FeatureModelGraph1(
			Session*                         session,
			const FeatureModelSourceOptions& options,
			FeatureNodeFactory*              factory,
			RefNodeOperationVector*          preMergeOperations,
			RefNodeOperationVector*          postMergeOperations);

		/**
		 * Loads and returns a subnode. Used internally for paging.
		 */
		osg::Node* load(
			unsigned lod, unsigned tileX, unsigned tileY,
			const std::string& uri,
			const osgDB::Options* readOptions);

		/**
		 * Style sheet associated with this feature graph.
		 */
		StyleSheet* getStyles() { return _session->styles(); }

		/**
		 * Sets the style sheet to use to render features
		 */
		void setStyles(StyleSheet* styles);

		/**
		 * Session associated with this feature graph.
		 */
		Session* getSession() { return _session; }

		/**
		 * UID given to this feature graph when registered with the pseudo-loader
		 */
		UID getUID() const { return _uid; }

		/**
		 * Mark the feature graph dirty and in need of regeneration
		 */
		void dirty();

		/**
		 * Access to the features levels
		 */
		const std::vector<const FeatureLevel*>& getLevels() const { return _lodmap; };

	public: // osg::Node

		virtual void traverse(osg::NodeVisitor& nv);

	protected:

		virtual ~FeatureModelGraph1();

		osg::Node* setupPaging();

		osg::Group* buildTile(
			const FeatureLevel&   level,
			const GeoExtent&      extent,
			const TileKey*        key,
			const osgDB::Options* readOptions);

		osg::Group* build(
			const Style&          baseStyle,
			const Query&          baseQuery,
			const GeoExtent&      extent,
			FeatureIndexBuilder*  index,
			const osgDB::Options* readOptions);


	private:

		void ctor();

		osg::Group* createStyleGroup(
			const Style&          style,
			const Query&          query,
			FeatureIndexBuilder*  index,
			const osgDB::Options* readOptions);

		osg::Group* createStyleGroup(
			const Style&          style,
			FeatureList&          workingSet,
			const FilterContext&  contextPrototype,
			const osgDB::Options* readOptions);

		void buildStyleGroups(
			const StyleSelector*  selector,
			const Query&          baseQuery,
			FeatureIndexBuilder*  index,
			osg::Group*           parent,
			const osgDB::Options* readOptions);

		void queryAndSortIntoStyleGroups(
			const Query&            query,
			const StringExpression& styleExpr,
			FeatureIndexBuilder*    index,
			osg::Group*             parent,
			const osgDB::Options*   readOptions);

		osg::Group* getOrCreateStyleGroupFromFactory(
			const Style& style);

		osg::BoundingSphered getBoundInWorldCoords(
			const GeoExtent& extent,
			const MapFrame*  mapf) const;

		void buildSubTilePagedLODs(
			unsigned lod, unsigned tileX, unsigned tileY,
			const MapFrame* mapFrame, osg::Group* parent,
			const osgDB::Options* readOptions);


		osg::Group* readTileFromCache(
			const std::string&    cacheKey,
			const osgDB::Options* readOptions);

		bool writeTileToCache(
			const std::string&    cacheKey,
			osg::Group*           tile,
			const osgDB::Options* readOptions);

		void redraw();

	private:
		FeatureModelSourceOptions        _options;
		osg::ref_ptr<FeatureNodeFactory> _factory;
		osg::ref_ptr<Session>            _session;
		UID                              _uid;
		std::set<std::string>            _blacklist;
		Threading::ReadWriteMutex        _blacklistMutex;
		GeoExtent                        _usableFeatureExtent;
		bool                             _featureExtentClamped;
		GeoExtent                        _usableMapExtent;
		osg::BoundingSphered             _fullWorldBound;
		bool                             _useTiledSource;
		osgEarth::Revision               _featureSourceRev, _modelSourceRev;
		bool                             _dirty;
		bool                             _pendingUpdate;
		std::vector<const FeatureLevel*> _lodmap;
		OpenThreads::ReentrantMutex      _clampableMutex;

		osg::Group*                      _overlayInstalled;
		osg::ref_ptr<osg::Group>         _overlayPlaceholder;
		osg::ref_ptr<ClampableNode>      _clampable;
		DepthOffsetAdapter               _depthOffsetAdapter;

		OpenThreads::Atomic _cacheReads;
		OpenThreads::Atomic _cacheHits;

		enum OverlayChange {
			OVERLAY_NO_CHANGE,
			OVERLAY_INSTALL_PLACEHOLDER,
			OVERLAY_INSTALL_CLAMPABLE,
			OVERLAY_INSTALL_DRAPEABLE
		};
		OverlayChange                    _overlayChange;

		osg::ref_ptr<osgDB::FileLocationCallback> _defaultFileLocationCallback;

		osg::ref_ptr<RefNodeOperationVector> _preMergeOperations;
		osg::ref_ptr<RefNodeOperationVector> _postMergeOperations;

		osg::observer_ptr<ModelSource> _modelSource;

		osg::ref_ptr<FeatureSourceIndex> _featureIndex;

		void runPreMergeOperations(osg::Node* node);
		void runPostMergeOperations(osg::Node* node);
		void applyRenderSymbology(const Style& style, osg::Node* node);
		void checkForGlobalStyles(const Style& style);
		void changeOverlay();
		bool createOrUpdateNode(FeatureCursor*, const Style&, FilterContext&, const osgDB::Options*, osg::ref_ptr<osg::Node>& output);
	};
}

#endif // GWUTIL_FeatureModelGraph1_H
