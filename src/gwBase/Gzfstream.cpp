//#include <gwUtil_config.h>

#include <compiler.h>
#include <gwBase/StringUtils.h>
#include <gwBase/LogStream.h>
#include <gwBase/Exception.h>
#include <gwBase/Gzfstream.h>
#include <zlib.h>

#include <cerrno>
#include <memory.h>
#include <stdio.h>
#include <fcntl.h>

//
// Construct a gzfilebuf object.
// Allocate memory for 'get' buffer and zero all buffer pointers.
//
gzfilebuf::gzfilebuf()
	: std::streambuf()
	, file(NULL)
	, mode(ios_openmode(0))
	, own_file_descriptor(false)
	, ibuf_size(0)
	, ibuffer(0)
	, obuf_size(0)
	, obuffer(0)
{
	ibuf_size = page_size / sizeof(char);
	ibuffer = new char[ibuf_size];

	// Null get and set pointers.
	this->setg(0, 0, 0);
	this->setp(0, 0);
}

gzfilebuf::~gzfilebuf()
{
	sync();
	if (own_file_descriptor)
	{
		this->close();
	}
	delete[] ibuffer;
	if (obuffer)
	{
		delete[] obuffer;
	}
}

void gzfilebuf::cvt_iomode(char* p, ios_openmode io_mode)
{
	if (io_mode & ios_in)
	{
		mode = ios_in;
		*p++ = 'r';
	}
	else if (io_mode & ios_app)
	{
		mode = ios_app;
		*p++ = 'a';
	}
	else
	{
		mode = ios_out;
		*p++ = 'w';
	}

	if (io_mode & ios_binary)
	{
		mode |= ios_binary;
		*p++ = 'b';
	}

	// Hard code the compression level
	if (io_mode & (ios_out | ios_app))
	{
		*p++ = '9';
	}

	*p = '\0';
}

gzfilebuf* gzfilebuf::open(const char *name, ios_openmode io_mode)
{
	if (is_open()) return NULL;

	char char_mode[10];
	cvt_iomode(char_mode, io_mode);

#if defined(GW_WINDOWS)
	std::wstring ws = gwBase::strutils::convertUtf8ToWString(std::string(name));
	if ((file = gzopen_w(ws.c_str(), char_mode)) == NULL) {

#else
	if ((file = gzopen(name, char_mode)) == NULL) {
#endif
		// perror( "gzfilebuf::open(): " );
		errno = 0;
		return NULL;
	}

	own_file_descriptor = true;

	return this;
}

gzfilebuf* gzfilebuf::attach(int file_descriptor, ios_openmode io_mode)
{
	if (is_open()) return NULL;

	char char_mode[10];
	cvt_iomode(char_mode, io_mode);
	if ((file = gzdopen(file_descriptor, char_mode)) == NULL) 
	{
		perror("gzfilebuf::attach(): ");
		errno = 0;
		return NULL;
	}
	own_file_descriptor = false;

	return this;
}

gzfilebuf* gzfilebuf::close()
{
	if (is_open())
	{
		sync();
		gzclose(file);
		file = NULL;
	}
	return this;
}

int gzfilebuf::setcompressionlevel(int comp_level)
{
	return gzsetparams(file, comp_level, -2);
}

int gzfilebuf::setcompressionstrategy(int comp_strategy)
{
	return gzsetparams(file, -2, comp_strategy);
}

z_off_t gzfilebuf::approxOffset()
{
	z_off_t res = gzoffset(file);

	if (res == -1)
	{
		int errnum;
		std::string errMsg = "gzoffset() error: ";
		const char *gzMsg = gzerror(file, &errnum);

		if (errnum == Z_ERRNO)
		{
			errMsg += gwBase::strutils::error_string(errno);
		}
		else
		{
			errMsg += std::string(gzMsg);
		}

		GW_LOG(GW_GENERAL, GW_ALERT, errMsg);
		throw gw_io_exception(errMsg);
	}

	return res;
}

std::streampos gzfilebuf::seekoff(std::streamoff, ios_seekdir, ios_openmode)
{
	return std::streampos(EOF);
}

gzfilebuf::int_type gzfilebuf::overflow(int_type c)
{
	if (!is_open() || !(mode & ios_out))
	{
		return EOF;
	}

	if (!base())
	{
		if (allocate() == EOF) return EOF;
		setg(0, 0, 0);
	}
	else
	{
		if (in_avail())
		{
			return EOF;
		}
		if (out_waiting())
		{
			if (flushbuf() == EOF) return EOF;
		}
	}

	int bl = blen();
	setp(base(), base() + bl);

	if (c != EOF)
	{
		*pptr() = c;
		pbump(1);
	}
	return 0;
}

int gzfilebuf::sync()
{
	if (!is_open()) return EOF;

	if (pptr() != 0 && pptr() > pbase())
	{
		return flushbuf();
	}

	return 0;
}

bool gzfilebuf::out_waiting()
{
	char* q = pbase();
	int n = pptr() - q;
	return n > 0;
}

char gzfilebuf::allocate()
{
	obuf_size = page_size / sizeof(char);
	obuffer = new char[obuf_size];
	return 0;
}

gzfilebuf::int_type gzfilebuf::flushbuf()
{
	char* q = pbase();
	int n = pptr() - q;

	if (gzwrite(file, q, n) < n)
	{
		return traits_type::eof();
	}
	setp(0, 0);

	return 0;
}

gzfilebuf::int_type gzfilebuf::underflow()
{
	// cerr << "gzfilebuf::underflow(): gptr()=" << (void*)gptr() << endl;
	// Error if the file not open for reading.
	if (!is_open() || !(mode & ios_in))
	{
		return traits_type::eof();
	}

	// If the input buffer is empty then try to fill it.
	if (gptr() != 0 && gptr() < egptr())
	{
		return int_type(*gptr());
	}
	else
	{
		return fillbuf() == EOF ? traits_type::eof() : (unsigned char)(*gptr());
	}
}

//
// Load the input buffer from the underlying gz file.
// Returns number of characters read, or EOF.
//
int gzfilebuf::fillbuf()
{
	int t = gzread(file, ibuffer, ibuf_size);
	if (t <= 0)
	{
		// disable get area
		setg(0, 0, 0);
		return EOF;
	}
	// Set the input (get) pointers
	setg(ibuffer, ibuffer, ibuffer + t);

	return t;
}

#if 0
gzifstream::gzifstream()
	: istream(&buffer), buffer()
{
	clear(ios_badbit);
}

gzifstream::gzifstream(const char *name, ios_openmode io_mode)
	: istream(&buffer), buffer()
{
	this->open(name, io_mode);
}

gzifstream::gzifstream(int fd, ios_openmode io_mode)
	: istream(&buffer), buffer()
{
	buffer.attach(fd, io_mode);
}

gzifstream::~gzifstream()
{
}

void gzifstream::open(const char *name, ios_openmode io_mode)
{
	if (!buffer.open(name, io_mode))
		clear(ios_failbit | ios_badbit);
	else
		clear();
}

void gzifstream::close()
{
	if (!buffer.close())
	{
		clear(ios_failbit | ios_badbit);
	}
}
#endif

