#include "us_circle.h"

namespace uniscope_globe
{
	circle::circle( vector3<double>& v_center, double v_radius )
	{
		m_center = v_center;
		m_radius = v_radius;
	}
}