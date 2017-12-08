//  倾斜摄影 点云类型的数据;
#ifndef GWDRIVERS_GENIUSWORLDDMODELOPTIONS_H
#define GWDRIVERS_GENIUSWORLDDMODELOPTIONS_H 1

#include <osgEarth/Common>
#include <osgEarth/ModelSource>
#include <osgEarth/URI>
#include <osgEarth/ShaderUtils>

using namespace osgEarth;

namespace gwDrivers
{
	class GeniusWorldModelOptions : public ModelSourceOptions // NO EXPORT; header only
	{
	public:
		optional<URI>& url() { return _url; }
		const optional<URI>& url() const { return _url; }

		optional<float>& lodScale() { return _lod_scale; }
		const optional<float>& lodScale() const { return _lod_scale; }

		optional<osg::Vec3>& location() { return _location; }
		const optional<osg::Vec3>& location() const { return _location; }

		optional<osg::Vec3>& orientation() { return _orientation; }
		const optional<osg::Vec3>& orientation() const { return _orientation; }

		optional<ShaderPolicy>& shaderPolicy() { return _shaderPolicy; }
		const optional<ShaderPolicy>& shaderPolicy() const { return _shaderPolicy; }

		optional<float>& loadingPriorityScale() { return _loadingPriorityScale; }
		const optional<float>& loadingPriorityScale() const { return _loadingPriorityScale; }

		optional<float>& loadingPriorityOffset() { return _loadingPriorityOffset; }
		const optional<float>& loadingPriorityOffset() const { return _loadingPriorityOffset; }

		optional<bool>& paged() { return _paged; }
		const optional<bool>& paged() const { return _paged; }

		/**
		If specified, use this node instead try to load from url
		*/
		osg::ref_ptr<osg::Node>& node() { return _node; }
		const osg::ref_ptr<osg::Node>& node() const { return _node; }

	public:
		GeniusWorldModelOptions(const ConfigOptions& options = ConfigOptions())
			: ModelSourceOptions(options),
			_lod_scale(1.0f),
			_shaderPolicy(SHADERPOLICY_GENERATE),
			_loadingPriorityScale(1.0f),
			_loadingPriorityOffset(0.0f),
			_paged(false)
		{
			setDriver("geniusworld");
			fromConfig(_conf);
		}

		virtual ~GeniusWorldModelOptions() { }

	public:
		Config getConfig() const {
			Config conf = ModelSourceOptions::getConfig();
			conf.updateIfSet("url", _url);
			conf.updateIfSet("lod_scale", _lod_scale);
			conf.updateIfSet("location", _location);
			conf.updateIfSet("orientation", _orientation);
			conf.updateIfSet("loading_priority_scale", _loadingPriorityScale);
			conf.updateIfSet("loading_priority_offset", _loadingPriorityOffset);
			conf.updateIfSet("paged", _paged);

			conf.addIfSet("shader_policy", "disable", _shaderPolicy, SHADERPOLICY_DISABLE);
			conf.addIfSet("shader_policy", "inherit", _shaderPolicy, SHADERPOLICY_INHERIT);
			conf.addIfSet("shader_policy", "generate", _shaderPolicy, SHADERPOLICY_GENERATE);

			conf.updateNonSerializable("GeniusWorldModelOptions::Node", _node.get());
			return conf;
		}

	protected:
		void mergeConfig(const Config& conf)
		{
			ModelSourceOptions::mergeConfig(conf);
			fromConfig(conf);
		}

	private:
		void fromConfig(const Config& conf)
		{
			conf.getIfSet("url", _url);
			conf.getIfSet("lod_scale", _lod_scale);
			conf.getIfSet("location", _location);
			conf.getIfSet("orientation", _orientation);
			conf.getIfSet("loading_priority_scale", _loadingPriorityScale);
			conf.getIfSet("loading_priority_offset", _loadingPriorityOffset);
			conf.getIfSet("paged", _paged);

			conf.getIfSet("shader_policy", "disable", _shaderPolicy, SHADERPOLICY_DISABLE);
			conf.getIfSet("shader_policy", "inherit", _shaderPolicy, SHADERPOLICY_INHERIT);
			conf.getIfSet("shader_policy", "generate", _shaderPolicy, SHADERPOLICY_GENERATE);

			_node = conf.getNonSerializable<osg::Node>("GeniusWorldModelOptions::Node");
		}

		optional<URI> _url;
		optional<float> _lod_scale;
		optional<osg::Vec3> _location;
		optional<osg::Vec3> _orientation;
		optional<ShaderPolicy> _shaderPolicy;
		optional<float> _loadingPriorityScale;
		optional<float> _loadingPriorityOffset;
		optional<bool> _paged;
		osg::ref_ptr<osg::Node> _node;
	};

}

#endif // GWDRIVERS_GENIUSWORLDDMODELOPTIONS_H
