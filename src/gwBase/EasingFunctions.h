#ifndef GWBASE_EASINGFUNCTIONS_H
#define GWBASE_EASINGFUNCTIONS_H 1

#include <compiler.h>

namespace gwBase
{

	typedef double(*easing_func_t)(double);
	struct EasingMapEntry { const char* name; easing_func_t func; };

	//List of all available easing functions and their names.
	extern const EasingMapEntry easing_functions[];

} // namespace gwBase

#endif //GWBASE_EASINGFUNCTIONS_H