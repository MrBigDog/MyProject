///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_template.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_template class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_TEMPLATE_H_
#define _US_USX_TEMPLATE_H_

#include <usDotx/Export.h>

namespace uniscope_globe
{
#define D3DRMXTEMPLATEBYTES 3278

	class USDOTX_EXPORT usx_template
	{
	public:
		usx_template(void);

		virtual ~usx_template(void);

	public:
		static unsigned char* get_default_template(void);

	};
}

#endif // _US_USX_TEMPLATE_H_ 