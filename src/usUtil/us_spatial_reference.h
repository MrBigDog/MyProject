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
		// ����������
		// EllipsoidWGS84 = 0,
		// EllipsoidBeijing54,
		// EllipsoidXian80,
		// EllipsoidNational2000,
		//////////////////////////////////////////////////////////////////////////
		long   mEllipsoidType;				// ����������

		//////////////////////////////////////////////////////////////////////////
		// ͶӰ����
		// PrjTypeNull = 0,	// ��γ��
		// PrjTypeGauss,
		// PrjTypeGaussCustomized,
		// PrjTypeUTM,
		// PrjTypeUTMCustomized,
		// PrjTypeMercator,
		//////////////////////////////////////////////////////////////////////////
		long   mProjectionType;				// ͶӰ����

		long   mZoneWidth;					// ����˹ͶӰ���ִ����
		long   mZoneIndex;					// ����

		double mFalseEast;					// ����ƫ��
		double mFalseNorth;					// ����ƫ��

		double mReferenceLatitude;			// ��׼γ��
		double mCentralMeridian;			// ����������
		double mProjectSurfaceElevation;	// ͶӰ��߳�
		double mScaleFactor;				// 

		double mLongitudeOffset;			// ����ƫ��
		double mLatitudeOffset;				// γ��ƫ��
		double mAltitudeOffset;				// �߳�ƫ��

		double mCoordinateOffsetX;			// X����ƫ��
		double mCoordinateOffsetY;			// Y����ƫ��
		double mCoordinateOffsetZ;			// Z����ƫ��


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