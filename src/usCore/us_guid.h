///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_guid.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : guid class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GUID_H_
#define _US_GUID_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>
#include <guiddef.h>

namespace uniscope_globe
{

	class USCORE_EXPORT guid
	{
	private:
		guid(void);

	public:
		virtual ~guid(void);

	public:
		static ustring create(void);

		static GUID create(cpstr str_guid);

		static ustring create(GUID& in_guid);

		static void guid_string_to_int64(cpstr str_guid, __int64& out_first, __int64& out_second);
	};
}

#endif // _US_GUID_H_ 