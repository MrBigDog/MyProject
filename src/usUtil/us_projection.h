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
#define US_PRJ_SEMI_MINOR_2K 6356752.3141403558		// ���ݱ��ʷ��㡣���õı���ֵ��298.257222101


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

		/// ����ӿڣ�

		///
		/// �趨�������������λ����
		/// 
		bool set_semi(double major, double minor);

		/// 
		/// �趨���뾭�ߣ�
		/// 
		bool set_central_meridian(double central_meridian);

		/// 
		/// �趨���뾭�ߣ�
		/// 
		bool set_false_east(double false_e);

		/// 
		/// ��ȡ����
		/// 
		int get_type();

		/// <summary>
		/// ͶӰ����
		/// �ɾ�γ�ȣ���λ��Decimal Degree�����㵽������꣨��λ��Metre�������ţ�
		/// </summary>
		/// <param name="longitude">����</param>
		/// <param name="latitude">γ��</param>
		virtual bool projection_caculate(double longitude, double latitude, double* coord_x, double* coord_y) = 0;

		/// <summary>
		/// ͶӰ����
		/// ������꣨��λ��Metre�������ţ����㵽��γ�����꣨��λ��Decimal Degree��
		/// </summary>
		/// <param name="X">�������Xֵ</param>
		/// <param name="Y">�������Yֵ</param>
		virtual bool projection_inverse_caculate(double coord_x, double coord_y, double* longitude, double* latitude) = 0;

		/// <summary>
		/// Rect����
		/// </summary>
		/// �����������Ϊ��λ��rect;
		lon_lat_rect rect_projection_calculate(lon_lat_rect rect_bl);

		/// <summary>
		/// Rect����
		/// </summary>
		/// ����ֵ����Ϊ��λ��rect��
		lon_lat_rect rect_projection_inverse_calculate(lon_lat_rect rect_xy);


	protected:
		int m_proj_type;

		double m_semi_major;        // �����峤����,��
		double m_semi_minor;        // ������̰���,��

		double m_scale_factor;			// scale factor // k0, ��˹ͶӰΪ1��UTMΪ0.9996
		double m_central_meridian;      // Center longitude (projection center) // ���뾭��, ���ȣ�
		double m_lat_origin;         // ��׼γ��,����

		double m_false_east;		 // 500,000;����ƫ��

	protected:
		bool m_has_set_central;

	};

}
#endif // _US_PROJECTION_H_ 