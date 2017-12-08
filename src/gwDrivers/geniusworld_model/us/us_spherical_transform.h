#ifndef _US_SPHERICAL_TRANSFORM_H_
#define _US_SPHERICAL_TRANSFORM_H_

#include "us_vector3.h"
#include "us_matrix4.h"
#include "us_spherical_pose.h"
#include "us_transform_base.h"
//#include <windef.h>

namespace uniscope_globe
{
	class spherical_transform : public spherical_pose, public transform_base
	{
	public:
		spherical_transform(void);
		spherical_transform(double lon, double lat, double alt, double heading, double tilt, double roll);

		virtual ~spherical_transform(void);

	public:
		virtual matrix_4f get_matrix_f(void);
		virtual matrix_4d get_matrix_d(void);

	public:
		virtual void create_from_cart(vector3<double>& in_pos_geo, vector3<double>& in_dir_cart, vector3<double>& in_up_cart, double range = 1);
		virtual void create_from_sphr(double lon, double lat, double alt, double heading, double tilt, double roll, double range = 1);
		virtual void create_from_geo(vector3<double> pos, vector3<double> dir, vector3<double> up, double range = 1);

		virtual void set_scale(vector3<double>& in_scale);
		vector3<double> get_scale(void) { return m_scale; }
		virtual void set_pivot(vector3<double>& in_pivot);
		vector3<double> get_pivot(void) { return m_pivot; }
		matrix_4f get_transform_no_scale_f(void);
		virtual void update_tnb(void);

	public:
		static matrix_4d matrix_between_spherical_transforms(spherical_transform* src_trans, spherical_transform* des_trans);
		static void spatial_relation_between_spherical_transforms(spherical_transform* src_trans, spherical_transform* des_trans, vector_3d& vec_translate, quaternion_4d& quater_rotate, vector_3d& vec_scale);

	protected:
		virtual matrix_4d compute_tnb_d(void);

	public:
		matrix4<double> m_mat_tnb;
		matrix4<double> m_mat_tnb_no_scale;
		matrix4<double> m_mat_tnb_inverse;
		matrix4<double> m_mat_tnb_no_scale_inverse;
		matrix4<double> m_mat_tnb_without_scale;
		matrix4<double> m_mat_tnb_without_scale_inverse;
		vector3<double> m_scale;
		vector3<double> m_pivot;
	};
}

#endif // _US_SPHERICAL_TRANSFORM_H_