//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_plane.inl
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_PLANE_INL_
#define _US_PLANE_INL_

template <class real>
plane<real>::plane (void)
{
	// uninitialized
}

template <class real>
plane<real>::~plane (void)
{
	// uninitialized
}

template <class real>
plane<real>::plane (const plane& a_plane)
:m_normal(a_plane.m_normal)
{
	m_constant = a_plane.m_constant;
}

template<class real>
plane<real>::plane( float* p )
{
	m_normal.x = p[0];
	m_normal.y = p[1];
	m_normal.z = p[2];
	m_constant = p[3];
}

template<class real>
plane<real>::plane( double* p )
{
	m_normal.x = p[0];
	m_normal.y = p[1];
	m_normal.z = p[2];
	m_constant = p[3];
}

template <class real>
plane<real>::plane (const vector3<real>& nm, real constant)
:m_normal(nm)
{
	m_constant = constant;
}

template <class real>
plane<real>::plane (const vector3<real>& nm, const vector3<real>& pt)
:m_normal(nm)
{
	m_constant = -vector3<real>::dot( nm, pt);
}

template <class real>
plane<real>::plane (const vector3<real>& p0, 
					const vector3<real>& p1,
					const vector3<real>& p2)
{
	vector3<real> edge1 = p1 - p0;
	vector3<real> edge2 = p2 - p0;
	edge1.normalize();
	edge2.normalize();
	m_normal = vector3<real>::cross( edge1, edge2 );
	m_normal.normalize();
	m_constant = -m_normal.dot(p0);
}

template <class real>
void plane<real>::create(const vector3<real>& normal, real constant)
{
	m_normal = normal;
	m_constant = constant;
}

template <class real>
void plane<real>::create(const vector3<real>& normal, const vector3<real>& pt)
{
	m_normal = normal;
	m_constant = -vector3<real>::dot( normal, pt );
}

template <class real>
void plane<real>::create(const vector3<real>& p0, const vector3<real>& p1,const vector3<real>& p2)
{
	vector3<real> edge1 = p1 - p0;
	vector3<real> edge2 = p2 - p0;
	edge1.normalize();
	edge2.normalize();
	m_normal = vector3<real>::cross( edge1, edge2 );
	m_normal.normalize();
	m_constant = -m_normal.dot(p0);
}

template <class real>
plane<real>& plane<real>::operator= (const plane& a_plane)
{
	m_normal = a_plane.m_normal;
	m_constant = a_plane.m_constant;
	return *this;
}

template <class real>
real plane<real>::distance_to (const vector3<real>& pt) const
{
	return vector3<real>::dot( m_normal, pt ) + m_constant;
}

template <class real>
int plane<real>::which_side (const vector3<real>& q) const
{
	real dist = distance_to(q);

	if ( dist < (real)0.0 )
		return -1;

	if ( dist > (real)0.0 )
		return +1;

	return 0;
}

template <class real>
vector3<real> plane<real>::projection( const vector3<real>& in_vec )
{
	vector3<real> normal( m_normal.x, m_normal.y, m_normal.z );
	return in_vec - vector3<real>::dot( in_vec, normal ) * normal;
}

template <class real>
vector3<real> plane<real>::point_projection( const vector3<real>& in_pt )
{
	real v_dist = distance_to(in_pt);
	return in_pt - m_normal * v_dist;
}

template <class real>
int	plane<real>::calcu_side( const vector3<real>& vec ) const
{
	double v_dist = distance_to_point( vec );

	if ( v_dist < 0.0 )
		return -1;

	if ( v_dist > 0.0 )
		return +1;

	return 0;
}

template <class real>
double plane<real>::distance_to_point( const vector3<real>& vec ) const
{
	return  vector3<real>::dot( m_normal, vec ) + m_constant;
}

template <class real>
real plane<real>::dot_normal(const vector3<real>& vec) const
{
	return  vector3<real>::dot( m_normal, vec );
}

template <class real>
real plane<real>::dot_coord(const vector3<real>& vec) const
{
	return  vector3<real>::dot( m_normal, vec ) + m_constant;
}

template <class real>
bool plane<real>::intersect( const ray_3d& v_ray, vector3<real>& ret_vec )
{
	const vector3<real>& v_org = v_ray.ray_origin;
	const vector3<real>& v_dir = v_ray.ray_direction;

	real temp = 0.0f;
	real dot_value = vector3<real>::dot( v_dir, m_normal );

	if ( math<real>::fabs_( dot_value ) < math<real>::s_min_real )
	{
		if ( v_org.x * m_normal.x * v_org.y * m_normal.y + v_org.z * m_normal.z + m_constant < math<real>::s_min_real )
		{
			ret_vec = v_org;
			return true;
		} 
		else 
		{
			return false;
		}			
	}

	temp = -( m_normal.x * v_org.x + m_normal.y * v_org.y + m_normal.z * v_org.z + m_constant );
	temp = temp / dot_value;

	ret_vec = v_org + v_dir * temp;

	return true;
}

template <class real>
bool plane<real>::intersect( const vector3<real>& v_bgein, const vector3<real>& v_end, vector3<real>& ret_vec )
{
	if(calcu_side(v_bgein) == calcu_side(v_end))  return false;
	const vector3<real>& v_org = v_bgein;
	vector3<real> v_dir = v_end - v_bgein;
	v_dir.normalize();

	real temp = 0.0f;
	real dot_value = vector3<real>::dot( v_dir, m_normal );

	if ( math<real>::fabs_( dot_value ) < math<real>::s_min_real )
	{
		if ( v_org.x * m_normal.x * v_org.y * m_normal.y + v_org.z * m_normal.z + m_constant < math<real>::s_min_real )
		{
			ret_vec = v_org;
			return true;
		} 
		else 
		{
			return false;
		}			
	}

	temp = -( m_normal.x * v_org.x + m_normal.y * v_org.y + m_normal.z * v_org.z + m_constant );
	temp = temp / dot_value;

	ret_vec = v_org + v_dir * temp;

	return true;
}

template <class real>
void plane<real>::transform( matrix4<real>& trans_mat )
{
	vector4<real> out_val = vector4<real>(m_normal.x, m_normal.y, m_normal.z, m_constant);

	out_val =  out_val * trans_mat;

	m_normal.x = out_val.x;
	m_normal.y = out_val.y;
	m_normal.z = out_val.z;
	m_constant = out_val.w;

	//m_constant = -vector3<real>::dot( normal, pt );
}


template <class real>
plane<real> plane<real>::transform( plane<real>& v_plane,  matrix4<real>& trans_mat )
{
	plane<real> out_plane = v_plane;
	out_plane.transform(trans_mat);
	return out_plane;
}


#endif //_US_PLANE_INL_