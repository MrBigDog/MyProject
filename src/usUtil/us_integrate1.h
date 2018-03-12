///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_integrate1.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : integrate1 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_INTEGRATE1_H_
#define _US_INTEGRATE1_H_

namespace uniscope_globe
{
	template <class T>
	void Allocate(int iCols, int iRows, T**& raatArray)
	{
		raatArray = new T*[iRows];
		raatArray[0] = new T[iRows*iCols];
		for (int iRow = 1; iRow < iRows; iRow++)
		{
			raatArray[iRow] = &raatArray[0][iCols*iRow];
		}
	}
	//----------------------------------------------------------------------------
	template <class T>
	void Deallocate(T**& raatArray)
	{
		delete[] raatArray[0];
		delete[] raatArray;
		raatArray = 0;
	}
	//----------------------------------------------------------------------------
	template <class T>
	void Allocate(int iCols, int iRows, int iSlices, T***& raaatArray)
	{
		raaatArray = new T**[iSlices];
		raaatArray[0] = new T*[iSlices*iRows];
		raaatArray[0][0] = new T[iSlices*iRows*iCols];
		for (int iSlice = 0; iSlice < iSlices; iSlice++)
		{
			raaatArray[iSlice] = &raaatArray[0][iRows*iSlice];
			for (int iRow = 0; iRow < iRows; iRow++)
			{
				raaatArray[iSlice][iRow] =
					&raaatArray[0][0][iCols*(iRow + iRows*iSlice)];
			}
		}
	}
	//----------------------------------------------------------------------------
	template <class T>
	void Deallocate(T***& raaatArray)
	{
		delete[] raaatArray[0][0];
		delete[] raaatArray[0];
		delete[] raaatArray;
		raaatArray = 0;
	}
	//----------------------------------------------------------------------------

	template <class real>
	class integrate1
	{
	public:
		// last parameter is for user-defined data
		typedef real(*function)(real, void*);

		// Romberg integration
		static real romberg_integral(int iOrder, real fA, real fB, function oF, void* pvUserData = 0);

		// Gaussian quadrature
		static real gaussian_quadrature(real fA, real fB, function oF, void* pvUserData = 0);
	};

	typedef integrate1<float> integrate1f;
	typedef integrate1<double> integrate1d;
}

#endif // _US_INTEGRATE_H_