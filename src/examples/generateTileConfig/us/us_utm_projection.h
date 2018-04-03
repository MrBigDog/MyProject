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
		/// ����ӿ�

		/// <summary>
		/// UTMͶӰ����
		/// �ɾ�γ�ȣ���λ��Decimal Degree�����㵽������꣨��λ��Metre�������ţ�
		/// </summary>
		/// <param name="longitude">����</param>
		/// <param name="latitude">γ��</param>
		bool projection_caculate(double longitude, double latitude, double* coord_x, double* coord_y);

		/// <summary>
		/// UTMͶӰ����
		/// ������꣨��λ��Metre�������ţ����㵽��γ�����꣨��λ��Decimal Degree��
		/// </summary>
		/// <param name="X">�������Xֵ</param>
		/// <param name="Y">�������Yֵ</param>
		bool projection_inverse_caculate(double coord_x, double coord_y, double* longitude, double* latitude);

	private:
		double m_e, m_es, m_esp, m_e1;        // ��һƫ����e��e��ƽ�����ڶ�ƫ����

	public:
		double m_offset_east;
		double m_offset_north;			// γ����ʼֵ��UTM������Ϊ0���ϰ���Ϊ10000000.0m(��˹ͶӰ�о�Ϊ0)��

		int m_zone_index;

	};
}

#endif // _US_UTM_PROJECTION_H_ 