#include "Environment.h"
//#include <tinyxml.h>
#include <osgEarth/URI>
#include <osgEarth/Config>
#include <osgEarth/IOTypes>

#include <osg/io_utils>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

namespace gwApp
{

	Environment::Environment(void)
	{
		_plugin_contain.clear();
		InitFilter();
	}

	Environment::~Environment(void)
	{
	}

	void Environment::InitFilter()
	{
		osgEarth::ReadResult r = osgEarth::URI("../../../../config/config.xml").readString();
		if (r.failed())
		{
			return;
		}

		osgEarth::Config conf;
		std::stringstream buf(r.getString());
		conf.fromXML(buf);

		const osgEarth::Config* plugins = conf.find("plugins");
		if (plugins)
		{
			const osgEarth::ConfigSet& pluginset = plugins->children("plugin");
			osgEarth::ConfigSet::const_iterator it;
			for (it = pluginset.begin(); it != pluginset.end(); ++it)
			{
				const osgEarth::Config& conf = *it;
				int pluginId = conf.value("id", -1);
				std::string pluginName = conf.value("name");
				if (pluginName.empty() || pluginId == -1)
				{
					continue;
				}
				_plugin_contain.push_back(pluginName);
			}
		}
	}

	void Environment::GetAllPluginsFromConfig(std::vector<std::string>& plugins)
	{
		for (unsigned int index = 0; index < _plugin_contain.size(); ++index)
		{
			plugins.push_back(_plugin_contain[index]);
		}
	}

	std::string Environment::GetModuleName() const
	{
		return _module_name;
	}
}
