#ifndef GWEARTH_JOINTSYMBOL_H
#define GWEARTH_JOINTSYMBOL_H 1

#include <gwBase/compiler.h>
#include <osgEarthSymbology/Symbol>

namespace gwEarth
{
	using namespace osgEarth::Symbology;

	class SolidJointSymbol : public Symbol
	{
	public:
		//META_Object(gwEarth, SolidJointSymbol);
		virtual ~SolidJointSymbol() { }

		//public:
		//	void setUnidSolidLineModel(osg::Node* node) { _unidSolidLineModel = node; }
		//	osg::Node* getUnidSolidLineModel() const { return _unidSolidLineModel.get(); }

	private:
		osg::ref_ptr<osg::Node> _unidSolidLineModel;
	};
}


#endif // JointSymbol_h__
