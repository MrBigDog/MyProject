#ifndef _US_SPHERICAL_POSE_H_
#define _US_SPHERICAL_POSE_H_

#include "us_vector3.h"

namespace uniscope_globe
{
	class spherical_pose
	{
	public:
		spherical_pose(void);
		virtual ~spherical_pose(void);

		bool operator != (const spherical_pose& v_pose);

		// construct from geo-location/heading/tilt/roll
		spherical_pose(double lon, double lat, double alt, double heading, double tilt, double roll);

		// construct form position/direction/up vectors;
		spherical_pose(vector3<double>& in_pos_geo, vector3<double>& in_dir_geo, vector3<double>& in_up_geo);

	public:
		virtual void create_from_sphr(double lon, double lat, double alt, double heading, double tilt, double roll, double range = 1);
		virtual void create_from_geo(vector3<double>& in_pos_geo, vector3<double>& in_dir_geo, vector3<double>& in_up_geo, double range = 1);
		virtual void create_from_cart(vector3<double>& in_pos_geo, vector3<double>& in_dir_cart, vector3<double>& in_up_cart, double range = 1);
		virtual void create_from_target_sphr(double target_lon, double target_lat, double target_alt, double heading, double tilt, double roll, double range);
		virtual void create_from_target_geo(vector3<double>& in_target_geo, vector3<double>& in_dir_geo, vector3<double>& in_up_geo, double range);
		virtual void create_from_target_cart(vector3<double>& in_target_geo, vector3<double>& in_dir_cart, vector3<double>& in_up_cart, double range);

	public:
		double m_longitude;
		double m_latitude;
		double m_altitude;
		double m_heading;
		double m_tilt;
		double m_roll;
		double m_range;

		vector3<double> m_position_geo;
		vector3<double> m_position_sphr;
		vector3<double> m_target_geo;
		vector3<double> m_target_sphr;
		vector3<double> m_direction_geo;
		vector3<double> m_up_geo;
		vector3<double> m_direction_cart;
		vector3<double> m_up_cart;
	};
}

#endif // _US_SPHERICAL_POSE_H_