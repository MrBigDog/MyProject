#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <osg/Image>
#include <osg/Texture>
#include <osg/GL>
#include <osg/NodeVisitor>
#include <vector>

 //These formats were not added to OSG until after 2.8.3 so we need to define them to use them.
#ifndef GL_EXT_texture_compression_rgtc
#define GL_COMPRESSED_RED_RGTC1_EXT                0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1_EXT         0x8DBC
#define GL_COMPRESSED_RED_GREEN_RGTC2_EXT          0x8DBD
#define GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT   0x8DBE
#endif

#ifndef GL_IMG_texture_compression_pvrtc
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG      0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG      0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG     0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG     0x8C03
#endif

class ImageUtils
{
public:

	static osg::Image* cloneImage(const osg::Image* image);

	/**
	 * Tweaks an image for consistency. OpenGL allows enums like "GL_RGBA" et.al. to be
	 * used in the internal texture format, when really "GL_RGBA8" is the proper things
	 * to use. This method accounts for that. Some parts of osgEarth (like the texture-
	 * array compositor) rely on the internal texture format being correct.
	 * (http://http.download.nvidia.com/developer/Papers/2005/Fast_Texture_Transfers/Fast_Texture_Transfers.pdf)
	 */
	static void fixInternalFormat(osg::Image* image);

	/**
	 * Marks an image as containing un-normalized data values.
	 *
	 * Normally the values in an image are "normalized", i.e. scaled so they are in the
	 * range [0..1]. This is normal for color values. But when the image is being used
	 * for coverage data (a value lookup table) it is desireable to store the raw
	 * values instead.
	 */
	static void markAsUnNormalized(osg::Image* image, bool value);

	/** Inverse of above. */
	static void markAsNormalized(osg::Image* image, bool value) { markAsUnNormalized(image, !value); }

	/**
	 * Whether the image has been marked as containing un-normalized values.
	 */
	static bool isUnNormalized(const osg::Image* image);

	/**
	 * Whether the image has been marked as containing normalized values.
	 */
	static bool isNormalized(const osg::Image* image) { return !isUnNormalized(image); }

	/**
	 * Copys a portion of one image into another.
	 */
	static bool copyAsSubImage(
		const osg::Image* src,
		osg::Image*       dst,
		int dst_start_col, int dst_start_row);

	/**
	 * Resizes an image. Returns a new image, leaving the input image unaltered.
	 *
	 * Note. If the output parameter is NULL, this method will allocate a new image and
	 * resize into that new image. If the output parameter is non-NULL, this method will
	 * assume that the output image is already allocated to the proper size, and will
	 * do a resize+copy into that image. In the latter case, it is your responsibility
	 * to make sure the output image is allocated to the proper size.
	 *
	 * If the output parameter is non-NULL, then the mipmapLevel is also considered.
	 * This lets you resize directly into a particular mipmap level of the output image.
	 */
	static bool resizeImage(
		const osg::Image* input,
		unsigned int new_s, unsigned int new_t,
		osg::ref_ptr<osg::Image>& output,
		unsigned int mipmapLevel = 0, bool bilinear = true);

	
	/**
	 * Reads color data out of an image, regardles of its internal pixel format.
	 */
	class PixelReader
	{
	public:
		/**
		 * Constructs a pixel reader. "Normalized" means that the values in the source
		 * image have been scaled to [0..1] and should be denormalized upon reading.
		 */
		PixelReader(const osg::Image* image);

		/** Sets an image to read. */
		void setImage(const osg::Image* image);

		/** Whether to use bilinear interpolation when reading with u,v coords (default=true) */
		void setBilinear(bool value) { _bilinear = value; }

		/** Whether PixelReader supports a given format/datatype combiniation. */
		static bool supports(GLenum pixelFormat, GLenum dataType);

		/** Whether PixelReader can read from the specified image. */
		static bool supports(const osg::Image* image) {
			return image && supports(image->getPixelFormat(), image->getDataType());
		}

		/** Reads a color from the image */
		osg::Vec4 operator()(int s, int t, int r = 0, int m = 0) const {
			return (*_reader)(this, s, t, r, m);
		}

		/** Reads a color from the image */
		osg::Vec4 operator()(unsigned s, unsigned t, unsigned r = 0, int m = 0) const {
			return (*_reader)(this, s, t, r, m);
		}

		/** Reads a color from the image by unit coords [0..1] */
		osg::Vec4 operator()(float u, float v, int r = 0, int m = 0) const;
		osg::Vec4 operator()(double u, double v, int r = 0, int m = 0) const;

		// internals:
		const unsigned char* data(int s = 0, int t = 0, int r = 0, int m = 0) const {
			return m == 0 ?
				_image->data() + s*_colMult + t*_rowMult + r*_imageSize :
				_image->getMipmapData(m) + s*_colMult + t*(_rowMult >> m) + r*(_imageSize >> m);
		}

		typedef osg::Vec4(*ReaderFunc)(const PixelReader* ia, int s, int t, int r, int m);
		ReaderFunc _reader;
		const osg::Image* _image;
		unsigned _colMult;
		unsigned _rowMult;
		unsigned _imageSize;
		bool     _normalized;
		bool     _bilinear;
	};

	/**
	 * Writes color data to an image, regardles of its internal pixel format.
	 */
	class PixelWriter
	{
	public:
		/**
		 * Constructs a pixel writer. "Normalized" means the values are scaled to [0..1]
		 * before writing.
		 */
		PixelWriter(osg::Image* image);

		/** Whether PixelWriter can write to an image with the given format/datatype combo. */
		static bool supports(GLenum pixelFormat, GLenum dataType);

		/** Whether PixelWriter can write to non-const version of an image. */
		static bool supports(const osg::Image* image) {
			return image && supports(image->getPixelFormat(), image->getDataType());
		}

		/** Writes a color to a pixel. */
		void operator()(const osg::Vec4& c, int s, int t, int r = 0, int m = 0) {
			(*_writer)(this, c, s, t, r, m);
		}

		void f(const osg::Vec4& c, float s, float t, int r = 0, int m = 0) {
			this->operator()(c,
				(int)(s * (float)(_image->s() - 1)),
				(int)(t * (float)(_image->t() - 1)),
				r, m);
		}

		// internals:
		osg::Image* _image;
		unsigned _colMult;
		unsigned _rowMult;
		unsigned _imageSize;
		bool     _normalized;

		unsigned char* data(int s = 0, int t = 0, int r = 0, int m = 0) const {
			return m == 0 ?
				_image->data() + s*_colMult + t*_rowMult + r*_imageSize :
				_image->getMipmapData(m) + s*_colMult + t*(_rowMult >> m) + r*(_imageSize >> m);
		}

		typedef void(*WriterFunc)(const PixelWriter* iw, const osg::Vec4& c, int s, int t, int r, int m);
		WriterFunc _writer;
	};

	/**
	 * Functor that visits every pixel in an image
	 */
	template<typename T>
	struct PixelVisitor : public T
	{
		/**
		 * Traverse an image, and call this method on the superclass:
		 *
		 *   bool operator(osg::Vec4& pixel);
		 *
		 * If that method returns true, write the value back at the same location.
		 */
		void accept(osg::Image* image) {
			PixelReader _reader(image);
			PixelWriter _writer(image);
			for (int r = 0; r < image->r(); ++r) {
				for (int t = 0; t < image->t(); ++t) {
					for (int s = 0; s < image->s(); ++s) {
						osg::Vec4f pixel = _reader(s, t, r);
						if ((*this)(pixel))
							_writer(pixel, s, t, r);
					}
				}
			}
		}

		/**
		 * Traverse an image, and call this method on the superclass:
		 *
		 *   bool operator(const osg::Vec4& srcPixel, osg::Vec4& destPixel);
		 *
		 * If that method returns true, write destPixel back at the same location
		 * in the destination image.
		 */
		void accept(const osg::Image* src, osg::Image* dest) {
			PixelReader _readerSrc(src);
			PixelReader _readerDest(dest);
			PixelWriter _writerDest(dest);
			for (int r = 0; r < src->r(); ++r) {
				for (int t = 0; t < src->t(); ++t) {
					for (int s = 0; s < src->s(); ++s) {
						const osg::Vec4f pixelSrc = _readerSrc(s, t, r);
						osg::Vec4f pixelDest = _readerDest(s, t, r);
						if ((*this)(pixelSrc, pixelDest))
							_writerDest(pixelDest, s, t, r);
					}
				}
			}
		}
	};

	/**
	 * Simple functor to copy pixels from one image to another.
	 *
	 * Usage:
	 *    PixelVisitor<CopyImage>().accept( fromImage, toImage );
	 */
	struct CopyImage {
		bool operator()(const osg::Vec4f& src, osg::Vec4f& dest) {
			dest = src;
			return true;
		}
	};
};


#endif //IMAGEUTILS_H
