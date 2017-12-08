#ifndef GWAPP_ENVIRONMENT_H
#define GWAPP_ENVIRONMENT_H 1

#include <Compiler.h>
#include <gwApp/Export.h>
#include <osg/Referenced>
#include <vector>

namespace gwApp
{
	class GW_APP_EXPORT Environment : public osg::Referenced
	{
	public:
		Environment(void);
		~Environment(void);
		virtual void InitFilter();

		virtual void GetAllPluginsFromConfig(std::vector<std::string>& plugins);

		virtual std::string GetModuleName() const;

		virtual void SetModuleName(std::string& name) { _module_name = name; }

		virtual void SetScaleBarImagePath(std::string& img) { _scale_bar_img = img; }
		virtual std::string GetScaleBarImagePath() const { return _scale_bar_img; };

		virtual void SetGeoModelPath(std::string& path) { _geo_model_name = path; }
		virtual std::string GetGeoModelPath() const { return _geo_model_name; };

	private:
		std::string _module_name;
		std::string _scale_bar_img;
		std::string _geo_model_name;
		std::vector<std::string> _plugin_contain;
	};
}

#endif // GWAPP_ENVIRONMENT_H
