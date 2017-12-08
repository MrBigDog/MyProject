#include "us_object_base.h"

namespace uniscope_globe
{
	object_base::object_base(void)
	{
		m_rtti = US_RTTI_NONE;
		m_mem_size = 0;
	}

	object_base::~object_base(void) {}
}
