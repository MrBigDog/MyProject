#include "EasingFunctions.h"
#include <gwUtil/Math.h>

namespace gwBase
{
	/// Simple linear easing.
	double easingLinear(double t)
	{
		return t;
	}

	/// http://easings.net/#easeInSine
	double easeInSine(double t)
	{
		return 1 - std::cos(t * /*M_PI_2*/gwUtil::Miscd::pi()*0.5);
	}

	/// http://easings.net/#easeOutSine
	double easeOutSine(double t)
	{
		return std::sin(t * /*M_PI_2*/gwUtil::Miscd::pi()*0.5);
	}

	/// http://easings.net/#easeInOutSine
	double easeInOutSine(double t)
	{
		return 0.5 - 0.5 * std::cos(t * /*M_PI*/gwUtil::Miscd::pi());
	}

	template<easing_func_t easeIn, easing_func_t easeOut>
	double easeInOut(double t)
	{
		if ((t *= 2) < 1)
			return 0.5 * (*easeIn)(t);
		else
			return 0.5 + 0.5 * (*easeOut)(t - 1);
	}

	/**
	 * Helper for exponential ease out with integer exponent.
	 *
	 * @tparam N      Exponent.
	 * @tparam is_odd If the exponent is odd.
	 */
	template<size_t N, bool is_odd>
	struct easeOutImpl;

	/**
	 * Ease out with odd integer exponent.
	 *
	 * @tparam N Exponent.
	 * @see http://easings.net/#easeOutCubic (N = 3)
	 * @see http://easings.net/#easeOutQuint (N = 5)
	 */
	template<size_t N>
	struct easeOutImpl<N, true>
	{
		static double calc(double t)
		{
			return gwUtil::Miscd::pow<N>(t - 1) + 1;
		}
	};

	/**
	 * Ease out with even integer exponent.
	 *
	 * @tparam N Exponent.
	 * @see http://easings.net/#easeOutQuad  (N = 2)
	 * @see http://easings.net/#easeOutQuart (N = 4)
	 */
	template<size_t N>
	struct easeOutImpl<N, false>
	{
		static double calc(double t)
		{
			return -gwUtil::Miscd::pow<N>(t - 1) + 1;
		}
	};

	/**
	 * Exponential ease out with integer exponent.
	 *
	 * @see http://easings.net/#easeOutQuad  (N = 2)
	 * @see http://easings.net/#easeOutCubic (N = 3)
	 * @see http://easings.net/#easeOutQuart (N = 4)
	 * @see http://easings.net/#easeOutQuint (N = 5)
	 * @see easeOutImpl
	 */
	template<size_t N>
	double easeOutPow(double t)
	{
		return easeOutImpl<N, N & 1>::calc(t);
	}

	/// http://easings.net/#easeInOutQuad  (N = 2)
	/// http://easings.net/#easeInOutCubic (N = 3)
	/// http://easings.net/#easeInOutQuart (N = 4)
	/// http://easings.net/#easeInOutQuint (N = 5)
	template<size_t N>
	double easeInOutPow(double t)
	{
		return easeInOut<&gwUtil::Miscd::pow<N>, &easeOutPow<N> >(t);
	}

	/// http://easings.net/#easeInExpo
	double easeInExpo(double t)
	{
		return (t == 0) ? 0 : std::pow(2, 10 * (t - 1));
	}

	/// http://easings.net/#easeOutExpo
	double easeOutExpo(double t)
	{
		return (t == 1) ? 1 : 1 - std::pow(2, -10 * t);
	}

	/// http://easings.net/#easeInCirc
	double easeInCirc(double t)
	{
		return 1 - std::sqrt(1 - gwUtil::Miscd::pow<2>(t));
	}

	/// http://easings.net/#easeOutCirc
	double easeOutCirc(double t)
	{
		return std::sqrt(1 - gwUtil::Miscd::pow<2>(t - 1));
	}

	static const double ease_s = 1.70158;

	/// http://easings.net/#easeInBack
	double easeInBack(double t)
	{

		return gwUtil::Miscd::pow<2>(t) * ((ease_s + 1) * t - ease_s);
	}

	/// http://easings.net/#easeOutBack
	double easeOutBack(double t)
	{
		t -= 1;
		return gwUtil::Miscd::pow<2>(t) * ((ease_s + 1) * t + ease_s) + 1;
	}

	/// http://easings.net/#easeOutBack
	double easeInElastic(double t)
	{
		if (t == 0)
			return 0;
		if (t == 1)
			return 1;

		t -= 1;
		const double p = .3;
		const double s = p * 0.25;

		return -std::pow(2, 10 * t) * std::sin((t - s) * 2 * /*M_PI*/gwUtil::Miscd::pi() / p);
	}

	/// http://easings.net/#easeOutBack
	double easeOutElastic(double t)
	{
		if (t == 0)
			return 0;
		if (t == 1)
			return 1;

		const double p = .3;
		const double s = p * 0.25;

		return std::pow(2, -10 * t) * std::sin((t - s) * 2 * /*M_PI*/gwUtil::Miscd::pi() / p) + 1;
	}

	/// http://easings.net/#easeOutBounce
	double easeOutBounce(double t)
	{
		if (t < 1 / 2.75)
			return 7.5625 * gwUtil::Miscd::pow<2>(t);
		else if (t < 2 / 2.75)
			return 7.5625 * gwUtil::Miscd::pow<2>(t - 1.5 / 2.75) + .75;
		else if (t < 2.5 / 2.75)
			return 7.5625 * gwUtil::Miscd::pow<2>(t - 2.25 / 2.75) + .9375;
		else
			return 7.5625 * gwUtil::Miscd::pow<2>(t - 2.625 / 2.75) + .984375;
	}

	/// http://easings.net/#easeInBounce
	double easeInBounce(double time)
	{
		return 1 - easeOutBounce(1 - time);
	}

#define SG_ADD_EASING(name) {#name, &name},
#define SG_STR(str) #str
#define SG_ADD_EASING_IN_OUT(name)\
          SG_ADD_EASING(easeIn##name)\
          SG_ADD_EASING(easeOut##name)\
          {SG_STR(easeInOut##name), &easeInOut<&easeIn##name, &easeOut##name>},

	const EasingMapEntry easing_functions[] = {
	  {"linear", &easingLinear},
	  {"swing", &easeInOutSine},
	  SG_ADD_EASING_IN_OUT(Sine)
	  {"easeInQuad",    &gwUtil::Miscd::pow<2>},
	  {"easeInCubic",   &gwUtil::Miscd::pow<3>},
	  {"easeInQuart",   &gwUtil::Miscd::pow<4>},
	  {"easeInQuint",   &gwUtil::Miscd::pow<5>},
	  {"easeOutQuad",   &easeOutPow<2>},
	  {"easeOutCubic",  &easeOutPow<3>},
	  {"easeOutQuart",  &easeOutPow<4>},
	  {"easeOutQuint",  &easeOutPow<5>},
	  {"easeInOutQuad", &easeInOutPow<2>},
	  {"easeInOutCubic",&easeInOutPow<3>},
	  {"easeInOutQuart",&easeInOutPow<4>},
	  {"easeInOutQuint",&easeInOutPow<5>},
	  SG_ADD_EASING_IN_OUT(Expo)
	  SG_ADD_EASING_IN_OUT(Circ)
	  SG_ADD_EASING_IN_OUT(Back)
	  SG_ADD_EASING_IN_OUT(Elastic)
	  SG_ADD_EASING_IN_OUT(Bounce)
	  {
  0, 0
}
	};

#undef SG_ADD_EASING
#undef SG_STR
#undef SG_ADD_EASING_IN_OUT

} // namespace simgear
