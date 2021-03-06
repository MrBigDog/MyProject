#ifndef _US_OBJECT_BASE_H_
#define _US_OBJECT_BASE_H_

#include "us_common_file.h"

namespace uniscope_globe
{
#define US_RTTI_NONE				-1

#define US_RTTI_DEM_LAYER			101
#define US_RTTI_DOM_LAYER			102
#define US_RTTI_BUILDING_LAYER		103
#define US_RTTI_BLOCK_LAYER			104
#define US_RTTI_BILLBOARD_LAYER		105
#define US_RTTI_MATCH_MODEL_LAYER	106
#define US_RTTI_POI_LAYER			107
#define US_RTTI_VECTOR_LAYER		108
#define US_RTTI_RASTER_VECTOR_LAYER	109
#define US_RTTI_COMMON_VECTOR_LAYER 110
#define US_RTTI_SHADOW_VECTOR_LAYER 111
#define US_RTTI_ANNOTATON_LAYER		112
#define US_RTTI_MODEL_LAYER			113
#define US_RTTI_PIPELINE_LAYER		114
#define US_RTTI_STATIC_LAYER		120


#define US_RTTI_DEM_SQUARE			131
#define US_RTTI_DOM_SQUARE			132
#define US_RTTI_GLOBE_OBSERVER		150

#define US_RTTI_ELEMENT_FOLDER		201
#define US_RTTI_ELEMENT_BOX			202
#define US_RTTI_ELEMENT_CYLINDER	203
#define US_RTTI_ELEMENT_CONE		204
#define US_RTTI_ELEMENT_PRISM		205
#define US_RTTI_ELEMENT_PYRAMID		206
#define US_RTTI_ELEMENT_VOLUME		207
#define US_RTTI_ELEMENT_MODEL_SET   208
#define US_RTTI_ELEMENT_ICON		209
#define US_RTTI_ELEMENT_POLYLINE	210
#define US_RTTI_ELEMENT_POLYGON		211
#define US_RTTI_ELEMENT_POINTS		212
#define US_RTTI_ELEMENT_MESH		213
#define US_RTTI_ELEMENT_BILLBOARD	214
#define US_RTTI_ELEMENT_GROUNDOVERLAY	215
#define US_RTTI_ELEMENT_SPHERE		216
#define US_RTTI_ELEMENT_SIMPLE_BILLBOARD	217
#define US_RTTI_ELEMENT_GROUP		218
#define US_RTTI_ELEMENT_TEXTURED_FACE	219



#define US_RTTI_ELEMENT_PYRAMID2	221
#define US_RTTI_ELEMENT_CONE2	    222
#define US_RTTI_ELEMENT_BOX2		223
#define US_RTTI_ELEMENT_CYLINDER2	224
#define US_RTTI_ELEMENT_PRISM2		225
#define US_RTTI_ELEMENT_VOLUME2		226
#define US_RTTI_ELEMENT_SPHERE2		227

#define US_RTTI_ELEMENT_MODEL2		228
#define US_RTTI_ELEMENT_MODEL_SET2	229
#define US_RTTI_ELEMENT_CHARACTOR2	230
#define US_RTTI_ELEMENT_VERTICAL_FACE2 231
#define US_RTTI_ELEMENT_LINE2		232
#define US_RTTI_ELEMENT_POLYGON2	233
#define US_RTTI_ELEMENT_PARAPET2    234
#define US_RTTI_ELEMENT_PARTICLE	235
#define US_RTTI_ELEMENT_SNOW		236
#define US_RTTI_ELEMENT_RAIN		237


#define US_RTTI_GEOMETRY_POINT		241
#define US_RTTI_GEOMETRY_POINTS		242
	//#define US_RTTI_GEOMETRY_LINE		243
	//#define US_RTTI_GEOMETRY_POLYGON	244
#	define US_RTTI_GEOMETRY_VERTEX		245

#	define US_RTTI_STYLE_BASE			260
#	define US_RTTI_PEN_STYLE			261
#	define US_RTTI_BRUSH_STYLE			262
#	define US_RTTI_ICON_STYLE			263
#	define US_RTTI_FONT_STYLE			264
#	define US_RTTI_SHADOW_STYLE		265
#	define US_RTTI_TEXTURE_FILL_STYLE	266
#	define US_RTTI_TEXTURE_MAP_DESCRIPTION	267
#	define US_RTTI_SIMPLE_LINE_STYLE	268
#	define US_RTTI_SIMPLE_POLYGON_STYLE 269


#	define US_RTTI_TIME_TRIGGER		280
#	define US_RTTI_DISTANCE_TRIGGER	281
#	define US_RTTI_SCRIPT_TRIGGER		282
#	define US_RTTI_REGION_TRIGGER		283

#	define US_RTTI_COMMON_DYNAMIC_MODEL	301
#	define US_RTTI_SKINNED_DYNAMIC_MODEL	302
#	define US_RTTI_DYNAMIC_LABEL			303
#	define US_RTTI_DYNAMIC_BILLBOARD_SET	304
#	define US_RTTI_DYNAMIC_SOUND			305
#	define US_RTTI_DYNAMIC_VOLUME			306
#	define US_RTTI_CHARACTER				307
#	define	US_RTTI_GEOMETRY_TEXTURED_MESH  308
#	define	US_RTTI_GEOMETRY_MESH			309


#	define US_RTTI_VOLUME_BUILDING			350
#	define US_RTTI_MODEL_BUILDING			351
#	define US_RTTI_BUILDING_ROOF_PARAPET	361


#	define US_RTTI_D3D9_HARDWARE_MESH			401
#	define US_RTTI_D3D9_SKINNING_HARDWARE_MESH 402
#	define US_RTTI_D3D9_COMPLEX_HARDWARE_MESH	403
#	define US_RTTI_SURFACE_OBJECT			    404
#	define US_RTTI_D3D9_MESH_SUBSET			405
#	define US_RTTI_GEOMETRY_MESH_SUBSET		406
#	define US_RTTI_SHADOW_VECTOR_REGION_OBJECT 407

#	define US_RTTI_MODEL_OBJECT					420
#	define US_RTTI_MODEL_HARDWARE_MESH				421
#	define US_RTTI_MODEL_LOD_HARDWARE_MESH			422
#	define US_RTTI_MODEL_COMPLEX_HARDWARE_MESH		423

#	define US_RTTI_STATION_BASE	  500
#	define US_RTTI_STATION_PASS      501
#	define US_RTTI_STATION_LOOKAT    502
#	define US_RTTI_STATION_SURROUND  503
#	define US_RTTI_STATION_ROUTE     504
#	define US_RTTI_STATION_STOP      505
#	define US_RTTI_STATION_STAMP     506
#	define US_RTTI_STATION_RANGE     507

#	define US_RTTI_TRACK			  560

#	define US_RTTI_CONTROL_POINT_LENGTH		520
#	define US_RTTI_CONTROL_POINT_POSITION		521
#	define US_RTTI_ELEMENT_360VIEW2 522

	class object_base
	{
	public:
		typedef int rtti;

	protected:
		object_base(void);

	public:
		virtual ~object_base(void);

	public:
		// Name
		virtual void	set_name(cpstr name) { m_name = name; }
		virtual ustring get_name(void) { return m_name; }

		void	set_guid(cpstr str_id) { m_guid_string = str_id; US_STRING_LOWER(m_guid_string) }
		ustring	get_guid(void) { return m_guid_string; }

		rtti	get_rtti(void) { return m_rtti; }

		virtual ulong get_mem_size(void) { return m_mem_size; }
		void set_mem_size(ulong mem_size) { m_mem_size = mem_size; }

	protected:
		ustring m_name;
		ustring m_guid_string;
		rtti	m_rtti;
		ulong   m_mem_size;
	};
}

#endif //_US_OBJECT_BASE_H_

