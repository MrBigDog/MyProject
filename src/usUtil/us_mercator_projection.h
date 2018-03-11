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

		/// ����ӿڣ�

		///
		/// �趨��׼γ�Ⱥ����������ߣ���λ����
		/// 
		bool set_central(double central_meridian, double ref_lat);

		/// <summary>
		/// ī����ͶӰ����
		/// �ɾ�γ�ȣ���λ��Decimal Degree�����㵽������꣨��λ��Metre�������ţ�
		/// </summary>
		/// <param name="longitude">����</param>
		/// <param name="latitude">γ��</param>
		bool projection_caculate(double longitude, double latitude, double* coord_x, double* coord_y);

		/// <summary>
		/// ī����ͶӰ����
		/// ������꣨��λ��Metre�������ţ����㵽��γ�����꣨��λ��Decimal Degree��
		/// </summary>
		/// <param name="X">�������Xֵ</param>
		/// <param name="Y">�������Yֵ</param>
		bool projection_inverse_caculate(double coord_x, double coord_y, double* longitude, double* latitude);

	private:
		int    m_interative_times;  // ����ת�������еĵ�������
		double m_interative_value;  // ����ת�������еĵ�����ʼֵ


	};
}

#endif // _US_MERCATOR_PROJECTION_H_ 