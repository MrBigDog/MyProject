#ifndef GWBASE_PROPERTYINTERPOLATOR_H
#define GWBASE_PROPERTYINTERPOLATOR_H 1

#include <compiler.h>
#include <gwBase/Export.h>
#include <gwBase/EasingFunctions.h>

#include <osg/ref_ptr>
#include <osg/Referenced>

#include <memory>
#include <cassert>
#include <string>
#include <iostream>

class GWPropertyNode;

namespace gwBase
{
	class PropertyInterpolator;
	typedef osg::ref_ptr<PropertyInterpolator> PropertyInterpolatorRef;

	/**
	 * Base class for interpolating different types of properties over time.
	 */
	class GW_BASE_EXPORT PropertyInterpolator : public osg::Referenced
	{
	public:
		virtual ~PropertyInterpolator();

		/**
		 * Resets animation timer to zero and prepares for interpolation to new
		 * target value.
		 */
		void reset(const GWPropertyNode& target);

		/**
		 * Set easing function to be used for interpolation.
		 */
		void setEasingFunction(easing_func_t easing);

		/**
		 * Calculate an animation step.
		 *
		 * @param prop    Property being animated
		 * @param dt      Current frame duration
		 * @return Time not used by the animation (>= 0 if animation has finished,
		 *         else time is negative indicating the remaining time until
		 *         finished)
		 */
		double update(GWPropertyNode& prop, double dt);

		const std::string& getType() const { return _type; }

	protected:
		friend class PropertyInterpolationMgr;

		std::string               _type;
		easing_func_t             _easing;
		PropertyInterpolatorRef   _next;
		double                    _duration;
		double					  _cur_t;

		PropertyInterpolator();

		virtual void setTarget(const GWPropertyNode& target) = 0;
		virtual void init(const GWPropertyNode& prop) = 0;
		virtual void write(GWPropertyNode& prop, double t) = 0;
	};

	class GW_BASE_EXPORT NumericInterpolator :public PropertyInterpolator
	{
	protected:
		double _end, _diff;

		virtual void setTarget(const GWPropertyNode& target);
		virtual void init(const GWPropertyNode& prop);
		virtual void write(GWPropertyNode& prop, double t);
	};

} // namespace gwBase


#endif /* GWBASE_PROPERTYINTERPOLATOR_H */
