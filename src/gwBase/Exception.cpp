#include <gwBase/Exception.h>
#include <gwBase/Path.h>
#include <stdio.h>
#include <cstring>
#include <sstream>

////////////////////////////////////////////////////////////////////////
// Implementation of gw_location class.
////////////////////////////////////////////////////////////////////////

gw_location::gw_location()
	: _line(-1)
	, _column(-1)
	, _byte(-1)
{
	_path[0] = '\0';
}

gw_location::gw_location(const std::string& path, int line, int column)
	: _line(line)
	, _column(column)
	, _byte(-1)
{
	setPath(path.c_str());
}

gw_location::gw_location(const GWPath& path, int line, int column)
	: _line(line),
	_column(column),
	_byte(-1)
{
	setPath(path.utf8Str().c_str());
}

gw_location::gw_location(const char* path, int line, int column)
	: _line(line)
	, _column(column)
	, _byte(-1)
{
	setPath(path);
}

gw_location::~gw_location() throw ()
{
}

const char* gw_location::getPath() const
{
	return _path;
}

void gw_location::setPath(const char* path)
{
	if (path)
	{
		strncpy(_path, path, max_path);
		_path[max_path - 1] = '\0';
	}
	else
	{
		_path[0] = '\0';
	}
}

int gw_location::getLine() const
{
	return _line;
}

void gw_location::setLine(int line)
{
	_line = line;
}

int gw_location::getColumn() const
{
	return _column;
}

void gw_location::setColumn(int column)
{
	_column = column;
}

int gw_location::getByte() const
{
	return _byte;
}

void gw_location::setByte(int byte)
{
	_byte = byte;
}

std::string gw_location::asString() const
{
	std::ostringstream out;
	if (_path[0])
	{
		out << _path;
		if (_line != -1 || _column != -1)
		{
			out << ",\n";
		}
	}
	if (_line != -1)
	{
		out << "line " << _line;
		if (_column != -1)
		{
			out << ", ";
		}
	}
	if (_column != -1)
	{
		out << "column " << _column;
	}
	return out.str();
}



////////////////////////////////////////////////////////////////////////
// Implementation of gw_throwable class.
////////////////////////////////////////////////////////////////////////

gw_throwable::gw_throwable()
{
	_message[0] = '\0';
	_origin[0] = '\0';
}

gw_throwable::gw_throwable(const char* message, const char* origin)
{
	setMessage(message);
	setOrigin(origin);
}

gw_throwable::~gw_throwable() throw ()
{
}

const char* gw_throwable::getMessage() const
{
	return _message;
}

const std::string gw_throwable::getFormattedMessage() const
{
	return std::string(getMessage());
}

void gw_throwable::setMessage(const char* message)
{
	strncpy(_message, message, MAX_TEXT_LEN);
	_message[MAX_TEXT_LEN - 1] = '\0';

}

const char* gw_throwable::getOrigin() const
{
	return _origin;
}

void gw_throwable::setOrigin(const char* origin)
{
	if (origin)
	{
		strncpy(_origin, origin, MAX_TEXT_LEN);
		_origin[MAX_TEXT_LEN - 1] = '\0';
	}
	else
	{
		_origin[0] = '\0';
	}
}

const char* gw_throwable::what() const throw()
{
	try
	{
		return getMessage();
	}
	catch (...)
	{
		return "";
	}
}


////////////////////////////////////////////////////////////////////////
// Implementation of gw_error class.
////////////////////////////////////////////////////////////////////////
gw_error::gw_error() : gw_throwable()
{
}

gw_error::gw_error(const char* message, const char *origin)
	: gw_throwable(message, origin)
{
}

gw_error::gw_error(const std::string& message, const std::string& origin)
	: gw_throwable(message.c_str(), origin.c_str())
{
}

gw_error::~gw_error() throw ()
{
}

////////////////////////////////////////////////////////////////////////
// Implementation of gw_exception class.
////////////////////////////////////////////////////////////////////////

gw_exception::gw_exception()
	: gw_throwable()
{
}

gw_exception::gw_exception(const char* message, const char* origin)
	: gw_throwable(message, origin)
{
}

gw_exception::gw_exception(const std::string& message, const std::string& origin)
	: gw_throwable(message.c_str(), origin.c_str())
{
}

gw_exception::~gw_exception() throw ()
{
}

////////////////////////////////////////////////////////////////////////
// Implementation of gw_io_exception.
////////////////////////////////////////////////////////////////////////

gw_io_exception::gw_io_exception()
	: gw_exception()
{
}

gw_io_exception::gw_io_exception(const char* message, const char* origin)
	: gw_exception(message, origin)
{
}

gw_io_exception::gw_io_exception(const char* message, const gw_location &location, const char* origin)
	: gw_exception(message, origin)
	, _location(location)
{
}

gw_io_exception::gw_io_exception(const std::string& message, const std::string& origin)
	: gw_exception(message, origin)
{
}

gw_io_exception::gw_io_exception(const std::string& message, const gw_location &location, const std::string& origin)
	: gw_exception(message, origin)
	, _location(location)
{
}

gw_io_exception::~gw_io_exception() throw ()
{
}

const std::string gw_io_exception::getFormattedMessage() const
{
	std::string ret = getMessage();
	std::string loc = getLocation().asString();
	if (loc.length())
	{
		ret += "\n at ";
		ret += loc;
	}
	return ret;
}

const gw_location & gw_io_exception::getLocation() const
{
	return _location;
}

void gw_io_exception::setLocation(const gw_location &location)
{
	_location = location;
}



////////////////////////////////////////////////////////////////////////
// Implementation of gw_format_exception.
////////////////////////////////////////////////////////////////////////

gw_format_exception::gw_format_exception()
	: gw_exception()
{
	_text[0] = '\0';
}

gw_format_exception::gw_format_exception(const char* message,
	const char* text,
	const char* origin)
	: gw_exception(message, origin)
{
	setText(text);
}

gw_format_exception::gw_format_exception(const std::string& message,
	const std::string& text,
	const std::string& origin)
	: gw_exception(message, origin)
{
	setText(text.c_str());
}

gw_format_exception::~gw_format_exception() throw ()
{
}

const char* gw_format_exception::getText() const
{
	return _text;
}

void gw_format_exception::setText(const char* text)
{
	if (text)
	{
		strncpy(_text, text, MAX_TEXT_LEN);
		_text[MAX_TEXT_LEN - 1] = '\0';
	}
	else
	{
		_text[0] = '\0';
	}
}



////////////////////////////////////////////////////////////////////////
// Implementation of gw_range_exception.
////////////////////////////////////////////////////////////////////////

gw_range_exception::gw_range_exception()
	: gw_exception()
{
}

gw_range_exception::gw_range_exception(const char* message, const char* origin)
	: gw_exception(message, origin)
{
}

gw_range_exception::gw_range_exception(const std::string& message, const std::string& origin)
	: gw_exception(message, origin)
{
}

gw_range_exception::~gw_range_exception() throw ()
{
}

// end of Exception.cpp
