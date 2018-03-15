///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_native_loader.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : native_loader class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_NATIVE_LOADER_H_
#define _US_NATIVE_LOADER_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>

namespace uniscope_globe
{
	class link_base;
	//class byte;

	class USCORE_EXPORT native_loader
	{
	protected:
		native_loader(void) { }

		~native_loader(void) { }

	public:
		static int load_from_root(link_base* v_link, byte* &data_stream, int& v_size);

		static int load_from_local(link_base* v_link, byte* &data_stream, int& v_size);
	};
}

#endif // _US_NATIVE_LOADER_H_