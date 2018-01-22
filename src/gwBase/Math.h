#ifndef GWBASE_MATH_H
#define GWBASE_MATH_H 1

#include <osg/Vec2d>
#include <osg/Vec3d>
#include <osg/Vec4d>
#include <osg/Uniform>
#include <osg/Matrixd>
#include <osg/Matrixf>
#include <osg/Plane>
#include <assert.h>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

#ifdef WIN32
#ifndef __MINGW32__
#ifdef __MSC_VER
# if __MSC_VER < 1600
#   include <xmath.h>
# endif
#endif
#define M_LOG2E    1.44269504088896340736
#endif
#endif

// avoid an 'unused parameter' compiler warning.
#define GW_UNUSED(x) (void)x

// easy way to disable the copy constructor and assignment operator
// on an object
#define GW_DISABLE_COPY(Class) \
    Class(const Class &); \
    Class &operator=(const Class &);

template<typename T>
class GWLimits : public std::numeric_limits<T> {};

namespace gwBase
{
	template<typename T>
	class Misc
	{
	public:
		static T pi() { return T(3.1415926535897932384626433832795029L); }
		static T twopi() { return 2 * T(3.1415926535897932384626433832795029L); }

		static T min(const T& a, const T& b)
		{
			return a < b ? a : b;
		}
		static T min(const T& a, const T& b, const T& c)
		{
			return min(min(a, b), c);
		}
		static T min(const T& a, const T& b, const T& c, const T& d)
		{
			return min(min(min(a, b), c), d);
		}
		static T max(const T& a, const T& b)
		{
			return a > b ? a : b;
		}
		static T max(const T& a, const T& b, const T& c)
		{
			return max(max(a, b), c);
		}
		static T max(const T& a, const T& b, const T& c, const T& d)
		{
			return max(max(max(a, b), c), d);
		}

		// clip the value of a to be in the range between and including _min and _max
		static T clip(const T& a, const T& _min, const T& _max)
		{
			return max(_min, min(_max, a));
		}

		/// Add two (integer) values taking care of overflows.
		static T addClipOverflow(T a, T b)
		{
			if (b > 0)
			{
				if (Limits<T>::max() - b < a)
					return Limits<T>::max();
			}
			else
			{
				if (Limits<T>::min() - b > a)
					return Limits<T>::min();
			}

			return a + b;
		}

		/// Add two (integer) values in place, taking care of overflows.
		static T& addClipOverflowInplace(T& a, T b)
		{
			return a = addClipOverflow(a, b);
		}

		/**
		* Seek a variable towards a target value with given rate and timestep
		*
		* @param var     Variable or eg. SGPropObj
		* @param target  Target value
		* @param rate    Max. change rate/sec
		* @param dt      Time step (sec)
		*/
		template<class Var>
		static T seek(Var& var, T target, T rate, T dt)
		{
			if (var < target)
			{
				return var = min(var + rate * dt, target);
			}
			else
			{
				return var = max(var - rate * dt, target);
			}
		}

		/**
		* Get @c base raised to the power of @c N
		*
		* @tparam N      Exponent
		* @param base    Base
		*/
		template<int N>
		static T pow(T base)
		{
			return (N < 0)
				? (1. / pow<-N>(base))
				: (((N & 1) ? base : 1) * ((N > 1) ? pow<N / 2>(base * base) : 1));
		}

		static int sign(const T& a)
		{
			if (a < -Limits<T>::min()) return -1;
			else if (Limits<T>::min() < a) return 1;
			else return 0;
		}

		static T rad2deg(const T& val)
		{
			return val * 180 / pi();
		}
		static T deg2rad(const T& val)
		{
			return val*pi() / 180;
		}

		// normalize the value to be in a range between [min, max[
		static T normalizePeriodic(const T& min, const T& max, const T& value)
		{
			T range = max - min;
			if (range < Limits<T>::min()) return min;
			T normalized = value - range*floor((value - min) / range);
			// two security checks that can only happen due to roundoff
			if (normalized <= min) return min;
			if (max <= normalized) return min;
			return normalized;
		}

		// normalize the angle to be in a range between [-pi, pi[
		static T normalizeAngle(const T& angle)
		{
			return normalizePeriodic(-pi(), pi(), angle);
		}

		// normalize the angle to be in a range between [0, 2pi[
		static T normalizeAngle2(const T& angle)
		{
			return normalizePeriodic(0, twopi(), angle);
		}

		static T round(const T& v)
		{
			return floor(v + T(0.5));
		}

		static int roundToInt(const T& v)
		{
			return int(round(v));
		}

		// Linear interpolation between two arbitrary typed values
		template<typename S>
		static S lerp(const S& val0, const S& val1, const T& t)
		{
			return val0*(T(1) - t) + val1*t;
		}

		/// Returns true if v is a NaN value
		/// Use with care: allways code that you do not need to use that!
		static bool isNaN(const T& v)
		{
			return std::isnan(v);
		}
	};

	typedef Misc<float> Miscf;
	typedef Misc<double> Miscd;







	// return the sign of a value
	template <class T>
	inline int Sign(const T x)
	{
		return x < T(0) ? -1 : 1;
	}

	// return the minimum of two values
	template <class T>
	inline T Min2(const T a, const T b)
	{
		return a < b ? a : b;
	}

	// return the minimum of three values
	template <class T>
	inline T Min3(const T a, const T b, const T c)
	{
		return (a < b ? Min2(a, c) : Min2(b, c));
	}

	// return the maximum of two values
	template <class T>
	inline T Max2(const T a, const T b)
	{
		return  a > b ? a : b;
	}

	// return the maximum of three values
	template <class T>
	inline T Max3(const T a, const T b, const T c)
	{
		return (a > b ? Max2(a, c) : Max2(b, c));
	}

	// return the minimum and maximum of three values
	template <class T>
	inline void MinMax3(T &min, T &max, const T a, const T b, const T c)
	{
		if (a > b)
		{
			if (a > c)
			{
				max = a;
				min = Min2(b, c);
			}
			else
			{
				max = c;
				min = Min2(a, b);
			}
		}
		else
		{
			if (b > c)
			{
				max = b;
				min = Min2(a, c);
			}
			else
			{
				max = c;
				min = Min2(a, b);
			}
		}
	}

	// swap two values
	template <class T>
	inline void Swap(T &a, T &b)
	{
		T c = a;  a = b;  b = c;
	}

	// clamp a value to lie between min and max
	template <class T>
	inline void ClampRange(T &x, const T min, const T max)
	{
		if (x < min) { x = min; }
		if (x > max) { x = max; }
	}

	// normalize a value to lie between min and max
	template <class T>
	inline void NormalizeRange(T &val, const T min, const T max)
	{
		T step = max - min;
		while (val >= max)  val -= step;
		while (val < min) val += step;
	}

	inline double fadeCoefficient(double currentValue, double referenceValue)
	{
		return exp(-currentValue / referenceValue);
	}

	template <typename S, typename T>
	inline T lerp(S x, T begin, T end)
	{
		return x*(end - begin) + begin;
	}

	template <typename T>
	inline T inverseLerp(T x, T begin, T end)
	{
		return begin != end ? (x - begin) / (end - begin) : T(0);
	}

	template <typename S, typename T>
	T quaderp(S x, T begin, T middle, T end)
	{
		const T a1 = S(2)*(middle - begin);
		const T a2 = S(2)*(end - middle) - a1;
		return x*((x - S(0.5))*a2 + a1) + begin;
	}

	template <typename T>
	inline T step(T x, T a)
	{
		return T(x >= a);
	}

	template <typename T>
	T smoothstep(T x, T a, T b)
	{
		assert(a <= b);
		T t = ClampRange((x - a) / (b - a), T(0), T(1));
		return t*t*(T(3) - T(2)*t);
	}

	template <typename T>
	T boxpulse(T x, T a, T b)
	{
		assert(a <= b);
		return inInterval(x, a, b);
	}

	template <typename T>
	T wrap(T x, T a, T b)
	{
		assert(a <= b);
		if (a == b)
		{
			return a;
		}
		else
		{
			T z = x < a ? b : a;
			return std::fmod(x - z, b - a) + z;
		}
	}

	// this implementation avoids a conditional branch
	inline float negativeOneNthPow(int n)
	{
		const float possibleResults[] = { 1.0, -1.0 };
		return possibleResults[n & 0x1];
	}


	template <typename T>
	std::pair<T, T> quadraticRoots(T a, T b, T c)
	{
		T s = sqrt(b*b - T(4)*a*c);
		T twoA = T(2)*a;
		return std::pair<T, T>((-b + s) / twoA, (-b - s) / twoA);
	}

	template <typename T>
	inline T fmodPos(T a, T b)
	{
		T x = std::fmod(a, b);
		return (x < T(0)) ? x + b : x;
	}

	template <typename T>
	inline T fmodPosAvg(T a, T b)
	{
		return fmodPos(a + b*T(0.5), b) - b*T(0.5);
	}

	template <typename T>
	inline T fmodPosClosest(T a, T b, T c)
	{
		return c + fmodPosAvg(a - c, b);
	}

	// x's binary reflected Gray code
	template <typename T>
	inline T grayCode(T x)
	{
		return (x >> 1) ^ x;
	}

	// is x a power of 2?  T should be integral
	template <typename T>
	inline bool isPower2(T x)
	{
		return !(x & (x - 1)) && x;
	}

	inline double round(double x)
	{
		return (x > 0) ? floor(x + 0.5) : ceil(x - 0.5);
	}

	template <typename T>
	inline T logbase(T x, T base)
	{
		return log(x) / log(base);
	}

	inline double log2(double x)
	{
		return M_LOG2E*log(x);
	}

	inline int log2int(double x)
	{
		return (int)round(log2(x));
	}

	// return the smallest power of two that is >= x; T should be integral
	template <typename T>
	T ceilPow2(T x)
	{
		T result(1);
		while (result < x)
		{
			assert(result != 0);  // assert we don't overflow result 
			result <<= 1;
		}
		return result;
	}

	// convert "modified" spherical coordinates (in radians) to cartesian coords.
	// "modified" since angle measurements don't match mathematicians' standard conventions,
	// instead use phi = lat and theta = lon convention, but in radians
	inline osg::Vec3d modsphere2xyz(double radius, double theta, double phi)
	{
		return osg::Vec3d(radius*cos(phi)*cos(theta), radius*cos(phi)*sin(theta), radius*sin(phi));
	}

	// convenient streaming operators not defined by osg
	inline std::ostream& operator<<(std::ostream& s, const osg::Vec3f& v)
	{
		return (s << v.x() << ' ' << v.y() << ' ' << v.z());
	}
	inline std::ostream& operator<<(std::ostream& s, const osg::Vec4f& v)
	{
		return (s << v.x() << ' ' << v.y() << ' ' << v.z() << ' ' << v.w());
	}
	inline std::ostream& operator<<(std::ostream& s, const osg::Vec3d& v)
	{
		return (s << v.x() << ' ' << v.y() << ' ' << v.z());
	}
	inline std::ostream& operator<<(std::ostream& s, const osg::Vec4d& v)
	{
		return (s << v.x() << ' ' << v.y() << ' ' << v.z() << ' ' << v.w());
	}

	template <typename T>
	inline void memClear(T& var) // zero out a variable's memory
	{
		memset(&var, 0, sizeof(T));
	}

	//////////////////////////////////////////////////////////////////////////

	inline osg::Vec3f normalize(const osg::Vec3f& v)
	{
		osg::Vec3f vv = v;
		vv.normalize();
		return vv;
	}

	inline osg::Vec3d normalize(const osg::Vec3d& v)
	{
		osg::Vec3d vv = v;
		vv.normalize();
		return vv;
	}

	inline bool isVec2Equal(const osg::Vec2f &p1, const osg::Vec2f &p2, float epsilon = FLT_EPSILON)
	{
		if (!osg::equivalent(p1.x(), p2.x(), epsilon)) return false;
		if (!osg::equivalent(p1.y(), p2.y(), epsilon)) return false;
		return true;
	}

	inline bool isVec2Equal(const osg::Vec2d &p1, const osg::Vec2d &p2, double epsilon = DBL_EPSILON)
	{
		if (!osg::equivalent(p1.x(), p2.x(), epsilon)) return false;
		if (!osg::equivalent(p1.y(), p2.y(), epsilon)) return false;
		return true;
	}

	inline bool isVec3Equal(const osg::Vec3f &p1, const osg::Vec3f &p2, float epsilon = FLT_EPSILON)
	{
		if (!osg::equivalent(p1.x(), p2.x(), epsilon)) return false;
		if (!osg::equivalent(p1.y(), p2.y(), epsilon)) return false;
		if (!osg::equivalent(p1.z(), p2.z(), epsilon)) return false;
		return true;
	}

	inline bool isVec3Equal(const osg::Vec3d &p1, const osg::Vec3d &p2, double epsilon = DBL_EPSILON)
	{
		if (!osg::equivalent(p1.x(), p2.x(), epsilon)) return false;
		if (!osg::equivalent(p1.y(), p2.y(), epsilon)) return false;
		if (!osg::equivalent(p1.z(), p2.z(), epsilon)) return false;
		return true;
	}

	inline bool isVec4Equal(const osg::Vec4f &p1, const osg::Vec4f &p2, float epsilon = FLT_EPSILON)
	{
		if (!osg::equivalent(p1.x(), p2.x(), epsilon)) return false;
		if (!osg::equivalent(p1.y(), p2.y(), epsilon)) return false;
		if (!osg::equivalent(p1.z(), p2.z(), epsilon)) return false;
		if (!osg::equivalent(p1.w(), p2.w(), epsilon)) return false;
		return true;
	}

	inline bool isVec4Equal(const osg::Vec4d &p1, const osg::Vec4d &p2, double epsilon = DBL_EPSILON)
	{
		if (!osg::equivalent(p1.x(), p2.x(), epsilon)) return false;
		if (!osg::equivalent(p1.y(), p2.y(), epsilon)) return false;
		if (!osg::equivalent(p1.z(), p2.z(), epsilon)) return false;
		if (!osg::equivalent(p1.w(), p2.w(), epsilon)) return false;
		return true;
	}

	inline bool isMatrixEqual(const osg::Matrixf &m1, const osg::Matrixf &m2, float epsilon = FLT_EPSILON)
	{
		const osg::Matrixf::value_type *p1 = m1.ptr();
		const osg::Matrixf::value_type *p2 = m2.ptr();
		for (unsigned int i = 0; i < 16; ++i)
		{
			if (!osg::equivalent(p1[i], p2[i], epsilon))
			{
				return false;
			}
		}
		return true;
	}

	inline bool isMatrixEqual(const osg::Matrixd &m1, const osg::Matrixd &m2, double epsilon = DBL_EPSILON)
	{
		const osg::Matrixd::value_type *p1 = m1.ptr();
		const osg::Matrixd::value_type *p2 = m2.ptr();
		for (unsigned int i = 0; i < 16; ++i)
		{
			if (!osg::equivalent(p1[i], p2[i], epsilon))
			{
				return false;
			}
		}
		return true;
	}

	inline bool isIdentity(const osg::Matrixf &mat)
	{
		osg::Matrixf ident;
		return isMatrixEqual(mat, ident);
	}

	inline bool isIdentity(const osg::Matrixd &mat)
	{
		osg::Matrixd ident;
		return isMatrixEqual(mat, ident);
	}

	inline osg::Matrixd asMatrix4(const osg::Matrix3d& mat)
	{
		osg::Matrixd mat4;
		mat4(0, 0) = mat(0, 0);
		mat4(0, 1) = mat(0, 1);
		mat4(0, 2) = mat(0, 2);
		mat4(0, 3) = 0.0;
		mat4(1, 0) = mat(1, 0);
		mat4(1, 1) = mat(1, 1);
		mat4(1, 2) = mat(1, 2);
		mat4(1, 3) = 0.0;
		mat4(2, 0) = mat(2, 0);
		mat4(2, 1) = mat(2, 1);
		mat4(2, 2) = mat(2, 2);
		mat4(2, 3) = 0.0;
		mat4(3, 0) = 0.0;
		mat4(3, 1) = 0.0;
		mat4(3, 2) = 0.0;
		mat4(3, 3) = 1.0;

		return mat4;
	}

	inline double getMaxDistance() { return 1.0e10; }

#define OSG_INNER_PRODUCT_3X3(a,b,r,c) \
	(((a(r,0)) * (b(0,c))) \
	+((a(r,1)) * (b(1,c))) \
	+((a(r,2)) * (b(2,c)))) 

	inline void mult3x3(osg::Matrixd& result, const osg::Matrixd& lhs, const osg::Matrixd& rhs)
	{
		result(0, 0) = OSG_INNER_PRODUCT_3X3(lhs, rhs, 0, 0);
		result(0, 1) = OSG_INNER_PRODUCT_3X3(lhs, rhs, 0, 1);
		result(0, 2) = OSG_INNER_PRODUCT_3X3(lhs, rhs, 0, 2);
		result(1, 0) = OSG_INNER_PRODUCT_3X3(lhs, rhs, 1, 0);
		result(1, 1) = OSG_INNER_PRODUCT_3X3(lhs, rhs, 1, 1);
		result(1, 2) = OSG_INNER_PRODUCT_3X3(lhs, rhs, 1, 2);
		result(2, 0) = OSG_INNER_PRODUCT_3X3(lhs, rhs, 2, 0);
		result(2, 1) = OSG_INNER_PRODUCT_3X3(lhs, rhs, 2, 1);
		result(2, 2) = OSG_INNER_PRODUCT_3X3(lhs, rhs, 2, 2);
	}


	struct LineSegmentd
	{
		LineSegmentd(const osg::Vec3d& s, const osg::Vec3d& e)
			:_s(s), _e(e)
		{}
		osg::Vec3d _s, _e;
	};
	typedef std::vector<LineSegmentd> LineSegmentVec;


	double distPointToSegment(const osg::Vec3d& P, LineSegmentd S)
	{
		osg::Vec3d v = S._e - S._s;
		osg::Vec3d w = P - S._s;

		double c1 = w*v;
		if (c1 <= 0) return (P - S._s).length();

		double c2 = v* v;
		if (c2 <= c1) return (P - S._e).length();

		double b = c1 / c2;
		osg::Vec3d Pb = S._s + v*b;

		return (P - Pb).length();
	}

	//inline bool isPointInLineSegment(const osg::Vec3d& s, const osg::Vec3d& e, const osg::Vec3d& p)
	//{
	//	if (isVec3Equal(s, p) || isVec3Equal(e, p))
	//	{
	//		return true;
	//	}
	//	double disse = (e - s).length();
	//	if ((p - s).length() > disse || (p - e).length() > disse)
	//	{
	//		return false;
	//	}

	//	return true;
	//}

	//inline double getSkewLinesDistance(const osg::Vec3d &r1, const osg::Vec3d &r2, const osg::Vec3d &v1, const osg::Vec3d &v2)
	//{
	//	osg::Vec3d u1 = r2 - r1;
	//	osg::Vec3d u2 = v2 - v1;
	//	osg::Vec3d u3 = u1^u2;
	//	osg::Vec3d dir = r1 - v2;
	//	if (u3.length() == 0)
	//	{
	//		return 1;
	//	}

	//	return std::fabs((dir*u3) / u3.length());
	//}


	////double t;
	////osg::Vec3d p = s1 + v1*t;
	////osg::Vec3d ps2 = p - s2;
	////osg::Vec3d res = ps2 ^ v2;

	////(v3 + v1*t) ^ v2;
	////v1^v2*t + v3^v2;

	////(v3[1] + v1[1] * t)*v2[2] - (v3[2] + v1[2] * t)*v2[1];
	////求两条线段的交点;
	//inline bool isLinesegmentIntersect(const osg::Vec3d& s1, const osg::Vec3d&e1, const osg::Vec3d& s2, const osg::Vec3d&e2, osg::Vec3d& hit)
	//{
	//	if (!osg::equivalent(osg::computeVolume(s1, e1, s2, e2), 0.0f))
	//	{
	//		return false;
	//	}

	//	osg::Vec3d v1 = e1 - s1;
	//	osg::Vec3d v2 = e2 - s2;
	//	osg::Vec3d v3 = s1 - s2;
	//	osg::Vec3d v4 = -(v3^v2);
	//	osg::Vec3d v5 = (v1^v2);

	//	//有v5*t=v4
	//	double t;
	//	if (!osg::equivalent(v5[0], 0.0))
	//	{
	//		t = v4[0] / v5[0];
	//	}
	//	else if (!osg::equivalent(v5[1], 0.0))
	//	{
	//		t = v4[1] / v5[1];
	//	}
	//	else if (!osg::equivalent(v5[2], 0.0))
	//	{
	//		t = v4[2] / v5[2];
	//	}
	//	else
	//	{
	//		if (isPointInLineSegment(s1, e1, s2))
	//		{
	//			hit = s2;
	//			return true;
	//		}
	//		else if (isPointInLineSegment(s1, e1, e2))
	//		{
	//			hit = e2;
	//			return true;
	//		}
	//		return false;
	//	}

	//	if (t > 1.0 || t < 0.0)
	//		return false;

	//	osg::Vec3d p = s1 + v1*t;
	//	if (!isPointInLineSegment(s2, e2, p))
	//	{
	//		return false;
	//	}

	//	hit = p;
	//	return true;
	//}

	//inline double norm2(const osg::Vec3d v)
	//{
	//	return v.x() * v.x() + v.y() * v.y() + v.z() * v.z();
	//}

	//inline bool isLinesegmentIntersect1(const osg::Vec3d& s1, const osg::Vec3d&e1, const osg::Vec3d& s2, const osg::Vec3d&e2, osg::Vec3d& hit)
	//{
	//	osg::Vec3d da = e1 - s1;
	//	osg::Vec3d db = e2 - s2;
	//	osg::Vec3d dc = s2 - s1;

	//	if (dc* (da^ db) != 0.0) //不共面
	//		return false;

	//	double n2 = norm2(da^db);
	//	if (osg::equivalent(n2, 0.0))//平行
	//	{
	//		if (isPointInLineSegment(s1, e1, s2))
	//		{
	//			hit = s2;
	//			return true;
	//		}
	//		else if (isPointInLineSegment(s1, e1, e2))
	//		{
	//			hit = e2;
	//			return true;
	//		}
	//		return false;
	//	}

	//	double s = ((dc^db)*(da^db)) / n2;
	//	if (s >= 0.0 && s <= 1.0)
	//	{
	//		hit = s1 + da * s;
	//		return true;
	//	}

	//	return false;
	//}
}

#endif // _SG_INLINES_H
