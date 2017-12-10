#include <gwUtil/TilePagedLOD.h>
#include <osg/Version>
#include <osg/ComputeBoundsVisitor>
#include <osgEarth/Registry>
#include <osgEarth/CullingUtils>
#include <cassert>

#include <iostream>
using namespace osgEarth;

namespace gwUtil
{
	TilePagedLOD::TilePagedLOD() :osg::PagedLOD()
	{
		osgDB::Options* options = Registry::instance()->cloneOrCreateOptions();
		setDatabaseOptions(options);
	}

	TilePagedLOD::~TilePagedLOD()
	{ }

	osgDB::Options* TilePagedLOD::getOrCreateDBOptions()
	{
		if (!getDatabaseOptions())
		{
			setDatabaseOptions(Registry::instance()->cloneOrCreateOptions());
		}
		return static_cast<osgDB::Options*>(getDatabaseOptions());
	}

	void TilePagedLOD::setChildBoundingBoxAndMatrix(int childNum, const osg::BoundingBox& bbox, const osg::Matrix& matrix)
	{
		_childBBoxes.resize(childNum + 1);
		_childBBoxes[childNum] = bbox;
		_childBBoxMatrices.resize(childNum + 1);
		_childBBoxMatrices[childNum] = matrix;
	}

	void TilePagedLOD::setComputeRangeCallback(ComputeRangeCallback * crcb)
	{
		_computeRangeCallback = crcb;
	}

	bool TilePagedLOD::addChild(osg::Node * child)
	{
		if (!PagedLOD::addChild(child))
		{
			return false;
		}

		//setRadius(getBound().radius());
		//setCenter(getBound().center());

		osg::BoundingSphere bs = computeBound();
		bs.expandRadiusBy(osg::Group::computeBound());
		setRadius(bs.radius()*2.0);
		setCenter(bs.center());

		return true;
	}


	// MOST of this is copied and pasted from OSG's osg::PagedLOD::traverse,
	// except where otherwise noted with an "osgEarth" comment.
	void TilePagedLOD::traverse(osg::NodeVisitor& nv)
	{
		// set the frame number of the traversal so that external nodes can find out how active this
		// node is.
		if (nv.getFrameStamp() && nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR)
		{
			setFrameNumberOfLastTraversal(nv.getFrameStamp()->getFrameNumber());
		}

		double timeStamp = nv.getFrameStamp() ? nv.getFrameStamp()->getReferenceTime() : 0.0;
		unsigned int frameNumber = nv.getFrameStamp() ? nv.getFrameStamp()->getFrameNumber() : 0;
		bool updateTimeStamp = nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR;

		switch (nv.getTraversalMode())
		{
		case(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN):
		{
			std::for_each(_children.begin(), _children.end(), osg::NodeAcceptOp(nv));
			break;
		}
		case(osg::NodeVisitor::TRAVERSE_ACTIVE_CHILDREN):
		{
			// Compute the required range.
			float required_range = -1.0;
			if (_computeRangeCallback)
			{
				required_range = (*_computeRangeCallback)(this, nv);
			}

			// If we don't have a callback or it return a negative number fallback on the original calculation.
			if (required_range < 0.0)
			{
				if (_rangeMode == DISTANCE_FROM_EYE_POINT)
				{
					required_range = nv.getDistanceToViewPoint(getCenter(), true);
					if (_rangeFactor.isSet())
					{
						required_range /= _rangeFactor.get();
					}
				}
				else
				{
					osg::CullStack* cullStack = dynamic_cast<osg::CullStack*>(&nv);
					if (cullStack && cullStack->getLODScale() > 0.0f)
					{
						required_range = cullStack->clampedPixelSize(getBound()) / cullStack->getLODScale();
					}
					else
					{
						// fallback to selecting the highest res tile by
						// finding out the max range
						for (unsigned int i = 0; i < _rangeList.size(); ++i)
						{
							required_range = osg::maximum(required_range, _rangeList[i].first);
						}
					}
				}
			}

			int lastChildTraversed = -1;
			bool needToLoadChild = false;
			for (unsigned int i = 0; i < _rangeList.size(); ++i)
			{
				if (_rangeList[i].first <= required_range && required_range < _rangeList[i].second)
				{
					if (i < _children.size())
					{
						if (updateTimeStamp)
						{
							_perRangeDataList[i]._timeStamp = timeStamp;
							_perRangeDataList[i]._frameNumber = frameNumber;
						}
						_children[i]->accept(nv);
						lastChildTraversed = (int)i;
					}
					else
					{
						needToLoadChild = true;
					}
				}
			}

#ifdef INHERIT_VIEWPOINT_CAMERAS_CANNOT_SUBDIVIDE
			// Prevents an INHERIT_VIEWPOINT camera from invoking tile subdivision
			if (needToLoadChild)
			{
				osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(&nv);
				if (cv && cv->getCurrentCamera() && cv->getCurrentCamera()->getReferenceFrame() == osg::Camera::ABSOLUTE_RF_INHERIT_VIEWPOINT)
					needToLoadChild = false;
			}
#endif

			if (needToLoadChild)
			{
				unsigned int numChildren = _children.size();

				// select the last valid child.
				if (numChildren > 0 && ((int)numChildren - 1) != lastChildTraversed)
				{
					if (updateTimeStamp)
					{
						_perRangeDataList[numChildren - 1]._timeStamp = timeStamp;
						_perRangeDataList[numChildren - 1]._frameNumber = frameNumber;
					}
					_children[numChildren - 1]->accept(nv);
				}

				// now request the loading of the next unloaded child.
				if (!_disableExternalChildrenPaging && nv.getDatabaseRequestHandler() && numChildren < _perRangeDataList.size())
				{
					// osgEarth: Perform a tile visibility check before requesting the new tile.
					// Intersect the tile's earth-aligned bounding box with the current culling frustum.
					bool tileIsVisible = true;

					if (nv.getVisitorType() == nv.CULL_VISITOR && numChildren < _childBBoxes.size() && _childBBoxes[numChildren].valid())
					{
						osgUtil::CullVisitor* cv = Culling::asCullVisitor(nv);
						// wish that CullStack::createOrReuseRefMatrix() was public
						osg::ref_ptr<osg::RefMatrix> mvm = new osg::RefMatrix(*cv->getModelViewMatrix());
						mvm->preMult(_childBBoxMatrices[numChildren]);
						cv->pushModelViewMatrix(mvm.get(), osg::Transform::RELATIVE_RF);
						tileIsVisible = !cv->isCulled(_childBBoxes[numChildren]);
						cv->popModelViewMatrix();
					}

					if (tileIsVisible)
					{
						// compute priority from where abouts in the required range the distance falls.
						float priority = (_rangeList[numChildren].second - required_range) / (_rangeList[numChildren].second - _rangeList[numChildren].first);

						// invert priority for PIXEL_SIZE_ON_SCREEN mode
						if (_rangeMode == PIXEL_SIZE_ON_SCREEN)
						{
							priority = -priority;
						}

						// modify the priority according to the child's priority offset and scale.
						priority = _perRangeDataList[numChildren]._priorityOffset + priority * _perRangeDataList[numChildren]._priorityScale;

						if (_databasePath.empty())
						{
							nv.getDatabaseRequestHandler()->requestNodeFile(_perRangeDataList[numChildren]._filename, nv.getNodePath(), priority, nv.getFrameStamp(), _perRangeDataList[numChildren]._databaseRequest, _databaseOptions.get());
						}
						else
						{
							// prepend the databasePath to the child's filename.
							nv.getDatabaseRequestHandler()->requestNodeFile(_databasePath + _perRangeDataList[numChildren]._filename, nv.getNodePath(), priority, nv.getFrameStamp(), _perRangeDataList[numChildren]._databaseRequest, _databaseOptions.get());
						}
					}
				}
			}
			break;
		}
		default:
			break;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	TilePagedLODWithNodeOperations::TilePagedLODWithNodeOperations(RefNodeOperationVector* postMergeOps)
		: _postMergeOps(postMergeOps)
	{}

	void TilePagedLODWithNodeOperations::runPostMerge(osg::Node* node)
	{
		if (_postMergeOps.valid())
		{
			_postMergeOps->mutex().readLock();
			for (NodeOperationVector::iterator i = _postMergeOps->begin(); i != _postMergeOps->end(); ++i)
			{
				i->get()->operator()(node);
			}
			_postMergeOps->mutex().readUnlock();
		}
	}

	bool TilePagedLODWithNodeOperations::addChild(osg::Node* child)
	{
		bool ok = false;
		if (child)
		{
			ok = TilePagedLOD::addChild(child);
			if (ok)
			{
				runPostMerge(child);
			}
		}
		return ok;
	}

	bool TilePagedLODWithNodeOperations::insertChild(unsigned index, Node* child)
	{
		bool ok = false;
		if (child)
		{
			ok = TilePagedLOD::insertChild(index, child);
			if (ok)
			{
				runPostMerge(child);
			}
		}
		return ok;
	}

	bool TilePagedLODWithNodeOperations::replaceChild(Node* origChild, Node* newChild)
	{
		bool ok = false;
		if (origChild && newChild)
		{
			ok = TilePagedLOD::replaceChild(origChild, newChild);
			if (ok)
			{
				runPostMerge(newChild);
			}
		}
		return ok;
	}
}