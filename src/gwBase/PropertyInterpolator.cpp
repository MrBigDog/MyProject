#include "PropertyInterpolator.h"
#include <gwBase/Props.h>

#include <cassert>
#include <cmath>

namespace gwBase
{
	PropertyInterpolator::~PropertyInterpolator()
	{
	}


	void PropertyInterpolator::reset(const GWPropertyNode& target)
	{
		_cur_t = 0;
		setTarget(target);
	}


	void PropertyInterpolator::setEasingFunction(easing_func_t easing)
	{
		_easing = easing ? easing : easing_functions[0].func;
	}


	double PropertyInterpolator::update(GWPropertyNode& prop, double dt)
	{
		if (_cur_t == 0)
		{
			init(prop);
		}

		_cur_t += dt / _duration;

		double unused = _cur_t - 1;
		if (unused > 0)
		{
			_cur_t = 1;
		}

		write(prop, _easing(_cur_t));

		if (_cur_t == 1)
		{
			// Reset timer to allow animation to be run again.
			_cur_t = 0;
		}

		return unused;
	}


	PropertyInterpolator::PropertyInterpolator()
		: _duration(1)
		, _cur_t(0)
	{
		setEasingFunction(0);
	}


	void NumericInterpolator::setTarget(const GWPropertyNode& target)
	{
		_end = target.getDoubleValue();
	}


	void NumericInterpolator::init(const GWPropertyNode& prop)
	{
		// If unable to get start value, immediately change to target value
		double value_start = prop.getType() == gwBase::props::NONE
			? _end : prop.getDoubleValue();

		_diff = _end - value_start;
	}


	void NumericInterpolator::write(GWPropertyNode& prop, double t)
	{
		double cur = _end - (1 - t) * _diff;

		if (prop.getType() == gwBase::props::INT || prop.getType() == gwBase::props::LONG)
		{
			prop.setLongValue(static_cast<long>(std::floor(cur + 0.5)));
		}
		else
		{
			prop.setDoubleValue(cur);
		}
	}

} // namespace gwBase
