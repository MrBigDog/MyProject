///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shared_ptr.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shared_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHARED_PTR_H_
#define _US_SHARED_PTR_H_
#include <usCore\Export.h>
namespace uniscope_globe
{

	template< class T > class shared_object;

	template< class T >
	class shared_ptr
	{
	public:
		// Ä¬ÈÏ¹¹Ôì
		shared_ptr() :m_shared_object(NULL)
		{
		}

		explicit shared_ptr(T* ptr) :m_shared_object(NULL)
		{
			m_shared_object = new shared_object<T>(ptr);
		}

		shared_ptr(const shared_ptr<T>& in_shared_ptr)
		{
			m_shared_object = in_shared_ptr.m_shared_object;

			if (m_shared_object != NULL)
			{
				m_shared_object->add_ref();
			}
		}

		virtual ~shared_ptr()
		{
			if (m_shared_object)
			{
				AUTO_RELEASE(m_shared_object);
			}
		}

		shared_ptr<T>& operator = (shared_ptr<T>& in_shared_ptr)
		{
			if (m_shared_object)
			{
				AUTO_RELEASE(m_shared_object);
			}

			m_shared_object = in_shared_ptr.m_shared_object;

			m_shared_object->add_ref();

			return *this;
		}

		T&  operator* () const
		{
			return m_shared_object->operator *();
		}

		T*  operator-> () const
		{
			return m_shared_object->operator ->();
		}

		bool operator == (shared_ptr<T>& in_shared_ptr) const
		{
			return in_shared_ptr.m_shared_object == m_shared_object;
		}

		bool operator == (T* ptr) const
		{
			if (m_shared_object == NULL)
			{
				return ptr == NULL;
			}

			return *m_shared_object == ptr;
		}

		T* get()
		{
			if (m_shared_object == NULL)
			{
				return NULL;
			}
			else
			{
				return m_shared_object->get();
			}
		}

		static void create_instance(shared_ptr<T>* in_shared_ptr)
		{
			in_shared_ptr->m_shared_object = new shared_object<T>(new T);
		}

	protected:
		shared_object<T>* m_shared_object;

	};
}

#endif // _US_SHARED_PTR_H_