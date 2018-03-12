///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_multiple_curve3.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : multiple_curve3 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_multiple_curve3.h"

namespace uniscope_globe
{
	//----------------------------------------------------------------------------
	template <class real>
	multiple_curve3<real>::multiple_curve3(int iSegments, real* afTime)
		: curve3<real>(afTime[0], afTime[iSegments])
	{
		m_iSegments = iSegments;
		m_afTime = afTime;
		m_afLength = 0;
		m_afAccumLength = 0;
	}
	//----------------------------------------------------------------------------
	template <class real>
	multiple_curve3<real>::~multiple_curve3()
	{
		delete[] m_afTime;
		delete[] m_afLength;
		delete[] m_afAccumLength;
	}
	//----------------------------------------------------------------------------
	template <class real>
	int multiple_curve3<real>::get_segments() const
	{
		return m_iSegments;
	}
	//----------------------------------------------------------------------------
	template <class real>
	const real* multiple_curve3<real>::get_times() const
	{
		return m_afTime;
	}
	//----------------------------------------------------------------------------
	template <class real>
	void multiple_curve3<real>::get_key_info(real fTime, int& riKey, real& rfDt)
		const
	{
		if (fTime <= m_afTime[0])
		{
			riKey = 0;
			rfDt = (real)0.0;
		}
		else if (fTime >= m_afTime[m_iSegments])
		{
			riKey = m_iSegments - 1;
			rfDt = m_afTime[m_iSegments] - m_afTime[m_iSegments - 1];
		}
		else
		{
			for (int i = 0; i < m_iSegments; i++)
			{
				if (fTime < m_afTime[i + 1])
				{
					riKey = i;
					rfDt = fTime - m_afTime[i];
					break;
				}
			}
		}
	}
	//----------------------------------------------------------------------------
	template <class real>
	real multiple_curve3<real>::get_speed_with_data(real fTime, void* pvData)
	{
		multiple_curve3* pvThis = *(multiple_curve3**)pvData;
		int iKey = *(int*)((char*)pvData + sizeof(pvThis));
		return pvThis->get_speed_key(iKey, fTime);
	}
	//----------------------------------------------------------------------------
	template <class real>
	void multiple_curve3<real>::initialize_length() const
	{
		m_afLength = new real[m_iSegments];
		m_afAccumLength = new real[m_iSegments];

		// arc lengths of the segments
		int iKey;
		for (iKey = 0; iKey < m_iSegments; iKey++)
		{
			m_afLength[iKey] = get_length_key(iKey, (real)0.0,
				m_afTime[iKey + 1] - m_afTime[iKey]);
		}

		// accumulative arc length
		m_afAccumLength[0] = m_afLength[0];
		for (iKey = 1; iKey < m_iSegments; iKey++)
			m_afAccumLength[iKey] = m_afAccumLength[iKey - 1] + m_afLength[iKey];
	}
	//----------------------------------------------------------------------------
	template <class real>
	real multiple_curve3<real>::get_length(real fT0, real fT1) const
	{
		assert(m_min_t <= fT0 && fT0 <= m_max_t);
		assert(m_min_t <= fT1 && fT1 <= m_max_t);
		assert(fT0 <= fT1);

		if (!m_afLength)
			initialize_length();

		int iKey0, iKey1;
		real fDt0, fDt1;
		get_key_info(fT0, iKey0, fDt0);
		get_key_info(fT1, iKey1, fDt1);

		real fLength;
		if (iKey0 < iKey1)
		{
			// accumulate full-segment lengths
			fLength = (real)0.0;
			for (int i = iKey0 + 1; i < iKey1; i++)
				fLength += m_afLength[i];

			// add on partial first segment
			fLength += get_length_key(iKey0, fDt0, m_afTime[iKey0 + 1] - m_afTime[iKey0]);

			// add on partial last segment
			fLength += get_length_key(iKey1, (real)0.0, fDt1);
		}
		else
		{
			fLength = get_length_key(iKey0, fDt0, fDt1);
		}

		return fLength;
	}
	//----------------------------------------------------------------------------
	template <class real>
	real multiple_curve3<real>::get_time(real fLength, int iIterations,
		real fTolerance) const
	{
		if (!m_afLength)
			initialize_length();

		if (fLength <= (real)0.0)
			return m_min_t;

		if (fLength >= m_afAccumLength[m_iSegments - 1])
			return m_max_t;

		int iKey;
		for (iKey = 0; iKey < m_iSegments; iKey++)
		{
			if (fLength < m_afAccumLength[iKey])
				break;
		}
		if (iKey >= m_iSegments)
			return m_afTime[m_iSegments];

		// try Newton's method first for rapid convergence
		real fL0, fL1;
		if (iKey == 0)
		{
			fL0 = fLength;
			fL1 = m_afAccumLength[0];
		}
		else
		{
			fL0 = fLength - m_afAccumLength[iKey - 1];
			fL1 = m_afAccumLength[iKey] - m_afAccumLength[iKey - 1];
		}

		// use Newton's method to invert the arc length integral
		real fDt1 = m_afTime[iKey + 1] - m_afTime[iKey];
		real fDt0 = fDt1*fL0 / fL1;
		for (int i = 0; i < iIterations; i++)
		{
			real fDifference = get_length_key(iKey, (real)0.0, fDt0) - fL0;
			if (math<real>::fabs_(fDifference) <= fTolerance)
				return m_afTime[iKey] + fDt0;

			fDt0 -= fDifference / get_speed_key(iKey, fDt0);
		}

		// Newton's method failed.  If this happens, increase iterations or
		// tolerance or integration accuracy.
		return math<real>::s_max_real;
	}
	//----------------------------------------------------------------------------
	template <class real>
	real multiple_curve3<real>::get_variation(real fT0, real fT1,
		const vector3<real>* pkP0, const vector3<real>* pkP1) const
	{
		assert(m_min_t <= fT0 && fT0 <= m_max_t);
		assert(m_min_t <= fT1 && fT1 <= m_max_t);
		assert(fT0 <= fT1);

		// construct line segment, A + (t-t0)*B
		vector3<real> kP0, kP1;
		if (!pkP0)
		{
			kP0 = get_position(fT0);
			pkP0 = &kP0;
		}
		if (!pkP1)
		{
			kP1 = get_position(fT1);
			pkP1 = &kP1;
		}
		real fInvDT = ((real)1.0) / (fT1 - fT0);
		vector3<real> kA, kB = fInvDT*(*pkP1 - *pkP0);

		int iKey0, iKey1;
		real fDt0, fDt1;
		get_key_info(fT0, iKey0, fDt0);
		get_key_info(fT1, iKey1, fDt1);

		real fVariation;
		if (iKey0 < iKey1)
		{
			// accumulate full-segment variations
			fVariation = (real)0.0;
			for (int i = iKey0 + 1; i < iKey1; i++)
			{
				kA = *pkP0 + (m_afTime[i] - fT0)*kB;
				fVariation += get_variation_key(i, (real)0.0,
					m_afTime[i + 1] - m_afTime[i], kA, kB);
			}

			// add on partial first segment
			kA = *pkP0 + (m_afTime[iKey0] - fT0)*kB;
			fVariation += get_variation_key(iKey0, fDt0,
				m_afTime[iKey0 + 1] - m_afTime[iKey0], kA, kB);

			// add on partial last segment
			kA = *pkP0 + (m_afTime[iKey1] - fT0)*kB;
			fVariation += get_variation_key(iKey1, 0.0f, fDt1, kA, kB);
		}
		else
		{
			kA = *pkP0 + (m_afTime[iKey0] - fT0)*kB;
			fVariation = get_variation_key(iKey0, fDt0, fDt1, kA, kB);
		}

		return fVariation;
	}
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	// explicit instantiation
	//----------------------------------------------------------------------------
	// float
	template class multiple_curve3<float>;

	// double
	template class multiple_curve3<double>;
	//----------------------------------------------------------------------------
}