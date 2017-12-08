#include "us_spherical_pose.h"
#include "us_math.h"
#include "us_plane.h"
#include "us_geocentric_coords.h"
#include "us_spherical_coords.h"
#include "us_cartesian_coords.h"
#include "us_common_file.h"
#include "us_quaternion.h"

namespace uniscope_globe
{
	spherical_pose::spherical_pose(void)
	{
		m_longitude = 0;
		m_latitude = 0;
		m_altitude = 0;
		m_heading = 0;
		m_tilt = 0;
		m_roll = 0;
		m_range = 1;
	}

	spherical_pose::~spherical_pose(void)
	{

	}

	bool spherical_pose::operator != (const spherical_pose& v_pose)
	{
		if (math<double>::fabs_(m_heading - v_pose.m_heading) > math<double>::s_zero_tolerance ||
			math<double>::fabs_(m_tilt - v_pose.m_tilt) > math<double>::s_zero_tolerance ||
			math<double>::fabs_(m_roll - v_pose.m_roll) > math<double>::s_zero_tolerance ||
			m_position_geo != v_pose.m_position_geo
			)
			return true;

		return false;
	}

	spherical_pose::spherical_pose(double lon, double lat, double alt, double heading, double tilt, double roll)
	{
		create_from_sphr(lon, lat, alt, heading, tilt, roll);
	}

	spherical_pose::spherical_pose(vector3<double>& in_pos_geo, vector3<double>& in_dir_geo, vector3<double>& in_up_geo)
	{
		create_from_geo(in_pos_geo, in_dir_geo, in_up_geo);
	}

	void spherical_pose::create_from_sphr(double lon, double lat, double alt, double heading, double tilt, double roll, double range)
	{
		m_longitude = lon;
		m_latitude = lat;
		m_altitude = alt;
		m_heading = heading;
		m_tilt = tilt;
		m_roll = roll;
		m_range = range;

		//
		// position
		//
		m_position_geo = geocentric_coords::from_spherical_d(vector3<double>(lon, lat, alt));
		m_position_sphr.x = lon;
		m_position_sphr.y = lat;
		m_position_sphr.z = alt;
		matrix4<double> tnb_matrix = geocentric_coords::transform_from_cartesian_d(m_position_geo);

		quaternion_4d quater_rotation = quaternion_4d::from_euler(tilt, heading, roll);
		matrix_4d mat_rotate = matrix_4d::from_quaternion(quater_rotation);

		//
		// direction
		//
		m_direction_cart = matrix4<double>::rotate_normal(vector_3d::s_unit_z, mat_rotate);
		m_direction_cart.normalize();

		m_direction_geo = matrix4<double>::rotate_normal(m_direction_cart, tnb_matrix);
		m_direction_geo.normalize();

		////
		//// up
		////
		m_up_cart = matrix4<double>::rotate_normal(vector_3d::s_unit_y, mat_rotate);
		m_up_cart.normalize();
		m_up_geo = matrix4<double>::rotate_normal(m_up_cart, tnb_matrix);
		m_up_geo.normalize();

		// target
		m_target_geo = m_position_geo + m_range * m_direction_geo;
		m_target_sphr = spherical_coords::from_geocentric_d(m_target_geo);
	}

	void spherical_pose::create_from_geo(vector3<double>& in_pos_geo, vector3<double>& in_dir_geo, vector3<double>& in_up_geo, double range)
	{
		m_position_geo = in_pos_geo;
		m_position_sphr = spherical_coords::from_geocentric_d(m_position_geo);
		m_longitude = m_position_sphr.x;
		m_latitude = m_position_sphr.y;
		m_altitude = m_position_sphr.z;

		m_direction_geo = in_dir_geo;
		m_up_geo = in_up_geo;
		m_range = range;
		m_target_geo = m_position_geo + m_range * m_direction_geo;
		m_target_sphr = spherical_coords::from_geocentric_d(m_target_geo);

		matrix4<double> tnb_matrix = cartesian_coords::transform_from_geocentric_d(m_position_geo);

		// m_direction_cart
		m_direction_cart = matrix4<double>::rotate_normal(m_direction_geo, tnb_matrix);
		m_direction_cart.normalize();

		// m_up_cart
		m_up_cart = matrix4<double>::rotate_normal(m_up_geo, tnb_matrix);
		m_up_cart.normalize();

		// m_tilt
		m_tilt = math<double>::acos_(vector3<double>(0, 1, 0).dot(m_direction_cart)) - HALF_PI;

		// m_heading
		plane<double> v_tangent_plane(vector3<double>(0, 1, 0), 0.0);
		vector3<double> v_direction_proj = v_tangent_plane.projection(m_direction_cart);
		v_direction_proj.normalize();
		m_heading = math<double>::acos_(vector3<double>(0, 0, 1).dot(v_direction_proj));
		vector3<double> v_test_cart = vector3<double>(0, 0, 1).cross(v_direction_proj);
		if (vector3<double>(0, 1, 0).dot(v_test_cart) < 0)
		{
			m_heading = DOUBLE_PI - m_heading;
			if (math<double>::fabs_(m_heading - 360) < math<double>::s_zero_tolerance)
			{
				m_heading = 0;
			}
		}

		// m_roll
		vector3<double> v_up_normal_geo = vector3<double>::normalize(m_position_geo);
		plane<double> v_direction_plane(m_direction_geo, 0.0);
		v_up_normal_geo = v_direction_plane.projection(v_up_normal_geo);
		v_up_normal_geo.normalize();
		m_roll = math<double>::acos_(m_up_geo.dot(v_up_normal_geo));
		if (m_up_geo.dot(v_up_normal_geo) < 0)
		{
			m_tilt = -m_tilt;
		}
	}

	void spherical_pose::create_from_cart(vector3<double>& in_pos_geo, vector3<double>& in_dir_cart, vector3<double>& in_up_cart, double range)
	{
		m_position_geo = in_pos_geo;
		m_position_sphr = spherical_coords::from_geocentric_d(in_pos_geo);
		m_longitude = m_position_sphr.x;
		m_latitude = m_position_sphr.y;
		m_altitude = m_position_sphr.z;

		m_direction_cart = in_dir_cart;
		m_up_cart = in_up_cart;
		m_range = range;

		// tnd_matrix
		matrix4<double> tnb_matrix = geocentric_coords::transform_from_cartesian_d(m_position_geo);


		// m_direction_geo
		m_direction_geo = matrix4<double>::rotate_normal(m_direction_cart, tnb_matrix);
		m_direction_geo.normalize();


		// m_up_geo
		m_up_geo = matrix4<double>::rotate_normal(m_up_cart, tnb_matrix);
		m_up_geo.normalize();

		// m_tilt
		m_tilt = math<double>::acos_(vector3<double>(0, 1, 0).dot(m_direction_cart)) - HALF_PI;

		// m_heading
		plane<double> v_tangent_plane(vector3<double>(0, 1, 0), 0.0);
		vector3<double> v_direction_proj = v_tangent_plane.projection(m_direction_cart);
		v_direction_proj.normalize();
		m_heading = math<double>::acos_(vector3<double>(0, 0, 1).dot(v_direction_proj));
		vector3<double> v_test_cart = vector3<double>(0, 0, 1).cross(v_direction_proj);
		if (vector3<double>(0, 1, 0).dot(v_test_cart) < 0)
		{
			m_heading = DOUBLE_PI - m_heading;
			if (math<double>::fabs_(m_heading - 360) < math<double>::s_zero_tolerance)
			{
				m_heading = 0;
			}
		}

		// m_roll
		vector3<double> v_up_normal_geo = vector3<double>::normalize(m_position_geo);
		plane<double> v_direction_plane(m_direction_geo, 0.0);
		v_up_normal_geo = v_direction_plane.projection(v_up_normal_geo);
		v_up_normal_geo.normalize();
		m_roll = math<double>::acos_(m_up_geo.dot(v_up_normal_geo));
		if (m_up_geo.dot(v_up_normal_geo) < 0)
		{
			m_tilt = -m_tilt;
		}

		// target;
		m_target_geo = m_position_geo + m_range * m_direction_geo;
		m_target_sphr = spherical_coords::from_geocentric_d(m_target_geo);
	}

	void spherical_pose::create_from_target_sphr(double target_lon, double target_lat, double target_alt, double heading, double tilt, double roll, double range)
	{
		// m_target_geo
		m_target_geo = geocentric_coords::from_spherical_d(target_lon, target_lat, target_alt);
		m_target_sphr.x = target_lon;
		m_target_sphr.y = target_lat;
		m_target_sphr.z = target_alt;

		// tnd_matrix
		matrix4<double> tnb_matrix = geocentric_coords::transform_from_cartesian_d(m_target_geo);

		m_heading = heading;
		m_tilt = tilt;
		m_roll = roll;
		m_range = range;

		//
		// direction
		//
		quaternion_4d quater_rotation = quaternion_4d::from_euler(tilt, heading, roll);
		matrix_4d mat_rotate = matrix_4d::from_quaternion(quater_rotation);

		//
		// direction
		//
		m_direction_cart = matrix4<double>::rotate_normal(vector_3d::s_unit_z, mat_rotate);
		m_direction_cart.normalize();

		m_direction_geo = matrix4<double>::rotate_normal(m_direction_cart, tnb_matrix);
		m_direction_geo.normalize();

		//
		// position
		//
		m_position_geo = m_target_geo - m_range * m_direction_geo;
		m_position_sphr = spherical_coords::from_geocentric_d(m_position_geo);
		m_longitude = m_position_sphr.x;
		m_latitude = m_position_sphr.y;
		m_altitude = m_position_sphr.z;

		//
		// up
		//
		vector3<double> v_up_normal_cart = vector3<double>::s_unit_y;
		vector3<double> v_right_cart = v_up_normal_cart.cross(m_direction_cart);
		v_right_cart.normalize();
		m_up_cart = m_direction_cart.cross(v_right_cart);
		m_up_cart.normalize();


		matrix4<double> rot_mat = matrix4<double>::rotate_axis_lh(vector3<double>::s_zero, m_direction_cart, -m_roll);
		m_up_cart = m_up_cart * rot_mat;
		m_up_geo = matrix4<double>::rotate_normal(m_up_cart, tnb_matrix);
		m_up_geo.normalize();

	}

	void spherical_pose::create_from_target_geo(vector3<double>& in_target_geo, vector3<double>& in_dir_geo, vector3<double>& in_up_geo, double in_range)
	{
		m_target_geo = in_target_geo;
		m_target_sphr = spherical_coords::from_geocentric_d(in_target_geo);
		m_direction_geo = in_dir_geo;
		m_up_geo = in_up_geo;
		m_range = in_range;

		m_position_geo = m_target_geo - m_direction_geo * m_range;
		m_position_sphr = spherical_coords::from_geocentric_d(m_position_geo);
		m_longitude = m_position_sphr.x;
		m_latitude = m_position_sphr.y;
		m_altitude = m_position_sphr.z;

		// tnb_matrix
		matrix4<double> tnb_matrix = cartesian_coords::transform_from_geocentric_d(m_target_geo);

		// m_direction_cart
		m_direction_cart = matrix4<double>::rotate_normal(m_direction_geo, tnb_matrix);
		m_direction_cart.normalize();

		// m_up_cart
		m_up_cart = matrix4<double>::rotate_normal(m_up_geo, tnb_matrix);
		m_up_cart.normalize();

		// m_tilt
		m_tilt = math<double>::acos_(vector3<double>(0, 1, 0).dot(m_direction_cart)) - HALF_PI;

		// m_heading
		plane<double> v_tangent_plane(vector3<double>(0, 1, 0), 0.0);
		vector3<double> v_direction_proj = v_tangent_plane.projection(m_direction_cart);
		v_direction_proj.normalize();
		m_heading = math<double>::acos_(vector3<double>(0, 0, 1).dot(v_direction_proj));
		vector3<double> v_test_cart = vector3<double>(0, 0, 1).cross(v_direction_proj);
		if (vector3<double>(0, 1, 0).dot(v_test_cart) < 0)
		{
			m_heading = DOUBLE_PI - m_heading;
			if (math<double>::fabs_(m_heading - 360) < math<double>::s_zero_tolerance)
			{
				m_heading = 0;
			}
		}

		// m_roll
		vector3<double> v_up_normal_geo = vector3<double>::normalize(m_position_geo);
		plane<double> v_direction_plane(m_direction_geo, 0.0);
		v_up_normal_geo = v_direction_plane.projection(v_up_normal_geo);
		v_up_normal_geo.normalize();
		m_roll = math<double>::acos_(m_up_geo.dot(v_up_normal_geo));
		if (m_up_geo.dot(v_up_normal_geo) < 0)
		{
			m_tilt = -m_tilt;
		}
	}

	void spherical_pose::create_from_target_cart(vector3<double>& in_target_geo, vector3<double>& in_dir_cart, vector3<double>& in_up_cart, double in_range)
	{
		m_target_geo = in_target_geo;
		m_target_sphr = spherical_coords::from_geocentric_d(in_target_geo);

		m_direction_cart = in_dir_cart;
		m_up_cart = in_up_cart;
		m_range = in_range;

		// tnd_matrix
		matrix4<double> tnb_matrix = geocentric_coords::transform_from_cartesian_d(in_target_geo);

		// m_direction_geo
		m_direction_geo = matrix4<double>::rotate_normal(m_direction_cart, tnb_matrix);
		m_direction_geo.normalize();

		// m_position_geo
		m_position_geo = m_target_geo - m_range * m_direction_geo;
		m_position_sphr = spherical_coords::from_geocentric_d(m_position_geo);
		m_longitude = m_position_sphr.x;
		m_latitude = m_position_sphr.y;
		m_altitude = m_position_sphr.z;

		// m_up_geo
		m_up_geo = matrix4<double>::rotate_normal(m_up_cart, tnb_matrix);
		m_up_geo.normalize();

		// m_tilt
		m_tilt = math<double>::acos_(vector3<double>(0, 1, 0).dot(m_direction_cart)) - HALF_PI;

		// m_heading
		plane<double> v_tangent_plane(vector3<double>(0, 1, 0), 0.0);
		vector3<double> v_direction_proj = v_tangent_plane.projection(m_direction_cart);
		v_direction_proj.normalize();
		m_heading = math<double>::acos_(vector3<double>(0, 0, 1).dot(v_direction_proj));
		vector3<double> v_test_cart = vector3<double>(0, 0, 1).cross(v_direction_proj);
		if (vector3<double>(0, 1, 0).dot(v_test_cart) < 0)
		{
			m_heading = DOUBLE_PI - m_heading;
			if (math<double>::fabs_(m_heading - 360) < math<double>::s_zero_tolerance)
			{
				m_heading = 0;
			}
		}

		// m_roll
		vector3<double> v_up_normal_geo = vector3<double>::normalize(m_position_geo);
		plane<double> v_direction_plane(m_direction_geo, 0.0);
		v_up_normal_geo = v_direction_plane.projection(v_up_normal_geo);
		v_up_normal_geo.normalize();
		m_roll = math<double>::acos_(m_up_geo.dot(v_up_normal_geo));
		if (m_up_geo.dot(v_up_normal_geo) < 0)
		{
			m_tilt = -m_tilt;
		}
	}

	//void spherical_pose::create_from_geo( vector3<double>& in_pos_geo, vector3<double>& in_dir_geo, vector3<double>& in_up_geo, double range )
	//{
	//	m_position_geo = in_pos_geo;
	//	m_position_sphr = spherical_coords::from_geocentric_d( m_position_geo );
	//	m_longitude = m_position_sphr.x;
	//	m_latitude  = m_position_sphr.y;
	//	m_altitude  = m_position_sphr.z;

	//	m_direction_geo = in_dir_geo;
	//	m_up_geo = in_up_geo;
	//	m_range = range;
	//	m_target_geo = m_position_geo + m_range * m_direction_geo;
	//	m_target_sphr = spherical_coords::from_geocentric_d( m_target_geo );

	//	matrix4<double> tnb_matrix = cartesian_coords::transform_from_geocentric_d( m_position_geo );

	//	// m_direction_cart
	//	m_direction_cart = matrix4<double>::rotate_normal( m_direction_geo, tnb_matrix );
	//	m_direction_cart.normalize();

	//	// m_up_cart
	//	m_up_cart = matrix4<double>::rotate_normal( m_up_geo, tnb_matrix );
	//	m_up_cart.normalize();

	//	// m_tilt
	//	m_tilt = math<double>::acos_( vector3<double>(0, 1, 0).dot( -m_direction_cart ) ) - HALF_PI;

	//	// m_heading
	//	plane<double> v_tangent_plane( vector3<double>(0, 1, 0), 0.0 );
	//	vector3<double> v_direction_proj = v_tangent_plane.projection( m_direction_cart );
	//	v_direction_proj.normalize();
	//	m_heading = math<double>::acos_( vector3<double>(0, 0, 1).dot( v_direction_proj ) );
	//	vector3<double> v_test_cart = vector3<double>(0, 0, 1).cross( v_direction_proj );
	//	if( vector3<double>(0, 1, 0).dot(v_test_cart) < 0)
	//	{
	//		m_heading = DOUBLE_PI - m_heading ;
	//		if ( math<double>::fabs_( m_heading - 360 ) < math<double>::s_zero_tolerance )
	//		{
	//			m_heading = 0;
	//		}
	//	}

	//	// m_roll
	//	vector3<double> v_up_normal_geo = vector3<double>::normalize( m_position_geo );
	//	plane<double> v_direction_plane( m_direction_geo, 0.0 );
	//	v_up_normal_geo = v_direction_plane.projection( v_up_normal_geo );
	//	v_up_normal_geo.normalize();
	//	vector3<double> v_test_dir = v_direction_plane.projection( v_up_normal_geo );
	//	m_roll = math<double>::acos_( m_up_geo.dot( v_up_normal_geo  ) );
	//	v_test_cart = m_up_geo.cross( v_up_normal_geo );
	//	if ( v_test_cart.dot( m_direction_geo ) < 0 )
	//	{
	//		m_roll = - m_roll;
	//	}
	//}

	//void spherical_pose::create_from_cart( vector3<double>& in_pos_geo, vector3<double>& in_dir_cart, vector3<double>& in_up_cart, double range )
	//{
	//	m_position_geo		= in_pos_geo;
	//	m_position_sphr     = spherical_coords::from_geocentric_d( in_pos_geo );
	//	m_longitude = m_position_sphr.x;
	//	m_latitude  = m_position_sphr.y;
	//	m_altitude  = m_position_sphr.z;

	//	m_direction_cart	= in_dir_cart;
	//	m_up_cart			= in_up_cart;
	//	m_range				= range;

	//	// tnd_matrix
	//	matrix4<double> tnb_matrix = geocentric_coords::transform_from_cartesian_d( m_position_geo );


	//	// m_direction_geo
	//	m_direction_geo = matrix4<double>::rotate_normal( m_direction_cart, tnb_matrix );
	//	m_direction_geo.normalize();


	//	// m_up_geo
	//	m_up_geo = matrix4<double>::rotate_normal( m_up_cart, tnb_matrix );
	//	m_up_geo.normalize();

	//	// m_tilt
	//	m_tilt = math<double>::acos_( vector3<double>(0, 1, 0).dot( -m_direction_cart ) ) - HALF_PI;

	//	// m_heading
	//	plane<double> v_tangent_plane( vector3<double>(0, 1, 0), 0.0 );
	//	vector3<double> v_direction_proj = v_tangent_plane.projection( m_direction_cart );
	//	v_direction_proj.normalize();
	//	m_heading = math<double>::acos_( vector3<double>(0, 0, 1).dot( v_direction_proj ) );
	//	vector3<double> v_test_cart = vector3<double>(0, 0, 1).cross( v_direction_proj );
	//	if( vector3<double>(0, 1, 0).dot(v_test_cart) < 0)
	//	{
	//		m_heading = DOUBLE_PI - m_heading ;
	//		if ( math<double>::fabs_( m_heading - 360 ) < math<double>::s_zero_tolerance )
	//		{
	//			m_heading = 0;
	//		}
	//	}

	//	// m_roll
	//	vector3<double> v_up_normal_geo = vector3<double>::normalize( m_position_geo );
	//	plane<double> v_direction_plane( m_direction_geo, 0.0 );
	//	v_up_normal_geo = v_direction_plane.projection( v_up_normal_geo );
	//	v_up_normal_geo.normalize();		
	//	m_roll = math<double>::acos_( m_up_geo.dot( v_up_normal_geo  ) );
	//	v_test_cart = m_up_geo.cross( v_up_normal_geo );
	//	if ( v_test_cart.dot( m_direction_geo ) < 0 )
	//	{
	//		m_roll = - m_roll;
	//	}

	//	// target;
	//	m_target_geo = m_position_geo + m_range * m_direction_geo;	
	//	m_target_sphr = spherical_coords::from_geocentric_d( m_target_geo );
	//}

	//void spherical_pose::create_from_target_sphr( double target_lon, double target_lat, double target_alt, double heading, double tilt, double roll, double range )
	//{
	//	// m_target_geo
	//	m_target_geo = geocentric_coords::from_spherical_d( target_lon, target_lat, target_alt );
	//	m_target_sphr.x = target_lon;
	//	m_target_sphr.y = target_lat;
	//	m_target_sphr.z = target_alt;

	//	// tnd_matrix
	//	matrix4<double> tnb_matrix = geocentric_coords::transform_from_cartesian_d( m_target_geo );

	//	m_heading		= heading;
	//	m_tilt			= tilt;
	//	m_roll			= roll;
	//	m_range			= range;

	//	//
	//	// direction
	//	//
	//	m_direction_cart.x = math<double>::sin_( HALF_PI - m_tilt ) * math<double>::sin_( m_heading );
	//	m_direction_cart.y = math<double>::cos_( HALF_PI - m_tilt );
	//	m_direction_cart.z = math<double>::sin_( HALF_PI - m_tilt ) * math<double>::cos_( m_heading ); 
	//	m_direction_cart.normalize();

	//	m_direction_geo = matrix4<double>::rotate_normal( m_direction_cart, tnb_matrix );
	//	m_direction_geo.normalize();

	//	//
	//	// position
	//	//
	//	m_position_geo = m_target_geo - m_range * m_direction_geo;
	//	m_position_sphr = spherical_coords::from_geocentric_d( m_position_geo );
	//	m_longitude = m_position_sphr.x;
	//	m_latitude  = m_position_sphr.y;
	//	m_altitude  = m_position_sphr.z;

	//	//
	//	// up
	//	//
	//	vector3<double> v_up_normal_cart = vector3<double>::s_unit_y;
	//	vector3<double> v_right_cart = v_up_normal_cart.cross( m_direction_cart );
	//	v_right_cart.normalize();
	//	m_up_cart = m_direction_cart.cross( v_right_cart );
	//	m_up_cart.normalize();


	//	matrix4<double> rot_mat = matrix4<double>::rotate_axis_lh( vector3<double>::s_zero, m_direction_cart, -m_roll );
	//	m_up_cart = m_up_cart * rot_mat;
	//	m_up_geo = matrix4<double>::rotate_normal( m_up_cart, tnb_matrix );
	//	m_up_geo.normalize();
	//}

	//void spherical_pose::create_from_target_geo( vector3<double>& in_target_geo, vector3<double>& in_dir_geo, vector3<double>& in_up_geo, double in_range )
	//{
	//	m_target_geo = in_target_geo;
	//	m_target_sphr = spherical_coords::from_geocentric_d( in_target_geo );
	//	m_direction_geo = in_dir_geo;
	//	m_up_geo = in_up_geo;
	//	m_range = in_range;

	//	m_position_geo = m_target_geo - m_direction_geo * m_range;
	//	m_position_sphr = spherical_coords::from_geocentric_d( m_position_geo );
	//	m_longitude = m_position_sphr.x;
	//	m_latitude  = m_position_sphr.y;
	//	m_altitude  = m_position_sphr.z;

	//	// tnb_matrix
	//	matrix4<double> tnb_matrix = cartesian_coords::transform_from_geocentric_d( m_target_geo );

	//	// m_direction_cart
	//	m_direction_cart = matrix4<double>::rotate_normal( m_direction_geo, tnb_matrix );
	//	m_direction_cart.normalize();

	//	// m_up_cart
	//	m_up_cart = matrix4<double>::rotate_normal( m_up_geo, tnb_matrix );
	//	m_up_cart.normalize();

	//	// m_tilt
	//	m_tilt = math<double>::acos_( vector3<double>(0, 1, 0).dot( -m_direction_cart ) )  - HALF_PI;

	//	// m_heading
	//	plane<double> v_tangent_plane( vector3<double>(0, 1, 0), 0.0 );
	//	vector3<double> v_direction_proj = v_tangent_plane.projection( m_direction_cart );
	//	v_direction_proj.normalize();
	//	m_heading = math<double>::acos_( vector3<double>(0, 0, 1).dot( v_direction_proj ) );
	//	vector3<double> v_test_cart = vector3<double>(0, 0, 1).cross( v_direction_proj );
	//	if( vector3<double>(0, 1, 0).dot(v_test_cart) < 0)
	//	{
	//		m_heading = DOUBLE_PI - m_heading ;
	//		if ( math<double>::fabs_( m_heading - 360 ) < math<double>::s_zero_tolerance )
	//		{
	//			m_heading = 0;
	//		}
	//	}

	//	// m_roll
	//	vector3<double> v_up_normal_geo = vector3<double>::normalize( m_target_geo );
	//	plane<double> v_direction_plane( m_direction_geo, 0.0 );
	//	v_up_normal_geo = v_direction_plane.projection( v_up_normal_geo );
	//	v_up_normal_geo.normalize();
	//	m_roll = math<double>::acos_( m_up_geo.dot( v_up_normal_geo  ) );
	//	v_test_cart = m_up_geo.cross( v_up_normal_geo );
	//	if ( v_test_cart.dot( m_direction_geo ) < 0 )
	//	{
	//		m_roll = - m_roll;
	//	}		
	//}

	//void spherical_pose::create_from_target_cart( vector3<double>& in_target_geo, vector3<double>& in_dir_cart, vector3<double>& in_up_cart, double in_range )
	//{
	//	m_target_geo		= in_target_geo;
	//	m_target_sphr = spherical_coords::from_geocentric_d( in_target_geo );

	//	m_direction_cart	= in_dir_cart;
	//	m_up_cart			= in_up_cart;
	//	m_range				= in_range;

	//	// tnd_matrix
	//	matrix4<double> tnb_matrix = geocentric_coords::transform_from_cartesian_d( in_target_geo );

	//	// m_direction_geo
	//	m_direction_geo = matrix4<double>::rotate_normal( m_direction_cart, tnb_matrix );
	//	m_direction_geo.normalize();

	//	// m_position_geo
	//	m_position_geo = m_target_geo - m_range * m_direction_geo;
	//	m_position_sphr = spherical_coords::from_geocentric_d( m_position_geo );
	//	m_longitude = m_position_sphr.x;
	//	m_latitude  = m_position_sphr.y;
	//	m_altitude  = m_position_sphr.z;

	//	// m_up_geo
	//	m_up_geo = matrix4<double>::rotate_normal( m_up_cart, tnb_matrix );
	//	m_up_geo.normalize();

	//	// m_tilt
	//	m_tilt = math<double>::acos_( vector3<double>(0, 1, 0).dot( -m_direction_cart ) ) - HALF_PI;

	//	// m_heading
	//	plane<double> v_tangent_plane( vector3<double>(0, 1, 0), 0.0 );
	//	vector3<double> v_direction_proj = v_tangent_plane.projection( m_direction_cart );
	//	v_direction_proj.normalize();
	//	m_heading = math<double>::acos_( vector3<double>(0, 0, 1).dot( v_direction_proj ) );
	//	vector3<double> v_test_cart = vector3<double>(0, 0, 1).cross( v_direction_proj );
	//	if( vector3<double>(0, 1, 0).dot(v_test_cart) < 0)
	//	{
	//		m_heading = DOUBLE_PI - m_heading ;
	//		if ( math<double>::fabs_( m_heading - 360 ) < math<double>::s_zero_tolerance )
	//		{
	//			m_heading = 0;
	//		}
	//	}

	//	// m_roll
	//	vector3<double> v_up_normal_geo = vector3<double>::normalize( m_target_geo );
	//	plane<double> v_direction_plane( m_direction_geo, 0.0 );
	//	v_up_normal_geo = v_direction_plane.projection( v_up_normal_geo );
	//	v_up_normal_geo.normalize();
	//	//vector3<double> v_test_dir = v_direction_plane.projection( v_up_normal_geo );
	//	m_roll = math<double>::acos_( m_up_geo.dot( v_up_normal_geo  ) );
	//	v_test_cart = m_up_geo.cross( v_up_normal_geo );
	//	if ( v_test_cart.dot( m_direction_geo ) < 0 )
	//	{
	//		m_roll = - m_roll;
	//	}		
	//}
}