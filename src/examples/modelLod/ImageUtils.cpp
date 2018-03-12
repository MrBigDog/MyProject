#include "ImageUtils.h"
#include <osg/Notify>
#include <osg/Texture>
#include <osg/ImageSequence>
#include <osg/Timer>
#include <osg/ValueObject>
#include <osgDB/Registry>
#include <string.h>
#include <memory.h>

#define LC "[ImageUtils] "

#if defined(OSG_GLES1_AVAILABLE) || defined(OSG_GLES2_AVAILABLE) || defined(OSG_GLES3_AVAILABLE)
#    define GL_RGB8_INTERNAL  GL_RGB8_OES
#    define GL_RGB8A_INTERNAL GL_RGBA8_OES
#else
#    define GL_RGB8_INTERNAL  GL_RGB8
#    define GL_RGB8A_INTERNAL GL_RGBA8
#endif


osg::Image* ImageUtils::cloneImage(const osg::Image* input)
{
	// Why not just call image->clone()? Because, the osg::Image copy constructor does not
	// clear out the underlying BufferData/BufferObject's GL handles. This can cause 
	// exepected results if you are cloning an image that has already been used in GL.
	// Calling clone->dirty() might work, but we are not sure.

	if (!input) return 0L;

	osg::Image* clone = osg::clone(input, osg::CopyOp::DEEP_COPY_ALL);
	clone->dirty();
	if (isNormalized(input) != isNormalized(clone)) {
		OSG_WARN << LC << "Fail in clone.\n";
	}
	return clone;
}

void ImageUtils::fixInternalFormat(osg::Image* image)
{
	// OpenGL is lax about internal texture formats, and e.g. allows GL_RGBA to be used
	// instead of the proper GL_RGBA8, etc. Correct that here, since some of our compositors
	// rely on having a proper internal texture format.
	if (image->getDataType() == GL_UNSIGNED_BYTE)
	{
		if (image->getPixelFormat() == GL_RGB)
			image->setInternalTextureFormat(GL_RGB8_INTERNAL);
		else if (image->getPixelFormat() == GL_RGBA)
			image->setInternalTextureFormat(GL_RGB8A_INTERNAL);
	}
}

void ImageUtils::markAsUnNormalized(osg::Image* image, bool value)
{
	if (image)
	{
		image->setUserValue("osgEarth.unnormalized", value);
	}
}

bool ImageUtils::isUnNormalized(const osg::Image* image)
{
	if (!image) return false;
	bool result;
	return image->getUserValue("osgEarth.unnormalized", result) && (result == true);
}

bool ImageUtils::copyAsSubImage(const osg::Image* src, osg::Image* dst, int dst_start_col, int dst_start_row)
{
	if (!src || !dst ||
		dst_start_col + src->s() > dst->s() ||
		dst_start_row + src->t() > dst->t() ||
		src->r() != dst->r())
	{
		return false;
	}

	// check for fast bytewise copy:
	if (src->getPacking() == dst->getPacking() &&
		src->getDataType() == dst->getDataType() &&
		src->getPixelFormat() == dst->getPixelFormat())
	{
		for (int r = 0; r < src->r(); ++r) // each layer
		{
			for (int src_row = 0, dst_row = dst_start_row; src_row < src->t(); src_row++, dst_row++)
			{
				const void* src_data = src->data(0, src_row, r);
				void* dst_data = dst->data(dst_start_col, dst_row, r);
				memcpy(dst_data, src_data, src->getRowSizeInBytes());
			}
		}
	}

	// otherwise loop through an convert pixel-by-pixel.
	else
	{
		if (!PixelReader::supports(src) || !PixelWriter::supports(dst))
			return false;

		PixelReader read(src);
		PixelWriter write(dst);

		for (int r = 0; r < src->r(); ++r)
		{
			for (int src_t = 0, dst_t = dst_start_row; src_t < src->t(); src_t++, dst_t++)
			{
				for (int src_s = 0, dst_s = dst_start_col; src_s < src->s(); src_s++, dst_s++)
				{
					write(read(src_s, src_t, r), dst_s, dst_t, r);
				}
			}
		}
	}

	return true;
}
//

bool ImageUtils::resizeImage(const osg::Image* input,
	unsigned int out_s, unsigned int out_t,
	osg::ref_ptr<osg::Image>& output,
	unsigned int mipmapLevel,
	bool bilinear)
{
	if (!input && out_s == 0 && out_t == 0)
		return false;

	if (!PixelReader::supports(input))
	{
		OSG_WARN << LC << "resizeImage: unsupported format" << std::endl;
		return false;
	}

	if (output.valid() && !PixelWriter::supports(output.get()))
	{
		OSG_WARN << LC << "resizeImage: pre-allocated output image is in an unsupported format" << std::endl;
		return false;
	}

	unsigned int in_s = input->s();
	unsigned int in_t = input->t();

	if (!output.valid())
	{
		output = new osg::Image();

		if (PixelWriter::supports(input))
		{
			output->allocateImage(out_s, out_t, input->r(), input->getPixelFormat(), input->getDataType(), input->getPacking());
			output->setInternalTextureFormat(input->getInternalTextureFormat());
			markAsNormalized(output.get(), isNormalized(input));
		}
		else
		{
			// for unsupported write formats, convert to normalized RGBA8 automatically.
			output->allocateImage(out_s, out_t, input->r(), GL_RGBA, GL_UNSIGNED_BYTE);
			output->setInternalTextureFormat(GL_RGB8A_INTERNAL);
		}
	}
	else
	{
		// make sure they match up
		output->setInternalTextureFormat(input->getInternalTextureFormat());
	}

	if (in_s == out_s && in_t == out_t && mipmapLevel == 0 && input->getInternalTextureFormat() == output->getInternalTextureFormat())
	{
		memcpy(output->data(), input->data(), input->getTotalSizeInBytes());
	}
	else
	{
		PixelReader read(input);
		PixelWriter write(output.get());

		for (unsigned int output_row = 0; output_row < out_t; output_row++)
		{
			// get an appropriate input row
			float output_row_ratio = (float)output_row / (float)out_t;
			float input_row = output_row_ratio * (float)in_t;
			if (input_row >= input->t()) input_row = in_t - 1;
			else if (input_row < 0) input_row = 0;

			for (unsigned int output_col = 0; output_col < out_s; output_col++)
			{
				float output_col_ratio = (float)output_col / (float)out_s;
				float input_col = output_col_ratio * (float)in_s;
				if (input_col >= (int)in_s) input_col = in_s - 1;
				else if (input_col < 0) input_col = 0.0f;

				osg::Vec4 color;

				for (int layer = 0; layer < input->r(); ++layer)
				{
					if (bilinear)
					{
						// Do a billinear interpolation for the image
						int rowMin = osg::maximum((int)floor(input_row), 0);
						int rowMax = osg::maximum(osg::minimum((int)ceil(input_row), (int)(input->t() - 1)), 0);
						int colMin = osg::maximum((int)floor(input_col), 0);
						int colMax = osg::maximum(osg::minimum((int)ceil(input_col), (int)(input->s() - 1)), 0);

						if (rowMin > rowMax) rowMin = rowMax;
						if (colMin > colMax) colMin = colMax;

						osg::Vec4 urColor = read(colMax, rowMax, layer);
						osg::Vec4 llColor = read(colMin, rowMin, layer);
						osg::Vec4 ulColor = read(colMin, rowMax, layer);
						osg::Vec4 lrColor = read(colMax, rowMin, layer);

						if ((colMax == colMin) && (rowMax == rowMin))
						{
							// Exact value
							color = urColor;
						}
						else if (colMax == colMin)
						{
							// Linear interpolate vertically            
							color = llColor * ((double)rowMax - input_row) + ulColor * (input_row - (double)rowMin);
						}
						else if (rowMax == rowMin)
						{
							// Linear interpolate horizontally
							color = llColor * ((double)colMax - input_col) + lrColor * (input_col - (double)colMin);
						}
						else
						{
							// Bilinear interpolate
							osg::Vec4 r1 = llColor * ((double)colMax - input_col) + lrColor * (input_col - (double)colMin);
							osg::Vec4 r2 = ulColor * ((double)colMax - input_col) + urColor * (input_col - (double)colMin);
							color = r1 * ((double)rowMax - input_row) + r2 * (input_row - (double)rowMin);
						}
					}
					else
					{
						// nearest neighbor:
						int col = (input_col - (int)input_col) <= (ceil(input_col) - input_col) ?
							(int)input_col :
							std::min(1 + (int)input_col, (int)in_s - 1);

						int row = (input_row - (int)input_row) <= (ceil(input_row) - input_row) ?
							(int)input_row :
							std::min(1 + (int)input_row, (int)in_t - 1);

						color = read(col, row, layer); // read pixel from mip level 0.

						// old code
						//color = read( (int)input_col, (int)input_row, layer ); // read pixel from mip level 0
					}

					write(color, output_col, output_row, layer, mipmapLevel); // write to target mip level
				}
			}
		}
	}

	return true;
}

//------------------------------------------------------------------------
namespace
{
	//static const double r10= 1.0/1023.0;
	//static const double r8 = 1.0/255.0;
	//static const double r6 = 1.0/63.0;
	static const double r5 = 1.0 / 31.0;
	//static const double r4 = 1.0/15.0;
	static const double r3 = 1.0 / 7.0;
	static const double r2 = 1.0 / 3.0;

	// The scale factors to convert from an image data type to a
	// float. This is copied from OSG; I think the factors for the signed
	// types are wrong, but need to investigate further.

	template<typename T> struct GLTypeTraits;

	template<> struct GLTypeTraits<GLbyte>
	{
		static double scale(bool norm) { return norm ? 1.0 / 128.0 : 1.0; } // XXX
	};

	template<> struct GLTypeTraits<GLubyte>
	{
		static double scale(bool norm) { return norm ? 1.0 / 255.0 : 1.0; }
	};

	template<> struct GLTypeTraits<GLshort>
	{
		static double scale(bool norm) { return norm ? 1.0 / 32768.0 : 1.0; } // XXX
	};

	template<> struct GLTypeTraits<GLushort>
	{
		static double scale(bool norm) { return norm ? 1.0 / 65535.0 : 1.0; }
	};

	template<> struct GLTypeTraits<GLint>
	{
		static double scale(bool norm) { return norm ? 1.0 / 2147483648.0 : 1.0; } // XXX
	};

	template<> struct GLTypeTraits<GLuint>
	{
		static double scale(bool norm) { return norm ? 1.0 / 4294967295.0 : 1.0; }
	};

	template<> struct GLTypeTraits<GLfloat>
	{
		static double scale(bool norm) { return 1.0; }
	};

	// The Reader function that performs the read.
	template<int Format, typename T> struct ColorReader;
	template<int Format, typename T> struct ColorWriter;

	template<typename T>
	struct ColorReader<GL_DEPTH_COMPONENT, T>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* ia, int s, int t, int r, int m)
		{
			const T* ptr = (const T*)ia->data(s, t, r, m);
			float l = float(*ptr) * GLTypeTraits<T>::scale(ia->_normalized);
			return osg::Vec4(l, l, l, 1.0f);
		}
	};

	template<typename T>
	struct ColorWriter<GL_DEPTH_COMPONENT, T>
	{
		static void write(const ImageUtils::PixelWriter* iw, const osg::Vec4f& c, int s, int t, int r, int m)
		{
			T* ptr = (T*)iw->data(s, t, r, m);
			(*ptr) = (T)(c.r() / GLTypeTraits<T>::scale(iw->_normalized));
		}
	};

	template<typename T>
	struct ColorReader<GL_LUMINANCE, T>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* ia, int s, int t, int r, int m)
		{
			const T* ptr = (const T*)ia->data(s, t, r, m);
			float l = float(*ptr) * GLTypeTraits<T>::scale(ia->_normalized);
			return osg::Vec4(l, l, l, 1.0f);
		}
	};

	template<typename T>
	struct ColorWriter<GL_LUMINANCE, T>
	{
		static void write(const ImageUtils::PixelWriter* iw, const osg::Vec4f& c, int s, int t, int r, int m)
		{
			T* ptr = (T*)iw->data(s, t, r, m);
			(*ptr) = (T)(c.r() / GLTypeTraits<T>::scale(iw->_normalized));
		}
	};

	template<typename T>
	struct ColorReader<GL_RED, T>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* ia, int s, int t, int r, int m)
		{
			const T* ptr = (const T*)ia->data(s, t, r, m);
			float l = float(*ptr) * GLTypeTraits<T>::scale(ia->_normalized);
			return osg::Vec4(l, l, l, 1.0f);
		}
	};

	template<typename T>
	struct ColorWriter<GL_RED, T>
	{
		static void write(const ImageUtils::PixelWriter* iw, const osg::Vec4f& c, int s, int t, int r, int m)
		{
			T* ptr = (T*)iw->data(s, t, r, m);
			(*ptr) = (T)(c.r() / GLTypeTraits<T>::scale(iw->_normalized));
		}
	};

	template<typename T>
	struct ColorReader<GL_ALPHA, T>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* ia, int s, int t, int r, int m)
		{
			const T* ptr = (const T*)ia->data(s, t, r, m);
			float a = float(*ptr) * GLTypeTraits<T>::scale(ia->_normalized);
			return osg::Vec4(1.0f, 1.0f, 1.0f, a);
		}
	};

	template<typename T>
	struct ColorWriter<GL_ALPHA, T>
	{
		static void write(const ImageUtils::PixelWriter* iw, const osg::Vec4f& c, int s, int t, int r, int m)
		{
			T* ptr = (T*)iw->data(s, t, r, m);
			(*ptr) = (T)(c.a() / GLTypeTraits<T>::scale(iw->_normalized));
		}
	};

	template<typename T>
	struct ColorReader<GL_LUMINANCE_ALPHA, T>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* ia, int s, int t, int r, int m)
		{
			const T* ptr = (const T*)ia->data(s, t, r, m);
			float l = float(*ptr++) * GLTypeTraits<T>::scale(ia->_normalized);
			float a = float(*ptr) * GLTypeTraits<T>::scale(ia->_normalized);
			return osg::Vec4(l, l, l, a);
		}
	};

	template<typename T>
	struct ColorWriter<GL_LUMINANCE_ALPHA, T>
	{
		static void write(const ImageUtils::PixelWriter* iw, const osg::Vec4f& c, int s, int t, int r, int m)
		{
			T* ptr = (T*)iw->data(s, t, r, m);
			*ptr++ = (T)(c.r() / GLTypeTraits<T>::scale(iw->_normalized));
			*ptr = (T)(c.a() / GLTypeTraits<T>::scale(iw->_normalized));
		}
	};

	template<typename T>
	struct ColorReader<GL_RGB, T>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* ia, int s, int t, int r, int m)
		{
			const T* ptr = (const T*)ia->data(s, t, r, m);
			float d = float(*ptr++) * GLTypeTraits<T>::scale(ia->_normalized);
			float g = float(*ptr++) * GLTypeTraits<T>::scale(ia->_normalized);
			float b = float(*ptr) * GLTypeTraits<T>::scale(ia->_normalized);
			return osg::Vec4(d, g, b, 1.0f);
		}
	};

	template<typename T>
	struct ColorWriter<GL_RGB, T>
	{
		static void write(const ImageUtils::PixelWriter* iw, const osg::Vec4f& c, int s, int t, int r, int m)
		{
			T* ptr = (T*)iw->data(s, t, r, m);
			*ptr++ = (T)(c.r() / GLTypeTraits<T>::scale(iw->_normalized));
			*ptr++ = (T)(c.g() / GLTypeTraits<T>::scale(iw->_normalized));
			*ptr++ = (T)(c.b() / GLTypeTraits<T>::scale(iw->_normalized));
		}
	};

	template<typename T>
	struct ColorReader<GL_RGBA, T>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* ia, int s, int t, int r, int m)
		{
			const T* ptr = (const T*)ia->data(s, t, r, m);
			float d = float(*ptr++) * GLTypeTraits<T>::scale(ia->_normalized);
			float g = float(*ptr++) * GLTypeTraits<T>::scale(ia->_normalized);
			float b = float(*ptr++) * GLTypeTraits<T>::scale(ia->_normalized);
			float a = float(*ptr) * GLTypeTraits<T>::scale(ia->_normalized);
			return osg::Vec4(d, g, b, a);
		}
	};

	template<typename T>
	struct ColorWriter<GL_RGBA, T>
	{
		static void write(const ImageUtils::PixelWriter* iw, const osg::Vec4f& c, int s, int t, int r, int m)
		{
			T* ptr = (T*)iw->data(s, t, r, m);
			*ptr++ = (T)(c.r() / GLTypeTraits<T>::scale(iw->_normalized));
			*ptr++ = (T)(c.g() / GLTypeTraits<T>::scale(iw->_normalized));
			*ptr++ = (T)(c.b() / GLTypeTraits<T>::scale(iw->_normalized));
			*ptr++ = (T)(c.a() / GLTypeTraits<T>::scale(iw->_normalized));
		}
	};

	template<typename T>
	struct ColorReader<GL_BGR, T>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* ia, int s, int t, int r, int m)
		{
			const T* ptr = (const T*)ia->data(s, t, r, m);
			float b = float(*ptr) * GLTypeTraits<T>::scale(ia->_normalized);
			float g = float(*ptr++) * GLTypeTraits<T>::scale(ia->_normalized);
			float d = float(*ptr++) * GLTypeTraits<T>::scale(ia->_normalized);
			return osg::Vec4(d, g, b, 1.0f);
		}
	};

	template<typename T>
	struct ColorWriter<GL_BGR, T>
	{
		static void write(const ImageUtils::PixelWriter* iw, const osg::Vec4f& c, int s, int t, int r, int m)
		{
			T* ptr = (T*)iw->data(s, t, r, m);
			*ptr++ = (T)(c.b() / GLTypeTraits<T>::scale(iw->_normalized));
			*ptr++ = (T)(c.g() / GLTypeTraits<T>::scale(iw->_normalized));
			*ptr++ = (T)(c.r() / GLTypeTraits<T>::scale(iw->_normalized));
		}
	};

	template<typename T>
	struct ColorReader<GL_BGRA, T>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* ia, int s, int t, int r, int m)
		{
			const T* ptr = (const T*)ia->data(s, t, r, m);
			float b = float(*ptr++) * GLTypeTraits<T>::scale(ia->_normalized);
			float g = float(*ptr++) * GLTypeTraits<T>::scale(ia->_normalized);
			float d = float(*ptr++) * GLTypeTraits<T>::scale(ia->_normalized);
			float a = float(*ptr) * GLTypeTraits<T>::scale(ia->_normalized);
			return osg::Vec4(d, g, b, a);
		}
	};

	template<typename T>
	struct ColorWriter<GL_BGRA, T>
	{
		static void write(const ImageUtils::PixelWriter* iw, const osg::Vec4f& c, int s, int t, int r, int m)
		{
			T* ptr = (T*)iw->data(s, t, r, m);
			*ptr++ = (T)(c.b() / GLTypeTraits<T>::scale(iw->_normalized));
			*ptr++ = (T)(c.g() / GLTypeTraits<T>::scale(iw->_normalized));
			*ptr++ = (T)(c.r() / GLTypeTraits<T>::scale(iw->_normalized));
			*ptr++ = (T)(c.a() / GLTypeTraits<T>::scale(iw->_normalized));
		}
	};

	template<typename T>
	struct ColorReader<0, T>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* ia, int s, int t, int r, int m)
		{
			return osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	};

	template<typename T>
	struct ColorWriter<0, T>
	{
		static void write(const ImageUtils::PixelWriter* iw, const osg::Vec4f& c, int s, int t, int r, int m)
		{
			//nop
		}
	};

	template<>
	struct ColorReader<GL_UNSIGNED_SHORT_5_5_5_1, GLushort>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* ia, int s, int t, int r, int m)
		{
			GLushort p = *(const GLushort*)ia->data(s, t, r, m);
			//internal format GL_RGB5_A1 is implied
			return osg::Vec4(
				r5*(float)(p >> 11),
				r5*(float)((p & 0x7c0) >> 6),
				r5*(float)((p & 0x3e) >> 1),
				(float)(p & 0x1));
		}
	};

	template<>
	struct ColorWriter<GL_UNSIGNED_SHORT_5_5_5_1, GLushort>
	{
		static void write(const ImageUtils::PixelWriter* iw, const osg::Vec4f& c, int s, int t, int r, int m)
		{
			GLushort
				red = (unsigned short)(c.r() * 255),
				g = (unsigned short)(c.g() * 255),
				b = (unsigned short)(c.b() * 255),
				a = c.a() < 0.15 ? 0 : 1;

			GLushort* ptr = (GLushort*)iw->data(s, t, r, m);
			*ptr = (((red) & (0xf8)) << 8) | (((g) & (0xf8)) << 3) | (((b) & (0xF8)) >> 2) | a;
		}
	};

	template<>
	struct ColorReader<GL_UNSIGNED_BYTE_3_3_2, GLubyte>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* ia, int s, int t, int r, int m)
		{
			GLubyte p = *(const GLubyte*)ia->data(s, t, r, m);
			// internal format GL_R3_G3_B2 is implied
			return osg::Vec4(r3*(float)(p >> 5), r3*(float)((p & 0x28) >> 2), r2*(float)(p & 0x3), 1.0f);
		}
	};

	template<>
	struct ColorWriter<GL_UNSIGNED_BYTE_3_3_2, GLubyte>
	{
		static void write(const ImageUtils::PixelWriter* iw, const osg::Vec4f& c, int s, int t, int r, int m)
		{
			GLubyte* ptr = (GLubyte*)iw->data(s, t, r, m);
			OSG_WARN << LC << "Target GL_UNSIGNED_BYTE_3_3_2 not yet implemented" << std::endl;
		}
	};

	template<>
	struct ColorReader<GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GLubyte>
	{
		static osg::Vec4 read(const ImageUtils::PixelReader* pr, int s, int t, int r, int m)
		{
			static const int BLOCK_BYTES = 8;

			unsigned int blocksPerRow = pr->_image->s() / 4;
			unsigned int bs = s / 4, bt = t / 4;
			unsigned int blockStart = (bt*blocksPerRow + bs) * BLOCK_BYTES;

			const GLushort* p = (const GLushort*)(pr->data() + blockStart);

			GLushort c0p = *p++;
			osg::Vec4f c0(
				(float)(c0p >> 11) / 31.0f,
				(float)((c0p & 0x07E0) >> 5) / 63.0f,
				(float)((c0p & 0x001F)) / 31.0f,
				1.0f);

			GLushort c1p = *p++;
			osg::Vec4f c1(
				(float)(c1p >> 11) / 31.0f,
				(float)((c1p & 0x07E0) >> 5) / 63.0f,
				(float)((c1p & 0x001F)) / 31.0f,
				1.0f);

			static const float one_third = 1.0f / 3.0f;
			static const float two_thirds = 2.0f / 3.0f;

			osg::Vec4f c2, c3;
			if (c0p > c1p)
			{
				c2 = c0*two_thirds + c1*one_third;
				c3 = c0*one_third + c1*two_thirds;
			}
			else
			{
				c2 = c0*0.5 + c1*0.5;
				c3.set(0, 0, 0, 1);
			}

			unsigned int table = *(unsigned int*)p;
			int ls = s - 4 * bs, lt = t - 4 * bt; //int ls = s % 4, lt = t % 4;
			int x = ls + (4 * lt);

			unsigned int index = (table >> (2 * x)) & 0x00000003;

			return index == 0 ? c0 : index == 1 ? c1 : index == 2 ? c2 : c3;
		}
	};

	template<int GLFormat>
	inline ImageUtils::PixelReader::ReaderFunc
		chooseReader(GLenum dataType)
	{
		switch (dataType)
		{
		case GL_BYTE:
			return &ColorReader<GLFormat, GLbyte>::read;
		case GL_UNSIGNED_BYTE:
			return &ColorReader<GLFormat, GLubyte>::read;
		case GL_SHORT:
			return &ColorReader<GLFormat, GLshort>::read;
		case GL_UNSIGNED_SHORT:
			return &ColorReader<GLFormat, GLushort>::read;
		case GL_INT:
			return &ColorReader<GLFormat, GLint>::read;
		case GL_UNSIGNED_INT:
			return &ColorReader<GLFormat, GLuint>::read;
		case GL_FLOAT:
			return &ColorReader<GLFormat, GLfloat>::read;
		case GL_UNSIGNED_SHORT_5_5_5_1:
			return &ColorReader<GL_UNSIGNED_SHORT_5_5_5_1, GLushort>::read;
		case GL_UNSIGNED_BYTE_3_3_2:
			return &ColorReader<GL_UNSIGNED_BYTE_3_3_2, GLubyte>::read;
		default:
			return &ColorReader<0, GLbyte>::read;
		}
	}

	inline ImageUtils::PixelReader::ReaderFunc
		getReader(GLenum pixelFormat, GLenum dataType)
	{
		switch (pixelFormat)
		{
		case GL_DEPTH_COMPONENT:
			return chooseReader<GL_DEPTH_COMPONENT>(dataType);
			break;
		case GL_LUMINANCE:
			return chooseReader<GL_LUMINANCE>(dataType);
			break;
		case GL_RED:
			return chooseReader<GL_RED>(dataType);
			break;
		case GL_ALPHA:
			return chooseReader<GL_ALPHA>(dataType);
			break;
		case GL_LUMINANCE_ALPHA:
			return chooseReader<GL_LUMINANCE_ALPHA>(dataType);
			break;
		case GL_RGB:
			return chooseReader<GL_RGB>(dataType);
			break;
		case GL_RGBA:
			return chooseReader<GL_RGBA>(dataType);
			break;
		case GL_BGR:
			return chooseReader<GL_BGR>(dataType);
			break;
		case GL_BGRA:
			return chooseReader<GL_BGRA>(dataType);
			break;
		case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
			return &ColorReader<GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GLubyte>::read;
			break;
		default:
			return 0L;
			break;
		}
	}
}

ImageUtils::PixelReader::PixelReader(const osg::Image* image) :
	_bilinear(false)
{
	setImage(image);
}

void
ImageUtils::PixelReader::setImage(const osg::Image* image)
{
	_image = image;
	if (image)
	{
		_normalized = ImageUtils::isNormalized(image);
		_colMult = _image->getPixelSizeInBits() / 8;
		_rowMult = _image->getRowSizeInBytes();
		_imageSize = _image->getImageSizeInBytes();
		GLenum dataType = _image->getDataType();
		_reader = getReader(_image->getPixelFormat(), dataType);
		if (!_reader)
		{
			OSG_WARN << "[PixelReader] No reader found for pixel format " << std::hex << _image->getPixelFormat() << std::endl;
			_reader = &ColorReader<0, GLbyte>::read;
		}
	}
}

osg::Vec4 ImageUtils::PixelReader::operator()(float u, float v, int r, int m) const
{
	return operator()((double)u, (double)v, r, m);
}

osg::Vec4 ImageUtils::PixelReader::operator()(double u, double v, int r, int m) const
{
	if (_bilinear)
	{
		double sizeS = (double)(_image->s() - 1);
		double sizeT = (double)(_image->t() - 1);

		// u, v => [0..1]
		double s = u * sizeS;
		double t = v * sizeT;

		double s0 = std::max(floorf(s), 0.0f);
		double s1 = std::min(s0 + 1.0f, sizeS);
		double smix = s0 < s1 ? (s - s0) / (s1 - s0) : 0.0f;

		double t0 = std::max(floorf(t), 0.0f);
		double t1 = std::min(t0 + 1.0f, sizeT);
		double tmix = t0 < t1 ? (t - t0) / (t1 - t0) : 0.0f;

		osg::Vec4 UL = (*_reader)(this, (int)s0, (int)t0, r, m); // upper left
		osg::Vec4 UR = (*_reader)(this, (int)s1, (int)t0, r, m); // upper right
		osg::Vec4 LL = (*_reader)(this, (int)s0, (int)t1, r, m); // lower left
		osg::Vec4 LR = (*_reader)(this, (int)s1, (int)t1, r, m); // lower right

		osg::Vec4 TOP = UL*(1.0f - smix) + UR*smix;
		osg::Vec4 BOT = LL*(1.0f - smix) + LR*smix;

		return TOP*(1.0f - tmix) + BOT*tmix;
	}
	else
	{
		return (*_reader)(this,
			(int)(u * (double)(_image->s() - 1)),
			(int)(v * (double)(_image->t() - 1)),
			r, m);
	}
}

bool ImageUtils::PixelReader::supports(GLenum pixelFormat, GLenum dataType)
{
	return getReader(pixelFormat, dataType) != 0L;
}

//------------------------------------------------------------------------

namespace
{
	template<int GLFormat>
	inline ImageUtils::PixelWriter::WriterFunc chooseWriter(GLenum dataType)
	{
		switch (dataType)
		{
		case GL_BYTE:
			return &ColorWriter<GLFormat, GLbyte>::write;
		case GL_UNSIGNED_BYTE:
			return &ColorWriter<GLFormat, GLubyte>::write;
		case GL_SHORT:
			return &ColorWriter<GLFormat, GLshort>::write;
		case GL_UNSIGNED_SHORT:
			return &ColorWriter<GLFormat, GLushort>::write;
		case GL_INT:
			return &ColorWriter<GLFormat, GLint>::write;
		case GL_UNSIGNED_INT:
			return &ColorWriter<GLFormat, GLuint>::write;
		case GL_FLOAT:
			return &ColorWriter<GLFormat, GLfloat>::write;
		case GL_UNSIGNED_SHORT_5_5_5_1:
			return &ColorWriter<GL_UNSIGNED_SHORT_5_5_5_1, GLushort>::write;
		case GL_UNSIGNED_BYTE_3_3_2:
			return &ColorWriter<GL_UNSIGNED_BYTE_3_3_2, GLubyte>::write;
		default:
			return 0L;
		}
	}

	inline ImageUtils::PixelWriter::WriterFunc getWriter(GLenum pixelFormat, GLenum dataType)
	{
		switch (pixelFormat)
		{
		case GL_DEPTH_COMPONENT:
			return chooseWriter<GL_DEPTH_COMPONENT>(dataType);
			break;
		case GL_LUMINANCE:
			return chooseWriter<GL_LUMINANCE>(dataType);
			break;
		case GL_RED:
			return chooseWriter<GL_RED>(dataType);
			break;
		case GL_ALPHA:
			return chooseWriter<GL_ALPHA>(dataType);
			break;
		case GL_LUMINANCE_ALPHA:
			return chooseWriter<GL_LUMINANCE_ALPHA>(dataType);
			break;
		case GL_RGB:
			return chooseWriter<GL_RGB>(dataType);
			break;
		case GL_RGBA:
			return chooseWriter<GL_RGBA>(dataType);
			break;
		case GL_BGR:
			return chooseWriter<GL_BGR>(dataType);
			break;
		case GL_BGRA:
			return chooseWriter<GL_BGRA>(dataType);
			break;
		default:
			return 0L;
			break;
		}
	}
}

ImageUtils::PixelWriter::PixelWriter(osg::Image* image) :_image(image)
{
	if (image)
	{
		_normalized = ImageUtils::isNormalized(image);
		_colMult = _image->getPixelSizeInBits() / 8;
		_rowMult = _image->getRowSizeInBytes();
		_imageSize = _image->getImageSizeInBytes();
		GLenum dataType = _image->getDataType();
		_writer = getWriter(_image->getPixelFormat(), dataType);
		if (!_writer)
		{
			OSG_WARN << "[PixelWriter] No writer found for pixel format " << std::hex << _image->getPixelFormat() << std::endl;
			_writer = &ColorWriter<0, GLbyte>::write;
		}
	}
}

bool ImageUtils::PixelWriter::supports(GLenum pixelFormat, GLenum dataType)
{
	return getWriter(pixelFormat, dataType) != 0L;
}

