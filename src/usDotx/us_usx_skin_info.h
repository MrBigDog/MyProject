///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_skin_info.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_skin_info class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_SKIN_INFO_H_
#define _US_USX_SKIN_INFO_H_

#include <usDotx/Export.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_matrix4.h>

#include <vector>

namespace uniscope_globe
{
	class USDOTX_EXPORT usx_weights_info
	{
	public:
		usx_weights_info() {}

		usx_weights_info(usx_weights_info* info);

	public:
		ustring m_frame_name;

		std::vector<ulong> m_indices;

		std::vector<double> m_weights;

		matrix4<double> m_offset_matrix;
	};

	class usx_skin_info
	{
	public:
		typedef std::vector<usx_weights_info*> weights_array;

	public:
		usx_skin_info(void);

		usx_skin_info(usx_skin_info* in_info);

		virtual ~usx_skin_info(void);

	public:
		ushort m_weights_per_vertex;

		ushort m_weights_per_face;

		ushort m_num_of_bones;

		weights_array m_weights_array;
	};
}

#endif // _US_USX_SKIN_INFO_H_