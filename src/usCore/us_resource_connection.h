///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_resource_connection.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : resource_connection class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RESOURCE_CONNECTION_H_
#define _US_RESOURCE_CONNECTION_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT resource_connection
	{
	protected:
		resource_connection( void );

	public:
		resource_connection( resource_manager<interior_index>* parent_mngr );

		virtual ~resource_connection( void );

		typedef std::map<interior_index, download_mission*> resource_request_map;

		// override from storage base
	public:
		virtual bool request_data( interior_index index );
		
		virtual void parse_raw_buffer( event_argument* args );

	protected:
		void remove_request( interior_index index );

		virtual ustring index_to_path( interior_index index ) = 0;

	protected:
		resource_request_map m_request_map;

		resource_manager<interior_index>* m_parent_mngr;
		 
	};
}

#endif // _US_RESOURCE_CONNETION_H_