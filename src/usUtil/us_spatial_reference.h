///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_spatial_reference.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : spatial_reference class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SPATIAL_REFERENCE_H_
#define _US_SPATIAL_REFERENCE_H_

#include <usUtil\Export.h>
#include <usUtil/us_vector3.h>
namespace uniscope_globe
{
	class uniscope_projection;

	class USUTIL_EXPORT spatial_reference
	{
	public:
		spatial_reference(void);
		virtual ~spatial_reference(void);

	public:
		bool project_to_spherical(vector_3d& vec);
		bool spherical_to_project(vector_3d& vec);

	public:
		bool project_to_spherical(double in_x, double in_y, double* out_lon, double* out_lat);
		bool spherical_to_project(double in_lon, double in_lat, double* out_x, double* out_y);
	public:
		// 		virtual transform_base* get_transform( void );

	protected:
		void clear_project_operator();
		void reset_project_operator();


	protected:

		//////////////////////////////////////////////////////////////////////////
		// 椭球体类型
		// EllipsoidWGS84 = 0,
		// EllipsoidBeijing54,
		// EllipsoidXian80,
		// EllipsoidNational2000,
		//////////////////////////////////////////////////////////////////////////
		long   mEllipsoidType;				// 椭球体类型

		//////////////////////////////////////////////////////////////////////////
		// 投影类型
		// PrjTypeNull = 0,	// 经纬度
		// PrjTypeGauss,
		// PrjTypeGaussCustomized,
		// PrjTypeUTM,
		// PrjTypeUTMCustomized,
		// PrjTypeMercator,
		//////////////////////////////////////////////////////////////////////////
		long   mProjectionType;				// 投影类型

		long   mZoneWidth;					// （高斯投影）分带宽度
		long   mZoneIndex;					// 带号

		double mFalseEast;					// 东向偏移
		double mFalseNorth;					// 北向偏移

		double mReferenceLatitude;			// 基准纬度
		double mCentralMeridian;			// 中央子午线
		double mProjectSurfaceElevation;	// 投影面高程
		double mScaleFactor;				// 

		double mLongitudeOffset;			// 经度偏移
		double mLatitudeOffset;				// 纬度偏移
		double mAltitudeOffset;				// 高程偏移

		double mCoordinateOffsetX;			// X方向偏移
		double mCoordinateOffsetY;			// Y方向偏移
		double mCoordinateOffsetZ;			// Z方向偏移


	protected:

		double mRotateX;
		double mRotateY;
		double mRotateZ;

		double mScaling;

		double mTranslateX;
		double mTranslateY;
		double mTranslateZ;

	protected:

		uniscope_projection* mProjectOperator;
	};
}

#endif // _US_SPATIAL_REFERENCE_H_ 