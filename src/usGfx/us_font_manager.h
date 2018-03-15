///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_font_manager.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : font_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_FONT_MANAGER_H_
#define _US_FONT_MANAGER_H_

namespace uniscope_globe
{
	struct LIB_GFX_API font_description
	{
		int  height;
		uint width;
		uint weight;
		uint mip;
		BOOL italic;
		uint char_set;
		uint output_precision;
		uint quality;
		uint pitch_family;
		ustring face_name;
		font_description( void )
		{
			height = 20;
			width  = 0;
			weight = 700,
			mip	   = 5;
			italic = false;

			char_set		 = DEFAULT_CHARSET;
			output_precision = OUT_OUTLINE_PRECIS;
			quality			 = DEFAULT_QUALITY;
			pitch_family	 = DEFAULT_PITCH;
			face_name		 = L"Arial";
		}
	};

	class LIB_GFX_API font_manager
	{
	public:
		font_manager( void );

	public:
		font_manager( render_device* device );

		virtual ~font_manager( void );
#if _MSC_VER >= 1900
		typedef std::unordered_map<ustring, LPD3DXFONT> font_map;
#else
		typedef stdext::hash_map<ustring, LPD3DXFONT> font_map;
#endif


	public:
		void create(  render_device* device  );

		void destroy( void );

		void* get_font( font_description& font_desp );

	private:
		render_device* m_device;

		font_map m_font_map;
		 
	};

	class LIB_GFX_API singleton_font_manager
		: public singleton<font_manager>
	{
	public:
		singleton_font_manager( void )
		{
		}

		virtual ~singleton_font_manager( void )
		{
		}
	};
}

#endif // _US_FONT_MANAGER_H_