
// us_d3d9_effect_weather.h

#ifndef _US_D3D9_EFFECT_WEATHER_H_
#define _US_D3D9_EFFECT_WEATHER_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_effect_base.h>
#include <usUtil/us_matrix4.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class USGFX_EXPORT d3d9_effect_weather
		: public d3d9_effect_base
	{
	protected:
		d3d9_effect_weather( void ){}

	public:
		d3d9_effect_weather(render_device* device, ulong resource_id );		


		virtual ~d3d9_effect_weather( void );

	public:
		bool set_transform( const matrix4<float>& wvp );

		bool set_texture( ULONG_PTR ht );
		
		virtual void set_vertex_declaration( uint decl );

	};
}

#endif // _US_D3D9_EFFECT_WEATHER_H_ 