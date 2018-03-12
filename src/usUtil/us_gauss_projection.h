//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_gauss_projection.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : angle template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_GAUSS_PROJECTION_H_
#define _US_GAUSS_PROJECTION_H_

#include <usUtil\Export.h>
#include <usUtil\us_projection.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT gauss_projection : public uniscope_projection
	{
	public:
		gauss_projection(void);
		virtual ~gauss_projection(void);

	public:
		/// 对外接口
		/// 
		/// 设定分带宽度
		/// 
		void set_zone_wide(int wide);

		/// <summary>
		/// 高斯投影正算
		/// 由经纬度（单位：Decimal Degree）正算到大地坐标（单位：Metre，含带号）
		/// </summary>
		/// <param name="longitude">经度</param>
		/// <param name="latitude">纬度</param>
		bool projection_caculate(double longitude, double latitude, double* coord_x, double* coord_y);

		/// <summary>
		/// 高斯投影反算
		/// 大地坐标（单位：Metre，含带号）反算到经纬度坐标（单位，Decimal Degree）
		/// </summary>
		/// <param name="X">大地坐标X值</param>
		/// <param name="Y">大地坐标Y值</param>
		bool projection_inverse_caculate(double coord_x, double coord_y, double* longitude, double* latitude);

		/// <summary>
		/// 十进制双精度角度转换成度分秒角度格式
		/// </summary>
		/// <param name="decimal degree">度，十进制型双精度</param>
		/// <param name="degree">度，整型</param>
		/// <param name="minute">分，整型</param>
		/// <param name="second">秒，双精度型</param>
		void decimal_to_degree_minute_second(double decimal, int* degree, int* minute, double* second);

		/// <summary>
		/// 度分秒角度格式转换成十进制度双精度角度格式
		/// </summary>
		/// <param name="degree">度，整型</param>
		/// <param name="minute">分，整型</param>
		/// <param name="second">秒，双精度型</param>
		/// <param name="decimal degree">度，十进制型双精度</param>   
		void degree_minute_second_to_decimal(int degree, int minute, double second, double *decimal);

	private:

		double adjust_lon(double x);

		void translate_four_param(double local_x, double local_y, double* nation_x, double* nation_y);

		int		m_zone_wide;

		double	m_e0, m_e1, m_e2, m_e3, m_e, m_es, m_esp;	// eccentricity constants
		double	m_ml0;										//small value m

	public:
		double m_offset_east;
		double m_offset_north;			// 纬度起始值，UTM北半球为0，南半球为10000000.0m(高斯投影中均为0)；

		int m_zone_index;

	}; // gauss_projection

} // uniscope_globe

#endif // _US_GAUSS_PROJECTION_H_
