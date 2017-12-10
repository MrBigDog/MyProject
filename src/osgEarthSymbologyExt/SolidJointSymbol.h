#ifndef OSGEARTHSYMBOLOGYEXT_JOINTSYMBOL_H
#define OSGEARTHSYMBOLOGYEXT_JOINTSYMBOL_H 1

#include <osgEarthSymbologyExt/Export.h>
#include <compiler.h>
#include <osgEarthSymbology/Symbol>

using namespace osgEarth::Symbology;

namespace oeSymbologyExt
{
	class OSGEARTHSYMBOLOGYEXT_EXPORT SolidJointSymbol : public Symbol
	{
	public:
		META_Object(osgEarthSymbologyExt, SolidJointSymbol);
		SolidJointSymbol(const SolidJointSymbol& rhs, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
		SolidJointSymbol(const osgEarth::Config& conf = osgEarth::Config());

		virtual ~SolidJointSymbol() { }

		//public:
		//	void setUnidSolidLineModel(osg::Node* node) { _unidSolidLineModel = node; }
		//	osg::Node* getUnidSolidLineModel() const { return _unidSolidLineModel.get(); }

	private:
		osg::ref_ptr<osg::Node> _unidSolidLineModel;
	};
}


#endif // JointSymbol_h__
