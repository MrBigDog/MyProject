///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_file_directory.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : win_directory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_FILE_DIRECTORY_H_
#define _US_FILE_DIRECTORY_H_

#include <usUtil/Export.h>
#include <usUtil/us_common_file.h>

#include <vector>
#include <wtypes.h>

namespace uniscope_globe
{
	typedef std::vector<ustring> file_list;

	class USUTIL_EXPORT file_directory
	{
	public:
		file_directory(void);

		virtual ~file_directory(void);

	public:
		static void get_file_list(cpstr str_dir, cpstr str_ext, file_list& out_files);

		static void create_directory(cpstr str_dir);

		static bool delete_file(cpstr str_dir);

		static bool delete_directory(cpstr str_dir);

	public:
		static void get_files(cpstr str_path, cpstr str_ext, file_list& out_files);

		static ustring get_file_ext(const LPCTSTR str_in_path);

		static ustring get_path_without_ext(const LPCTSTR str_in_path);

		static ustring get_file_name_without_ext(const LPCTSTR str_in_path);

		static ustring get_directory(const LPCTSTR str_in_path);

		static ustring get_file_name(const LPCTSTR str_in_path);

		static bool is_local_file(cpstr in_file_name);

		static ustring trim_start(const LPCTSTR base_path, const LPCTSTR str_trim);

		static ustring get_absolute_path(const LPCTSTR str_file_path, const LPCTSTR base_path);

		static ustring get_relative_path(const LPCTSTR str_file_path, const LPCTSTR base_path);

		static bool is_relative_path(const LPCTSTR str_file_path);

		static bool is_path_file_exist(const LPCTSTR str_file_path);

	};
}

#endif // _US_WIN_DIRECTORY_H_