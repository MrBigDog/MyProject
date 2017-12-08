#ifndef GWBASE_PROPERTYINTERPOLATIONMGR_H
#define GWBASE_PROPERTYINTERPOLATIONMGR_H 1

#include <compiler.h>
#include <gwBase/Export.h>
#include <gwBase/PropertyInterpolator.h>

#include <gwBase/Props.h>
#include <gwBase/MakeNew.h>
#include <gwBase/SubsystemMgr.h>

#include <list>
#include <map>

namespace gwBase
{
	/*** Subsystem that manages interpolation of properties.
	*
	* By default the numeric values of the properties are interpolated. For
	* example, for strings this is probably not the wanted behavior. For this
	* adapter classes can be registered to allow providing specific
	* interpolations for certain types of properties. Using the type "color",
	* provided by ColorInterpolator, strings containing %CSS colors can also be
	* interpolated.
	*
	* Additionally different functions can be used for easing of the animation.
	* By default "linear" (constant animation speed) and "swing" (smooth
	* acceleration and deceleration) are available.*/
	class GW_BASE_EXPORT PropertyInterpolationMgr : public Subsystem
	{
	public:
		typedef PropertyInterpolator* (*InterpolatorFactory)();

		PropertyInterpolationMgr();

		/*** Update all active interpolators.*/
		void update(double dt);

		/*** Create a new property interpolator.
		*
		* @note To actually use it the interpolator needs to be attached to a
		*       property using PropertyInterpolationMgr::interpolate.
		*
		* @param type        Type of animation ("numeric", "color", etc.)
		* @param target      Property containing target value
		* @param duration    Duration if the animation (in seconds)
		* @param easing      Type of easing ("linear", "swing", etc.)*/
		PropertyInterpolator* createInterpolator(const std::string& type,
			const GWPropertyNode& target,
			double duration,
			const std::string& easing);

		/*** Add animation of the given property from its current value to the
		* target value of the interpolator. If no interpolator is given any
		* existing animation of the given property is aborted.
		*
		* @param prop    Property to be interpolated
		* @param interp  Interpolator used for interpolation*/
		bool interpolate(GWPropertyNode* prop, PropertyInterpolatorRef interp = 0);

		bool interpolate(GWPropertyNode* prop,
			const std::string& type,
			const GWPropertyNode& target,
			double duration,
			const std::string& easing);

		bool interpolate(GWPropertyNode* prop,
			const std::string& type,
			const gwBase::PropertyList& values,
			const double_list& deltas,
			const std::string& easing);

		///* Register factory for interpolation type.
		void addInterpolatorFactory(const std::string& type, InterpolatorFactory factory);

		template<class T>
		void addInterpolatorFactory(const std::string& type)
		{
			addInterpolatorFactory(type, &gwBase::make_new_derived<PropertyInterpolator, T>);
		}

		///* Register easing function.
		void addEasingFunction(const std::string& type, easing_func_t func);

		/*** Set property containing real time delta (not sim time)
		* TODO better pass both deltas to all update methods...*/
		void setRealtimeProperty(GWPropertyNode* node);

	protected:
		typedef std::map<std::string, InterpolatorFactory> InterpolatorFactoryMap;
		typedef std::map<std::string, easing_func_t> EasingFunctionMap;
		typedef std::pair< GWPropertyNode*, PropertyInterpolatorRef > PropertyInterpolatorPair;
		typedef std::list<PropertyInterpolatorPair> InterpolatorList;

		struct PredicateIsSameProp;

		InterpolatorFactoryMap    _interpolator_factories;
		EasingFunctionMap         _easing_functions;
		InterpolatorList          _interpolators;

		GWPropertyNodePtr        _rt_prop;
	};

} // namespace GWBASE

#endif /* GWBASE_PROPERTYINTERPOLATIONMGR_H */
