
// us_d3d9_effect_weather.h

#ifndef _US_D3D9_EFFECT_WEATHER_H_
#define _US_D3D9_EFFECT_WEATHER_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_effect_weather
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