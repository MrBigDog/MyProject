///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_software_raster.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : software_raster class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SOFTWARE_RASTER_H_
#define _US_SOFTWARE_RASTER_H_

#include <usGfx/Export.h>
#include <usUtil/us_matrix4.h>

namespace uniscope_globe
{
	struct USGFX_EXPORT query_result
	{
		vector3<float>	m_face_vertex[3];
		float			m_depth;

		void set_default(void)
		{
			memset(m_face_vertex, 0, sizeof(vector3<float>) * 3);
			m_depth = 1.0;
		}
	};

	class USGFX_EXPORT software_raster
	{
	public:
		software_raster(void);

		virtual ~software_raster(void);

	public:
		void set_world_matrix(matrix4<float>& mat);

		void set_view_matrix(matrix4<float>& mat);

		void set_projection_matrix(matrix4<float>& mat);

		void update_matrix(void);

		void begin(void);

		bool raster(vector3<float>* face_vertex1, vector3<float>* face_vertex2, vector3<float>* face_vertex3);

		void end(void);

		void clear(void);

		bool get_query(query_result& in_result);

	private:
		matrix4<float>	m_world_matrix;

		matrix4<float>	m_view_matrix;

		matrix4<float>	m_projection_matrix;

		matrix4<float>	m_all_matrix;

		query_result	m_query_result;

		bool			m_is_quary;
	};
}

#endif // _US_SOFTWARE_RASTER_H_
