///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mercator_projection.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mercator_projection class
//	Reference : http://blog.csdn.net/cleverysm/archive/2008/02/26/2121633.aspx
//            : http://www.gissky.net/Article/200607/81.htm
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MERCATOR_PROJECTION_H_
#define _US_MERCATOR_PROJECTION_H_

#include <usUtil\Export.h>
#include <usUtil\us_projection.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT mercator_projection : public uniscope_projection
	{
	public:
		mercator_projection(void);
		virtual ~mercator_projection(void);

	public:

		/// 对外接口：

		///
		/// 设定基准纬度和中央子午线，单位：度
		/// 
		bool set_central(double central_meridian, double ref_lat);

		/// <summary>
		/// 墨卡托投影正算
		/// 由经纬度（单位：Decimal Degree）正算到大地坐标（单位：Metre，含带号）
		/// </summary>
		/// <param name="longitude">经度</param>
		/// <param name="latitude">纬度</param>
		bool projection_caculate(double longitude, double latitude, double* coord_x, double* coord_y);

		/// <summary>
		/// 墨卡托投影反算
		/// 大地坐标（单位：Metre，含带号）反算到经纬度坐标（单位，Decimal Degree）
		/// </summary>
		/// <param name="X">大地坐标X值</param>
		/// <param name="Y">大地坐标Y值</param>
		bool projection_inverse_caculate(double coord_x, double coord_y, double* longitude, double* latitude);

	private:
		int    m_interative_times;  // 反向转换程序中的迭代次数
		double m_interative_value;  // 反向转换程序中的迭代初始值


	};
}

#endif // _US_MERCATOR_PROJECTION_H_ 