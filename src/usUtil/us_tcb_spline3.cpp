///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_tcb_spline3.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : tcb_spline3 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_tcb_spline3.h"

namespace uniscope_globe
{
	//----------------------------------------------------------------------------
	template <class real>
	tcb_spline3<real>::tcb_spline3 (int iSegments, real* afTime, vector3<real>* akPoint, real* afTension, real* afContinuity, real* afBias)
		: multiple_curve3<real>(iSegments,afTime)
	{
		// TO DO.  Add 'boundary type' just as in natural splines.
		assert( m_iSegments >= 2 );

		// all four of these arrays have m_iSegments+1 elements
		m_akPoint = akPoint;
		m_afTension = afTension;
		m_afContinuity = afContinuity;
		m_afBias = afBias;

		int nSegments = m_iSegments;
		if (m_iSegments < 2)
		{
			nSegments = 2;
		}
		m_akA = new vector3<real>[nSegments];
		m_akB = new vector3<real>[nSegments];
		m_akC = new vector3<real>[nSegments];
		m_akD = new vector3<real>[nSegments];

		//-只有一个点的情况下，不能走后面的函数，会把内存写乱 mahc
		if (m_iSegments < 2)
		{
			return;
		}

		// For now, treat the first point as if it occurred twice.
		compute_poly(0,0,1,2);

		for (int i = 1; i < m_iSegments-1; i++)
			compute_poly(i-1,i,i+1,i+2);

		// For now, treat the last point as if it occurred twice.
		compute_poly(m_iSegments-2,m_iSegments-1,m_iSegments,m_iSegments);

	}
	//----------------------------------------------------------------------------
	template <class real>
	tcb_spline3<real>::~tcb_spline3 ()
	{
		delete[] m_akPoint;
		delete[] m_afTension;
		delete[] m_afContinuity;
		delete[] m_afBias;
		delete[] m_akA;
		delete[] m_akB;
		delete[] m_akC;
		delete[] m_akD;
	}
	//----------------------------------------------------------------------------
	template <class real>
	const vector3<real>* tcb_spline3<real>::get_points () const
	{
		return m_akPoint;
	}
	//----------------------------------------------------------------------------
	template <class real>
	const real* tcb_spline3<real>::get_tensions () const
	{
		return m_afTension;
	}
	//----------------------------------------------------------------------------
	template <class real>
	const real* tcb_spline3<real>::get_continuities () const
	{
		return m_afContinuity;
	}
	//----------------------------------------------------------------------------
	template <class real>
	const real* tcb_spline3<real>::get_biases () const
	{
		return m_afBias;
	}
	//----------------------------------------------------------------------------
	template <class real>
	void tcb_spline3<real>::compute_poly (int i0, int i1, int i2, int i3)
	{
		vector3<real> kDiff = m_akPoint[i2] - m_akPoint[i1];
		real fDt = m_afTime[i2] - m_afTime[i1];

		// build multipliers at P1
		real fOmt0 = (real)1.0 - m_afTension[i1];
		real fOmc0 = (real)1.0 - m_afContinuity[i1];
		real fOpc0 = (real)1.0 + m_afContinuity[i1];
		real fOmb0 = (real)1.0 - m_afBias[i1];
		real fOpb0 = (real)1.0 + m_afBias[i1];
		real fAdj0 = ((real)2.0)*fDt/(m_afTime[i2]-m_afTime[i0]);
		real fOut0 = ((real)0.5)*fAdj0*fOmt0*fOpc0*fOpb0;
		real fOut1 = ((real)0.5)*fAdj0*fOmt0*fOmc0*fOmb0;

		// build outgoing tangent at P1
		vector3<real> kTOut = fOut1*kDiff + fOut0*(m_akPoint[i1] - m_akPoint[i0]);

		// build multipliers at point P2
		real fOmt1 = (real)1.0 - m_afTension[i2];
		real fOmc1 = (real)1.0 - m_afContinuity[i2];
		real fOpc1 = (real)1.0 + m_afContinuity[i2];
		real fOmb1 = (real)1.0 - m_afBias[i2];
		real fOpb1 = (real)1.0 + m_afBias[i2];
		real fAdj1 = ((real)2.0)*fDt/(m_afTime[i3] - m_afTime[i1]);
		real fIn0 = ((real)0.5)*fAdj1*fOmt1*fOmc1*fOpb1;
		real fIn1 = ((real)0.5)*fAdj1*fOmt1*fOpc1*fOmb1;

		// build incoming tangent at P2
		vector3<real> kTIn = fIn1*(m_akPoint[i3] - m_akPoint[i2]) + fIn0*kDiff;

		m_akA[i1] = m_akPoint[i1];
		m_akB[i1] = kTOut;
		m_akC[i1] = ((real)3.0)*kDiff - ((real)2.0)*kTOut - kTIn;
		m_akD[i1] = ((real)-2.0)*kDiff + kTOut + kTIn;
	}
	//----------------------------------------------------------------------------
	template <class real>
	vector3<real> tcb_spline3<real>::get_position (real fTime) const
	{
		int iKey;
		real fDt;
		get_key_info(fTime,iKey,fDt);

		fDt /= (m_afTime[iKey+1] - m_afTime[iKey]);

		vector3<real> kResult = m_akA[iKey] + fDt*(m_akB[iKey] + fDt*(m_akC[iKey]
		+ fDt*m_akD[iKey]));

 		return kResult;
	}

	template <class real>
	void tcb_spline3<real>::get_key (real fTime, int& riKey, real& rfDt)
		const
	{
		get_key_info(fTime, riKey, rfDt );
	}

	//----------------------------------------------------------------------------
	template <class real>
	vector3<real> tcb_spline3<real>::get_first_derivative (real fTime) const
	{
		int iKey;
		real fDt;
		get_key_info(fTime,iKey,fDt);

		fDt /= (m_afTime[iKey+1] - m_afTime[iKey]);

		vector3<real> kResult = m_akB[iKey] + fDt*(((real)2.0)*m_akC[iKey] +
			((real)3.0)*fDt*m_akD[iKey]);

		return kResult;
	}
	//----------------------------------------------------------------------------
	template <class real>
	vector3<real> tcb_spline3<real>::get_second_derivative (real fTime) const
	{
		int iKey;
		real fDt;
		get_key_info(fTime,iKey,fDt);

		fDt /= (m_afTime[iKey+1] - m_afTime[iKey]);

		vector3<real> kResult = ((real)2.0)*m_akC[iKey] +
			((real)6.0)*fDt*m_akD[iKey];

		return kResult;
	}
	//----------------------------------------------------------------------------
	template <class real>
	vector3<real> tcb_spline3<real>::get_third_derivative (real fTime) const
	{
		int iKey;
		real fDt;
		get_key_info(fTime,iKey,fDt);

		fDt /= (m_afTime[iKey+1] - m_afTime[iKey]);

		vector3<real> kResult = ((real)6.0)*m_akD[iKey];

		return kResult;
	}
	//----------------------------------------------------------------------------
	template <class real>
	real tcb_spline3<real>::get_speed_key (int iKey, real fTime) const
	{
		vector3<real> kVelocity = m_akB[iKey] + fTime*(((real)2.0)*m_akC[iKey] +
			((real)3.0)*fTime*m_akD[iKey]);
		return kVelocity.length();
	}
	//----------------------------------------------------------------------------
	template <class real>
	real tcb_spline3<real>::get_length_key (int iKey, real fT0, real fT1) const
	{
		ThisPlusKey kData(this,iKey);
		return integrate1<real>::romberg_integral(8,fT0,fT1,get_speed_with_data,
			(void*)&kData);
		return 0.0f;
	}
	//----------------------------------------------------------------------------
	template <class real>
	real tcb_spline3<real>::get_variation_key (int iKey, real fT0, real fT1,
		const vector3<real>& rkA, const vector3<real>& rkB) const
	{
		//Polynomial1<real> kXPoly(3);
		//kXPoly[0] = m_akA[iKey].x();
		//kXPoly[1] = m_akB[iKey].x();
		//kXPoly[2] = m_akC[iKey].x();
		//kXPoly[3] = m_akD[iKey].x();

		//Polynomial1<real> kYPoly(3);
		//kYPoly[0] = m_akA[iKey].y();
		//kYPoly[1] = m_akB[iKey].y();
		//kYPoly[2] = m_akC[iKey].y();
		//kYPoly[3] = m_akD[iKey].y();

		//Polynomial1<real> kZPoly(3);
		//kZPoly[0] = m_akA[iKey].z();
		//kZPoly[1] = m_akB[iKey].z();
		//kZPoly[2] = m_akC[iKey].z();
		//kZPoly[3] = m_akD[iKey].z();

		//// construct line segment A + t*B
		//Polynomial1<real> kLx(1), kLy(1), kLz(1);
		//kLx[0] = rkA.x();
		//kLx[1] = rkB.x();
		//kLy[0] = rkA.y();
		//kLy[1] = rkB.y();
		//kLz[0] = rkA.z();
		//kLz[1] = rkB.z();

		//// compute |X(t) - L(t)|^2
		//Polynomial1<real> kDx = kXPoly - kLx;
		//Polynomial1<real> kDy = kYPoly - kLy;
		//Polynomial1<real> kDz = kZPoly - kLz;
		//Polynomial1<real> kNormSqr = kDx*kDx + kDy*kDy + kDz*kDz;

		//// compute indefinite integral of |X(t)-L(t)|^2
		//Polynomial1<real> kIntegral(kNormSqr.GetDegree()+1);
		//kIntegral[0] = (real)0.0;
		//for (int i = 1; i <= kIntegral.GetDegree(); i++)
		//	kIntegral[i] = kNormSqr[i-1]/i;

		//// compute definite Integral(t0,t1,|X(t)-L(t)|^2)
		//real fResult = kIntegral(fT1) - kIntegral(fT0);
		//return fResult;

		return 0.0;
	}
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	// explicit instantiation
	//----------------------------------------------------------------------------
	// float
	template class tcb_spline3<float>;

	// double
	template class tcb_spline3<double>;
	//----------------------------------------------------------------------------
	
}