#ifndef GWEARTH_VIDEOSYSTEM_H
#define GWEARTH_VIDEOSYSTEM_H 1

#include <compiler.h>

namespace gwEarth
{
	template<typename VIDEO_CLASS>
	class VideoSystem :public gwBase::Subsystem
	{
	public:
		VideoSystem(const std::string& name, gwApp::IApplication* app)
			:_name, _app(app), _map(0L), _mapNode(0L)
		{
			if (_app)
			{
				_map = _app->getMap();
				_mapNode = _app->getMapNode();
			}
		}

		void addLayer(VIDEO_CLASS* layer)
		{

		}







	private:
		std::string _name;
		osgEarth::UID _uid;
		osg::ref_ptr<osgEarth::Map> _map;
		osg::ref_ptr<osgEarth::MapNode> _mapNode;
		osg::ref_ptr<gwApp::IApplication> _app;
	};
}


#endif // GWEARTH_VIDEOSYSTEM_H
