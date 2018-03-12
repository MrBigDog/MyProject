//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_singleton.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : singleton
//	Reference : 
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_SINGLETON_H_
#define _US_SINGLETON_H_

namespace uniscope_globe
{
	template <class T>
	class singleton //: private T
	{
	protected:
		singleton();
		~singleton();

	protected:
		static T* s_ins;

	public:
		static T& instance();

		static T* ptr();

		static void destroy();
	};

	template <class T>
	inline singleton<T>::singleton()
	{
		/* no-op */
		s_ins = NULL;
	}

	template <class T>
	inline singleton<T>::~singleton()
	{
		/* no-op */
	}

	template <class T>
	/*static*/ T &singleton<T>::instance()
	{
		// function-local static to force this to work correctly at static
		// initialization time.
		if (NULL == s_ins)
		{
			s_ins = new T;
		}
		return *(s_ins);
	}

	template <class T>
	T* singleton<T>::ptr()
	{
		if (NULL == s_ins)
		{
			s_ins = new T;
		}
		return (s_ins);
	}

	template <class T>
	void singleton<T>::destroy()
	{
		AUTO_DELETE(s_ins);
	}

}

#endif