///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_spatial_reference.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : spatial_reference class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_spatial_reference.h"
//#include <usUtil/us_vector3.h>
#include <usUtil/us_projection.h>
#include <usUtil/us_gauss_projection.h>
#include <usUtil/us_utm_projection.h>
#include <usUtil/us_mercator_projection.h>

namespace uniscope_globe
{
	spatial_reference::spatial_reference(void)
	{
		mProjectOperator = NULL;


		mEllipsoidType = 0;
		mProjectionType = 5;

		mZoneWidth = 6;
		mZoneIndex = 21;

		mCentralMeridian = 0.0;
		mReferenceLatitude = 0.0;

		mFalseEast = 500000.0;
		mFalseNorth = 0.0;
		mProjectSurfaceElevation = 0.0;
		mScaleFactor = 1.0;

		mLongitudeOffset = 0.0;
		mLatitudeOffset = 0.0;
		mAltitudeOffset = 0.0;

		mCoordinateOffsetX = 0.0;
		mCoordinateOffsetY = 0.0;
		mCoordinateOffsetZ = 0.0;

		mTranslateX = 0.0;
		mTranslateY = 0.0;
		mTranslateZ = 0.0;

		mRotateX = 0.0;
		mRotateY = 0.0;
		mRotateZ = 0.0;

		mScaling = 0.0;
	}

	spatial_reference::~spatial_reference(void)
	{
		clear_project_operator();
	}

	bool spatial_reference::spherical_to_project(vector_3d &vec)
	{
		if (mProjectOperator == NULL)
		{
			reset_project_operator();
		}
		if (mProjectOperator == NULL)
		{
			return false;
		}
		else
		{
			double v_height = vec.z;
			mProjectOperator->projection_caculate(vec.x, vec.y, &vec.x, &vec.z);
			vec.y = v_height;
			return true;
		}
	}

	bool spatial_reference::project_to_spherical(vector_3d &vec)
	{
		if (mProjectOperator == NULL)
		{
			reset_project_operator();
		}
		if (mProjectOperator == NULL)
		{
			return false;
		}
		else
		{
			double v_height = vec.y;
			mProjectOperator->projection_inverse_caculate(vec.x, vec.z, &vec.x, &vec.y);
			vec.z = v_height;
			return true;
		}
	}

	bool spatial_reference::spherical_to_project(double in_lon, double in_lat, double* out_x, double* out_y)
	{
		if (mProjectOperator == NULL)
		{
			reset_project_operator();
		}
		if (mProjectOperator == NULL)
		{
			return false;
		}
		else
		{
			mProjectOperator->projection_caculate(in_lon, in_lat, out_x, out_y);
			return true;
		}
	}

	bool spatial_reference::project_to_spherical(double in_x, double in_y, double* out_lon, double* out_lat)
	{
		if (mProjectOperator == NULL)
		{
			reset_project_operator();
		}
		if (mProjectOperator == NULL)
		{
			return false;
		}
		else
		{
			mProjectOperator->projection_inverse_caculate(in_x, in_y, out_lon, out_lat);
			return true;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void spatial_reference::clear_project_operator()
	{
		if (mProjectOperator)
		{
			AUTO_DELETE(mProjectOperator);
			mProjectOperator = NULL;
		}
	}

	void spatial_reference::reset_project_operator()
	{
		// 投影类型
		// PrjTypeNull = 0,	// 经纬度
		// PrjTypeGauss,
		// PrjTypeGaussCustomized,
		// PrjTypeUTM,
		// PrjTypeUTMCustomized,
		// PrjTypeMercator,
		switch (mProjectionType)
		{
		case 1:
		{
			mProjectOperator = new gauss_projection();
			((gauss_projection*)mProjectOperator)->set_zone_wide(mZoneWidth);
			((gauss_projection*)mProjectOperator)->m_zone_index = mZoneIndex;
		}
		break;
		case 2:
		{
			mProjectOperator = new gauss_projection();
			mProjectOperator->set_central_meridian(mCentralMeridian);
			mProjectOperator->set_false_east(mFalseEast);
		}
		break;
		case 3:
		{
			mProjectOperator = new utm_projection();
			((utm_projection*)mProjectOperator)->m_zone_index = mZoneIndex;
		}
		break;
		case 4:
		{
			mProjectOperator = new utm_projection();
			mProjectOperator->set_central_meridian(mCentralMeridian);
			mProjectOperator->set_false_east(mFalseEast);
		}
		break;
		case 5:
			mProjectOperator = new mercator_projection();
			((mercator_projection*)mProjectOperator)->set_central(mCentralMeridian, mReferenceLatitude);
			break;
		default:
			return;
		}

		// 椭球体类型
		// EllipsoidWGS84 = 0,
		// EllipsoidBeijing54,
		// EllipsoidXian80,
		// EllipsoidNational2000,
		switch (mEllipsoidType)
		{
		case 0:
			mProjectOperator->set_semi(US_PRJ_SEMI_MAJOR_84, US_PRJ_SEMI_MINOR_84);
			break;
		case 1:
			mProjectOperator->set_semi(US_PRJ_SEMI_MAJOR_54, US_PRJ_SEMI_MINOR_54);
			break;
		case 2:
			mProjectOperator->set_semi(US_PRJ_SEMI_MAJOR_80, US_PRJ_SEMI_MINOR_80);
			break;
		case 3:
			mProjectOperator->set_semi(US_PRJ_SEMI_MAJOR_2K, US_PRJ_SEMI_MINOR_2K);
			break;
		default:
			AUTO_DELETE(mProjectOperator);
			mProjectOperator = NULL;
			return;
		}
	}

	// 	transform_base* spatial_reference::get_transform( void )
	// 	{
	// 		return NULL;
	// 	}

}