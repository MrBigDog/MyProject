#include "GeologicalProfileLayer.h"

//////////////////////////////////////////////////////////////////////////
GeologicalProfileLayer::GeologicalProfileLayer(const std::string & name, GeologicalProfileModelSource * modelSource)
	: _name(name)
{}

GeologicalProfileLayer::GeologicalProfileLayer(const std::string & name, const BaseProfileOptions & options)
	: _name(name)
	, _options(options)
{}

const std::string & GeologicalProfileLayer::getName() const
{
	return _name;
}

void GeologicalProfileLayer::addCallback(Callback * cb)
{
	if (!cb) return;
	_callbacks.push_back(cb);
}

void GeologicalProfileLayer::removeCallback(Callback * cb)
{
	if (!cb) return;
	CallbackVector::iterator it = std::find(_callbacks.begin(), _callbacks.end(), cb);
	if (it == _callbacks.end()) return;
	_callbacks.erase(it);
}
