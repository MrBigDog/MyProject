///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_texture.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_texture class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_HARDWARE_TEXTURE_H_
#define _US_HARDWARE_TEXTURE_H_
#include <usCore\Export.h>
#pragma warning ( disable:4482 )
namespace uniscope_globe
{
	// texture format
	enum texture_format
	{
		US_TF_X8R8G8B8 = 0,
		US_TF_A8R8G8B8,
		US_TF_R8G8B8,
		US_TF_R5G6B5,
		US_TF_A1R5G5B5,
		US_TF_DXT1,
		US_TF_DXT5
	};

	// texture info
	struct texture_info
	{
		int width;
		int height;
		int mipmap;
		int format;
		texture_info()
		{
			width = height = mipmap = 0;
			format = 0;
		}
	};


	class USCORE_EXPORT hardware_texture : public hardware_base
	{
	protected:
		hardware_texture(void);

	public:
		hardware_texture(raw_buffer* buffer);

		virtual ~hardware_texture(void);

		// override from hardware_base
	public:
		virtual long create(render_device* device);

		virtual bool destroy(void);

		virtual bool clear_buffer();

		// own
	public:
		texture_info get_texture_info(void);

		virtual void draw(render_device* device, uint stage_index);

	protected:
		texture_info m_texture_info;

	public:
		hardware_creator* m_creator;
	};
}

#endif // _US_HARDWARE_TEXTURE_H_