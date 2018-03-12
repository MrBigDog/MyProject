///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_exporter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_exporter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_EXPORTER_H_
#define _US_USX_EXPORTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class usx_exporter
	{
	public:
		usx_exporter( void );

		virtual ~usx_exporter( void );

	public:
		//virtual bool parse_object( ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container );

		//virtual bool save( usx_exporter_args* exporter_args, ulong depth, void** data, hardware_mesh* in_container ) { return true;}
		 
	};
}

#endif // _US_USX_EXPORTER_H_ 