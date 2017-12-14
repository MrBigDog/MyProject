#include "us_math.h"
#include "us_spherical_coords.h"
#include "us_interior_index_factory.h"

namespace uniscope_globe
{
	lon_lat_rect interior_index_factory::m_world_rect = lon_lat_rect(HALF_PI, -HALF_PI, PI, -PI);

	interior_index_factory::interior_index_factory(void)
	{

	}

	interior_index_factory::~interior_index_factory(void)
	{

	}

	interior_index interior_index_factory::assemble(int level, int longtitude_index, int latitude_index)
	{
		interior_index out_index;
		out_index = (interior_index)level << 48;
		out_index |= ((interior_index)longtitude_index & MASK_24_BITS) << 24;
		out_index |= latitude_index & MASK_24_BITS;
		return out_index;
	}

	interior_index interior_index_factory::assemble(int level, double longtitude, double latitude)
	{
		double v_bound_size_reciprocal = 1.0 / get_bound_size(level);
		int v_lon_index = int(floor((longtitude - m_world_rect.m_west) * v_bound_size_reciprocal));
		int v_lat_index = int(floor((latitude - m_world_rect.m_south) * v_bound_size_reciprocal));
		return assemble(level, v_lon_index, v_lat_index);

	}

	interior_index interior_index_factory::assemble(int level, vector3<double> in_vector)
	{
		return assemble(level, in_vector.x, in_vector.y);
	}

	void interior_index_factory::disassemble(interior_index index, int& level, int& longtitude_index, int& latitude_index)
	{
		level = INTERIOR_INDEX_LEVEL(index);
		longtitude_index = INTERIOR_INDEX_LON(index);
		latitude_index = INTERIOR_INDEX_LAT(index);
	}

	interior_index interior_index_factory::get_child(interior_index index, int quadrant)
	{
		int v_level = INTERIOR_INDEX_LEVEL(index) + 1;
		int v_lon_index = INTERIOR_INDEX_LON(index) << 1;
		int v_lat_index = INTERIOR_INDEX_LAT(index) << 1;

		interior_index v_child_index = 0;

		switch (quadrant)
		{
		case QUADRANT_SOUTH_WEST:
		{
			v_child_index = assemble(v_level, v_lon_index, v_lat_index);
		}
		break;

		case QUADRANT_NORTH_WEST:
		{
			v_child_index = assemble(v_level, v_lon_index, v_lat_index + 1);
		}
		break;
		case QUADRANT_SOUTH_EAST:
		{
			v_child_index = assemble(v_level, v_lon_index + 1, v_lat_index);
		}
		break;
		case QUADRANT_NORTH_EAST:
		{
			v_child_index = assemble(v_level, v_lon_index + 1, v_lat_index + 1);
		}
		break;

		default:
			break;
		}
		return v_child_index;

	}

	void interior_index_factory::get_children(interior_index index,
		interior_index& index_sw,
		interior_index& index_nw,
		interior_index& index_se,
		interior_index& index_ne)
	{
		int v_level = INTERIOR_INDEX_LEVEL(index) + 1;
		int v_lon_index = INTERIOR_INDEX_LON(index) << 1;
		int v_lat_index = INTERIOR_INDEX_LAT(index) << 1;

		index_sw = assemble(v_level, v_lon_index, v_lat_index);
		index_nw = assemble(v_level, v_lon_index, v_lat_index + 1);
		index_se = assemble(v_level, v_lon_index + 1, v_lat_index);
		index_ne = assemble(v_level, v_lon_index + 1, v_lat_index + 1);
	}

	interior_index interior_index_factory::get_parent(interior_index index, uint upper_level)
	{
		int v_level = INTERIOR_INDEX_LEVEL(index) - upper_level;
		int v_lon_index = INTERIOR_INDEX_LON(index) >> upper_level;
		int v_lat_index = INTERIOR_INDEX_LAT(index) >> upper_level;
		return assemble(v_level, v_lon_index, v_lat_index);
	}

	path_array interior_index_factory::get_path_array(interior_index upper_index, interior_index down_index)
	{
		path_array v_path_array;

		int v_upper_level = INTERIOR_INDEX_LEVEL(upper_index);
		int v_down_level = INTERIOR_INDEX_LEVEL(down_index);

		interior_index v_child_index = upper_index;
		interior_index v_parent_index = get_parent(v_child_index);

		for (int i = v_down_level; i < v_upper_level; i++)
		{
			vector3<double> v_parent_vector = get_bound(v_parent_index).get_center();
			vector3<double> v_child_vector = get_bound(v_parent_index).get_center();

			int v_quadrant = 0;
			if (v_child_vector.x > v_parent_vector.x)
				v_quadrant = 1 << 1;

			if (v_child_vector.y > v_parent_vector.y)
				v_quadrant = v_quadrant + 1;

			v_path_array.push_back(v_quadrant);

			v_child_index = v_parent_index;
			v_parent_index = get_parent(v_child_index);
		}
		return v_path_array;
	}

	interior_index_array interior_index_factory::get_index_array(lon_lat_rect& rect, uint level)
	{
		double v_bound_size_reciprocal = 1.0 / get_bound_size(level);
		int v_lon_index_min = int(floor((rect.m_west - m_world_rect.m_west) * v_bound_size_reciprocal));
		int v_lat_index_min = int(floor((rect.m_south - m_world_rect.m_south) * v_bound_size_reciprocal));
		int v_lon_index_max = int(floor((rect.m_east - m_world_rect.m_west) * v_bound_size_reciprocal));
		int v_lat_index_max = int(floor((rect.m_north - m_world_rect.m_south) * v_bound_size_reciprocal));

		interior_index_array v_index_array;
		for (int ni = v_lon_index_min; ni <= v_lon_index_max; ni++)
		{
			for (int nj = v_lat_index_min; nj <= v_lat_index_max; nj++)
			{
				interior_index out_index;
				out_index = (interior_index)level << 48;
				out_index |= ((interior_index)ni & MASK_24_BITS) << 24;
				out_index |= nj & MASK_24_BITS;
				v_index_array.push_back(out_index);
			}
		}

		return v_index_array;
	}

	int interior_index_factory::get_level(interior_index index)
	{
		return INTERIOR_INDEX_LEVEL(index);
	}

	int interior_index_factory::get_lon_index(interior_index index)
	{
		return INTERIOR_INDEX_LON(index);
	}

	int interior_index_factory::get_lat_index(interior_index index)
	{
		return INTERIOR_INDEX_LAT(index);
	}

	lon_lat_rect interior_index_factory::get_bound(interior_index index)
	{
		int v_level = INTERIOR_INDEX_LEVEL(index);
		int v_lon_index = INTERIOR_INDEX_LON(index);
		int v_lat_index = INTERIOR_INDEX_LAT(index);

		double v_bound_size = get_bound_size(v_level);

		lon_lat_rect v_rect;
		v_rect.m_west = v_lon_index  * v_bound_size + m_world_rect.m_west;
		v_rect.m_south = v_lat_index  * v_bound_size + m_world_rect.m_south;
		v_rect.m_east = (v_lon_index + 1) * v_bound_size + m_world_rect.m_west;
		v_rect.m_north = (v_lat_index + 1) * v_bound_size + m_world_rect.m_south;

		return v_rect;
	}

	double interior_index_factory::get_bound_size(int level)
	{
		return (m_world_rect.m_east - m_world_rect.m_west) / (1 << (level + 1));
	}

	ustring interior_index_factory::to_encrypt_string(interior_index index)
	{
		int level = INTERIOR_INDEX_LEVEL(index);
		long lon_index = INTERIOR_INDEX_LON(index);
		long lat_index = INTERIOR_INDEX_LAT(index);

		long lon1 = (lon_index & 0xff0000) >> 16;
		long lon2 = (lon_index & 0x00ff00) >> 8;
		long lat1 = (lat_index & 0xff0000) >> 16;
		long lat2 = (lat_index & 0x00ff00) >> 8;

		ustring str_path;
#ifdef WIN32

#ifdef _UNICODE
		wchar_t str_temp[MAX_FILE_PATH];
		swprintf(str_temp, MAX_FILE_PATH, L"%x/%x%x/%x%x/%I64x__", level, lon1, lat1, lon2, lat2, index);
		str_path = str_temp;
#else
		char str_temp[MAX_FILE_PATH];
		sprintf_s(str_temp, "%x/%x%x/%x%x/%I64x__", level, lon1, lat1, lon2, lat2, index);
		str_path = str_temp;
#endif //_UNICODE

#else

		char str_temp[MAX_FILE_PATH];
		sprintf(str_temp, "%x/%x%x/%x%x/%llx__", level, lon1, lat1, lon2, lat2, index);

#ifdef _UNICODE
		str_path = string_ext::to_wstring(str_temp);
#else
		str_path = str_temp;
#endif //_UNICODE


#endif //WIN32


		return str_path;
	}

	ustring interior_index_factory::to_wmts_string(interior_index index)
	{
		int level = INTERIOR_INDEX_LEVEL(index);
		long lon_index = INTERIOR_INDEX_LON(index);
		long lat_index = INTERIOR_INDEX_LAT(index);

		int sizeY = 1 << (level);

		lat_index = sizeY - lat_index - 1;


		ustring str_path;
#ifdef WIN32

#ifdef _UNICODE
		wchar_t str_temp[MAX_FILE_PATH];
		swprintf(str_temp, MAX_FILE_PATH, L"TileMatrix=%d&TileRow=%d&TileCol=%d", level, lat_index, lon_index);
		str_path = str_temp;
#else
		char str_temp[MAX_FILE_PATH];
		sprintf_s(str_temp, "TileMatrix=%d&TileRow=%d&TileCol=%d", level, lat_index, lon_index);
		str_path = str_temp;
#endif //_UNICODE

#else

		char str_temp[MAX_FILE_PATH];
		sprintf(str_temp, "TileMatrix=%d&TileRow=%d&TileCol=%d", level, lat_index, lon_index);

#ifdef _UNICODE
		str_path = string_ext::to_wstring(str_temp);
#else
		str_path = str_temp;
#endif //_UNICODE


#endif //WIN32

		return str_path;
	}

	ustring interior_index_factory::to_string(interior_index index)
	{
		int level = INTERIOR_INDEX_LEVEL(index);
		long lon_index = INTERIOR_INDEX_LON(index);
		long lat_index = INTERIOR_INDEX_LAT(index);

		long lon1 = (lon_index & 0xff0000) >> 16;
		long lon2 = (lon_index & 0x00ff00) >> 8;
		long lat1 = (lat_index & 0xff0000) >> 16;
		long lat2 = (lat_index & 0x00ff00) >> 8;

		ustring str_path;
#ifdef WIN32

#ifdef _UNICODE
		wchar_t str_temp[MAX_FILE_PATH];
		//swprintf( str_temp, MAX_FILE_PATH ,L"%x/%x%x/%x%x/%I64x", level, lon1, lat1, lon2, lat2, index );
		swprintf(str_temp, MAX_FILE_PATH, L"%x/%x%x/%x%x/%I64x", level, lon1, lat1, lon2, lat2, index);
		str_path = str_temp;
#else
		char str_temp[MAX_FILE_PATH];
		//sprintf( str_temp, "%x/%x%x/%x%x/%I64x", level, lon1, lat1, lon2, lat2, index );
		sprintf_s(str_temp, "%x/%x%x/%x%x/%I64x", level, lon1, lat1, lon2, lat2, index);
		str_path = str_temp;
#endif //_UNICODE

#else

		char str_temp[MAX_FILE_PATH];
		sprintf(str_temp, "%x/%x%x/%x%x/%llx", level, lon1, lat1, lon2, lat2, index);

#ifdef _UNICODE
		str_path = string_ext::to_wstring(str_temp);
#else
		str_path = str_temp;
#endif //_UNICODE


#endif //WIN32


		return str_path;
	}

	double interior_index_factory::calculate_elevation(int level, double fov, int view_port_height, int pixel_size)
	{
		double v_tile_width = interior_index_factory::get_bound_size(level) * spherical_coords::s_semi_major;

		double v_elevation = v_tile_width  * ((double)view_port_height * 0.5) / (pixel_size * math<double>::tan_(fov * 0.5));

		return v_elevation;
	}

	void interior_index_factory::get_int_rect(const lon_lat_rect& rect, int level, int& north_index, int& south_index, int& east_index, int& west_index)
	{
		double v_bound_size_reciprocal = 1.0 / get_bound_size(level);
		west_index = int(floor((rect.m_west - m_world_rect.m_west) * v_bound_size_reciprocal));
		south_index = int(floor((rect.m_south - m_world_rect.m_south) * v_bound_size_reciprocal));
		east_index = int(floor((rect.m_east - m_world_rect.m_west) * v_bound_size_reciprocal));
		north_index = int(floor((rect.m_north - m_world_rect.m_south) * v_bound_size_reciprocal));
	}









	//////////////////////////////////////////////////////////////////////////
	interior_index_helper::interior_index_helper(void)
	{
		m_world_rect = lon_lat_rect(HALF_PI, -HALF_PI, PI, -PI);
	}

	interior_index_helper::~interior_index_helper(void)
	{

	}

	interior_index interior_index_helper::assemble(int level, int longtitude_index, int latitude_index)
	{
		interior_index out_index;
		out_index = (interior_index)level << 48;
		out_index |= ((interior_index)longtitude_index & MASK_24_BITS) << 24;
		out_index |= latitude_index & MASK_24_BITS;
		return out_index;
	}

	interior_index interior_index_helper::assemble(int level, double longtitude, double latitude)
	{
		double v_bound_size_reciprocal = 1.0 / get_bound_size(level);
		int v_lon_index = int(floor((longtitude - m_world_rect.m_west) * v_bound_size_reciprocal));
		int v_lat_index = int(floor((latitude - m_world_rect.m_south) * v_bound_size_reciprocal));
		return assemble(level, v_lon_index, v_lat_index);

	}

	interior_index interior_index_helper::assemble(int level, vector3<double> in_vector)
	{
		return assemble(level, in_vector.x, in_vector.y);
	}

	void interior_index_helper::disassemble(interior_index index, int& level, int& longtitude_index, int& latitude_index)
	{
		level = INTERIOR_INDEX_LEVEL(index);
		longtitude_index = INTERIOR_INDEX_LON(index);
		latitude_index = INTERIOR_INDEX_LAT(index);
	}

	interior_index interior_index_helper::get_child(interior_index index, int quadrant)
	{
		int v_level = INTERIOR_INDEX_LEVEL(index) + 1;
		int v_lon_index = INTERIOR_INDEX_LON(index) << 1;
		int v_lat_index = INTERIOR_INDEX_LAT(index) << 1;

		interior_index v_child_index = 0;

		switch (quadrant)
		{
		case QUADRANT_SOUTH_WEST:
		{
			v_child_index = assemble(v_level, v_lon_index, v_lat_index);
		}
		break;

		case QUADRANT_NORTH_WEST:
		{
			v_child_index = assemble(v_level, v_lon_index, v_lat_index + 1);
		}
		break;
		case QUADRANT_SOUTH_EAST:
		{
			v_child_index = assemble(v_level, v_lon_index + 1, v_lat_index);
		}
		break;
		case QUADRANT_NORTH_EAST:
		{
			v_child_index = assemble(v_level, v_lon_index + 1, v_lat_index + 1);
		}
		break;

		default:
			break;
		}
		return v_child_index;

	}

	void interior_index_helper::get_children(interior_index index,
		interior_index& index_sw,
		interior_index& index_nw,
		interior_index& index_se,
		interior_index& index_ne)
	{
		int v_level = INTERIOR_INDEX_LEVEL(index) + 1;
		int v_lon_index = INTERIOR_INDEX_LON(index) << 1;
		int v_lat_index = INTERIOR_INDEX_LAT(index) << 1;

		index_sw = assemble(v_level, v_lon_index, v_lat_index);
		index_nw = assemble(v_level, v_lon_index, v_lat_index + 1);
		index_se = assemble(v_level, v_lon_index + 1, v_lat_index);
		index_ne = assemble(v_level, v_lon_index + 1, v_lat_index + 1);
	}

	interior_index interior_index_helper::get_parent(interior_index index, uint upper_level)
	{
		int v_level = INTERIOR_INDEX_LEVEL(index) - upper_level;
		int v_lon_index = INTERIOR_INDEX_LON(index) >> upper_level;
		int v_lat_index = INTERIOR_INDEX_LAT(index) >> upper_level;
		return assemble(v_level, v_lon_index, v_lat_index);
	}

	path_array interior_index_helper::get_path_array(interior_index upper_index, interior_index down_index)
	{
		path_array v_path_array;

		int v_upper_level = INTERIOR_INDEX_LEVEL(upper_index);
		int v_down_level = INTERIOR_INDEX_LEVEL(down_index);

		interior_index v_child_index = upper_index;
		interior_index v_parent_index = get_parent(v_child_index);

		for (int i = v_down_level; i < v_upper_level; i++)
		{
			vector3<double> v_parent_vector = get_bound(v_parent_index).get_center();
			vector3<double> v_child_vector = get_bound(v_parent_index).get_center();

			int v_quadrant = 0;
			if (v_child_vector.x > v_parent_vector.x)
				v_quadrant = 1 << 1;

			if (v_child_vector.y > v_parent_vector.y)
				v_quadrant = v_quadrant + 1;

			v_path_array.push_back(v_quadrant);

			v_child_index = v_parent_index;
			v_parent_index = get_parent(v_child_index);
		}
		return v_path_array;
	}

	interior_index_array interior_index_helper::get_index_array(lon_lat_rect& rect, uint level)
	{
		double v_bound_size_reciprocal = 1.0 / get_bound_size(level);
		int v_lon_index_min = int(floor((rect.m_west - m_world_rect.m_west) * v_bound_size_reciprocal));
		int v_lat_index_min = int(floor((rect.m_south - m_world_rect.m_south) * v_bound_size_reciprocal));
		int v_lon_index_max = int(floor((rect.m_east - m_world_rect.m_west) * v_bound_size_reciprocal));
		int v_lat_index_max = int(floor((rect.m_north - m_world_rect.m_south) * v_bound_size_reciprocal));

		interior_index_array v_index_array;
		for (int ni = v_lon_index_min; ni <= v_lon_index_max; ni++)
		{
			for (int nj = v_lat_index_min; nj <= v_lat_index_max; nj++)
			{
				interior_index out_index;
				out_index = (interior_index)level << 48;
				out_index |= ((interior_index)ni & MASK_24_BITS) << 24;
				out_index |= nj & MASK_24_BITS;
				v_index_array.push_back(out_index);
			}
		}

		return v_index_array;
	}

	int interior_index_helper::get_level(interior_index index)
	{
		return INTERIOR_INDEX_LEVEL(index);
	}

	int interior_index_helper::get_lon_index(interior_index index)
	{
		return INTERIOR_INDEX_LON(index);
	}

	int interior_index_helper::get_lat_index(interior_index index)
	{
		return INTERIOR_INDEX_LAT(index);
	}

	lon_lat_rect interior_index_helper::get_bound(interior_index index)
	{
		int v_level = INTERIOR_INDEX_LEVEL(index);
		int v_lon_index = INTERIOR_INDEX_LON(index);
		int v_lat_index = INTERIOR_INDEX_LAT(index);

		double v_bound_size = get_bound_size(v_level);

		lon_lat_rect v_rect;
		v_rect.m_west = v_lon_index  * v_bound_size + m_world_rect.m_west;
		v_rect.m_south = v_lat_index  * v_bound_size + m_world_rect.m_south;
		v_rect.m_east = (v_lon_index + 1) * v_bound_size + m_world_rect.m_west;
		v_rect.m_north = (v_lat_index + 1) * v_bound_size + m_world_rect.m_south;

		return v_rect;
	}

	double interior_index_helper::get_bound_size(int level)
	{
		return (m_world_rect.m_east - m_world_rect.m_west) / (1 << (level + 1));
	}

	ustring interior_index_helper::to_encrypt_string(interior_index index)
	{
		int level = INTERIOR_INDEX_LEVEL(index);
		long lon_index = INTERIOR_INDEX_LON(index);
		long lat_index = INTERIOR_INDEX_LAT(index);

		long lon1 = (lon_index & 0xff0000) >> 16;
		long lon2 = (lon_index & 0x00ff00) >> 8;
		long lat1 = (lat_index & 0xff0000) >> 16;
		long lat2 = (lat_index & 0x00ff00) >> 8;

		ustring str_path;
#ifdef WIN32

#ifdef _UNICODE
		wchar_t str_temp[MAX_FILE_PATH];
		swprintf(str_temp, MAX_FILE_PATH, L"%x/%x%x/%x%x/%I64x__", level, lon1, lat1, lon2, lat2, index);
		str_path = str_temp;
#else
		char str_temp[MAX_FILE_PATH];
		sprintf_s(str_temp, "%x/%x%x/%x%x/%I64x__", level, lon1, lat1, lon2, lat2, index);
		str_path = str_temp;
#endif //_UNICODE

#else

		char str_temp[MAX_FILE_PATH];
		sprintf(str_temp, "%x/%x%x/%x%x/%llx__", level, lon1, lat1, lon2, lat2, index);

#ifdef _UNICODE
		str_path = string_ext::to_wstring(str_temp);
#else
		str_path = str_temp;
#endif //_UNICODE


#endif //WIN32


		return str_path;
	}



	ustring interior_index_helper::to_string(interior_index index)
	{
		int level = INTERIOR_INDEX_LEVEL(index);
		long lon_index = INTERIOR_INDEX_LON(index);
		long lat_index = INTERIOR_INDEX_LAT(index);

		long lon1 = (lon_index & 0xff0000) >> 16;
		long lon2 = (lon_index & 0x00ff00) >> 8;
		long lat1 = (lat_index & 0xff0000) >> 16;
		long lat2 = (lat_index & 0x00ff00) >> 8;

		ustring str_path;
#ifdef WIN32

#ifdef _UNICODE
		wchar_t str_temp[MAX_FILE_PATH];
		//swprintf( str_temp, MAX_FILE_PATH ,L"%x/%x%x/%x%x/%I64x", level, lon1, lat1, lon2, lat2, index );
		swprintf(str_temp, MAX_FILE_PATH, L"%x/%x%x/%x%x/%I64x", level, lon1, lat1, lon2, lat2, index);
		str_path = str_temp;
#else
		char str_temp[MAX_FILE_PATH];
		//sprintf( str_temp, "%x/%x%x/%x%x/%I64x", level, lon1, lat1, lon2, lat2, index );
		sprintf_s(str_temp, "%x/%x%x/%x%x/%I64x", level, lon1, lat1, lon2, lat2, index);
		str_path = str_temp;
#endif //_UNICODE

#else

		char str_temp[MAX_FILE_PATH];
		sprintf(str_temp, "%x/%x%x/%x%x/%llx", level, lon1, lat1, lon2, lat2, index);

#ifdef _UNICODE
		str_path = string_ext::to_wstring(str_temp);
#else
		str_path = str_temp;
#endif //_UNICODE


#endif //WIN32


		return str_path;
	}

	double interior_index_helper::calculate_elevation(int level, double fov, int view_port_height, int pixel_size)
	{
		double v_tile_width = interior_index_helper::get_bound_size(level) * spherical_coords::s_semi_major;

		double v_elevation = v_tile_width  * ((double)view_port_height * 0.5) / (pixel_size * math<double>::tan_(fov * 0.5));

		return v_elevation;
	}

	void interior_index_helper::get_int_rect(const lon_lat_rect& rect, int level, int& north_index, int& south_index, int& east_index, int& west_index)
	{
		double v_bound_size_reciprocal = 1.0 / get_bound_size(level);
		west_index = int(floor((rect.m_west - m_world_rect.m_west) * v_bound_size_reciprocal));
		south_index = int(floor((rect.m_south - m_world_rect.m_south) * v_bound_size_reciprocal));
		east_index = int(floor((rect.m_east - m_world_rect.m_west) * v_bound_size_reciprocal));
		north_index = int(floor((rect.m_north - m_world_rect.m_south) * v_bound_size_reciprocal));
	}




}