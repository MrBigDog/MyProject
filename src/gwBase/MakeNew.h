#ifndef GWBASE_MAKENEW_H
#define GWBASE_MAKENEW_H 1

#include <compiler.h>

namespace gwBase
{
	template<class T>
	T* make_new() { return new T; }

	template<class T, class A1>
	T* make_new(const A1& a1) { return new T(a1); }

	template<class T, class A1, class A2>
	T* make_new(const A1& a1, const A2& a2) { return new T(a1, a2); }

	template<class Base, class Derived>
	Base* make_new_derived() { return new Derived; }

	template<class Base, class Derived, class A1>
	Base* make_new_derived(const A1& a1) { return new Derived(a1); }

	template<class Base, class Derived, class A1, class A2>
	Base* make_new_derived(const A1& a1, const A2& a2)
	{
		return new Derived(a1, a2);
	}

	// Add more if needed (Variadic templates would be really nice!)
} // namespace gwBase

#endif /* GWBASE_MAKENEW_H */
