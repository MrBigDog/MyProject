#pragma once
////#include "stdafx.h"
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_effect_base.h>
#include <usUtil/us_matrix4.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class USGFX_EXPORT d3d9_effect_flow_direction :
		public d3d9_effect_base
	{
	public:
		d3d9_effect_flow_direction( render_device* rd );
		~d3d9_effect_flow_direction(void);

	public:
		bool set_texture( ULONG_PTR ht );
		bool set_view_matrix( const matrix4<double>& matrix );
		bool set_proj_matrix( const matrix4<double>& matrix );
		bool set_world_matrix(const matrix4<double>& matrix );
		bool set_flow_offset(const float offset);
		bool set_flow_uvdistance(const float uvdistance);
		bool set_viewworld_proj(const matrix4<double>& matrix);
	protected:
		virtual bool setup_handle( void );


		D3DXHANDLE _ColorTexture;              // Color texture for mesh
		D3DXHANDLE _matWorld;                 // World matrix for object
		D3DXHANDLE _matView;
		D3DXHANDLE _matProject;
		D3DXHANDLE _UVOffset;
		D3DXHANDLE _UVDistance;               //距离
		D3DXHANDLE _matViewWorldProj; 
	
	private:
		//ID3DXEffect* _waterFX;//水流的特效文件
	};
}


