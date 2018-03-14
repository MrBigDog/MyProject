///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_HARDWARE_BASE_H_
#define _US_HARDWARE_BASE_H_

#include <usCore\Export.h>
#include <usCore\us_resource_base.h>
#include <usUtil\us_common_file.h>
#include <basetsd.h>

namespace uniscope_globe
{
	class render_device;
	class raw_buffer;

	class USCORE_EXPORT hardware_base : public resource_base
	{
	protected:
		hardware_base(void);

	public:
		virtual ~hardware_base(void);

	public:
		virtual long create(render_device* device) = 0;

		virtual bool destroy(void) = 0;

		virtual bool refresh(void);

		virtual ULONG_PTR get_handle(void);

		uint	get_rank(void);

		void	set_rank(uint n_rank);

		bool	operator < (hardware_base& buffer2)
		{
			return m_rank < buffer2.get_rank();
		}

		bool	operator > (hardware_base& buffer2)
		{
			return m_rank > buffer2.get_rank();
		}

		raw_buffer* get_raw_buffer(void) { return m_raw_buffer; }

	protected:
		uint			m_rank;

		raw_buffer*		m_raw_buffer;
	};

	inline bool compare_mesh_rank(hardware_base* h1, hardware_base* h2)
	{
		return h1->get_rank() > h2->get_rank();
	}
}

#endif // _US_HARDWARE_BASE_H_