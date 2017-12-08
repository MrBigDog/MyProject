#ifndef GWBASE_STRINGUTIL_H
#define GWBASE_STRINGUTIL_H 1

#include <compiler.h>
#include <gwBase/export.h>
#include <string>
#include <vector>
#include <cstdlib>

typedef std::vector < std::string > string_list;

namespace gwBase
{
	namespace strutils
	{
		///* utf8ToLatin1() convert utf8 to latin, useful for accent character (i.e éâàîè...)
		std::string GW_BASE_EXPORT utf8ToLatin1(std::string & s_utf8);

		/**
		 * Strip leading and/or trailing whitespace from s.
		 * @param s String to strip.
		 * @return The stripped string.
		 */
		std::string GW_BASE_EXPORT lstrip(const std::string& s);
		std::string GW_BASE_EXPORT rstrip(const std::string& s);
		std::string GW_BASE_EXPORT strip(const std::string& s);

		/**
		 * Return a new string with any trailing \r and \n characters removed.
		 * Typically useful to clean a CR-terminated line obtained from
		 * std::getline() which, upon reading CRLF (\r\n), discards the Line
		 * Feed character (\n) but leaves the Carriage Return (\r) in the
		 * string.
		 * @param s Input string
		 * @return The cleaned string
		 */
		std::string GW_BASE_EXPORT stripTrailingNewlines(const std::string& s);

		/**
		 * Strip any trailing \r and \n characters from a string.
		 * Should have slightly less overhead than stripTrailingNewlines().
		 * @param s Input string (modified in-place)
		 */
		void GW_BASE_EXPORT stripTrailingNewlines_inplace(std::string& s);

		/**
		 * Right-padding of a string to a given length
		 * @param s String to pad
		 * @param length The total length of the resulting string
		 * @param c The character to pad with
		 * @return The padded string
		 */
		std::string GW_BASE_EXPORT rpad(const std::string & s, size_t length, char c);

		/**
		 * Left-padding of a string to a given length
		 * @param s String to pad
		 * @param length The total length of the resulting string
		 * @param c The character to pad with
		 * @return The padded string
		 */
		std::string GW_BASE_EXPORT lpad(const std::string & s, size_t length, char c);

		/**
		 * Split a string into a words using 'sep' as the delimiter string.
		 * Produces a result similar to the perl and python functions of the
		 * same name.
		 *
		 * @param s The string to split into words,
		 * @param sep Word delimiters.  If not specified then any whitespace is a separator,
		 * @param maxsplit If given, splits at no more than maxsplit places,
		 * resulting in at most maxsplit+1 words.
		 * @return Array of words.
		 */
		string_list GW_BASE_EXPORT split(const std::string& s, const char* sep = 0, int maxsplit = 0);

		/**
		 * split a string on any of several characters. Commonly used to deal
		 * with strings containing whitespace, newlines. To parse CSS style
		 * string, use with '\n\t ,' as the seperator list.
		 *
		 * Note consecutive seperators will not produce empty entries in the
		 * the result, i.e splitting 'a,b,,c,d' with a ',' will produce a result
		 * with four entries, not five.
		 */
		string_list GW_BASE_EXPORT split_on_any_of(const std::string&, const char* seperators);

		/**
		 * create a single string by joining the elements of a list with
		 * another string.
		 */
		std::string GW_BASE_EXPORT join(const string_list& l, const std::string& joinWith = "");

		/**
		 * Test if a string starts with a string
		 *
		 * @param s The string to be tested
		 * @param substr The string to test
		 * @return True, if s starts with substr, False otherwise
		 */
		bool GW_BASE_EXPORT starts_with(const std::string & s, const std::string & substr);

		/**
		 * Test if a string ends with a string
		 *
		 * @param s The string to be tested
		 * @param substr The string to test
		 * @return True, if s ends with substr, False otherwise
		 */
		bool GW_BASE_EXPORT ends_with(const std::string & s, const std::string & substr);

		/**
		 * Strip all leading/trailing whitespace, and transform all interal
		 * whitespace into a single ' ' character - i.e newlines/carriage returns/
		 * tabs/multiple spaces will be condensed.
		 */
		std::string GW_BASE_EXPORT simplify(const std::string& s);

		/**
		 * convert a string representing a decimal number, to an int
		 */
		int GW_BASE_EXPORT to_int(const std::string& s, int base = 10);

		/**
		 * Like strcmp(), but for dotted versions strings NN.NN.NN
		 * any number of terms are supported.
		 * @return 0 if versions match, -ve number if v1 is lower, +ve if v1
		 * is greater
		 * @param maxComponents is the maximum number of components to look at.
		 * This can be used to ignore (say) the patch level by setting it to 2
		 */
		int GW_BASE_EXPORT compare_versions(const std::string& v1, const std::string& v2, int maxComponents = 0);

		/**
		 * Convert a string to upper case.
		 * @return upper case string
		 */
		std::string GW_BASE_EXPORT uppercase(const std::string &s);

		/**
		 * Convert a string to lower case.
		 * @return lower case string
		 */
		std::string GW_BASE_EXPORT lowercase(const std::string &s);

		/**
		 * Convert a string to lower case in place
		 */
		void GW_BASE_EXPORT lowercase(std::string &s);

		/**
		 * convert a string in the local Windows 8-bit encoding to UTF-8
		 * (no-op on other platforms)
		 */
		std::string GW_BASE_EXPORT convertWindowsLocal8BitToUtf8(const std::string& a);
		std::string GW_BASE_EXPORT convertUtf8ToWindowsLocal8Bit(const std::string& a);

		std::wstring GW_BASE_EXPORT convertUtf8ToWString(const std::string& a);
		std::string GW_BASE_EXPORT convertWStringToUtf8(const std::wstring& w);

		/**
		 * Get md5 hash of raw data.
		 */
		std::string GW_BASE_EXPORT md5(const unsigned char* data, size_t num);
		std::string GW_BASE_EXPORT md5(const char* data, size_t num);
		std::string GW_BASE_EXPORT md5(const std::string& str);

		/**
		 * convert base-64 encoded data to raw bytes (possibly with embedded
		 * NULs). Throws an exception if input data is not base64, or is
		 * malformed
		 */
		void GW_BASE_EXPORT decodeBase64(const std::string& a, std::vector<unsigned char>& output);

		/**
		 * convert bytes to hexadecimal equivalent
		 */
		std::string GW_BASE_EXPORT encodeHex(const std::string& bytes);
		std::string GW_BASE_EXPORT encodeHex(const unsigned char* rawBytes, unsigned int length);

		/**
		 * Unescape string.
		 *
		 * @param str String possibly containing escaped characters.
		 * @return string with escaped characters replaced by single character values.
		 */
		std::string GW_BASE_EXPORT unescape(const char* str);
		inline std::string unescape(const std::string& str)
		{
			return unescape(str.c_str());
		}

		std::wstring GW_BASE_EXPORT StringToWString(const std::string &str);
		std::string GW_BASE_EXPORT WStringToString(const std::wstring &wstr);

		/**
		 * Check a printf-style format string for dangerous (buffer-overflowing,
		 * memory re-writing) format tokens. If a problematic token is
		 * found, logs an error (GW_WARN) and returns an empty format string.
		 */
		std::string GW_BASE_EXPORT sanitizePrintfFormat(const std::string& input);

		/**
		 * Get the message corresponding to a given value of errno.
		 *
		 * Similar to strerror(), except it should be thread-safe and returns an
		 * std::string.
		 */
		std::string GW_BASE_EXPORT error_string(int errnum);

		inline bool isIncludeChinese(const char *str)
		{
			char c;
			while (c = *str++)
			{
				//如果字符高位为1且下一字符高位也是1则有中文字符
				if ((c & 0x80) && (*str & 0x80))
				{
					return true;
				}
			}
			return false;
		}

	} // end namespace strutils
} // end namespace gwBase

#endif // GWUTIL_STRINGUTIL_H

