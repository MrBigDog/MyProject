#ifndef GWPIPELINE_PIPELINEGENERATEPARAMETER_H
#define GWPIPELINE_PIPELINEGENERATEPARAMETER_H 1

#include <osg/Vec4>
#include <string>
#include <map>

// 共用参数;
#define CYCLE_SEGMENT				16
#define CYCLE_SEGMENT_PLUS1			17
#define DOUBLE_CYCLE_SEGMENT_PLUS1	34

#define BLOCK_SEGMENT				4
#define BLOCK_SEGMENT_PLUS1			5
#define DOUBLE_BLOCK_SEGMENT_PLUS1	10

namespace gwPipeline
{
	struct PplMaterialEntry
	{
		osg::Vec4 m_diffuse_color;
		osg::Vec4 m_ambient_color;
		osg::Vec4 m_specular_color;
		osg::Vec4 m_emissive_color;
		float m_power;

		std::string m_name;
		std::string m_diffuse;
		std::string m_self_illuminant;
		std::string m_specular;
		std::string m_bump;
		PplMaterialEntry()
		{
			m_power = 1;
		}
	};

	class pipelineGenerateParameter
	{
	public:
		pipelineGenerateParameter( void );
		pipelineGenerateParameter( const pipelineGenerateParameter& v_param );
		~pipelineGenerateParameter( void );

		typedef std::map<std::string, int> string_int_value_map;
		typedef std::map<std::string, std::string> int_string_value_map;
		typedef std::map<std::string, PplMaterialEntry> ppl_material_entry_map;

	public:
		std::wstring m_data_type;

	public:
		bool m_valid;
		std::wstring m_project_file;

		std::wstring m_input_dir;
		std::wstring m_output_dir;

		long m_pipeline_type;

		std::string m_line_id;
		std::string m_line_start_id;
		std::string m_line_end_id;
		std::string m_line_start_deep;
		std::string m_line_end_deep;
		std::string m_line_size;
		std::string m_line_layout_type;
		std::string m_line_material;
		std::string m_line_flow_direction;

		std::string m_point_id;
		std::string m_point_type;
		std::string m_point_elevation;
		std::string m_point_attach_feature;
		std::string m_well_type;

		string_int_value_map m_point_type_map;
		string_int_value_map m_well_type_map;
		string_int_value_map m_layout_type_map;
		string_int_value_map m_material_type_map;

		int_string_value_map m_match_model_styles;

		PplMaterialEntry m_default_container_material;
		PplMaterialEntry m_default_joint_material;
		PplMaterialEntry m_default_well_material;
		PplMaterialEntry m_default_plate_material;

		ppl_material_entry_map m_ppl_material_entry_map;

		std::string m_terrain_path;
		int m_max_terrain_level;

		// 为局部更新：添加rect限定;
		bool m_defined_rect;
		//lon_lat_rect m_rect;	// 若m_defined_rect为false则无效；
	};
}

#endif // GWPIPELINE_PIPELINEGENERATEPARAMETER_H
