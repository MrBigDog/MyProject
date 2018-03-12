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
		/// ����ӿ�
		/// 
		/// �趨�ִ����
		/// 
		void set_zone_wide(int wide);

		/// <summary>
		/// ��˹ͶӰ����
		/// �ɾ�γ�ȣ���λ��Decimal Degree�����㵽������꣨��λ��Metre�������ţ�
		/// </summary>
		/// <param name="longitude">����</param>
		/// <param name="latitude">γ��</param>
		bool projection_caculate(double longitude, double latitude, double* coord_x, double* coord_y);

		/// <summary>
		/// ��˹ͶӰ����
		/// ������꣨��λ��Metre�������ţ����㵽��γ�����꣨��λ��Decimal Degree��
		/// </summary>
		/// <param name="X">�������Xֵ</param>
		/// <param name="Y">�������Yֵ</param>
		bool projection_inverse_caculate(double coord_x, double coord_y, double* longitude, double* latitude);

		/// <summary>
		/// ʮ����˫���ȽǶ�ת���ɶȷ���Ƕȸ�ʽ
		/// </summary>
		/// <param name="decimal degree">�ȣ�ʮ������˫����</param>
		/// <param name="degree">�ȣ�����</param>
		/// <param name="minute">�֣�����</param>
		/// <param name="second">�룬˫������</param>
		void decimal_to_degree_minute_second(double decimal, int* degree, int* minute, double* second);

		/// <summary>
		/// �ȷ���Ƕȸ�ʽת����ʮ���ƶ�˫���ȽǶȸ�ʽ
		/// </summary>
		/// <param name="degree">�ȣ�����</param>
		/// <param name="minute">�֣�����</param>
		/// <param name="second">�룬˫������</param>
		/// <param name="decimal degree">�ȣ�ʮ������˫����</param>   
		void degree_minute_second_to_decimal(int degree, int minute, double second, double *decimal);

	private:

		double adjust_lon(double x);

		void translate_four_param(double local_x, double local_y, double* nation_x, double* nation_y);

		int		m_zone_wide;

		double	m_e0, m_e1, m_e2, m_e3, m_e, m_es, m_esp;	// eccentricity constants
		double	m_ml0;										//small value m

	public:
		double m_offset_east;
		double m_offset_north;			// γ����ʼֵ��UTM������Ϊ0���ϰ���Ϊ10000000.0m(��˹ͶӰ�о�Ϊ0)��

		int m_zone_index;

	}; // gauss_projection

} // uniscope_globe

#endif // _US_GAUSS_PROJECTION_H_
