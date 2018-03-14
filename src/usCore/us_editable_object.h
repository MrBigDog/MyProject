
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_editable_object.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : editable_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_EDITABLE_OBJECT_H_
#define _US_EDITABLE_OBJECT_H_

#include <usCore\Export.h>
#include <usUtil\us_matrix4.h>

namespace uniscope_globe
{
	class USCORE_EXPORT editable_object
	{
	public:
		editable_object(void);

		virtual ~editable_object(void);

		// editable object;
	public:
		virtual void translate_object(vector_3d& vec_offset) {}
		virtual void rotate_object(quaternion_4d& quater) {}
		virtual void scale_object(vector_3d& vec) {}
		virtual void transform_object(matrix_4d& mat) { }
		virtual void extrude_object(double v_height, int v_flag) { }
		virtual void extrude_finish() { }

	};
}

#endif // _US_EDITABLE_OBJECT_H_ 