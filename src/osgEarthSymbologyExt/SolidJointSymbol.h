#ifndef OSGEARTHSYMBOLOGYEXT_JOINTSYMBOL_H
#define OSGEARTHSYMBOLOGYEXT_JOINTSYMBOL_H 1

#include <osgEarthSymbologyExt/Export.h>
#include <compiler.h>
#include <osgEarthSymbology/Symbol>

namespace oeSymbologyExt
{
	using namespace osgEarth::Symbology;

	class OSGEARTHSYMBOLOGYEXT_EXPORT SolidJointSymbol : public Symbol
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
