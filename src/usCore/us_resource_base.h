///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_resource_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : resource_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RESOURCE_BASE_H_
#define _US_RESOURCE_BASE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT resource_base : virtual public object_base, virtual public shared_data
	{
	public:
		resource_base(void);

		virtual ~resource_base(void);

		bool is_valid(void) { return m_valid; }

	public:
		virtual void set_resource_status(int v_status) {}

	protected:
		bool m_valid;
	};
}

#endif // _US_RESOURCE_BASE_H_