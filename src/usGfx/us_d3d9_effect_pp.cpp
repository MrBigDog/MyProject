
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_pp.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_pp class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_effect_pp.h"
#include <usCore/us_view_port.h>
#include <usCore/us_render_device.h>

namespace uniscope_globe
{
	d3d9_effect_pp::d3d9_effect_pp(render_device* device)
		:d3d9_effect_base(device)
	{
	}

	d3d9_effect_pp::~d3d9_effect_pp(void)
	{

	}

	long d3d9_effect_pp::on_reset_device(void)
	{
		if (!m_d3dx_effect) return 0;
		m_d3dx_effect->OnResetDevice();

		D3DXHANDLE hAnno;
		D3DXHANDLE hParamToConvert;
		UINT uIndex = 0;

		view_port v_view_port = m_device->get_view_port();

		while (NULL != (hParamToConvert = m_d3dx_effect->GetParameter(NULL, uIndex++)))
		{
			if (NULL != (hAnno = m_d3dx_effect->GetAnnotationByName(hParamToConvert, "ConvertPixelsToTexels")))
			{
				LPCSTR szSource;
				m_d3dx_effect->GetString(hAnno, &szSource);
				D3DXHANDLE hConvertSource = m_d3dx_effect->GetParameterByName(NULL, szSource);

				if (hConvertSource)
				{
					// Kernel source exists. Proceed.
					// Retrieve the kernel size
					D3DXPARAMETER_DESC desc;
					m_d3dx_effect->GetParameterDesc(hConvertSource, &desc);
					// Each element has 2 floats
					DWORD cKernel = desc.Bytes / (2 * sizeof(float));
					D3DXVECTOR4 *pvKernel = new D3DXVECTOR4[cKernel];
					if (!pvKernel) return false;
					m_d3dx_effect->GetVectorArray(hConvertSource, pvKernel, cKernel);
					// Convert
					for (DWORD i = 0; i < cKernel; ++i)
					{
						pvKernel[i].x = pvKernel[i].x / float(v_view_port.m_width);  //must be edit
						pvKernel[i].y = pvKernel[i].y / float(v_view_port.m_height);
					}
					// Copy back
					m_d3dx_effect->SetVectorArray(hParamToConvert, pvKernel, cKernel);
					delete[] pvKernel;
				}
			}
			//hAnno = m_d3dx_effect->GetAnnotationByName( hParamToConvert, "fScaleX");
			//if(hAnno)
			//{
			//	m_d3dx_effect->GetFloat(hAnno, &m_fScale[0] );
			//}
			//hAnno = m_pEffect->GetAnnotationByName( hParamToConvert, "fScaleY");
			//if(hAnno)
			//{
			//	m_pEffect->GetFloat(hAnno, &m_fScale[1] );
			//}
		}
		return 0;

	}
}