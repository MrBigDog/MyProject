#ifndef GWTUIL_TILEPAGEDLOD_H
#define GWTUIL_TILEPAGEDLOD_H 1
#include <gwUtil/Export.h>

#include <osg/PagedLOD>
#include <osgDB/Options>
#include <osgEarth/optional>
#include <osgEarth/NodeUtils>
#include <osgEarth/ThreadingUtils>

using namespace osgEarth;

namespace gwUtil
{
	class GW_UTIL_EXPORT TilePagedLOD : public osg::PagedLOD
	{
	public:
		struct ComputeRangeCallback : public osg::Referenced
		{
			virtual float operator()(osg::Node* node, osg::NodeVisitor& nv) = 0;
		};

	public:
		TilePagedLOD();

		void setChildBoundingBoxAndMatrix(int childNum, const osg::BoundingBox& bbox, const osg::Matrix& world2local);

		void setComputeRangeCallback(ComputeRangeCallback* crcb);

		osgDB::Options* getOrCreateDBOptions();

		void setRangeFactor(double f) { _rangeFactor = f; }

	public: // osg::Group
		virtual bool addChild(osg::Node *child);
		//virtual bool insertChild(unsigned index, osg::Node* child);
		//virtual bool replaceChild(Node* origChild, osg::Node* newChild);
		void traverse(osg::NodeVisitor& nv);

	protected:
		virtual ~TilePagedLOD();

	private:
		Threading::Mutex               _updateMutex;
		std::vector<osg::BoundingBox>  _childBBoxes;
		std::vector<osg::Matrix>       _childBBoxMatrices;
		optional<double>               _rangeFactor;

		osg::ref_ptr<ComputeRangeCallback> _computeRangeCallback;
	};


	class GW_UTIL_EXPORT TilePagedLODWithNodeOperations : public TilePagedLOD
	{
	public:
		TilePagedLODWithNodeOperations(RefNodeOperationVector* postMergeOps);

	public: // osg::Group
		virtual bool addChild(osg::Node* child);
		virtual bool insertChild(unsigned index, Node* child);
		virtual bool replaceChild(Node* origChild, Node* newChild);

	protected:
		osg::observer_ptr<RefNodeOperationVector> _postMergeOps;

		void runPostMerge(osg::Node* node);
	};
}

#endif
