#include "us_spherical_transform.h"
#include "us_math.h"
#include "us_spherical_pose.h"
#include "us_cartesian_coords.h"
#include "us_quaternion.h"

namespace uniscope_globe
{
	spherical_transform::spherical_transform(void)
	{
		m_scale.x = 1.0;
		m_scale.y = 1.0;
		m_scale.z = 1.0;

		m_mat_tnb = matrix_4d::s_identity;
		m_mat_tnb_no_scale = matrix_4d::s_identity;

		m_mat_tnb_inverse = matrix_4d::s_identity;
		m_mat_tnb_no_scale_inverse = matrix_4d::s_identity;

		m_mat_tnb_without_scale = matrix_4d::s_identity;
		m_mat_tnb_without_scale_inverse = matrix_4d::s_identity;

		m_pivot = vector3<double>::s_zero;
	}

	spherical_transform::spherical_transform(double lon, double lat, double alt, double heading, double tilt, double roll)
		: spherical_pose(lon, lat, alt, heading, tilt, roll)
	{
		m_scale.x = 1.0;
		m_scale.y = 1.0;
		m_scale.z = 1.0;

		m_mat_tnb = matrix_4d::s_identity;
		m_mat_tnb_no_scale = matrix_4d::s_identity;

		m_mat_tnb_inverse = matrix_4d::s_identity;
		m_mat_tnb_no_scale_inverse = matrix_4d::s_identity;

		m_mat_tnb_without_scale = matrix_4d::s_identity;
		m_mat_tnb_without_scale_inverse = matrix_4d::s_identity;

		m_pivot = vector3<double>::s_zero;

		compute_tnb_d();
	}

	spherical_transform::~spherical_transform(void)
	{

	}

	matrix_4f spherical_transform::get_matrix_f(void)
	{
		matrix4<double> trans_mat = matrix4<double>::translation(-cartesian_coords::s_reference_position_geo.x,
			-cartesian_coords::s_reference_position_geo.y,
			-cartesian_coords::s_reference_position_geo.z);


		trans_mat = m_mat_tnb * trans_mat;


		return matrix4<float>(trans_mat.m);
	}

	matrix_4d spherical_transform::get_matrix_d(void)
	{
		matrix4<double> trans_mat = matrix4<double>::translation(-cartesian_coords::s_reference_position_geo.x,
			-cartesian_coords::s_reference_position_geo.y,
			-cartesian_coords::s_reference_position_geo.z);


		trans_mat = m_mat_tnb * trans_mat;

		return trans_mat;
	}


	void spherical_transform::create_from_cart(vector3<double>& in_pos_geo, vector3<double>& in_dir_cart, vector3<double>& in_up_cart, double range)
	{
		spherical_pose::create_from_cart(in_pos_geo, in_dir_cart, in_up_cart);

		compute_tnb_d();
	}

	void spherical_transform::create_from_sphr(double lon, double lat, double alt, double heading, double tilt, double roll, double range)
	{
		spherical_pose::create_from_sphr(lon, lat, alt, heading, tilt, roll, range);

		compute_tnb_d();
	}

	void spherical_transform::update_tnb(void)
	{
		spherical_pose::create_from_sphr(m_longitude, m_latitude, m_altitude, m_heading, m_tilt, m_roll, m_range);
		compute_tnb_d();
	}

	void spherical_transform::create_from_geo(vector3<double> pos, vector3<double> dir, vector3<double> up, double range)
	{
		spherical_pose::create_from_geo(pos, dir, up, range);

		compute_tnb_d();
	}

	void spherical_transform::set_scale(vector3<double>& in_scale)
	{
		m_scale = in_scale;

		compute_tnb_d();
	}

	void spherical_transform::set_pivot(vector3<double>& in_pivot)
	{
		m_pivot = in_pivot;

		compute_tnb_d();
	}

	matrix_4f spherical_transform::get_transform_no_scale_f(void)
	{
		matrix4<double> trans_mat = matrix4<double>::translation(-cartesian_coords::s_reference_position_geo.x,
			-cartesian_coords::s_reference_position_geo.y,
			-cartesian_coords::s_reference_position_geo.z);


		trans_mat = m_mat_tnb_no_scale * trans_mat;


		return matrix4<float>(trans_mat.m);
	}

	/*matrix_4d spherical_transform::compute_tnb_d( void )
	{
		vector3<double> lat_dir, up_dir, north_dir;

		up_dir = m_up_geo;
		up_dir.normalize();

		north_dir = m_direction_geo;
		north_dir.normalize();

		lat_dir = vector3<double>::cross( up_dir, north_dir );
		lat_dir.normalize();

		matrix4<double> v_rotation_mat = matrix4<double>::from_tnbp( lat_dir, up_dir, north_dir, vector3<double>::s_zero );

		m_mat_tnb = matrix4<double>::translation( -m_pivot.x, -m_pivot.y, -m_pivot.z ) *
					matrix4<double>::scaling( m_scale.x * 0.001, m_scale.y * 0.001, m_scale.z * 0.001 ) *
					v_rotation_mat *
					matrix4<double>::translation( m_pivot.x, m_pivot.y, m_pivot.z ) *
					matrix4<double>::translation( m_position_geo.x, m_position_geo.y, m_position_geo.z );

		m_mat_tnb_inverse = m_mat_tnb.inverse();

		m_mat_tnb_no_scale =	matrix4<double>::translation( -m_pivot.x, -m_pivot.y, -m_pivot.z ) *
								matrix4<double>::scaling( m_scale.x, m_scale.y, m_scale.z ) *
								v_rotation_mat *
								matrix4<double>::translation( m_pivot.x + m_position_geo.x, m_pivot.y + m_position_geo.y, m_pivot.z + m_position_geo.z );

		m_mat_tnb_no_scale_inverse = m_mat_tnb_no_scale.inverse();

		m_mat_tnb_without_scale = matrix4<double>::translation( -m_pivot.x, -m_pivot.y, -m_pivot.z ) *
			matrix4<double>::scaling( 0.001, 0.001, 0.001 ) *
			v_rotation_mat *
			matrix4<double>::translation( m_pivot.x + m_position_geo.x, m_pivot.y + m_position_geo.y, m_pivot.z + m_position_geo.z );

		m_mat_tnb_without_scale_inverse = m_mat_tnb_without_scale.inverse();

		return m_mat_tnb;
	}*/

	matrix_4d spherical_transform::compute_tnb_d(void)
	{
		vector3<double> lat_dir, up_dir, north_dir;

		up_dir = m_up_geo;
		up_dir.normalize();

		north_dir = m_direction_geo;
		north_dir.normalize();

		lat_dir = vector3<double>::cross(up_dir, north_dir);
		lat_dir.normalize();

		matrix4<double> v_rotation_mat = matrix4<double>::from_tnbp(lat_dir, up_dir, north_dir, vector3<double>::s_zero);

		m_mat_tnb = matrix4<double>::translation(-m_pivot.x, -m_pivot.y, -m_pivot.z) *
			matrix4<double>::scaling(m_scale.x, m_scale.y, m_scale.z) *
			v_rotation_mat *
			matrix4<double>::translation(m_pivot.x, m_pivot.y, m_pivot.z) *
			matrix4<double>::scaling(0.001, 0.001, 0.001)*
			matrix4<double>::translation(m_position_geo.x, m_position_geo.y, m_position_geo.z);

		m_mat_tnb_inverse = m_mat_tnb.inverse();

		m_mat_tnb_no_scale = matrix4<double>::translation(-m_pivot.x, -m_pivot.y, -m_pivot.z) *
			matrix4<double>::scaling(m_scale.x, m_scale.y, m_scale.z) *
			v_rotation_mat *
			matrix4<double>::translation(m_pivot.x + m_position_geo.x, m_pivot.y + m_position_geo.y, m_pivot.z + m_position_geo.z);

		m_mat_tnb_no_scale_inverse = m_mat_tnb_no_scale.inverse();

		m_mat_tnb_without_scale = matrix4<double>::translation(-m_pivot.x, -m_pivot.y, -m_pivot.z) *
			//matrix4<double>::scaling( 0.001, 0.001, 0.001 ) * 
			v_rotation_mat *
			matrix4<double>::translation(m_pivot.x, m_pivot.y, m_pivot.z) *
			matrix4<double>::scaling(0.001, 0.001, 0.001)*
			matrix4<double>::translation(m_position_geo.x, m_position_geo.y, m_position_geo.z);

		m_mat_tnb_without_scale_inverse = m_mat_tnb_without_scale.inverse();

		return m_mat_tnb;
	}

	matrix_4d spherical_transform::matrix_between_spherical_transforms(spherical_transform* src_trans, spherical_transform* des_trans)
	{
		// 计算平移，旋转，缩放参数
		vector_3d vec_position(0.0, 0.0, 0.0);
		quaternion_4d quater_rotate(0.0, 0.0, 0.0, 1.0);
		vector_3d vec_scale(1.0, 1.0, 1.0);

		spherical_transform::spatial_relation_between_spherical_transforms(src_trans, des_trans, vec_position, quater_rotate, vec_scale);

		matrix_4d v_mat_tranlate = matrix_4d::translation(vec_position.x, vec_position.y, vec_position.z);
		matrix_4d v_mat_rotation = matrix_4d::from_quaternion(quater_rotate);
		matrix_4d v_mat_scaling = matrix_4d::scaling(vec_scale.x, vec_scale.y, vec_scale.z);

		return v_mat_scaling * v_mat_rotation * v_mat_tranlate;
	}

	void spherical_transform::spatial_relation_between_spherical_transforms(spherical_transform* src_trans, spherical_transform* des_trans,
		vector_3d& vec_translate, quaternion_4d& quater_rotate, vector_3d& vec_scale)
	{
		// 计算平移
		vector_3d vec_position(0.0, 0.0, 0.0);
		vec_translate = vec_position * src_trans->m_mat_tnb * des_trans->m_mat_tnb_inverse;

		// 计算旋转
		matrix4<double> mat1 = src_trans->m_mat_tnb_no_scale;
		mat1.m41 = mat1.m42 = mat1.m43 = 0;

		matrix4<double> mat2 = des_trans->m_mat_tnb_no_scale_inverse;
		mat2.m41 = mat2.m42 = mat2.m43 = 0;

		matrix4<double> des_mat = mat1 * mat2;

		quater_rotate = quaternion_4d::from_matrix(des_mat);

		// 计算缩放
		vec_scale = des_trans->get_scale() / src_trans->get_scale();
	}
}