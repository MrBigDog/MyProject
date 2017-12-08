//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_axis_aligned_box.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//
//////////////////////////////////////////////////////////////////////////

/*
Fast Ray-Box Intersection
by Andrew Woo
from "Graphics Gems", Academic Press, 1990
*/
#define NUMDIM	3
#define RIGHT	0
#define LEFT	1
#define MIDDLE	2

template <class real>
bool axis_aligned_box<real>::HitBoundingBox(real* minB, real* maxB, real* origin, real* dir, real* coord)
{
	bool inside = true;
	int quadrant[NUMDIM];
	register int i;
	int whichPlane;
	real maxT[NUMDIM];
	real candidatePlane[NUMDIM];

	/* Find candidate planes; this loop can be avoided if
	rays cast all from the eye(assume perpsective view) */
	for (i = 0; i < NUMDIM; i++)
		if (origin[i] < minB[i]) {
			quadrant[i] = LEFT;
			candidatePlane[i] = minB[i];
			inside = false;
		}
		else if (origin[i] > maxB[i]) {
			quadrant[i] = RIGHT;
			candidatePlane[i] = maxB[i];
			inside = false;
		}
		else {
			quadrant[i] = MIDDLE;
		}

		/* Ray origin inside bounding box */
		if (inside) {
			coord = origin;
			return true;
		}


		/* Calculate T distances to candidate planes */
		for (i = 0; i < NUMDIM; i++)
			if (quadrant[i] != MIDDLE && math<real>::fabs_((double)dir[i]) > math<real>::s_zero_tolerance)
				maxT[i] = (candidatePlane[i] - origin[i]) / dir[i];
			else
				maxT[i] = -1.;

		/* Get largest of the maxT's for final choice of intersection */
		whichPlane = 0;
		for (i = 1; i < NUMDIM; i++)
			if (maxT[whichPlane] < maxT[i])
				whichPlane = i;

		/* Check final candidate actually inside box */
		if (maxT[whichPlane] < (real)0) return false;
		for (i = 0; i < NUMDIM; i++)
			if (whichPlane != i) {
				coord[i] = origin[i] + maxT[whichPlane] * dir[i];
				if (coord[i] < minB[i] || coord[i] > maxB[i])
					return false;
			}
			else {
				coord[i] = candidatePlane[i];
			}


			return true;				/* ray hits box */
}

template <class real>
axis_aligned_box<real>::axis_aligned_box()
{
	m_min_vector.x = m_min_vector.y = m_min_vector.z = math<real>::s_max_real;
	m_max_vector.x = m_max_vector.y = m_max_vector.z = -math<real>::s_max_real;
	//update_corners();
}

template <class real>
axis_aligned_box<real>::axis_aligned_box(const vector3<real>& min_val, const vector3<real>& max_val)
{
	m_min_vector = vector3<real>(min_val.x, min_val.y, min_val.z);
	m_max_vector = vector3<real>(max_val.x, max_val.y, max_val.z);
	update_corners();
}

template <class real>
axis_aligned_box<real>::axis_aligned_box(vector3<real>* verts, int count)
{
	m_min_vector.x = m_min_vector.y = m_min_vector.z = math<real>::s_max_real;
	m_max_vector.x = m_max_vector.y = m_max_vector.z = -math<real>::s_max_real;

	for (int ni = 0; ni < count; ni++)
	{
		m_min_vector.x = (std::min)(m_min_vector.x, verts[ni].x);
		m_min_vector.y = (std::min)(m_min_vector.y, verts[ni].y);
		m_min_vector.z = (std::min)(m_min_vector.z, verts[ni].z);

		m_max_vector.x = (std::max)(m_max_vector.x, verts[ni].x);
		m_max_vector.y = (std::max)(m_max_vector.y, verts[ni].y);
		m_max_vector.z = (std::max)(m_max_vector.z, verts[ni].z);
	}
	update_corners();
}

template <class real>
axis_aligned_box<real>::~axis_aligned_box()
{

}

template <class real>
void axis_aligned_box<real>::set_extent(const vector3<real>& min_val, const vector3<real>& max_val)
{
	m_min_vector = vector3<real>(min_val.x, min_val.y, min_val.z);
	m_max_vector = vector3<real>(max_val.x, max_val.y, max_val.z);
	update_corners();
}

template <class real>
void axis_aligned_box<real>::translate(vector3<real>& vec)
{
	m_min_vector += vec;
	m_max_vector += vec;
	update_corners();
}

template <class real>
void axis_aligned_box<real>::combine(axis_aligned_box<real>& a_box)
{
	if (a_box.is_valid() == false) return;
	combine(a_box.m_min_vector);
	combine(a_box.m_max_vector);
	update_corners();
}

template <class real>
void axis_aligned_box<real>::combine(const vector3<real>& vertex)
{
	if (vertex.x < m_min_vector.x) { m_min_vector.x = vertex.x; }
	if (vertex.x > m_max_vector.x) { m_max_vector.x = vertex.x; }

	if (vertex.y < m_min_vector.y) { m_min_vector.y = vertex.y; }
	if (vertex.y > m_max_vector.y) { m_max_vector.y = vertex.y; }

	if (vertex.z < m_min_vector.z) { m_min_vector.z = vertex.z; }
	if (vertex.z > m_max_vector.z) { m_max_vector.z = vertex.z; }

	update_corners();
}

template <class real>
real axis_aligned_box<real>::get_max_radius(void) const
{
	return (m_max_vector - m_min_vector).length();
}

template <class real>
vector3<real> axis_aligned_box<real>::center(void) const
{
	vector3<real> v;
	v.x = (m_min_vector.x + m_max_vector.x) * (real)0.5;
	v.y = (m_min_vector.y + m_max_vector.y) * (real)0.5;
	v.z = (m_min_vector.z + m_max_vector.z) * (real)0.5;

	return v;
}

template <class real>
vector3<real> axis_aligned_box<real>::get_min() const
{
	return m_min_vector;
}

template <class real>
vector3<real> axis_aligned_box<real>::get_max() const
{
	return m_max_vector;
}

template <class real>
const vector3<real>* axis_aligned_box<real>::get_corners() const
{
	return m_corners;
}

template <class real>
bool axis_aligned_box<real>::inside(const vector3<real>& vec)
{
	return	vec.x >= m_min_vector.x && vec.x <= m_max_vector.x &&
		vec.y >= m_min_vector.y && vec.y <= m_max_vector.y &&
		vec.z >= m_min_vector.z && vec.z <= m_max_vector.z;
}

template <class real>
bool axis_aligned_box<real>::is_in(axis_aligned_box<real> &bbox)
{
	return	bbox.inside(m_corners[0]) &&
		bbox.inside(m_corners[1]) &&
		bbox.inside(m_corners[2]) &&
		bbox.inside(m_corners[3]) &&
		bbox.inside(m_corners[4]) &&
		bbox.inside(m_corners[5]) &&
		bbox.inside(m_corners[6]) &&
		bbox.inside(m_corners[7]);
}

template <class real>
real axis_aligned_box<real>::volumn(void)
{
	real det_x = m_max_vector.x - m_min_vector.x;
	real det_y = m_max_vector.y - m_min_vector.y;
	real det_z = m_max_vector.z - m_min_vector.z;

	return det_x * det_y * det_z;
}

template <class real>
real axis_aligned_box<real>::width(void)
{
	return m_max_vector.x - m_min_vector.x;
}

template <class real>
real axis_aligned_box<real>::length(void)
{
	return m_max_vector.z - m_min_vector.z;
}

template <class real>
real axis_aligned_box<real>::height(void)
{
	return m_max_vector.y - m_min_vector.y;
}

template <class real>
void axis_aligned_box<real>::make_invalid()
{
	m_min_vector.x = m_min_vector.y = m_min_vector.z = math<real>::s_max_real;
	m_max_vector.x = m_max_vector.y = m_max_vector.z = -math<real>::s_max_real;
	//update_corners();
}

template <class real>
bool axis_aligned_box<real>::is_valid(void)
{
	if (m_min_vector.x > m_max_vector.x ||
		m_min_vector.y > m_max_vector.y ||
		m_min_vector.z > m_max_vector.z)
	{
		return false;
	}

	return true;
}

template <class real>
bool axis_aligned_box<real>::intersect(const ray<real>& a_ray)
{
	vector3<real> hit_point;
	bool val = HitBoundingBox((real*)&m_min_vector, (real*)&m_max_vector, (real*)&a_ray.ray_origin, (real*)&a_ray.ray_direction, (real*)&hit_point);
	return val;
}

template <class real>
bool axis_aligned_box<real>::intersect(const axis_aligned_box<real>& a_box)
{
	//
	// ÅÐÀë
	//
	if (a_box.m_min_vector.x > m_max_vector.x || a_box.m_max_vector.x < m_min_vector.x ||
		a_box.m_min_vector.y > m_max_vector.y || a_box.m_max_vector.y < m_min_vector.y ||
		a_box.m_min_vector.z > m_max_vector.z || a_box.m_max_vector.z < m_min_vector.z
		)
	{
		return false;
	}

	//
	// °üÈÝÅÐ¶Ï
	//
	if (a_box.m_min_vector.x > m_min_vector.x && a_box.m_max_vector.x < m_max_vector.x &&
		a_box.m_min_vector.y > m_min_vector.y && a_box.m_max_vector.y < m_max_vector.y &&
		a_box.m_min_vector.z > m_min_vector.z && a_box.m_max_vector.z < m_max_vector.z &&
		m_min_vector.x > a_box.m_min_vector.x && m_max_vector.x < a_box.m_max_vector.x &&
		m_min_vector.y > a_box.m_min_vector.y && m_max_vector.y < a_box.m_max_vector.y &&
		m_min_vector.z > a_box.m_min_vector.z && m_max_vector.z < a_box.m_max_vector.z
		)
	{
		return false;
	}

	return true;
}

template <class real>
bool axis_aligned_box<real>::intersect(const sphere<real>& a_sphere)
{
	vector3<real> nearest = a_sphere.sphere_center;

	if (nearest.x < m_min_vector.x)
	{
		nearest.x = m_min_vector.x;
	}
	else if (nearest.x > m_max_vector.x)
	{
		nearest.x = m_max_vector.x;
	}


	if (nearest.y < m_min_vector.y)
	{
		nearest.y = m_min_vector.y;
	}
	else if (nearest.y > m_max_vector.y)
	{
		nearest.y = m_max_vector.y;
	}

	if (nearest.z < m_min_vector.z)
	{
		nearest.z = m_min_vector.z;
	}
	else if (nearest.z > m_max_vector.z)
	{
		nearest.z = m_max_vector.z;
	}

	real len_val = (nearest - a_sphere.sphere_center).length();

	if (len_val > a_sphere.sphere_radius)
		return false;// no intersection

	return true;
}



//////////////////////////////////////////////////////////////////////////
/*
   5-----4
  /|    /|
 / |   / |
1-----2  |
|  6--|--7
| /   | /
|/    |/
0-----3
*/
//////////////////////////////////////////////////////////////////////////
template <class real>
void axis_aligned_box<real>::update_corners()
{
	m_corners[0] = m_min_vector;
	m_corners[1].x = m_min_vector.x; m_corners[1].y = m_max_vector.y; m_corners[1].z = m_min_vector.z;
	m_corners[2].x = m_max_vector.x; m_corners[2].y = m_max_vector.y; m_corners[2].z = m_min_vector.z;
	m_corners[3].x = m_max_vector.x; m_corners[3].y = m_min_vector.y; m_corners[3].z = m_min_vector.z;

	m_corners[4] = m_max_vector;
	m_corners[5].x = m_min_vector.x; m_corners[5].y = m_max_vector.y; m_corners[5].z = m_max_vector.z;
	m_corners[6].x = m_min_vector.x; m_corners[6].y = m_min_vector.y; m_corners[6].z = m_max_vector.z;
	m_corners[7].x = m_max_vector.x; m_corners[7].y = m_min_vector.y; m_corners[7].z = m_max_vector.z;
}

template <class real>
void axis_aligned_box<real>::transform(matrix4<real>& trans_mat)
{
	m_min_vector = vector3<real>(math<real>::s_max_real, math<real>::s_max_real, math<real>::s_max_real);
	m_max_vector = vector3<real>(-math<real>::s_max_real, -math<real>::s_max_real, -math<real>::s_max_real);

	for (int i = 0; i < 8; i++)
	{
		m_corners[i] = m_corners[i] * trans_mat;
		m_min_vector.x = (std::min)(m_min_vector.x, m_corners[i].x);
		m_min_vector.y = (std::min)(m_min_vector.y, m_corners[i].y);
		m_min_vector.z = (std::min)(m_min_vector.z, m_corners[i].z);

		m_max_vector.x = (std::max)(m_max_vector.x, m_corners[i].x);
		m_max_vector.y = (std::max)(m_max_vector.y, m_corners[i].y);
		m_max_vector.z = (std::max)(m_max_vector.z, m_corners[i].z);
	}
}

template <class real>
double axis_aligned_box<real>::nearest_distance_to_point(vector3<real>& in_pt)
{
	// calcularate nearest distance
	vector3<real> vec_min = get_min();
	vector3<real> vec_max = get_max();

	vector3<real> v_nearest_pos = in_pt;

	v_nearest_pos.x = (std::max)(vec_min.x, v_nearest_pos.x);
	v_nearest_pos.x = (std::min)(vec_max.x, v_nearest_pos.x);

	v_nearest_pos.y = (std::max)(vec_min.y, v_nearest_pos.y);
	v_nearest_pos.y = (std::min)(vec_max.y, v_nearest_pos.y);

	v_nearest_pos.z = (std::max)(vec_min.z, v_nearest_pos.z);
	v_nearest_pos.z = (std::min)(vec_max.z, v_nearest_pos.z);

	double v_dis = (double)(v_nearest_pos - in_pt).length();

	return v_dis;
}

template <class real>
vector3<real> axis_aligned_box<real>::nearest_point_to_point(vector3<real>& in_pt)
{
	// calcularate nearest distance
	vector3<real> vec_min = get_min();
	vector3<real> vec_max = get_max();

	vector3<real> v_nearest_pos = in_pt;

	v_nearest_pos.x = (std::max)(vec_min.x, v_nearest_pos.x);
	v_nearest_pos.x = (std::min)(vec_max.x, v_nearest_pos.x);

	v_nearest_pos.y = (std::max)(vec_min.y, v_nearest_pos.y);
	v_nearest_pos.y = (std::min)(vec_max.y, v_nearest_pos.y);

	v_nearest_pos.z = (std::max)(vec_min.z, v_nearest_pos.z);
	v_nearest_pos.z = (std::min)(vec_max.z, v_nearest_pos.z);

	return v_nearest_pos;
}

template <class real>
axis_aligned_box<real> axis_aligned_box<real>::transform(axis_aligned_box<real>& v_aabb, matrix4<real>& trans_mat)
{
	axis_aligned_box<real> out_aabb = v_aabb;
	out_aabb.transform(trans_mat);
	return out_aabb;
}

