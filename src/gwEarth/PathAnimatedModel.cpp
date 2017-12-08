#include <gwEarth/PathAnimatedModel.h>
#include <osgEarth/GeoTransform>
#include <osg/PositionAttitudeTransform>

namespace gwEarth
{
	PathAnimatedModel::PathAnimatedModel(osgEarth::MapNode* mapnode, EarthManipulator* earthManip)
		: showPathFlag_(true)
		, animationPathColor_(1.0, 0.0, 0.0, 0.5)
		, animationPathLineThickness_(20.0)
	{
		pathMatrixTransform_ = new osg::MatrixTransform();
		pathColor_ = new osg::Vec4Array();
		pathVertices_ = new osg::Vec3Array();
		pathGeode_ = new osg::Geode();
		pathGeometry_ = new osg::Geometry();
		lineWidth_ = new osg::LineWidth(animationPathLineThickness_);
		pathColor_->push_back(animationPathColor_);

		pathGeode_->addDrawable(pathGeometry_.get());
		pathMatrixTransform_->addChild(pathGeode_.get());
		pathMatrixTransform_->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

		animationPath_ = new PlanetAnimationPath;
		animationPath_->setMapNode(mapnode);
		animationPathCallback_ = new PathAnimatedModel::PathCallback(earthManip);
		animationPathCallback_->setAnimationPath(animationPath_);
		animationPathCallback_->setPause(true);

		this->setUpdateCallback(animationPathCallback_);
	}

	PathAnimatedModel::~PathAnimatedModel()
	{
		this->setUpdateCallback(0);
		if (animationPathCallback_.valid())
		{
			animationPathCallback_->setAnimationPath(0);
		}
	}

	void PathAnimatedModel::setAnimationPathColor(const osg::Vec4f& value)
	{
		setDirtyBit(COLOR_DIRTY); //
		animationPathColor_ = value;
	}

	void PathAnimatedModel::setAnimationPathLineThickness(double value)
	{
		setDirtyBit(COLOR_DIRTY);  //
		animationPathLineThickness_ = value;
	}

	void PathAnimatedModel::setShowPathFlag(bool flag)
	{
		showPathFlag_ = flag;
	}

	void PathAnimatedModel::setTimeScale(double scale)
	{
		animationPathCallback_->setTimeMultiplier(scale);
	}

	void PathAnimatedModel::setTimeOffset(double offset)
	{
		animationPathCallback_->setTimeOffset(offset);
	}

	void PathAnimatedModel::traverse(osg::NodeVisitor& nv)
	{
		//switch(nv.getVisitorType())
		//{
		//case osg::NodeVisitor::UPDATE_VISITOR:
		//	{
		if (isDirtyBitSet(COLOR_DIRTY))
		{
			updateColor();
		}
		if (isDirtyBitSet(COORDINATE_DIRTY))
		{
			updateCoordinates();
		}
		//		break;
		//	}
		//default:
		//	{
		//		break;
		//	}
		//}
		if (pathVertices_->size() > 0)
		{
			if (nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR)
			{
				if (showPathFlag_)
				{
					pathMatrixTransform_->accept(nv);
				}
			}
			else
			{
				pathMatrixTransform_->accept(nv);
			}
		}

		osg::MatrixTransform::traverse(nv);
	}

	void PathAnimatedModel::stage()
	{
		updateCoordinates();
		updateColor();
		setStagedFlag(true);
	}

	void PathAnimatedModel::updateCoordinates()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(updateCoordinatesMutex_);
		if (animationPath_.valid())
		{
			PlanetAnimationPath::PointList pointList;
			animationPath_->generateWorldCoordinates(pointList);
			bool updatePrimitiveSetFlag = false;
			if (pathVertices_->size() != pointList.size())
			{
				updatePrimitiveSetFlag = true;
				pathVertices_->resize(pointList.size());
			}
			long idx = 0;
			long size = pointList.size();
			double normalizer = 1.0 / static_cast<double>(size);
			osg::Vec3d theCenter;
			if (pointList.size())
			{
				theCenter = pointList[0] * normalizer;
			}
			for (idx = 1; idx < size; ++idx)
			{
				theCenter += pointList[idx] * normalizer;
			}
			osg::Matrixd m;
			m.setTrans(theCenter);
			pathMatrixTransform_->setMatrix(m);
			for (idx = 0; idx < size; ++idx)
			{
				osg::Vec3f localPoint = pointList[idx] - theCenter;
				(*pathVertices_)[idx] = localPoint;
			}
			pathGeometry_->setVertexArray(pathVertices_.get());
			if (updatePrimitiveSetFlag)
			{
				if (pathGeometry_->getNumPrimitiveSets() > 0)
				{
					pathGeometry_->setPrimitiveSet(0, new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, pointList.size()));
				}
				else
				{
					pathGeometry_->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, pointList.size()));
				}
			}
			pathGeode_->dirtyBound();
			pathMatrixTransform_->dirtyBound();
		}
		clearDirtyBit(COORDINATE_DIRTY);
	}

	void PathAnimatedModel::updateColor()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(updateColorMutex_);
		if (pathColor_->size() != 1)
		{
			pathColor_->push_back(animationPathColor_);
		}
		else
		{
			(*pathColor_)[0] = animationPathColor_;
		}
		osg::StateSet* stateset = pathGeometry_->getOrCreateStateSet();
		lineWidth_->setWidth(animationPathLineThickness_);
		pathGeometry_->setColorArray(pathColor_.get());
		pathGeometry_->setColorBinding(osg::Geometry::BIND_OVERALL);
		stateset->setAttribute(lineWidth_.get());

		pathGeometry_->setUseVertexBufferObjects(true);
		pathGeometry_->setUseDisplayList(false);
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
		stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		clearDirtyBit(COLOR_DIRTY);
	}

	PathAnimatedModel::PathCallback::PathCallback(EarthManipulator* earthManip)
		: osg::AnimationPathCallback()
		, earthManip_(earthManip)
	{ }

	PathAnimatedModel::PathCallback::PathCallback(const PathCallback& apc, const osg::CopyOp& copyop)
		: osg::AnimationPathCallback(apc, copyop)
		, earthManip_(apc.earthManip_)
	{ }

	PathAnimatedModel::PathCallback::PathCallback(osg::AnimationPath* ap, double timeOffset, double timeMultiplier, EarthManipulator* earthManip)
		: osg::AnimationPathCallback(ap, timeOffset, timeMultiplier)
		, earthManip_(earthManip)
	{ }

	void PathAnimatedModel::PathCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::MatrixTransform* mat = dynamic_cast<osg::MatrixTransform*>(node);
		if (mat || earthManip_ != NULL)
		{
			if (_animationPath.valid() && nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR && nv->getFrameStamp())
			{
				double time = nv->getFrameStamp()->getSimulationTime();

				_latestTime = time;
				_firstTime = 0.0;

				if (!_pause)
				{
					osg::AnimationPath::ControlPoint cp;
					if (_animationPath->getInterpolatedControlPoint(getAnimationTime(), cp))
					{
						osg::Matrixd m;
						cp.getMatrix(m);
						if (mat)
						{
							mat->setMatrix(m);
						}
						if (earthManip_ != NULL)
						{
							earthManip_->setByMatrix(m);
						}
					}
				}
			}
			NodeCallback::traverse(node, nv);
		}
		else
		{
			osg::AnimationPathCallback::operator()(node, nv);
		}
	}

	void PathAnimatedModel::readAnimationPathFromXml(const std::string& xml_in)
	{
		setDirtyBit(COORDINATE_DIRTY);
		animationPath_->openAnimationPathByXml(xml_in);
	}

	void PathAnimatedModel::saveAnimationPathToXml(const std::string& xml_out)
	{
		animationPath_->saveAnimationPathToXml(xml_out);
	}

	void PathAnimatedModel::setLoopMode(osg::AnimationPath::LoopMode lm)
	{
		animationPath_->setLoopMode(lm);
	}

	void PathAnimatedModel::addControlPoint(double t, const osgEarth::GeoPoint& pos, const osg::Vec3d& orient, const osg::Vec3d& scale)
	{
		setDirtyBit(COORDINATE_DIRTY);
		animationPath_->geospatialPath()->timeTupleMap().insert(std::make_pair(t, PlanetAnimationPath::Tuple(pos, orient, scale)));
	}

	void PathAnimatedModel::play()
	{
		this->setUpdateCallback(0);
		this->setUpdateCallback(animationPathCallback_);
		animationPathCallback_->setPause(false);
	}

	void PathAnimatedModel::pause()
	{
		animationPathCallback_->setPause(true);
	}
}
