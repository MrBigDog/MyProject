#ifndef US_D3D9_EFFECT_FLOOD_HEAD
#define US_D3D9_EFFECT_FLOOD_HEAD
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_effect_base.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_matrix4.h>
namespace uniscope_globe
{
	class USGFX_EXPORT d3d9_effect_flood : public d3d9_effect_base
	{
	public:
		d3d9_effect_flood(render_device* rd);
		~d3d9_effect_flood(void);

	public:
		bool set_texture(ULONG_PTR ht);
		bool set_view_matrix(const matrix4<double>& matrix);
		bool set_proj_matrix(const matrix4<double>& matrix);
		bool set_world_matrix(const matrix4<double>& matrix);
		bool set_flood_offset(const float offset);
		bool set_flood_uvdistance(const float uvdistance);
		bool set_viewworld_proj(const matrix4<double>& matrix);
		bool set_upDataTime(const float time);
	protected:
		virtual bool setup_handle(void);


		D3DXHANDLE _ColorTexture;              // Color texture for mesh
		D3DXHANDLE _matWorld;                 // World matrix for object
		D3DXHANDLE _matView;
		D3DXHANDLE _matProject;
		D3DXHANDLE _UVOffset;
		D3DXHANDLE _UVDistance;               //æ‡¿Î
		D3DXHANDLE _upDataTime;
	};
}


#endif //US_D3D9_EFFECT_FLOOD_HEAD
