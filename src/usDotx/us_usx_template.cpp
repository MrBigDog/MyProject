///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_template.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_template class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include <rmxftmpl.h>
#include "us_usx_template.h"

extern unsigned char D3DRM_XTEMPLATES[];

namespace uniscope_globe
{
	// 	char const * usxTemplates =
	// 		"xof 0303txt 0032\n"
	// 		"\n" 
	// 		"template MultiTextureFilename {\n"
	// 		"	<a42790e1-7c10-11cf-8c52-0040333594a3>\n"
	// 		"	DWORD nTextureCount;\n"
	// 		"	array STRING filename[nTextureCount];\n"
	// 		"}\n"
	// 		"\n"
	// 		;

	usx_template::usx_template(void)
	{

	}

	usx_template::~usx_template(void)
	{

	}

	unsigned char* usx_template::get_default_template(void)
	{
		return D3DRM_XTEMPLATES;
	}


}