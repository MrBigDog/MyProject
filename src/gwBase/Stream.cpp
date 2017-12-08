#include <compiler.h>
#include <gwBase/Stream.h>
#include <gwBase/Path.h>

#include <zlib.h>

#include <string>
#include <ctype.h> // isspace()
#include <cerrno>

using std::istream;
using std::ostream;

gw_gzifstream::gw_gzifstream() : istream(&gzbuf)
{}

//-----------------------------------------------------------------------------
//
// Open a possibly gzipped file for reading.
gw_gzifstream::gw_gzifstream(const GWPath& name, ios_openmode io_mode, bool use_exact_name)
	: istream(&gzbuf)
{
	this->open(name, io_mode, use_exact_name);
}

//-----------------------------------------------------------------------------
//
// Attach a stream to an already opened file descriptor.
gw_gzifstream::gw_gzifstream(int fd, ios_openmode io_mode)
	: istream(&gzbuf)
{
	gzbuf.attach(fd, io_mode);
}

//-----------------------------------------------------------------------------
//
// Open a possibly gzipped file for reading.
// If 'use_exact_name' is true, just try to open the indicated file, nothing
// else. Otherwise:
//   - if the initial open fails and the filename has a ".gz" extension, then
//     remove it and try again;
//   - if the initial open fails and the filename doesn't have a ".gz"
//     extension, then append ".gz" and try again.
//
void gw_gzifstream::open(const GWPath& name, ios_openmode io_mode, bool use_exact_name)
{
	std::string s = name.utf8Str();
	gzbuf.open(s.c_str(), io_mode);
	if (!(gzbuf.is_open() || use_exact_name))
	{
		if (s.substr(s.length() - 3, 3) == ".gz")
		{
			// remove ".gz" suffix
			s.replace(s.length() - 3, 3, "");
			// s.erase( s.length() - 3, 3 );
		}
		else
		{
			s += ".gz"; // Append ".gz" suffix
		}

		// Try again.
		gzbuf.open(s.c_str(), io_mode);
	}
}

void gw_gzifstream::attach(int fd, ios_openmode io_mode)
{
	gzbuf.attach(fd, io_mode);
}

z_off_t gw_gzifstream::approxOffset() 
{
	return gzbuf.approxOffset();
}

//
// Manipulators
istream& skipeol(istream& in)
{
	char c = '\0';
	// make sure we detect LF, CR and CR/LF
	while (in.get(c))
	{
		if (c == '\n')
			break;
		else if (c == '\r')
		{
			if (in.peek() == '\n')
				in.get(c);
			break;
		}
	}
	return in;
}

istream& skipws(istream& in)
{
	char c;
	while (in.get(c))
	{
		if (!isspace(c))
		{
			// put pack the non-space character
			in.putback(c);
			break;
		}
	}
	return in;
}

istream& skipcomment(istream& in)
{
	while (in)
	{
		// skip whitespace
		in >> skipws;
		char c;
		if (in.get(c) && c != '#')
		{
			// not a comment
			in.putback(c);
			break;
		}
		in >> skipeol;
	}
	return in;
}


gw_gzofstream::gw_gzofstream()
	: ostream(&gzbuf)
{ }

//-----------------------------------------------------------------------------
//
// Open a file for gzipped writing.
gw_gzofstream::gw_gzofstream(const GWPath& name, ios_openmode io_mode)
	: ostream(&gzbuf)
{
	this->open(name, io_mode);
}

//-----------------------------------------------------------------------------
//
// Attach a stream to an already opened file descriptor.
gw_gzofstream::gw_gzofstream(int fd, ios_openmode io_mode)
	: ostream(&gzbuf)
{
	gzbuf.attach(fd, io_mode);
}

//-----------------------------------------------------------------------------
//
// Open a file for gzipped writing.
void gw_gzofstream::open(const GWPath& name, ios_openmode io_mode)
{
	std::string s = name.utf8Str();
	gzbuf.open(s.c_str(), io_mode);
}

void gw_gzofstream::attach(int fd, ios_openmode io_mode)
{
	gzbuf.attach(fd, io_mode);
}


gw_ifstream::gw_ifstream(const GWPath& path, ios_openmode io_mode)
{
#if defined(GW_WINDOWS)
	std::wstring ps = path.wstr();
#else
	std::string ps = path.local8BitStr();
#endif
	std::ifstream::open(ps.c_str(), io_mode);
}

void gw_ifstream::open(const GWPath& name, ios_openmode io_mode)
{
#if defined(GW_WINDOWS)
	std::wstring ps = name.wstr();
#else
	std::string ps = name.local8BitStr();
#endif
	std::ifstream::open(ps.c_str(), io_mode);
}

gw_ofstream::gw_ofstream(const GWPath& path, ios_openmode io_mode)
{
#if defined(GW_WINDOWS)
	std::wstring ps = path.wstr();
#else
	std::string ps = path.local8BitStr();
#endif
	std::ofstream::open(ps.c_str(), io_mode);
}

void gw_ofstream::open(const GWPath& name, ios_openmode io_mode)
{
#if defined(GW_WINDOWS)
	std::wstring ps = name.wstr();
#else
	std::string ps = name.local8BitStr();
#endif
	std::ofstream::open(ps.c_str(), io_mode);
}
