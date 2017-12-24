#ifndef string_util_h__
#define string_util_h__

#include <string>
#include <iostream>
#include <sstream>
#include <cctype>
#include <cstring>
#include <iomanip>

#include "us_vector2.h"
#include "us_vector3.h"

namespace uniscope_globe
{
	static vector_3d string_to_vec3d(const std::string& str, const vector_3d& default_value)
	{
		std::stringstream buf(str);
		vector_3d out = default_value;
		buf >> out.x;
		if (!buf.eof())
		{
			buf >> out.y >> out.z;
		}
		else
		{
			out.y = out.x;
			out.z = out.x;
		}
		return out;
	}

	static vector_2d string_to_vec2d(const std::string& str, const vector_2d& default_value)
	{
		std::stringstream buf(str);
		vector_2d out = default_value;
		buf >> out.x;
		if (!buf.eof())
		{
			buf >> out.y/* >> out.z*/;
		}
		else
		{
			out.y = out.x;
			//out.z = out.x;
		}
		return out;
	}

	static vector_3f string_to_vec3f(const std::string& str, const vector_3f& default_value)
	{
		std::stringstream buf(str);
		vector_3f out = default_value;
		buf >> out.x;
		if (!buf.eof())
		{
			buf >> out.y >> out.z;
		}
		else
		{
			out.y = out.x;
			out.z = out.x;
		}
		return out;
	}

	static std::string vec3d_to_string(const vector_3d& v)
	{
		std::stringstream buf;
		buf << std::setprecision(16)
			<< v.x << " " << v.y << " " << v.z
			<< std::endl;
		std::string result;
		result = buf.str();
		return result;
	}

	static std::string vec3f_to_string(const vector_3f& v)
	{
		std::stringstream buf;
		buf << std::setprecision(6)
			<< v.x << " " << v.y << " " << v.z
			<< std::endl;
		std::string result;
		result = buf.str();
		return result;
	}


	static double string_to_double(const std::string& str)
	{
		double result = std::strtod(str.c_str(), NULL);
		return result;
	}

	static float string_to_float(const std::string& str)
	{
		float result = std::atof(str.c_str());
		return result;
	}

	static void split(const std::string& s, const std::string& delim, std::vector< std::string >* ret)
	{
		size_t last = 0;
		size_t index = s.find_first_of(delim, last);
		while (index != std::string::npos)
		{
			ret->push_back(s.substr(last, index - last));
			last = index + 1;
			index = s.find_first_of(delim, last);
		}
		if (index - last > 0)
		{
			ret->push_back(s.substr(last, index - last));
		}
	}

	static std::string HexToStr(const std::string& str)
	{
		long len = str.size();
		std::string newString;
		for (long i = 0; i < len; i += 2)
		{
			char t[2];
			t[0] = str[i];
			t[1] = str[i + 1];
			std::string byte = t;
			char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
			newString.push_back(chr);
		}
		return newString;
	}

	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	static inline bool is_base64(unsigned char c)
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	}

	static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
	{
		std::string ret;
		int i = 0;
		int j = 0;
		unsigned char char_array_3[3];
		unsigned char char_array_4[4];

		while (in_len--)
		{
			char_array_3[i++] = *(bytes_to_encode++);
			if (i == 3)
			{
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i < 4); i++)
					ret += base64_chars[char_array_4[i]];
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 3; j++)
				char_array_3[j] = '\0';

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++)
				ret += base64_chars[char_array_4[j]];

			while ((i++ < 3))
				ret += '=';
		}

		return ret;
	}

	static std::string base64_decode(std::string const& encoded_string)
	{
		int in_len = encoded_string.size();
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		std::string ret;

		while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
		{
			char_array_4[i++] = encoded_string[in_]; in_++;
			if (i == 4)
			{
				for (i = 0; i < 4; i++)
					char_array_4[i] = base64_chars.find(char_array_4[i]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++)
					ret += char_array_3[i];
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 4; j++)
				char_array_4[j] = 0;

			for (j = 0; j < 4; j++)
				char_array_4[j] = base64_chars.find(char_array_4[j]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
		}

		return ret;
	}


	static void trim2(std::string& str)
	{
		static const std::string whitespace(" \t\f\v\n\r");
		std::string::size_type pos = str.find_last_not_of(whitespace);
		if (pos != std::string::npos)
		{
			str.erase(pos + 1);
			pos = str.find_first_not_of(whitespace);
			if (pos != std::string::npos)
				str.erase(0, pos);
		}
		else
		{
			str.erase(str.begin(), str.end());
		}
	}

	//去掉字符串的空格;
	static std::string trim(const std::string& in)
	{
		std::string str = in;
		trim2(str);
		return str;
	}

	static std::string& replaceIn(std::string& s, const std::string& sub, const std::string& other)
	{
		if (sub.empty()) return s;
		size_t b = 0;
		for (; ; )
		{
			b = s.find(sub, b);
			if (b == s.npos) break;
			s.replace(b, sub.size(), other);
			b += other.size();
		}
		return s;
	}

	static std::string& ciReplaceIn(std::string& s, const std::string& pattern, const std::string& replacement)
	{
		if (pattern.empty()) return s;

		std::string upperSource = s;
		std::transform(upperSource.begin(), upperSource.end(), upperSource.begin(), (int(*)(int))std::toupper);

		std::string upperPattern = pattern;
		std::transform(upperPattern.begin(), upperPattern.end(), upperPattern.begin(), (int(*)(int))std::toupper);

		for (size_t b = 0; ; )
		{
			b = upperSource.find(upperPattern, b);
			if (b == s.npos) break;
			s.replace(b, pattern.size(), replacement);
			upperSource.replace(b, upperPattern.size(), replacement);
			b += replacement.size();
		}

		return s;
	}


	static std::string& jsonTrim(std::string& s)
	{
		s = replaceIn(s, "\\", "");
		s = replaceIn(s, "\"{", "{");
		s = replaceIn(s, "\"[", "[");
		s = replaceIn(s, "}\"", "}");
		s = replaceIn(s, "]\"", "]");

		return s;
	}

	//bool StringToHex11(const std::string &inStr, unsigned char *outStr)
	//{
	//	size_t len = inStr.length();
	//	for (size_t i = 0; i < len; i += 2) 
	//	{
	//		sscanf(inStr.c_str() + i, "%2hhx", outStr);
	//		++outStr;
	//	}
	//	return true;
	//}

	//
	//static int ascillToValue(const char ch)
	//{
	//	int result = 0;
	//	//获取16进制的高字节位数据
	//	if(ch >= '0' && ch <= '9')
	//	{
	//		result = (int)(ch - '0');
	//	}
	//	else if(ch >= 'a' && ch <= 'z')
	//	{
	//		result = (int)(ch - 'a') + 10;
	//	}
	//	else if(ch >= 'A' && ch <= 'Z')
	//	{
	//		result = (int)(ch - 'A') + 10;
	//	}
	//	else
	//	{
	//		result = -1;
	//	}
	//	return result;
	//}
	//
	//static int hexToStr(const char *hex, char *ch)
	//{
	//	int high,low;
	//	int tmp = 0;
	//	if(hex == NULL || ch == NULL)
	//	{
	//		return -1;
	//	}
	//
	//	if(strlen(hex) %2 == 1)
	//	{
	//		return -2;
	//	}
	//
	//	while(*hex)
	//	{
	//		high = ascillToValue(*hex);
	//		if(high < 0)
	//		{
	//			*ch = '\0';
	//			return -3;
	//		}
	//		hex++; //指针移动到下一个字符上;
	//		low = ascillToValue(*hex);
	//		if(low < 0)
	//		{
	//			*ch = '\0';
	//			return -3;
	//		}
	//		tmp = (high << 4) + low;
	//		*ch++ = (char)tmp;
	//		hex++;
	//	}
	//	*ch = '\0';
	//	return 0;
	//}
}

#endif // string_util_h__
