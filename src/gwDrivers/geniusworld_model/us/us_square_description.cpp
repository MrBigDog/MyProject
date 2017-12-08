#include "us_square_description.h"
#include "us_interior_index_factory.h"

namespace uniscope_globe
{
	square_description::square_description(void)
	{}

	square_description::~square_description(void)
	{}

	square_description::square_description(interior_index index)
	{
		m_index = index;
		m_level = interior_index_factory::get_level(index);
		m_bound = interior_index_factory::get_bound(index);
	}

	square_description::square_description(lon_lat_rect& rect, int level)
	{
		m_bound = rect;
		m_level = level;
		m_index = interior_index_factory::assemble(m_level, m_bound.get_center());
	}

	square_description::square_description(int level, double north, double south, double east, double west, int quadrant)
	{
		m_bound = lon_lat_rect(north, south, east, west);
		m_level = level;
		m_index = interior_index_factory::assemble(m_level, m_bound.get_center());
		m_quadrant = quadrant;
	}

	void square_description::create(interior_index index)
	{
		m_index = index;
		m_level = interior_index_factory::get_level(index);
		m_bound = interior_index_factory::get_bound(index);
	}

	void square_description::create(lon_lat_rect& rect, int level)
	{
		m_bound = rect;
		m_level = level;
		m_index = interior_index_factory::assemble(m_level, m_bound.get_center());
	}

	void square_description::create(int level, double north, double south, double east, double west)
	{
		m_bound = lon_lat_rect(north, south, east, west);
		m_level = level;
		m_index = interior_index_factory::assemble(m_level, m_bound.get_center());
	}


	square_description square_description::from_index(interior_index index)
	{
		square_description square_desc(index);

		return square_desc;
	}

	square_description square_description::from_rect(lon_lat_rect& rect, int level)
	{
		square_description square_desc(rect, level);

		return square_desc;
	}

	square_description square_description::from_rect(int level, double north, double south, double east, double west)
	{
		square_description square_desc(level, north, south, east, west);

		return square_desc;
	}
}