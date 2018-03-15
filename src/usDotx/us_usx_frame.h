///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_frame.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_frame class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_FRAME_H_
#define _US_USX_FRAME_H_

#include <usDotx/Export.h>
#include <usUtil/us_matrix4.h>
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	class usx_mesh;
	class usx_animation;

	class USDOTX_EXPORT usx_frame
	{
	public:
		usx_frame(void);

		virtual ~usx_frame(void);

		usx_frame* find_frame(cpstr frame_name);

		void reset(void);

		void update_hierarchy(matrix4<double> in_mat);

		usx_frame* clone(void);

	public:
		ustring m_name;

		matrix4<double> m_original_mat;
		matrix4<double> m_transform_mat;

		matrix4<double> m_combined_mat;

		usx_frame* m_frame_parent;
		usx_frame* m_frame_sibling;
		usx_frame* m_frame_first_child;

		usx_animation* m_animation;

		usx_mesh* m_mesh;
	};
}

#endif // _US_USX_FRAME_H_