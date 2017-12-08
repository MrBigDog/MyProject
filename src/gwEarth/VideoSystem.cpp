#include "VideoSystem.h"

gwEarth::VideoSystem::VideoSystem(const std::string & name, gwApp::Application * app)
	: _app(app), _map(0L), _mapNode(0L)
{
	setName(name);
	if (_app)
	{
		_map = _app->getMap();
		_mapNode = _app->getMapNode();
	}
}

gwEarth::VideoSystem::~VideoSystem()
{
}

void gwEarth::VideoSystem::init()
{
}

void gwEarth::VideoSystem::shutdown()
{
}

void gwEarth::VideoSystem::update(double delta_t)
{
}

osgEarth::VideoLayer * gwEarth::VideoSystem::getLayer(const std::string & layername) const
{
	return nullptr;
}

void gwEarth::VideoSystem::getLayers(VideoLayerVec & out_layers) const
{
}

void gwEarth::VideoSystem::addLayer(osgEarth::VideoLayer * layer)
{
}

bool gwEarth::VideoSystem::addLayer(const osgEarth::Config & conf)
{
	return false;
}

bool gwEarth::VideoSystem::removeLayer(osgEarth::VideoLayer * layer)
{
	return false;
}

bool gwEarth::VideoSystem::removeLayer(const std::string & layername)
{
	return false;
}

void gwEarth::VideoSystem::removeAllLayers()
{
}

void gwEarth::VideoSystem::addCallback(Callback * cb)
{
}

void gwEarth::VideoSystem::removeCallback(Callback * cb)
{
}
