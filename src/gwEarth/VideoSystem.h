#ifndef GWEARTH_VIDEOSYSTEM_H
#define GWEARTH_VIDEOSYSTEM_H 1

#include <compiler.h>
#include <gwEarth/Export.h>
#include <gwBase/SubsystemMgr.h>
#include <gwBase/Exception.h>
#include <gwBase/LogStream.h>

#include <osgEarth/MapNode>
#include <osgEarth/VideoLayer>

namespace gwApp { class Application; };

namespace gwEarth
{
	typedef std::vector<osgEarth::VideoLayer> VideoLayerVec;

	class VideoSystem :public gwBase::Subsystem
	{
	public:
		class Callback :public osg::Referenced
		{
		public:
			virtual void onLayerAdded(osgEarth::VideoLayer* layer) {}
			virtual void onLayerRemoved(osgEarth::VideoLayer* layer) {}
		};

	public:
		VideoSystem(const std::string& name, gwApp::Application* app);
		~VideoSystem();

		void init();
		void shutdown();
		void update(double delta_t);

		osgEarth::VideoLayer* getLayer(const std::string& layername) const;
		void getLayers(VideoLayerVec& out_layers) const;

		void addLayer(osgEarth::VideoLayer* layer);
		bool addLayer(const osgEarth::Config& conf);

		bool removeLayer(osgEarth::VideoLayer* layer);
		bool removeLayer(const std::string& layername);
		void removeAllLayers();

		void addCallback(Callback* cb);
		void removeCallback(Callback* cb);

	private:
		osg::ref_ptr<osgEarth::Map> _map;
		osg::ref_ptr<osgEarth::MapNode> _mapNode;
		osg::ref_ptr<gwApp::Application> _app;
	};
}


#endif // GWEARTH_VIDEOSYSTEM_H
