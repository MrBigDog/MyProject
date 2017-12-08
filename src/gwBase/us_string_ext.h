///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_string_ext.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : value_convert class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_VALUE_CONVERT_H_
#define _US_VALUE_CONVERT_H_

namespace uniscope_globe
{
	class LIB_UTIL_API string_ext
	{
	public:
		string_ext( void ) { }
		~string_ext( void ) { }

	public:
        // multibyte version
		static bool		to_bool( string value );

		static int		to_int ( string value );// base = 10

		static dword    to_dword ( string value );// base = 16

		static float	to_float( string value );

		static double	to_double( string value );

		static interior_index  to_int64( string value );// base = 16

        static wstring  to_wstring ( string value );

#ifndef _UNICODE
		static void     to_vector3_array ( string value, std::vector< vector3<double> >& vec3_array );
#endif //_UNICODE

	public:
		static string	from_bool( bool value );

		static string   from_int ( int value );// base = 10

		static string   from_dword ( dword value );// base = 16

		static string	from_float( float value );

		static string	from_double( double value );

		static string   from_int64 ( interior_index value );// base = 16

		static string   from_wstring ( wstring value );

		static string	from_vector3f( vector_3f& vec );

		static string	from_vector3d( vector_3d& vec );

		static string	from_lon_lat_rect( lon_lat_rect& v_rect );

		static string	from_quaternion_4d( quaternion_4d& v_quater );

		static string	from_quaternion_4f( quaternion_4f& v_quater );

		static string	from_vector_3d_array( std::vector<vector_3d>& in_vec_array );

		static string	from_vector_3f_array( std::vector<vector_3f>& in_vec_array );


	public:
        // unicode version
		static bool		wto_bool( wstring value );

		static int		wto_int( wstring value );

		static dword	wto_dword( wstring value );

		static float	wto_float( wstring value );

		static double	wto_double( wstring value );

		static interior_index	wto_int64( wstring value );

	public:
		// w from
		static wstring	wfrom_bool( bool value );

		static wstring	wfrom_int( int value );

		static wstring	wfrom_dword( dword value );

		static wstring	wfrom_float( float value );

		static wstring	wfrom_double( double value );

		static wstring	wfrom_int64( interior_index value );

		static wstring wfrom_vector3d( vector_3d& vec );

		static wstring wfrom_vector_3d_array( std::vector<vector_3d>& in_vec_array );

		static wstring wfrom_vector_3d_array_2d( std::vector<vector_3d>& in_vec_array );
	};

}

#endif // _US_VALUE_CONVERT_H_