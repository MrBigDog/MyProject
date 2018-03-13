///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_tcb_spline3.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : tcb_spline3 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TCB_SPLINE3_H_
#define _US_TCB_SPLINE3_H_

#include <usUtil\Export.h>
#include <usUtil\us_multiple_curve3.h>

namespace uniscope_globe
{
	template <class real>
	class tcb_spline3 : public multiple_curve3<real>
	{
	public:
		// Construction and destruction.  tcb_spline3 accepts responsibility for
		// deleting the input arrays.
		tcb_spline3(int iSegments, real* afTime, vector3<real>* akPoint,
			real* afTension, real* afContinuity, real* afBias);

		virtual ~tcb_spline3();

		const vector3<real>* get_points() const;
		const real* get_tensions() const;
		const real* get_continuities() const;
		const real* get_biases() const;

		virtual vector3<real> get_position(real fTime) const;
		virtual vector3<real> get_first_derivative(real fTime) const;
		virtual vector3<real> get_second_derivative(real fTime) const;
		virtual vector3<real> get_third_derivative(real fTime) const;


		virtual void get_key(real fTime, int& riKey, real& rfDt) const;

	protected:
		using multiple_curve3<real>::m_iSegments;
		using multiple_curve3<real>::m_afTime;
		using multiple_curve3<real>::get_speed_with_data;

		void compute_poly(int i0, int i1, int i2, int i3);

		virtual real get_speed_key(int iKey, real fTime) const;
		virtual real get_length_key(int iKey, real fT0, real fT1) const;
		virtual real get_variation_key(int iKey, real fT0, real fT1,
			const vector3<real>& rkA, const vector3<real>& rkB) const;

		vector3<real>* m_akPoint;
		real* m_afTension;
		real* m_afContinuity;
		real* m_afBias;
		vector3<real>* m_akA;
		vector3<real>* m_akB;
		vector3<real>* m_akC;
		vector3<real>* m_akD;

		class ThisPlusKey
		{
		public:
			ThisPlusKey(const tcb_spline3* pkThis, int iKey)
				: This(pkThis), Key(iKey)
			{
			}

			const tcb_spline3* This;
			int Key;
		};
	};

	typedef tcb_spline3<float> tcb_spline3f;
	typedef tcb_spline3<double> tcb_spline3d;
}

#endif // _US_TCB_SPLINE3_H_