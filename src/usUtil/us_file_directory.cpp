///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_file_directory.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : file_directory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_file_directory.h"

namespace uniscope_globe
{
	file_directory::file_directory( void )
	{

	}

	file_directory::~file_directory( void )
	{

	}

	void file_directory::get_files( cpstr str_path, cpstr str_ext, file_list& out_files )
	{
		ustring str_dir = ustring( str_path );
		if ( str_dir.rfind( L"\\" ) != ( str_dir.size() - 1 ) ) 
			str_dir += L"\\";

		ustring str_file = str_dir + L"*." + str_ext;

		WIN32_FIND_DATA fd;
		HANDLE h_file = NULL;

		BOOL v_working = true;
		for ( h_file = FindFirstFile( str_file.c_str(), &fd ); h_file != INVALID_HANDLE_VALUE; v_working = FindNextFile( h_file, &fd ) )
		{	
			if ( !v_working )
				break;

			if ( 0 == wcscmp( fd.cFileName, TEXT(".") ) || 0 == wcscmp(fd.cFileName, TEXT("..")) ) 
			{
				continue;
			}

			if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				continue;
			}

			out_files.push_back( fd.cFileName );
		}

		FindClose( h_file );
		h_file = NULL;
	}

	void file_directory::get_file_list( cpstr str_dir, cpstr str_ext, file_list& out_files )
	{
		ustring str_directory = ustring( str_dir ) + _gstr( "\\" );
		ustring str_file = str_directory + _gstr( "*." ) + str_ext;

		WIN32_FIND_DATA fd;
		HANDLE h_file = NULL;

		h_file = FindFirstFile( str_file.c_str(), &fd );
		if ( h_file != INVALID_HANDLE_VALUE )
		{
			if ( wcscmp( fd.cFileName, _gstr(".") ) != 0 && wcscmp(fd.cFileName, _gstr("..")) != 0 ) 
			{
				ustring str_file_path;
				str_file_path = str_directory + fd.cFileName;
				out_files.push_back( str_file_path );
			}
		}

		BOOL v_working = false;
		while( h_file )
		{	
			v_working = FindNextFile( h_file, &fd );
			if ( !v_working )
			{
				break;
			}

			if ( 0 == wcscmp( fd.cFileName, TEXT(".") ) || 0 == wcscmp(fd.cFileName, _gstr("..")) ) 
			{
				continue;
			}

			if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				get_file_list( ( str_directory + fd.cFileName ).c_str(), str_ext, out_files );
			}
			else 
			{
				ustring str_file_path;
				str_file_path = str_directory + fd.cFileName;
				out_files.push_back( str_file_path );
			}		
		}

		FindClose( h_file );
		h_file = NULL;
	}

	void file_directory::create_directory( cpstr str_dir )
	{
		ustring str_directory = str_dir;

		int v_pos  = 0;
		v_pos = str_directory.find_first_of( _gstr("\\"), v_pos );
		if ( v_pos == -1 )
		{
			v_pos = str_directory.find_first_of( _gstr("/"), 0 );
		}

		while( v_pos != -1 )
		{
			ustring str_sub = str_directory.substr( 0, v_pos );

			::CreateDirectory( str_sub.c_str(), NULL );

			int v_pos_bk = v_pos;
			v_pos = str_directory.find_first_of( _gstr("\\"), v_pos + 1 );
			if ( v_pos == -1 )
			{
				v_pos = str_directory.find_first_of( _gstr("/"), v_pos_bk + 1 );
			}
		}

	}

	bool file_directory::delete_file( cpstr str_dir )
	{
		return ::DeleteFile( str_dir );
	}

	bool file_directory::delete_directory( cpstr str_dir )
	{
		ustring str_directory = ustring( str_dir ) + _gstr( "\\" );
		ustring str_file = str_directory + _gstr( "*.*" );

		WIN32_FIND_DATA fd;
		HANDLE h_file = NULL;

		h_file = FindFirstFile( str_file.c_str(), &fd );
		if ( h_file != INVALID_HANDLE_VALUE )
		{
			if ( wcscmp( fd.cFileName, _gstr(".") ) != 0 && wcscmp(fd.cFileName, _gstr("..")) != 0 ) 
			{
				if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					delete_directory( (str_directory + fd.cFileName).c_str() );
				}
				else 
				{
					delete_file( (str_directory + fd.cFileName).c_str() );
				}		
			}
		}

		BOOL v_working = false;
		while( h_file )
		{	
			v_working = FindNextFile( h_file, &fd );
			if ( !v_working )
			{
				break;
			}

			if ( 0 == wcscmp( fd.cFileName, TEXT(".") ) || 0 == wcscmp(fd.cFileName, _gstr("..")) ) 
			{
				continue;
			}

			if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				delete_directory( (str_directory + fd.cFileName).c_str() );
			}
			else 
			{
				delete_file( (str_directory + fd.cFileName).c_str() );
			}		
		}

		FindClose( h_file );
		h_file = NULL;

		::RemoveDirectory( str_directory.c_str() );
		return true;
	}

	ustring file_directory::get_file_ext( const LPCTSTR str_in_path )
	{
		ustring str_in = str_in_path;
		int v_pos = (int)str_in.rfind( L"." );
		if ( v_pos > 0 )
			return str_in.substr( v_pos+1, str_in.size() - v_pos  );

		str_in.clear();
		return str_in;
	} 

	ustring file_directory::get_path_without_ext( const LPCTSTR str_in_path )
	{
		ustring str_in = str_in_path;
		int v_pos = (int)str_in.rfind( L"." );
		if ( v_pos > 0 )
			return str_in.substr( 0, v_pos );

		str_in.clear();
		return str_in;
	}

	ustring file_directory::get_file_name_without_ext( const LPCTSTR str_in_path )
	{
		ustring str_in = file_directory::get_file_name( str_in_path );
		int v_pos = (int)str_in.rfind( L"." );
		if ( v_pos > 0 )
			return str_in.substr( 0, v_pos );

		str_in.clear();
		return str_in;
	}

	ustring file_directory::get_directory( const LPCTSTR str_in_path )
	{
		ustring str_link = str_in_path;
		ustring::size_type v_pos = str_link.find_last_of( '/' );
		if(v_pos != ustring::npos)
		{
			str_link = str_link.substr( 0, v_pos + 1 );
		}
		else
		{
			ustring::size_type v_pos = str_link.find_last_of( '\\' );
			if(v_pos != ustring::npos)
				str_link = str_link.substr( 0, v_pos + 1 );
		}
		return str_link;
	}

	ustring file_directory::get_file_name( const LPCTSTR str_in_path )
	{
		if ( str_in_path == NULL )
			return L"";

		ustring str_file = str_in_path;

		// 修改处理顺序，先找'\\'后找'/'。用于解决网络路径后面又加了'\\'的情况
		ustring::size_type v_pos = str_file.find_last_of( '\\' );
		if ( v_pos != ustring::npos )
		{
			str_file = str_file.substr( v_pos + 1, str_file.size() - v_pos - 1 );
		}
		else
		{
			v_pos = str_file.find_last_of( '/' );
			if ( v_pos != ustring::npos )
				str_file = str_file.substr( v_pos + 1, str_file.size() - v_pos - 1 );
		}

// 		ustring::size_type v_pos = str_file.find_last_of( '/' );
// 		if ( v_pos != ustring::npos )
// 		{
// 			str_file = str_file.substr( v_pos + 1, str_file.size() - v_pos - 1 );
// 		}
// 		else
// 		{
// 			v_pos = str_file.find_last_of( '\\' );
// 			if ( v_pos != ustring::npos )
// 				str_file = str_file.substr( v_pos + 1, str_file.size() - v_pos - 1 );
// 		}

		return str_file;
	}

	bool file_directory::is_local_file( cpstr in_file_name )
	{
		ustring src_link = in_file_name;
		ustring::size_type v_pos1 = src_link.find( L"http://" );
		ustring::size_type v_pos2 = src_link.find( L"db://" );
		ustring::size_type v_pos3 = src_link.find( L"root://" );
		ustring::size_type v_pos4 = src_link.find( L"uds://" );

		if( v_pos1 != ustring::npos ||
			v_pos2 != ustring::npos ||
			v_pos3 != ustring::npos ||
			v_pos4 != ustring::npos )
		{
			return false;
		}

		return true;
	}


	ustring file_directory::trim_start( const LPCTSTR base_path, const LPCTSTR str_trim )
	{
		ustring str_path = base_path;
		US_STRING_LOWER( str_path );

		int v_pos = str_path.find( str_trim );
		if ( v_pos == 0 )
		{
			int trim_len = wcslen( str_trim );
			str_path = str_path.substr( trim_len, str_path.size() - trim_len ); 
		}

		return str_path;
	}

	bool file_directory::is_relative_path( const LPCTSTR str_file_path )
	{
		return PathIsRelative(str_file_path);
	}

	bool file_directory::is_path_file_exist( const LPCTSTR str_file_path )
	{
		return PathFileExists(str_file_path);
	}

	ustring file_directory::get_absolute_path( const LPCTSTR str_file_path, const LPCTSTR base_path )
	{
		ustring absolute_path = L"";
		if ( PathIsRelative(str_file_path) )
		{
			ustring str_base = file_directory::trim_start( base_path, L"file://" );
			absolute_path = file_directory::get_directory( str_base.c_str() ) + str_file_path;
		}
		else
		{
			absolute_path = str_file_path;
		}

		US_STRING_LOWER( absolute_path );

		return absolute_path;
	}

	ustring file_directory::get_relative_path( const LPCTSTR str_file_path, const LPCTSTR base_path)
	{
		ustring relative_path = L"";

		ustring file_path = file_directory::trim_start( str_file_path, L"file://" );
		ustring str_base = file_directory::trim_start( base_path, L"file://" );
		str_base = file_directory::get_directory( str_base.c_str() );

		int pos = file_path.find(str_base);
		if(pos != -1)
		{
			relative_path = file_path.substr(pos + str_base.size());
		}
		else
		{
			relative_path = file_path;
		}

		US_STRING_LOWER( relative_path );

		return relative_path;
	}
	
}