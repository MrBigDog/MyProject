#ifndef GWBASE_GZFSTREAM_H
#define GWBASE_GZFSTREAM_H 1

#include <compiler.h>
#include <gwBase/export.h>
#include <zlib.h>
#include <streambuf>
#include <istream>

#define ios_openmode std::ios_base::openmode
#define ios_in       std::ios_base::in
#define ios_out      std::ios_base::out
#define ios_app      std::ios_base::app
#define ios_binary   std::ios_base::binary

#define ios_seekdir  std::ios_base::seekdir

#define ios_badbit   std::ios_base::badbit
#define ios_failbit  std::ios_base::failbit

/**
 * A C++ I/O streams interface to the zlib gz* functions.
 */
#ifdef SG_NEED_STREAMBUF_HACK
class GW_BASE_EXPORT gzfilebuf : public __streambuf
{
	typedef __streambuf parent;
#else
class GW_BASE_EXPORT gzfilebuf : public std::streambuf
{
	typedef std::streambuf parent;
#endif

public:
	/** Constructor */
	gzfilebuf();

	/** Destructor */
	virtual ~gzfilebuf();

	/**
	 * Open a stream
	 * @param name file name, UTF-8 encoded
	 * @param io_mode mode flags
	 * @return file stream
	 */
	gzfilebuf* open(const char* name, ios_openmode io_mode);

	/**
	 * Attach to an existing file descriptor
	 * @param file_descriptor file descriptor
	 * @param io_mode mode flags
	 * @return file stream
	 */
	gzfilebuf* attach(int file_descriptor, ios_openmode io_mode);

	/** Close stream */
	gzfilebuf* close();

	int setcompressionlevel(int comp_level);
	int setcompressionstrategy(int comp_strategy);

	/** @return true if open, false otherwise */
	bool is_open() const { return (file != NULL); }

	/**
	 * @return the current offset in the file being read or written.
	 * The offset corresponds to compressed data if the file is compressed,
	 * and is influenced by buffering performed in zlib, hence the "approx"
	 * qualifier. It should be suitable for progress indicators and such,
	 * though.
	 */
	z_off_t approxOffset();

	/** @return stream position */
	virtual std::streampos seekoff(std::streamoff off, ios_seekdir way, ios_openmode which);

	/** sync the stream */
	virtual int sync();

protected:

	virtual int_type underflow();

	virtual int_type overflow(int_type c = parent::traits_type::eof());
	bool    out_waiting();
	char*   base() { return obuffer; }
	int     blen() { return obuf_size; }
	char    allocate();

private:

	int_type flushbuf();
	int fillbuf();

	// Convert io_mode to "rwab" string.
	void cvt_iomode(char* mode_str, ios_openmode io_mode);

private:

	gzFile file;
	ios_openmode mode;
	bool own_file_descriptor;

	// Get (input) buffer.
	int ibuf_size;
	char* ibuffer;

	// Put (output) buffer.
	int obuf_size;
	char* obuffer;

	enum { page_size = 65536 };

private:
	// Not defined
	gzfilebuf(const gzfilebuf&);
	void operator= (const gzfilebuf&);
};

/**
 * document me
 */
struct gzifstream_base
{
	gzifstream_base() {}

	gzfilebuf gzbuf;
};

/**
 * document me too
 */
struct gzofstream_base
{
	gzofstream_base() {}

	gzfilebuf gzbuf;
};

#endif // GWUTIL_GZFSTREAM_H
