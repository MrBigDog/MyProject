///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_utm_projection.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : utm_projection class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_UTM_PROJECTION_H_
#define _US_UTM_PROJECTION_H_

#include "us_projection.h"

namespace uniscope_globe
{
	class utm_projection : public uniscope_projection
	{
	public:
		utm_projection(void);
		virtual ~utm_projection(void);

	public:
		/// 对外接口

		/// <summary>
		/// UTM投影正算
		/// 由经纬度（单位：Decimal Degree）正算到大地坐标（单位：Metre，含带号）
		/// </summary>
		/// <param name="longitude">经度</param>
		/// <param name="latitude">纬度</param>
		bool projection_caculate(double longitude, double latitude, double* coord_x, double* coord_y);

		/// <summary>
		/// UTM投影反算
		/// 大地坐标（单位：Metre，含带号）反算到经纬度坐标（单位，Decimal Degree）
		/// </summary>
		/// <param name="X">大地坐标X值</param>
		/// <param name="Y">大地坐标Y值</param>
		bool projection_inverse_caculate(double coord_x, double coord_y, double* longitude, double* latitude);

	private:
		double m_e, m_es, m_esp, m_e1;        // 第一偏心率e，e的平方，第二偏心率

	public:
		double m_offset_east;
		double m_offset_north;			// 纬度起始值，UTM北半球为0，南半球为10000000.0m(高斯投影中均为0)；

		int m_zone_index;

	};
}

#endif // _US_UTM_PROJECTION_H_ 