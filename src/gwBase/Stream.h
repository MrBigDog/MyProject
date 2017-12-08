#ifndef GWBASE_STREAM_H
#define GWBASE_STREAM_H 1

#ifndef __cplusplus
# error This library requires C++
#endif

#include <compiler.h>
#include <gwBase/export.h>
#include <gwBase/Gzfstream.h>

#include <istream>
#include <ostream>
#include <fstream>

#include <string>

#include <zlib.h>

class GWPath;

/**
 * An envelope class for gzifstream.
 */
class GW_BASE_EXPORT gw_gzifstream : private gzifstream_base, public std::istream
{
public:
	gw_gzifstream();

	/**
	 * Constructor that attempts to open a file.
	 * @param name name of file
	 * @param io_mode file open mode(s) "or'd" together
	 * @param use_exact_name if false, try to add or remove a ".gz" extension
	 *                       in case the indicated file can't be opened
	 */
	gw_gzifstream(const GWPath& name, ios_openmode io_mode = ios_in | ios_binary, bool use_exact_name = false);

	/**
	 * Constructor that attaches itself to an existing file descriptor.
	 * @param fd file descriptor
	 * @param io_mode file open mode(s) "or'd" together
	 */
	gw_gzifstream(int fd, ios_openmode io_mode = ios_in | ios_binary);

	/**
	 * Attempt to open a file.
	 * @param name name of file
	 * @param io_mode file open mode(s) "or'd" together
	 * @param use_exact_name if false, try to add or remove a ".gz" extension
	 *                       in case the indicated file can't be opened
	 */
	void open(const GWPath& name,
		ios_openmode io_mode = ios_in | ios_binary,
		bool use_exact_name = false);

	/**
	 * Attach to an existing file descriptor.
	 * @param fd file descriptor
	 * @param io_mode file open mode(s) "or'd" together
	 */
	void attach(int fd, ios_openmode io_mode = ios_in | ios_binary);

	/**
	 * Close the stream.
	 */
	void close() { gzbuf.close(); }

	/** @return true if the file is successfully opened, false otherwise. */
	bool is_open() { return gzbuf.is_open(); }

	/**
	 * @return the current offset in the file being read or written.
	 * The offset corresponds to compressed data if the file is compressed,
	 * and is influenced by buffering performed in zlib, hence the "approx"
	 * qualifier. It should be suitable for progress indicators and such,
	 * though.
	 */
	z_off_t approxOffset();

private:
	// Not defined!
	gw_gzifstream(const gw_gzifstream&);
	void operator= (const gw_gzifstream&);
};

/**
 * \relates gw_gzifstream
 * An istream manipulator that skips to end of line.
 * @param in input stream
 */
std::istream& /*GW_BASE_EXPORT*/ skipeol(std::istream& in);

/**
 * \relates gw_gzifstream
 * An istream manipulator that skips over white space.
 * @param in input stream
 */
std::istream& /*GW_BASE_EXPORT*/ skipws(std::istream& in);

/**
 * \relates gw_gzifstream
 * An istream manipulator that skips comments and white space.
 * Ignores comments that start with '#'.
 * @param in input stream
 */
std::istream& /*GW_BASE_EXPORT*/ skipcomment(std::istream& in);

/**
 * An envelope class for gzofstream.*/
class GW_BASE_EXPORT gw_gzofstream : private gzofstream_base, public std::ostream
{
public:
	gw_gzofstream();

	/**
	 * Constructor to open a file for writing.
	 * @param name name of file
	 * @param io_mode file open mode(s) "or'd" together
	 */
	gw_gzofstream(const GWPath& name, ios_openmode io_mode = ios_out | ios_binary);

	/**
	 * Constructor that attaches itself to an existing file descriptor.
	 * @param fd file descriptor
	 * @param io_mode file open mode(s) "or'd" together
	 */
	gw_gzofstream(int fd, ios_openmode io_mode = ios_out | ios_binary);

	/**
	 * Attempt to open a file for writing.
	 * @param name name of file
	 * @param io_mode file open mode(s) "or'd" together
	 */
	void open(const GWPath& name, ios_openmode io_mode = ios_out | ios_binary);

	/**
	 * Attach to an existing file descriptor.
	 * @param fd file descriptor
	 * @param io_mode file open mode(s) "or'd" together
	 */
	void attach(int fd, ios_openmode io_mode = ios_out | ios_binary);

	/**
	 * Close the stream.
	 */
	void close() { gzbuf.close(); }

	/** @return true if the file is successfully opened, false otherwise. */
	bool is_open() { return gzbuf.is_open(); }

private:
	// Not defined!
	gw_gzofstream(const gw_gzofstream&);
	void operator= (const gw_gzofstream&);
};

class GW_BASE_EXPORT gw_ifstream : public std::ifstream
{
public:
	gw_ifstream() {}

	gw_ifstream(const GWPath& path, ios_openmode io_mode = ios_in | ios_binary);

	void open(const GWPath& name, ios_openmode io_mode = ios_in | ios_binary);
};

class GW_BASE_EXPORT gw_ofstream : public std::ofstream
{
public:
	gw_ofstream() { }
	gw_ofstream(const GWPath& path, ios_openmode io_mode = ios_out | ios_binary);

	void open(const GWPath& name, ios_openmode io_mode = ios_out | ios_binary);
};

#endif /* _SGSTREAM_HXX */
