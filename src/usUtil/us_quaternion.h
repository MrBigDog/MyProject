//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_quaternion.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 2008-04-15
//  Purpose : quaternion template
//	Reference : http://www.wild-magic.com
//				http://mathworld.wolfram.com/Quaternion.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_QUATERNION_H_
#define _US_QUATERNION_H_

#include <usUtil\Export.h>

namespace uniscope_globe
{
	template <class real>
	class matrix4;

	template<typename real>
	class quaternion
	{
	public:
		union
		{
			struct
			{
				real x, y, z, w;
			};
			real value[4];
		};

		// construct / destruct
	public:
		inline quaternion(real _x = 0.0, real _y = 0.0, real _z = 0.0, real _w = 1.0)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}

		inline quaternion(const quaternion& in_quater)
		{
			x = in_quater.x;
			y = in_quater.y;
			z = in_quater.z;
			w = in_quater.w;
		}

		inline quaternion(float* p)
		{
			x = real(p[0]);
			y = real(p[1]);
			z = real(p[2]);
			w = real(p[3]);
		}

		inline quaternion(double* p)
		{
			x = real(p[0]);
			y = real(p[1]);
			z = real(p[2]);
			w = real(p[3]);
		}

		//inline quaternion( const matrix4<real>& mat )
		//{

		//}

		// vec must be normalized.
		inline quaternion(const vector3<real>& vec, float angle)
		{
			real sin_val = (real)(math<real>::sin_(angle * (real)0.5));
			real cos_val = (real)(math<real>::cos_(angle * (real)0.5));

			x = vec.x * sin_val;
			y = vec.y * sin_val;
			z = vec.z * sin_val;
			w = cos_val;
		}

		inline quaternion(const vector3<real>& vec1, const vector3<real>& vec2)
		{
			vector3<real> bisector = vec1 + vec2;
			bisector.normalize();

			real cos_half_angle = vec1.dot(bisector);
			vector3<real> v_cross;

			w = cos_half_angle;

			if (cos_half_angle != (real)0.0)
			{
				v_cross = vec1.cross(bisector);
				x = v_cross.x;
				y = v_cross.y;
				z = v_cross.z;
			}
			else
			{
				real inv_length;
				if (math<real>::fabs_(vec1.x) >= math<real>::fabs_(vec1.y))
				{
					// V1.x or V1.z is the largest magnitude component
					inv_length = math<real>::inv_sqrt_(vec1.x * vec1.x + vec1.z*vec1.z);
					x = -vec1.z * inv_length;
					y = (real)0.0;
					z = +vec1.x * inv_length;
				}
				else
				{
					// V1.y or V1.z is the largest magnitude component
					inv_length = math<real>::inv_sqrt_(vec1.x * vec1.x + vec1.z*vec1.z);
					x = (real)0.0;
					y = +vec1.z * inv_length;
					z = -vec1.y * inv_length;
				}
			}
		}

		/// Construct a quaternion from an angle/axis
		//inline quaternion( const radian& r, const vector_3d& axis )
		//{
		//}

		//inline quaternion(const vector_3d& xaxis, const vector_3d& yaxis, const vector_3d& zaxis)
		//{
		//}

		//quaternion( void )
		//{
		//	x = y = z = 0.0;
		//	w = 1.0;
		//}

		~quaternion(void) { }

		// operator
	public:
		bool operator == (const quaternion& quat) const
		{
			return !(*this != quat);
		}

		bool operator != (const quaternion& quat) const
		{
			return ((x != quat.x) || (y != quat.y) || (z != quat.z) || (w != quat.w));
		}

		// method
	public:
		inline bool is_identity(void)
		{
			if (x == 0 && y == 0 && z == 0 && w == 1) return true;
			return false;
		}

		inline void normalize(void)
		{
			real len = length();
			if (len > math<real>::s_zero_tolerance)
			{
				real t = (real)1.0 / length();
				x *= t; y *= t; z *= t; w *= t;
			}
			else
			{
				x = (real)0.0;
				y = (real)0.0;
				z = (real)0.0;
				w = (real)0.0;
			}
		}

		inline real length(void) const
		{
			return math<real>::sqrt_(x * x + y * y + z * z + w * w);

		}

		inline real squared_length(void) const
		{
			return (real)(x * x + y * y + z * z + w * w);
		}

		quaternion operator* (const quaternion& q)
		{
			// NOTE:  Multiplication is not generally commutative, so in most
			// cases p*q != q*p.

			quaternion out_q;

			out_q.w = w * q.w - x * q.x - y * q.y - z * q.z;
			out_q.x = w * q.x + x * q.w + z * q.y - y * q.z;
			out_q.y = w * q.y + y * q.w + x * q.z - z * q.x;
			out_q.z = w * q.z + z * q.w + y * q.x - x * q.y;

			return out_q;
		}

	public:
		static quaternion normalize(const quaternion& q)
		{
			quaternion v_quaternion;
			v_quaternion = q;
			v_quaternion.normalize();
			return v_quaternion;
		}

		// conjugate and re-norm
		static quaternion inverse(const quaternion& q)
		{
			real normal = q.length();
			if (normal > (real)0.0)
			{
				real inverse_normal = 1.0 / normal;
				return quaternion(-q.x * inverse_normal,
					-q.y * inverse_normal,
					-q.z * inverse_normal,
					q.w * inverse_normal
				);
			}
			else
			{
				return quaternion(0, 0, 0, 0);
			}
		}

		// spherical linear interpolation between q1 (t == 0) and q2 (t == 1).
		static quaternion slerp(const quaternion& q1, const quaternion& q2, float t)
		{
			//quaternion q;
			//real cos_omega = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
			//if( cos_omega < 0 )
			//{
			//	q = -q2;
			//	cos_omega = -cos_omega;
			//}

			//real k1, k2;
			//if( ( 1 - cos_omega ) < math<real>::s_zero_tolerance  )
			//{
			//	k1 = 1.0 - t;
			//	k2 = t;
			//}
			//else
			//{
			//	real sin_omega =  math<real>::sqrt_( 1.0 - cos_omega * cos_omega );
			//	real omega = math<real>::atan2_( sin_omega, cos_omega );
			//	real one_over_sin_omega = 1.0 / sin_omega;
			//	k1 = math<real>::sin_((1.0 - t ) * omega) * one_over_sin_omega;
			//	k2 = math<real>::sin(t * omega ) * one_over_sin_omega;

			//}
			//
			//q.w = q1.w * k1 + q.w * k2;
			//q.x = q1.x * k1 + q.x * k2;
			//q.y = q1.y * k1 + q.y * k2;
			//q.z = q1.z * k1 + q.z * k2;

			//return q;

			//////////////////////////////////////////////////////////////////////////

			quaternion q;
			real cos_omega = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
			real angle = math<real>::acos_(cos_omega);

			if (math<real>::fabs_(angle) >= math<real>::s_zero_tolerance)
			{
				real sin_angle = math<real>::sin_(angle);
				real sin_angle_inv = ((real)1.0) / sin_angle;
				real coeff0 = math<real>::sin_(((real)1.0 - t) * angle) * sin_angle_inv;
				real coeff1 = math<real>::sin_(t * angle) * sin_angle_inv;

				q.w = q1.w * coeff0 + q2.w * coeff1;
				q.x = q1.x * coeff0 + q2.x * coeff1;
				q.y = q1.y * coeff0 + q2.y * coeff1;
				q.z = q1.z * coeff0 + q2.z * coeff1;
			}
			else
			{
				q = q1;
			}

			return q;
		}

		static quaternion nlerp(void)
		{
			quaternion q;

			return q;
		}

		//static quaternion from_rotate_matrix( const matrix4<real>& mat )
		//{
		//	return quaternion( mat );
		//}

		static quaternion from_yaw_pitch_roll(real yaw, real pitch, real roll)
		{
			real cy = math<real>::cos_(yaw * 0.5);
			real cp = math<real>::cos_(pitch * 0.5);
			real cr = math<real>::cos_(roll * 0.5);
			real sy = math<real>::sin_(yaw * 0.5);
			real sp = math<real>::sin_(pitch * 0.5);
			real sr = math<real>::sin_(roll * 0.5);

			real qw = cy * cp * cr + sy * sp * sr;
			real qx = sy * cp * cr - cy * sp * sr;
			real qy = cy * sp * cr + sy * cp * sr;
			real qz = cy * cp * sr - sy * sp * cr;

			return quaternion(qx, qy, qz, qw);
		}

		static quaternion from_matrix(matrix4<real>& v_mat)
		{
			quaternion<real> quater(real(0.0), real(0.0), real(0.0), real(1.0));

			real fTrace = v_mat.m11 + v_mat.m22 + v_mat.m33;
			real fRoot;

			if (fTrace > (real)0.0)
			{
				// |w| > 1/2, may as well choose w > 1/2

				fRoot = math<real>::sqrt_(fTrace + (real)1.0); // 2w

				quater.w = ((real)0.5) * fRoot;

				fRoot = ((real)0.5) / fRoot;  // 1/(4w)
				quater.x = (v_mat.m23 - v_mat.m32) * fRoot;
				quater.y = (v_mat.m31 - v_mat.m13) * fRoot;
				quater.z = (v_mat.m12 - v_mat.m21) * fRoot;
			}
			else
			{
				// |w| <= 1/2
				int i = 0;
				if (v_mat.m22 > v_mat.m11)
					i = 1;

				if (v_mat.m33 > v_mat.m[i * 4 + i])
					i = 2;

				static int ms_iNext[3] = { 1, 2, 0 };

				int j = ms_iNext[i];
				int k = ms_iNext[j];

				fRoot = math<real>::sqrt_(v_mat.m[i * 4 + i] - v_mat.m[j * 4 + j] - v_mat.m[k * 4 + k] + (real)1.0);

				real* apfQuat[3] = { &quater.x, &quater.y, &quater.z };

				*apfQuat[i] = ((real)0.5) * fRoot;

				fRoot = ((real)0.5) / fRoot;

				quater.w = (v_mat.m[j * 4 + k] - v_mat.m[k * 4 + j]) * fRoot;

				*apfQuat[j] = (v_mat.m[j * 4 + i] + v_mat.m[i * 4 + j]) * fRoot;

				*apfQuat[k] = (v_mat.m[i * 4 + k] + v_mat.m[k * 4 + i]) * fRoot;
			}

			return quater;
		}

		static quaternion from_euler(real x, real y, real z)
		{
			real c1 = math<real>::cos_(y * 0.5);
			real c2 = math<real>::cos_(z * 0.5);
			real c3 = math<real>::cos_(x * 0.5);
			real s1 = math<real>::sin_(y * 0.5);
			real s2 = math<real>::sin_(z * 0.5);
			real s3 = math<real>::sin_(x * 0.5);
			real qw = c1*c2*c3 - s1*s2*s3;
			real qx = s1*s2*c3 + c1*c2*s3;
			real qy = s1*c2*c3 + c1*s2*s3;
			real qz = c1*s2*c3 - s1*c2*s3;

			return quaternion(qx, qy, qz, qw);
		}

		static vector3<real> to_euler(quaternion& q)
		{
			real qw = q.w;
			real qx = q.x;
			real qy = q.y;
			real qz = q.z;

			real qw2 = qw*qw;
			real qx2 = qx*qx;
			real qy2 = qy*qy;
			real qz2 = qz*qz;
			real test = qx*qy + qz*qw;
			if (test > 0.499)
			{
				//y = 360/Math.PI * Math.Atan2(qx,qw);
				//z = 90;
				//x = 0;
				return vector3<real>(0.0f, math<real>::atan2_(qx, qw) * 2, PI * 0.5);
			}
			if (test < -0.499)
			{
				//y = -360/Math.PI * Math.Atan2(qx,qw);
				//z = -90;
				//x = 0;
				return vector3<real>(0.0f, -math<real>::atan2_(qx, qw) * 2, -PI * 0.5);
			}

			double y = math<real>::atan2_(2 * qy * qw - 2 * qx * qz, 1 - 2 * qy2 - 2 * qz2);
			double z = math<real>::asin_(2 * qx * qy + 2 * qz * qw);
			double x = math<real>::atan2_(2 * qx * qw - 2 * qy * qz, 1 - 2 * qx2 - 2 * qz2);
			return vector3<real>(x, y, z);
		}

		static vector3<real> to_euler(real qx, real qy, real qz, real qw)
		{
			real qw2 = qw*qw;
			real qx2 = qx*qx;
			real qy2 = qy*qy;
			real qz2 = qz*qz;
			real test = qx*qy + qz*qw;
			if (test > 0.499)
			{
				//y = 360/Math.PI * Math.Atan2(qx,qw);
				//z = 90;
				//x = 0;
				return vector3<real>(0.0f, math<real>::atan2_(qx, qw) * 2, PI * 0.5);
			}
			if (test < -0.499)
			{
				//y = -360/Math.PI * Math.Atan2(qx,qw);
				//z = -90;
				//x = 0;
				return vector3<real>(0.0f, -math<real>::atan2_(qx, qw) * 2, -PI * 0.5);
			}

			double y = math<real>::atan2_(2 * qy * qw - 2 * qx * qz, 1 - 2 * qy2 - 2 * qz2);
			double z = math<real>::asin_(2 * qx * qy + 2 * qz * qw);
			double x = math<real>::atan2_(2 * qx * qw - 2 * qy * qz, 1 - 2 * qx2 - 2 * qz2);
			return vector3<real>(x, y, z);
		}


		static quaternion from_rotate_axis(const vector3<real>& vec, float radians)
		{
			return quaternion(vec, radians);
		}

		static quaternion from_vector_to_vector(const vector3<real> vec1, const vector3<real> vec2)
		{
			return quaternion(vec1, vec1);
		}
	};

	typedef quaternion<float>  quaternion_4f;
	typedef quaternion<double> quaternion_4d;
}

#endif // _US_QUATERNION_H_
