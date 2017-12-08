#include <gwTool/QueryTool.h>
#include <gwApp/Application.h>
#include <osgEarth/Registry>

static osg::Uniform* s_highlightUniform;

using namespace gwEarth;

namespace gwTool
{
	const char* qhighlightVert =
		"#version " GLSL_VERSION_STR "\n"
		"uniform uint objectid_to_highlight; \n"
		"uint oe_index_objectid;      // Stage global containing object id \n"
		"flat out int selected; \n"
		"void checkForHighlight(inout vec4 vertex) \n"
		"{ \n"
		"    selected = (objectid_to_highlight > 1u && objectid_to_highlight == oe_index_objectid) ? 1 : 0; \n"
		"} \n";

	const char* qhighlightFrag =
		"#version " GLSL_VERSION_STR "\n"
		"flat in int selected; \n"
		"void highlightFragment(inout vec4 color) \n"
		"{ \n"
		"    if ( selected == 1 ) \n"
		"        color.rgb = mix(color.rgb, clamp(vec3(2.0,2.0,0.0)*(1.0-color.rgb), 0.0, 1.0), 0.5); \n"//0.5,2.0,2.0
		"}\n";

	QueryTool::QueryToolCallback::QueryToolCallback()
	{
	}

	void QueryTool::QueryToolCallback::onHit(osgEarth::ObjectID id)
	{
		////这是得到模型的id;
		//std::string mid = Application::instance()->gwModelSourceIndex()->getModelUid(id);
		//osg::Node* nn = Application::instance()->gwModelSourceIndex()->get<osg::Node>(mid);
		//if (nn)
		//{
		//	std::cout << "got a building model" << std::endl;
		//}
		//highlightPickResult(id);
	}

	void QueryTool::QueryToolCallback::onMiss()
	{
		cancelHighlight();
	}

	bool QueryTool::QueryToolCallback::accept(const osgGA::GUIEventAdapter& ea, const osgGA::GUIActionAdapter& aa)
	{
		return ea.getEventType() == ea.PUSH;
	}

	void QueryTool::QueryToolCallback::highlightPickResult(const osgEarth::ObjectID& id)
	{
		s_highlightUniform->set(id);
	}

	void QueryTool::QueryToolCallback::cancelHighlight()
	{
		s_highlightUniform->set(0u);
	}

	//======================================================================================================
	QueryTool::QueryTool() :osgEarth::Util::RTTPicker()
	{
	}

	void QueryTool::removeAllChildren()
	{
		_group->removeChildren(0, _group->getNumChildren());
	}

	void QueryTool::installHighlighter(osg::StateSet* stateSet, int attrLocation)
	{
		osg::ref_ptr<osgEarth::VirtualProgram> vp = osgEarth::VirtualProgram::getOrCreate(stateSet);
		vp->setFunction("checkForHighlight", qhighlightVert, osgEarth::ShaderComp::LOCATION_VERTEX_CLIP);
		vp->setFunction("highlightFragment", qhighlightFrag, osgEarth::ShaderComp::LOCATION_FRAGMENT_COLORING);

		osgEarth::Registry::objectIndex()->loadShaders(vp);

		s_highlightUniform = new osg::Uniform("objectid_to_highlight", 0u);
		stateSet->addUniform(s_highlightUniform);
	}
}