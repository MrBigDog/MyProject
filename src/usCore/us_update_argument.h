///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_update_argument.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : update_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_UPDATE_ARGUMENT_H_
#define _US_UPDATE_ARGUMENT_H_

#include <usCore\Export.h>
#include <usCore\us_shadow_parameter.h>

namespace uniscope_globe
{
#define  us_dr_dem		0x0001
#define  us_dr_dom		0x0002
#define  us_dr_model	0x0004

	class document_base;
	class observer_base;
	class render_node;


	class USCORE_EXPORT update_argument
	{
	public:
		update_argument(void);

		update_argument(document_base* v_document, observer_base* v_observer, render_node* v_render, shadow_parameter& v_shadow_param, int data_ready = 0);

		virtual ~update_argument(void);

	public:
		observer_base*			m_observer;

		render_node*			m_render;

		document_base*			m_document;

		shadow_parameter		m_shadow_param;

		int						m_data_ready;

	};
}

#endif // _US_UPDATE_ARGUMENT_H_ 