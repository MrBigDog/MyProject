///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_multiple_curve3.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : multiple_curve3 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MULTIPLE_CURVE3_H_
#define _US_MULTIPLE_CURVE3_H_

#include <usUtil\Export.h>
#include <usUtil\us_curve3.h>

namespace uniscope_globe
{
	template <class real>
	class USUTIL_EXPORT multiple_curve3 : public curve3<real>
	{
	public:
		// Construction and destruction for abstract base class.  MultipleCurve3
		// accepts responsibility for deleting the input array.
		multiple_curve3(int iSegments, real* afTime);
		virtual ~multiple_curve3();

		// member access
		int get_segments() const;
		const real* get_times() const;

		// length-from-time and time-from-length
		virtual real get_length(real fT0, real fT1) const;
		virtual real get_time(real fLength, int iIterations = 32, real fTolerance = (real)1e-06) const;

		// support for subdivision
		virtual real get_variation(real fT0, real fT1, const vector3<real>* pkP0 = 0, const vector3<real>* pkP1 = 0) const;

		virtual void get_key(real fTime, int& riKey, real& rfDt) const {}

	protected:
		using curve3<real>::m_min_t;
		using curve3<real>::m_max_t;

		int m_iSegments;
		real* m_afTime;

		// These quantities are allocated by GetLength when they are needed the
		// first time.  The allocations occur in InitializeLength (called by
		// GetLength), so this member function must be 'const'. In order to
		// allocate the arrays in a 'const' function, they must be declared as
		// 'mutable'.
		mutable real* m_afLength;
		mutable real* m_afAccumLength;

		void get_key_info(real fTime, int& riKey, real& rfDt) const;

		void initialize_length() const;
		virtual real get_speed_key(int iKey, real fTime) const = 0;
		virtual real get_length_key(int iKey, real fT0, real fT1) const = 0;
		virtual real get_variation_key(int iKey, real fT0, real fT1,
			const vector3<real>& rkA, const vector3<real>& rkB) const = 0;

		static real get_speed_with_data(real fTime, void* pvData);
	};

	typedef multiple_curve3<float> multiple_curve3f;
	typedef multiple_curve3<double> multiple_curve3d;
}

#endif // _US_MULTIPLE_CURVE3_H_