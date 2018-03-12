#ifndef GWGEOLOGICALUTIL_STRING_UTILS_H
#define GWGEOLOGICALUTIL_STRING_UTILS_H 1

//#include "Common.h"
#include <osg/Vec3>
#include <osg/Vec3d>
#include <osg/Vec4>
#include <osg/Vec4ub>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <locale>
#include <iomanip>
#include <map>
#include <ctype.h>

namespace gwUtil
{
	extern /*GWGEOLOGICALUTIL_EXPORT*/ const std::string EMPTY_STRING;

	typedef std::vector<std::string> StringVector;

	/** Replaces all the instances of "pattern" with "replacement" in "in_out" */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ std::string& replaceIn(
		std::string&       in_out,
		const std::string& pattern,
		const std::string& replacement);

	/** Replaces all the instances of "pattern" with "replacement" in "in_out" (case-insensitive) */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ std::string& ciReplaceIn(
		std::string&       in_out,
		const std::string& pattern,
		const std::string& replacement);

	/**
	 * Trims whitespace from the ends of a string.
	 */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ std::string trim(const std::string& in);

	/**
	 * Trims whitespace from the ends of a string; in-place modification on the string to reduce string copies.
	 */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ void trim2(std::string& str);

	/**
	 * True is "ref" starts with "pattern"
	 */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ bool startsWith(
		const std::string& ref,
		const std::string& pattern,
		bool               caseSensitive = true,
		const std::locale& locale = std::locale());

	/**
	 * True is "ref" ends with "pattern"
	 */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ bool endsWith(
		const std::string& ref,
		const std::string& pattern,
		bool               caseSensitive = true,
		const std::locale& locale = std::locale());

	/**
	 * Case-insensitive compare
	 */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ bool ciEquals(
		const std::string& lhs,
		const std::string& rhs,
		const std::locale& local = std::locale());

	/**
	 * Case-insensitive STL comparator
	 */
	struct /*GWGEOLOGICALUTIL_EXPORT*/ CIStringComp {
		bool operator()(const std::string& lhs, const std::string& rhs) const;
	};

	extern /*GWGEOLOGICALUTIL_EXPORT*/ std::string joinStrings(const StringVector& input, char delim);

	/** Returns a lower-case version of the input string. */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ std::string toLower(const std::string& input);

	/** Parses a color string in the form "255 255 255 255" (r g b a [0..255]) into an OSG color. */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ osg::Vec4ub stringToColor(const std::string& str, osg::Vec4ub default_value);

	/** Creates a string in the form "255 255 255 255" (r g b a [0..255]) from a color */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ std::string colorToString(const osg::Vec4ub& c);

	/** Converts a string to a vec3f */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ osg::Vec3f stringToVec3f(const std::string& str, const osg::Vec3f& default_value);

	/** Converts a vec3f to a string */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ std::string vec3fToString(const osg::Vec3f& v);

	/** Parses an HTML color ("#rrggbb" or "#rrggbbaa") into an OSG color. */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ osg::Vec4f htmlColorToVec4f(const std::string& html);

	/** Makes an HTML color ("#rrggbb" or "#rrggbbaa") from an OSG color. */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ std::string vec4fToHtmlColor(const osg::Vec4f& c);

	/** Makes a valid filename, hopefully, out of a string (without touching slashes) */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ std::string toLegalFileName(const std::string& input);

	/** Generates a hashed integer for a string (poor man's MD5) */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ unsigned hashString(const std::string& input);

	/** Same as hashString but returns a string value. */
	extern /*GWGEOLOGICALUTIL_EXPORT*/ std::string hashToString(const std::string& input);

	/**
	* Gets the total number of seconds formatted as H:M:S
	*/
	extern /*GWGEOLOGICALUTIL_EXPORT*/ std::string prettyPrintTime(double seconds);

	/**
	* Gets a pretty printed version of the given size in MB.
	*/
	extern /*GWGEOLOGICALUTIL_EXPORT*/ std::string prettyPrintSize(double mb);

	//------------------------------------------------------------------------
	// conversion templates

	// converts a string to primitive using serialization
	template<typename T>
	inline T as(const std::string& str, const T& default_value)
	{
		T temp = default_value;
		std::istringstream strin(str);
		if (!strin.eof()) strin >> temp;
		return temp;
	}

	// template specialization for integers (to handle hex)
#define AS_INT_DEC_OR_HEX(TYPE) \
    template<> inline TYPE \
    as< TYPE >(const std::string& str, const TYPE & dv) { \
        TYPE temp = dv; \
        std::istringstream strin( trim(str) ); \
        if ( !strin.eof() ) { \
            if ( str.length() >= 2 && str.at(0) == '0' && str.at(1) == 'x' ) { \
                strin.seekg( 2 ); \
                strin >> std::hex >> temp; \
            } \
            else { \
                strin >> temp; \
            } \
        } \
        return temp; \
    }

	AS_INT_DEC_OR_HEX(int)
		AS_INT_DEC_OR_HEX(unsigned)
		AS_INT_DEC_OR_HEX(short)
		AS_INT_DEC_OR_HEX(unsigned short)
		AS_INT_DEC_OR_HEX(long)
		AS_INT_DEC_OR_HEX(unsigned long)

		// template specialization for a bool
		template<> inline bool
		as<bool>(const std::string& str, const bool& default_value)
	{
		std::string temp = toLower(str);
		return
			temp == "true" || temp == "yes" || temp == "on" ? true :
			temp == "false" || temp == "no" || temp == "off" ? false :
			default_value;
	}

	template<>
	inline osg::Vec3f as<osg::Vec3f>(const std::string& str, const osg::Vec3f& default_value)
	{
		return stringToVec3f(str, default_value);
	}

	// template specialization for string
	template<>
	inline std::string as<std::string>(const std::string& str, const std::string& default_value)
	{
		return str;
	}

	// snips a substring and parses it.
	template<typename T>
	inline bool as(const std::string& in, unsigned start, unsigned len, T default_value)
	{
		std::string buf;
		std::copy(in.begin() + start, in.begin() + start + len, std::back_inserter(buf));
		return as<T>(buf, default_value);
	}

	// converts a primitive to a string
	// TODO: precision??
	template<typename T>
	inline std::string toString(const T& value)
	{
		std::stringstream out;
		//out << std::setprecision(20) << std::fixed << value;
		out << std::setprecision(20) << value;
		std::string outStr;
		outStr = out.str();
		return outStr;
	}

	// template speciallization for a bool to print out "true" or "false"
	template<>
	inline std::string toString<bool>(const bool& value)
	{
		return value ? "true" : "false";
	}

	template<>
	inline std::string toString<osg::Vec3f>(const osg::Vec3f& value)
	{
		return vec3fToString(value);
	}

	/**
	 * Assembles and returns an inline string using a stream-like << operator.
	 * Example:
	 *     std::string str = Stringify() << "Hello, world " << variable;
	 */
	struct Stringify
	{
		operator std::string() const
		{
			std::string result;
			result = buf.str();
			return result;
		}

		template<typename T>
		Stringify& operator << (const T& val) { buf << val; return (*this); }

		Stringify& operator << (const Stringify& val) { buf << (std::string)val; return (*this); }

	protected:
		std::stringstream buf;
	};

	template<>
	inline Stringify& Stringify::operator << <bool>(const bool& val)
	{
		buf << (val ? "true" : "false"); return (*this);
	}

	template<>
	inline Stringify& Stringify::operator << <osg::Vec3f>(const osg::Vec3f& val)
	{
		buf << val.x() << " " << val.y() << " " << val.z(); return (*this);
	}

	template<>
	inline Stringify& Stringify::operator << <osg::Vec3d>(const osg::Vec3d& val)
	{
		buf << val.x() << " " << val.y() << " " << val.z(); return (*this);
	}

	template<>
	inline Stringify& Stringify::operator << <osg::Vec4f>(const osg::Vec4f& val)
	{
		buf << val.r() << " " << val.g() << " " << val.b() << " " << val.a(); return (*this);
	}

	/**
	 * Splits a string up into a vector of strings based on a set of
	 * delimiters, quotes, and rules.
	 */
	class /*GWGEOLOGICALUTIL_EXPORT*/ StringTokenizer
	{
	public:
		StringTokenizer(const std::string& delims = " \t\r\n", const std::string& quotes = "'\"");

		StringTokenizer(
			const std::string& input, StringVector& output,
			const std::string& delims = " \t\r\n", const std::string& quotes = "'\"",
			bool keepEmpties = true, bool trimTokens = true);

		void tokenize(const std::string& input, StringVector& output) const;

		bool& keepEmpties() { return _allowEmpties; }

		bool& trimTokens() { return _trimTokens; }

		void addDelim(char delim, bool keepAsToken = false);

		void addDelims(const std::string& delims, bool keepAsTokens = false);

		void addQuote(char delim, bool keepInToken = false);

		void addQuotes(const std::string& delims, bool keepInTokens = false);

	private:
		typedef std::map<char, bool> TokenMap;
		TokenMap _delims;
		TokenMap _quotes;
		bool     _allowEmpties;
		bool     _trimTokens;
	};



	/////Custom method
	//static bool stringToMatix3(const std::string& matstr, osg::Matrix3d& mat)
	//{
	//	std::vector<std::string> stringVector;
	//	StringTokenizer st(matstr, stringVector, ",");
	//	if (stringVector.size() != 9) return false;
	//	osg::Matrix3d matrix;
	//	matrix(0, 0) = as(stringVector[0], 0.0);
	//	matrix(0, 1) = as(stringVector[1], 0.0);
	//	matrix(0, 2) = as(stringVector[2], 0.0);
	//	matrix(1, 0) = as(stringVector[3], 0.0);
	//	matrix(1, 1) = as(stringVector[4], 0.0);
	//	matrix(1, 2) = as(stringVector[5], 0.0);
	//	matrix(2, 0) = as(stringVector[6], 0.0);
	//	matrix(2, 1) = as(stringVector[7], 0.0);
	//	matrix(2, 2) = as(stringVector[8], 0.0);
	//	mat = matrix;
	//	return true;
	//}

	//static osg::Matrixd asMatrix4(const osg::Matrix3d& mat)
	//{
	//	osg::Matrixd mat4;
	//	mat4(0, 0) = mat(0, 0);
	//	mat4(0, 1) = mat(0, 1);
	//	mat4(0, 2) = mat(0, 2);
	//	mat4(0, 3) = 0.0;
	//	mat4(1, 0) = mat(1, 0);
	//	mat4(1, 1) = mat(1, 1);
	//	mat4(1, 2) = mat(1, 2);
	//	mat4(1, 3) = 0.0;
	//	mat4(2, 0) = mat(2, 0);
	//	mat4(2, 1) = mat(2, 1);
	//	mat4(2, 2) = mat(2, 2);
	//	mat4(2, 3) = 0.0;
	//	mat4(3, 0) = 0.0;
	//	mat4(3, 1) = 0.0;
	//	mat4(3, 2) = 0.0;
	//	mat4(3, 3) = 1.0;
	//	return mat4;
	//}
}

#endif // OSGEARTH_STRING_UTILS_H

