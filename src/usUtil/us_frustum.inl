//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_frustum.inl
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _US_FRUSTUM_INL_
#define _US_FRUSTUM_INL_


template < class real >
frustum<real>::frustum(void)
{
	m_planes[0] = &m_near_plane;
	m_planes[1] = &m_left_plane;
	m_planes[2] = &m_right_plane;
	m_planes[3] = &m_up_plane;
	m_planes[4] = &m_down_plane;
	m_planes[5] = &m_far_plane;
}

template < class real >
frustum<real>::frustum( const frustum& f)
{
	memcpy( this, &f, sizeof(f));
}

template < class real >
frustum<real>::~frustum(void)
{

}

template < class real >
void frustum<real>::update(real fov, real aspect, real near_plane_range, real far_plane_range,matrix4<real> &mat )
{
	m_fov       = fov;
	m_aspect    = aspect;
	m_near_plane_range = near_plane_range;
	m_far_plane_range  = far_plane_range;

	vector3<real> v_positive_x  = vector3<real>( m_aspect * m_near_plane_range * (double)math<real>::tan_( m_fov * (real)0.5 ), 
												 (real)0.0, 
												 m_near_plane_range );

	vector3<real> v_positive_y  = vector3<real>((real)0.0, 
												 m_near_plane_range * (double)math<real>::tan_(m_fov * (real)0.5), 
												 m_near_plane_range);


	vector3<real> v_negative_x = v_positive_x * (real)-1.0;
	vector3<real> v_negative_y = v_positive_y * (real)-1.0;
	v_negative_x.z = v_negative_y.z = m_near_plane_range;

	matrix4<real> inv_view_mat;
	inv_view_mat = mat.inverse();

	m_camera_pos    = vector3<real>::s_zero   * inv_view_mat;
	m_help_x_normal	= vector3<real>::s_unit_x * inv_view_mat;
	m_help_y_normal	= vector3<real>::s_unit_y * inv_view_mat;

	m_positive_x = v_positive_x * inv_view_mat;
	m_positive_y = v_positive_y * inv_view_mat;

	m_negative_x = v_negative_x * inv_view_mat;
	m_negative_y = v_negative_y * inv_view_mat;

	m_near_plane_center	= vector3<real>( (real)0.0, (real)0.0, m_near_plane_range) * inv_view_mat;
	m_far_plane_center	= vector3<real>( (real)0.0, (real)0.0, m_far_plane_range) * inv_view_mat;

	m_near_plane = plane<real>(m_near_plane_center, m_positive_x, m_positive_y);

	double constant_value = m_far_plane_center.length();
	m_far_plane = plane<real>(-m_near_plane.m_normal, constant_value);

	m_left_plane = plane<real>(m_camera_pos, m_help_y_normal, m_negative_x);

	m_right_plane = plane<real>(m_camera_pos, m_positive_x, m_help_y_normal);

	m_up_plane = plane<real>(m_camera_pos, m_help_x_normal, m_positive_y);

	m_down_plane = plane<real>(m_camera_pos ,m_negative_y, m_help_x_normal);
}

template < class real >
void frustum<real>::update_ex(real v_width, real v_height, real near_plane_range, real far_plane_range,matrix4<real> &mat )
{
	m_near_plane_range = near_plane_range;
	m_far_plane_range  = far_plane_range;

	vector3<real> v_near_left_up  = vector3<real>(  -v_width * (real)0.5,  v_height * (real)0.5,  m_near_plane_range  );
	vector3<real> v_near_right_up  = vector3<real>(  v_width * (real)0.5,  v_height * (real)0.5,  m_near_plane_range  );
	vector3<real> v_near_left_down  = vector3<real>(  -v_width * (real)0.5,  -v_height * (real)0.5,  m_near_plane_range  );
	vector3<real> v_near_right_down  = vector3<real>(  v_width * (real)0.5,  -v_height * (real)0.5,  m_near_plane_range  );

	vector3<real> v_far_left_up  = vector3<real>(  -v_width * (real)0.5,  v_height * (real)0.5,  m_far_plane_range  );
	vector3<real> v_far_right_up  = vector3<real>(  v_width * (real)0.5,  v_height * (real)0.5,  m_far_plane_range  );
	vector3<real> v_far_left_down  = vector3<real>(  -v_width * (real)0.5,  -v_height * (real)0.5,  m_far_plane_range  );
	vector3<real> v_far_right_down  = vector3<real>(  v_width * (real)0.5,  -v_height * (real)0.5,  m_far_plane_range  );

	matrix4<real> inv_view_mat;
	inv_view_mat = mat.inverse();

	v_near_left_up = v_near_left_up * inv_view_mat;
	v_near_right_up = v_near_right_up * inv_view_mat;
	v_near_left_down = v_near_left_down * inv_view_mat;
	v_near_right_down = v_near_right_down * inv_view_mat;

	v_far_left_up = v_far_left_up * inv_view_mat;
	v_far_right_up = v_far_right_up * inv_view_mat;
	v_far_left_down = v_far_left_down * inv_view_mat;
	v_far_right_down = v_far_right_down * inv_view_mat;

	// 近裁面
	m_near_plane = plane<real>(v_near_right_down, v_near_right_up, v_near_left_up);

	// 远裁面
	m_far_plane = plane<real>( v_far_left_up, v_far_right_up, v_far_right_down );

	// 左裁面
	m_left_plane = plane<real>( v_near_left_down, v_near_left_up, v_far_left_up );

	// 右裁面
	m_right_plane = plane<real>( v_near_right_up, v_near_right_down, v_far_right_down );

	// 上裁面
	m_up_plane = plane<real>( v_near_left_up, v_near_right_up, v_far_right_up );

	// 下裁面
	m_down_plane = plane<real>( v_near_right_down ,v_near_left_down, v_far_left_down );
}

template < class real >
void frustum<real>::update_ex(real v_width, real v_height, real x, real y, real w, real h, real near_plane_range, real far_plane_range,matrix4<real> &mat )
{
	m_near_plane_range = near_plane_range;
	m_far_plane_range  = far_plane_range;

	vector3<real> v_near_left_up		= vector3<real>(  v_width * (real)0.5 * x,			v_height * (real)0.5 * (y + h), m_near_plane_range  );
	vector3<real> v_near_right_up		= vector3<real>(  v_width * (real)0.5 * (x + w),	v_height * (real)0.5 * (y + h), m_near_plane_range  );
	vector3<real> v_near_left_down		= vector3<real>(  v_width * (real)0.5 * x,			v_height * (real)0.5 * y,		m_near_plane_range  );
	vector3<real> v_near_right_down		= vector3<real>(  v_width * (real)0.5 * (x + w),	v_height * (real)0.5 * y,		m_near_plane_range  );

	vector3<real> v_far_left_up			= vector3<real>(  v_width * (real)0.5 * x,			v_height * (real)0.5 * (y + h), m_far_plane_range  );
	vector3<real> v_far_right_up		= vector3<real>(  v_width * (real)0.5 * (x + w),	v_height * (real)0.5 * (y + h), m_far_plane_range  );
	vector3<real> v_far_left_down		= vector3<real>(  v_width * (real)0.5 * x,			v_height * (real)0.5 * y,		m_far_plane_range  );
	vector3<real> v_far_right_down		= vector3<real>(  v_width * (real)0.5 * (x + w),	v_height * (real)0.5 * y,		m_far_plane_range  );

	matrix4<real> inv_view_mat;
	inv_view_mat = mat.inverse();

	v_near_left_up = v_near_left_up * inv_view_mat;
	v_near_right_up = v_near_right_up * inv_view_mat;
	v_near_left_down = v_near_left_down * inv_view_mat;
	v_near_right_down = v_near_right_down * inv_view_mat;

	v_far_left_up = v_far_left_up * inv_view_mat;
	v_far_right_up = v_far_right_up * inv_view_mat;
	v_far_left_down = v_far_left_down * inv_view_mat;
	v_far_right_down = v_far_right_down * inv_view_mat;

	// 近裁面
	m_near_plane = plane<real>(v_near_right_down, v_near_right_up, v_near_left_up);

	// 远裁面
	m_far_plane = plane<real>( v_far_left_up, v_far_right_up, v_far_right_down );

	// 左裁面
	m_left_plane = plane<real>( v_near_left_down, v_near_left_up, v_far_left_up );

	// 右裁面
	m_right_plane = plane<real>( v_near_right_up, v_near_right_down, v_far_right_down );

	// 上裁面
	m_up_plane = plane<real>( v_near_left_up, v_near_right_up, v_far_right_up );

	// 下裁面
	m_down_plane = plane<real>( v_near_right_down ,v_near_left_down, v_far_left_down );
}



template < class real >
bool frustum<real>::contain_point(const vector3<real>& v)
{
	for(int ni = 0; ni < 6; ni++)
	{
		if(	v.x * m_planes[ni]->m_normal.x + 
			v.y * m_planes[ni]->m_normal.y + 
			v.z * m_planes[ni]->m_normal.z + 
			m_planes[ni]->m_constant < 0)
			return false;
	}

	return true;
}

template < class real >
bool frustum<real>::contain( const lon_lat_box& bb )
{
	int iTotalIn = 0;
	for(int ni = 0; ni < 6; ni++)
	{
		int iInCount = 8;
		int iPtIn = 1;
		// TODO: Modify bounding box and only check 2 corners.
		for(int nj = 0; nj < 8; nj++)
		{
			if(	bb.corner_value[nj].x * m_planes[ni]->m_normal.x + 
				bb.corner_value[nj].y * m_planes[ni]->m_normal.y + 
				bb.corner_value[nj].z * m_planes[ni]->m_normal.z + 
				m_planes[ni]->m_constant < 0)
			{
				iPtIn = 0;
				--iInCount;
			}
		}

		if(iInCount == 0)
			return false;

		iTotalIn += iPtIn;
	}

	if(iTotalIn == 6)
		return true;

	return false;
}

template < class real >
int frustum<real>::contain_box( const axis_aligned_box<real>& aabb )
{
	const vector3<real>* corner = aabb.get_corners();

	int iTotalIn = 0;
	for(int ni = 0; ni < 6; ni++)
	{
		int iInCount = 8;
		int iPtIn = 1;
		// TODO: Modify bounding box and only check 2 corners.
		for(int nj = 0; nj < 8; nj++)
		{
			if(	corner[nj].x * m_planes[ni]->m_normal.x + 
				corner[nj].y * m_planes[ni]->m_normal.y + 
				corner[nj].z * m_planes[ni]->m_normal.z + 
				m_planes[ni]->m_constant < 0)
			{
				iPtIn = 0;
				--iInCount;
			}
		}

		if(iInCount == 0)
			return -1;

		iTotalIn += iPtIn;
	}

	return iTotalIn;

	/*
	int cull_side = 0;
	for ( int i = 0 ; i < 6 ; i++ )
	{
		int seek = 0;

		for ( int j = 0 ; j < 8 ; j++ )
		{			
			double tmp = m_planes[i]->m_normal.x * corner[j].x +
						 m_planes[i]->m_normal.y * corner[j].y +
						 m_planes[i]->m_normal.z * corner[j].z +
						 m_planes[i]->m_constant;

			if ( tmp > 0 )
			{
				seek++;
			}
		}

		if ( seek == 0x00 )
		{
			cull_side++;
		}

		if ( seek == 0x08 )
		{
			return -6;
		}
	}

	return cull_side;
//*/
}


template < class real >
bool frustum<real>::intersect( const lon_lat_box& lbox )
{

	for(int ni = 0; ni < 5; ni++)
	{
		bool bInside = false;
		for(int nj = 0; nj < 8; nj++)
			//for(int nj = 0; nj < 4; nj++)
		{
			if( lbox.corner_value[nj].x * m_planes[ni]->m_normal.x + 
				lbox.corner_value[nj].y * m_planes[ni]->m_normal.y + 
				lbox.corner_value[nj].z * m_planes[ni]->m_normal.z + 
				m_planes[ni]->m_constant >= 0)
			{
				bInside = true;
				break;
			}
		}
		if(!bInside)
		{
			return false;
		}
	}

	return true;
}


template < class real >
bool frustum<real>::intersect( const axis_aligned_box<real> &bbox )
{

	const vector3<real> *corners = bbox.get_corners();

	for(int ni = 0; ni < 5; ni++)
	{
		bool bInside = false;
		for(int nj = 0; nj < 8; nj++)
		{
			if( corners[nj].x * m_planes[ni]->m_normal.x + 
				corners[nj].y * m_planes[ni]->m_normal.y + 
				corners[nj].z * m_planes[ni]->m_normal.z + 
				m_planes[ni]->m_constant >= 0)
			{
				bInside = true;
				break;
			}
		}
		if(!bInside)
		{
			return false;
		}
	}

	return true;
}


#endif // _US_FRUSTUM_INL_