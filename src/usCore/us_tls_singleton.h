//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_tls_singleton.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : tls_singleton
//	Reference : 
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_TLS_SINGLETON_H_
#define _US_TLS_SINGLETON_H_
//#include <usCore\Export.h>
namespace uniscope_globe
{
	template <class T>
	class tls_singleton //: private T
	{
	protected:
		tls_singleton();
		~tls_singleton();

	protected:
		static DWORD m_tls_index;

	public:
		static T& instance();

		static T* ptr();

		static void attach( T* in_ptr );

		//static void destroy();
	};

	template <class T>
	inline tls_singleton<T>::tls_singleton()
	{		
		m_tls_index = TLS_OUT_OF_INDEXES;
	}

	template <class T>
	inline tls_singleton<T>::~tls_singleton()
	{	
		TlsFree( m_tls_index );	
	}

	template <class T>
	/*static*/ T &tls_singleton<T>::instance()
	{
		return *ptr();
	}

	template <class T>
	T* tls_singleton<T>::ptr()
	{
		return (T*)TlsGetValue( m_tls_index );		
	}
	
	template <class T>
	void tls_singleton<T>::attach( T* in_ptr )
	{
		// function-local static to force this to work correctly at static
		// initialization time.
/*		if ( m_tls_index == TLS_OUT_OF_INDEXES )
		{
			m_tls_index = TlsAlloc();	*/		
			TlsSetValue( m_tls_index, in_ptr );
		//}
	}

	//template <class T>
	//void tls_singleton<T>::destroy()
	//{
	//	TlsFree( m_tls_index );		
	//}

}

#endif