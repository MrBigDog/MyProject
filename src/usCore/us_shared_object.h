///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shared_object.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shared_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHARED_OBJECT_H_
#define _US_SHARED_OBJECT_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	template< class T >
	class shared_ptr;

	template< class T >
	class shared_object
	{
	public:
		// Ä¬ÈÏ¹¹Ôì
		shared_object()
		{
			m_ptr = NULL;

			m_reference_count = 0;
		}

		explicit shared_object( T* ptr ):m_ptr( ptr ) 
		{
			m_reference_count = 0;
		}

		virtual ~shared_object( void )
		{
			AUTO_DELETE( m_ptr );			
		}

		T&  operator* () const 
		{
			return *m_ptr;
		}
		
		T*  operator-> () const 
		{
			return m_ptr;
		}

		bool operator== (T* ptr) const 
		{
			return m_ptr == ptr;
		}

		T*   get( void ) 
		{
			return m_ptr;
		}

		virtual void add_ref( void ) 
		{
			m_reference_count++; 
		}

		virtual void Release( void )
		{
			m_reference_count--;
			if ( m_reference_count == -1 )
			{
				delete this;
			} 
		}

		int get_ref_count( void ) 
		{
			return m_reference_count;
		}

	protected:
		int m_reference_count;

		T* m_ptr;
	};
}

#endif // _US_SHARED_OBJECT_H_