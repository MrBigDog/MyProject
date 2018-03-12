///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_integrate1.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : integrate1 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "us_integrate1.h"
#include <assert.h>

namespace uniscope_globe
{
	//----------------------------------------------------------------------------
	template <class real>
	real integrate1<real>::romberg_integral (int iOrder, real fA, real fB, function oF, void* pvUserData)
	{
		assert( iOrder > 0 );
		real** aafRom;
		Allocate<real>(iOrder,2,aafRom);

		real fH = fB - fA;

		aafRom[0][0] = ((real)0.5)*fH*(oF(fA,pvUserData)+oF(fB,pvUserData));
		for (int i0=2, iP0=1; i0 <= iOrder; i0++, iP0 *= 2, fH *= (real)0.5)
		{
			// approximations via the trapezoid rule
			real fSum = (real)0.0;
			int i1;
			for (i1 = 1; i1 <= iP0; i1++)
				fSum += oF(fA + fH*(i1-((real)0.5)),pvUserData);

			// Richardson extrapolation
			aafRom[1][0] = ((real)0.5)*(aafRom[0][0] + fH*fSum);
			for (int i2 = 1, iP2 = 4; i2 < i0; i2++, iP2 *= 4)
				aafRom[1][i2] = (iP2*aafRom[1][i2-1] - aafRom[0][i2-1])/(iP2-1);

			for (i1 = 0; i1 < i0; i1++)
				aafRom[0][i1] = aafRom[1][i1];
		}

		real fResult = aafRom[0][iOrder-1];
		Deallocate<real>(aafRom);
		return fResult;
	}
	//----------------------------------------------------------------------------
	template <class real>
	real integrate1<real>::gaussian_quadrature (real fA, real fB, function oF, void* pvUserData)
	{
		// Legendre polynomials
		// P_0(x) = 1
		// P_1(x) = x
		// P_2(x) = (3x^2-1)/2
		// P_3(x) = x(5x^2-3)/2
		// P_4(x) = (35x^4-30x^2+3)/8
		// P_5(x) = x(63x^4-70x^2+15)/8

		// generation of polynomials
		//   d/dx[ (1-x^2) dP_n(x)/dx ] + n(n+1) P_n(x) = 0
		//   P_n(x) = sum_{k=0}^{floor(n/2)} c_k x^{n-2k}
		//     c_k = (-1)^k (2n-2k)! / [ 2^n k! (n-k)! (n-2k)! ]
		//   P_n(x) = ((-1)^n/(2^n n!)) d^n/dx^n[ (1-x^2)^n ]
		//   (n+1)P_{n+1}(x) = (2n+1) x P_n(x) - n P_{n-1}(x)
		//   (1-x^2) dP_n(x)/dx = -n x P_n(x) + n P_{n-1}(x)

		// roots of the Legendre polynomial of specified degree
		const int iDegree = 5;
		const real afRoot[iDegree] =
		{
			(real)-0.9061798459,
			(real)-0.5384693101,
			(real) 0.0,
			(real)+0.5384693101,
			(real)+0.9061798459
		};
		const real afCoeff[iDegree] =
		{
			(real)0.2369268850,
			(real)0.4786286705,
			(real)0.5688888889,
			(real)0.4786286705,
			(real)0.2369268850
		};

		// Need to transform domain [a,b] to [-1,1].  If a <= x <= b and
		// -1 <= t <= 1, then x = ((b-a)*t+(b+a))/2.
		real fRadius = ((real)0.5)*(fB - fA);
		real fCenter = ((real)0.5)*(fB + fA);

		real fResult = (real)0.0;
		for (int i = 0; i < iDegree; i++)
			fResult += afCoeff[i]*oF(fRadius*afRoot[i]+fCenter,pvUserData);
		fResult *= fRadius;

		return fResult;
	}
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	// explicit instantiation
	//----------------------------------------------------------------------------
	// float
	template class integrate1<float>;

	// double
	template class integrate1<double>;
	//----------------------------------------------------------------------------
	
}