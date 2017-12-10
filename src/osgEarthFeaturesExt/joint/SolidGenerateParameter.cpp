#include <mdPipeline/PipelineDeclare.h>
#include <mdPipeline/PipelineGenerateParameter.h>

namespace mdPipeline
{
	pipelineGenerateParameter::pipelineGenerateParameter( void )
	{
		m_valid = false;
		m_pipeline_type = PIPELINE_FEEDWATER;
		m_max_terrain_level = 0;

		m_defined_rect = false;
	}

	pipelineGenerateParameter::pipelineGenerateParameter( const pipelineGenerateParameter& v_param )
	{
		m_valid = v_param.m_valid;
		m_project_file = v_param.m_project_file;

		m_input_dir = v_param.m_input_dir;
		m_output_dir = v_param.m_output_dir;

		m_pipeline_type = v_param.m_pipeline_type;

		//m_line_files = v_param.m_line_files;
		//m_point_files = v_param.m_point_files;

		m_line_id = v_param.m_line_id;
		m_line_start_id = v_param.m_line_start_id;
		m_line_end_id = v_param.m_line_end_id;
		m_line_start_deep = v_param.m_line_start_deep;
		m_line_end_deep = v_param.m_line_end_deep;
		m_line_size = v_param.m_line_size;
		m_line_layout_type = v_param.m_line_layout_type;
		m_line_material = v_param.m_line_material;
		m_line_flow_direction = v_param.m_line_flow_direction;

		m_point_id = v_param.m_point_id;
		m_point_type = v_param.m_point_type;
		m_point_elevation = v_param.m_point_elevation;
		m_point_attach_feature = v_param.m_point_attach_feature;
		m_well_type = v_param.m_well_type;

		m_layout_type_map = v_param.m_layout_type_map;
		m_material_type_map = v_param.m_material_type_map;

		m_point_type_map = v_param.m_point_type_map;
		m_well_type_map = v_param.m_well_type_map;

		m_match_model_styles = v_param.m_match_model_styles;

		m_default_container_material = v_param.m_default_container_material;
		m_default_joint_material = v_param.m_default_joint_material;
		m_default_well_material = v_param.m_default_well_material;
		m_default_plate_material = v_param.m_default_plate_material;
		m_ppl_material_entry_map = v_param.m_ppl_material_entry_map;

		m_terrain_path = v_param.m_terrain_path;
		m_max_terrain_level = v_param.m_max_terrain_level;


		//todo;
		//m_geography_parameter = v_param.m_geography_parameter;

		m_defined_rect = v_param.m_defined_rect;
		//m_rect = v_param.m_rect;
	}

	pipelineGenerateParameter::~pipelineGenerateParameter( void )
	{
	}
}
