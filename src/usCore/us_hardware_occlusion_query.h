///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_occlusion_query.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_occlusion_query class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_HARDWARE_OCCLUSION_QUERY_H_
#define _US_HARDWARE_OCCLUSION_QUERY_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT hardware_occlusion_query
	{
	public:
		hardware_occlusion_query(void);

		virtual ~hardware_occlusion_query(void);

	public:
		virtual void begin_occlusion_query(void) = 0;

		/**
		* Ends the hardware occlusion test
		*/
		virtual void end_occlusion_query(void) = 0;

		virtual void issue_begin(void) = 0;

		virtual void issue_end(void) = 0;

		/**
		* Pulls the hardware occlusion query.
		* @note Waits until the query result is available; use isStillOutstanding
		*		if just want to test if the result is available.
		* @retval NumOfFragments will get the resulting number of fragments.
		* @return True if success or false if not.
		*/
		virtual bool pull_occlusion_query(uint* num_of_fragments) = 0;

		/**
		* Let's you get the last pixel count with out doing the hardware occlusion test
		* @return The last fragment count from the last test.
		* Remarks This function won't give you new values, just the old value.
		*/
		unsigned int get_last_querys_pixel_count(void) const { return m_pixel_count; }

		/**
		* Lets you know when query is done, or still be processed by the Hardware
		* @return true if query isn't finished.
		*/
		virtual bool is_still_outstanding(void) = 0;

		virtual long on_lost_device(void) = 0;

		virtual long on_reset_device(void) = 0;

		//----------------------------------------------------------------------
		// protected members
		//--
	protected:
		// numbers of visible pixels determined by last query
		uint	 m_pixel_count;
		// is query hasn't yet returned a result.
		bool	 m_is_query_result_still_outstanding;

	};
}

#endif // _US_HARDWARE_OCCLUSION_QUERY_H_