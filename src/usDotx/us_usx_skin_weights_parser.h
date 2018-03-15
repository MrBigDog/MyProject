///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_skin_weights_parser.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_skin_weights_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_SKIN_WEIGHTS_PARSER_H_
#define _US_USX_SKIN_WEIGHTS_PARSER_H_

#include <usDotx/Export.h>
#include <usDotx/us_usx_parser.h>

#include <usUtil/us_common_file.h>



struct ID3DXFileData;

namespace uniscope_globe
{
	class usx_parser_object_container;

	class USDOTX_EXPORT usx_skin_weights_parser
		: public usx_parser
	{
	public:
		usx_skin_weights_parser(void);

		virtual ~usx_skin_weights_parser(void);

	public:
		virtual bool parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container);
	};
}

#endif // _US_USX_SKIN_WEIGHTS_PARSER_H_