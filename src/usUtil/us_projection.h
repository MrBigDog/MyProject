///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_projection.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : uniscope_projection class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_PROJECTION_H_
#define _US_PROJECTION_H_

#include <usUtil\Export.h>
#include <usUtil\us_object_base.h>

#define US_PRJ_SEMI_MAJOR_54 6378245.0
#define US_PRJ_SEMI_MINOR_54 6356863.0187730473 
#define US_PRJ_SEMI_MAJOR_80 6378140.0
#define US_PRJ_SEMI_MINOR_80 6356755.2881575283
#define US_PRJ_SEMI_MAJOR_84 6378137.0
#define US_PRJ_SEMI_MINOR_84 6356752.3142451793
#define US_PRJ_SEMI_MAJOR_2K 6378137.0
#define US_PRJ_SEMI_MINOR_2K 6356752.3141403558		// 根据扁率反算。采用的扁率值：298.257222101


#define US_PRJ_TYPE_BL       0
#define US_PRJ_TYPE_GAUSS    2
#define US_PRJ_TYPE_MERCATOR 3
#define US_PRJ_TYPE_UTM      4

namespace uniscope_globe
{
	class lon_lat_rect;

	class USUTIL_EXPORT uniscope_projection : public object_base
	{
	public:
		uniscope_projection(void);
		virtual ~uniscope_projection(void);

	public:

		/// 对外接口：

		///
		/// 设定长短轴参数，单位：米
		/// 
		bool set_semi(double major, double minor);

		/// 
		/// 设定中央经线；
		/// 
		bool set_central_meridian(double central_meridian);

		/// 
		/// 设定中央经线；
		/// 
		bool set_false_east(double false_e);

		/// 
		/// 获取类型
		/// 
		int get_type();

		/// <summary>
		/// 投影正算
		/// 由经纬度（单位：Decimal Degree）正算到大地坐标（单位：Metre，含带号）
		/// </summary>
		/// <param name="longitude">经度</param>
		/// <param name="latitude">纬度</param>
		virtual bool projection_caculate(double longitude, double latitude, double* coord_x, double* coord_y) = 0;

		/// <summary>
		/// 投影反算
		/// 大地坐标（单位：Metre，含带号）反算到经纬度坐标（单位，Decimal Degree）
		/// </summary>
		/// <param name="X">大地坐标X值</param>
		/// <param name="Y">大地坐标Y值</param>
		virtual bool projection_inverse_caculate(double coord_x, double coord_y, double* longitude, double* latitude) = 0;

		/// <summary>
		/// Rect正算
		/// </summary>
		/// 输入参数：度为单位的rect;
		lon_lat_rect rect_projection_calculate(lon_lat_rect rect_bl);

		/// <summary>
		/// Rect反算
		/// </summary>
		/// 返回值：度为单位的rect；
		lon_lat_rect rect_projection_inverse_calculate(lon_lat_rect rect_xy);


	protected:
		int m_proj_type;

		double m_semi_major;        // 椭球体长半轴,米
		double m_semi_minor;        // 椭球体短半轴,米

		double m_scale_factor;			// scale factor // k0, 高斯投影为1，UTM为0.9996
		double m_central_meridian;      // Center longitude (projection center) // 中央经线, 弧度；
		double m_lat_origin;         // 标准纬度,弧度

		double m_false_east;		 // 500,000;东向偏移

	protected:
		bool m_has_set_central;

	};

}
#endif // _US_PROJECTION_H_ 