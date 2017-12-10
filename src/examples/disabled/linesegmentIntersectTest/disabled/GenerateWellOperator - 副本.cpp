#include "generateWellOperator.h"
#include "SliceOperator.h"
#include "Cutter.h"
#include "GeologicalOperatorHelp.h"

#include <osgUtil/Optimizer>
namespace
{
	struct CutPlnae
	{
		CutPlnae(const osg::Plane& plane, const osg::Polytope& polytope)
			:_plane(plane)
			, _polytope(polytope)
		{}
		osg::Plane _plane;
		osg::Polytope _polytope;
	};
	typedef std::vector<CutPlnae> CutPlaneVector;
	typedef std::vector<CutPlnae>::const_iterator CutPlaneVectorConstIt;

	//struct GeometryClass
	//{
	//	GeometryClass(osg::Geometry* geom) :_geom(geom)
	//	{}

	//	inline bool operator ==(const GeometryClass& gc)
	//	{
	//		if (!isVec4Equal(getGeomColor(_geom), getGeomColor(gc._geom), FLT_EPSILON))
	//		{
	//			return false;
	//		}
	//		if (getGeomTextureFileName(_geom) != getGeomTextureFileName(gc._geom))
	//		{
	//			return false;
	//		}
	//		return true;
	//	}

	//private:
	//	std::string getGeomTextureFileName(const osg::Geometry* geom)
	//	{
	//		const osg::StateSet* ss = geom->getStateSet();
	//		if (ss)
	//		{
	//			const osg::StateSet::TextureAttributeList& a = ss->getTextureAttributeList();
	//			for (osg::StateSet::TextureAttributeList::const_iterator i = a.begin(); i != a.end(); ++i)
	//			{
	//				const osg::StateSet::AttributeList& b = *i;
	//				for (osg::StateSet::AttributeList::const_iterator j = b.begin(); j != b.end(); ++j)
	//				{
	//					osg::StateAttribute* sa = j->second.first.get();
	//					if (sa)
	//					{
	//						osg::Texture* tex = dynamic_cast<osg::Texture*>(sa);
	//						return tex->getImage(0)->getFileName();
	//					}
	//				}
	//			}
	//		}
	//		return "";
	//	}

	//	osg::Vec4 getGeomColor(const osg::Geometry* geom)
	//	{
	//		osg::Vec4Array* ca = dynamic_cast<osg::Vec4Array*>(_geom->getColorArray());
	//		if (!ca || ca->empty()) return osg::Vec4(-1, -1, -1, -1);
	//		return ca->at(0);
	//	}

	//public:
	//	osg::ref_ptr<osg::Geometry> _geom;
	//};

	struct WellGeometryOptimizer :public osg::NodeVisitor
	{
		WellGeometryOptimizer() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
		{}

		void apply(osg::Geode& geode)
		{
			for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
			{
				osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
				osg::Drawable* d = geode.getDrawable(i);
				bool ishave = false;
				for (GeodeVector::const_iterator it = _geodeVector.begin(); it != _geodeVector.end(); ++it)
				{
					if (d->getUserData() == (*it)->getUserData())
					{
						(*it)->addDrawable(d);
						ishave = true;
						break;
					}
				}
				if (!ishave)
				{
					osg::ref_ptr<osg::Geode> geode = new osg::Geode;
					geode->setUserData(d->getUserData());
					geode->addDrawable(d);
					_geodeVector.push_back(geode);
				}
			}
		}

		osg::Node* optimize()
		{
			for (GeodeVector::const_iterator it = _geodeVector.begin(); it != _geodeVector.end(); ++it)
			{
				osgUtil::Optimizer::MergeGeometryVisitor mgv;
				mgv.setTargetMaximumNumberOfVertices(100000);
				(*it)->accept(mgv);


			}
		}

	private:
		typedef std::vector<osg::ref_ptr<osg::Geode> > GeodeVector;
		GeodeVector _geodeVector;
	};

}


//-----------------------------------------------------------------------------------
WellGenerator::WellGenerator(bool isOnEarth, int segmentNum)
	: _isOnEarth(isOnEarth)
	, _segmentNum(segmentNum)
{ }

osg::Node * WellGenerator::operator()(osg::Group * geoModel, double well_radius, const osg::Vec3d & pos)
{
	osg::Vec3d up = _isOnEarth ? normalize(pos) : osg::Vec3d(0, 0, 1);
	double deltaAngle = osg::PI*2.0 / (double)_segmentNum;
	osg::Quat rot(deltaAngle, up);
	osg::Vec3d currentDir = _isOnEarth ? normalize(osg::Vec3d(0, 0, 1) ^ up) : osg::Vec3d(1, 0, 0);

	osg::ref_ptr<osg::Vec3dArray> va = new osg::Vec3dArray;
	for (unsigned int i = 0; i < _segmentNum; ++i)
	{
		osg::Vec3d currentPos = pos + currentDir*well_radius;
		va->push_back(currentPos);
		currentDir = normalize(rot*currentDir);
	} va->push_back(va->at(0));

	std::vector<CutPlnae> planeVector; planeVector.reserve(va->size() - 1);
	for (osg::Vec3dArray::const_iterator it = va->begin() + 1; it != va->end(); ++it)
	{
		osg::Vec3d planenormal = normalize(normalize(*it - pos) + normalize(*(it - 1) - pos));
		osg::Vec3d polytopePlaneNormal = normalize(*it - *(it - 1));

		osg::Polytope polytope;
		polytope.add(osg::Plane(polytopePlaneNormal, *(it - 1)));
		polytope.add(osg::Plane(-polytopePlaneNormal, *(it)));

		planeVector.push_back(CutPlnae(osg::Plane(planenormal, *it), polytope));
	}

	osg::ref_ptr<osg::Group> profileRoot = new osg::Group;
	for (CutPlaneVectorConstIt it = planeVector.begin(); it != planeVector.end(); ++it)
	{
		SliceOperator sliceer(_isOnEarth);
		osg::ref_ptr<osg::Node> profileNode = sliceer.slice(geoModel, (*it)._plane, 0L);

		Cutter cutter(it->_polytope);
		profileNode->accept(cutter);
		profileRoot->addChild(cutter.getResult());
	}
	return profileRoot.release();
}
