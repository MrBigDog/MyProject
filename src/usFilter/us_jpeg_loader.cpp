///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_jpeg_loader.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_jpeg_loader.h"

namespace uniscope_globe
{

	bool jpeg_loader::load_jpg_file( const LPCTSTR str_file, byte*& out_buffer, int& out_width, int& out_height, int tex_format )
	{
		HANDLE h_file = CreateFile( str_file,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_RANDOM_ACCESS | FILE_ATTRIBUTE_NORMAL,
			NULL
			);

		if ( h_file == NULL || h_file == INVALID_HANDLE_VALUE )
			return false;

		DWORD v_low = 0, v_high = 0, v_cnt = 0;
		v_low = GetFileSize( h_file, &v_high );

		BYTE *v_file_buffer = new BYTE[v_low];
		ReadFile( h_file, v_file_buffer, v_low, &v_cnt, NULL );

		bool b_suc = jpeg_loader::load( v_file_buffer, v_low, out_buffer, out_width ,out_height, tex_format );

		AUTO_DELETE( v_file_buffer );
		AUTO_CLOSE_HANDLE( h_file );

		return true;
	}

	bool jpeg_loader::load( byte* in_buffer, int buffer_size, byte*& out_buffer, int& out_width, int& out_height, int tex_format )
	{
		if ( in_buffer == NULL || buffer_size <= 0 ) 
		{
			return NULL;
		}

		JPEG_CORE_PROPERTIES jpeg_engine;
		memset( &jpeg_engine, 0, sizeof( JPEG_CORE_PROPERTIES ) );

		if( ijlInit( &jpeg_engine ) != IJL_OK )
		{
			ijlFree( &jpeg_engine );
			return NULL;
		}

		jpeg_engine.JPGBytes = in_buffer;
		jpeg_engine.JPGSizeBytes = buffer_size;
		if( ijlRead( &jpeg_engine, IJL_JBUFF_READPARAMS ) != IJL_OK )
		{
			ijlFree( &jpeg_engine );
			return NULL;
		}

		
		switch(jpeg_engine.JPGChannels)
		{
		case 1:
			jpeg_engine.JPGColor    = IJL_G;
			jpeg_engine.DIBColor    = IJL_BGR;
			jpeg_engine.DIBChannels = 3;
			break;

		case 3:
			jpeg_engine.JPGColor    = IJL_YCBCR;
			jpeg_engine.DIBColor    = IJL_BGR;
			jpeg_engine.DIBChannels = 3;
			break;

		case 4:
			jpeg_engine.JPGColor    = IJL_YCBCRA_FPX;
			jpeg_engine.DIBColor    = IJL_RGBA_FPX;
			jpeg_engine.DIBChannels = 4;
			break;

		default:
			jpeg_engine.DIBColor = (IJL_COLOR)IJL_OTHER;
			jpeg_engine.JPGColor = (IJL_COLOR)IJL_OTHER;
			jpeg_engine.DIBChannels = jpeg_engine.JPGChannels;
			break;
		}

		jpeg_engine.DIBWidth    = jpeg_engine.JPGWidth;
		jpeg_engine.DIBHeight   = jpeg_engine.JPGHeight;
		jpeg_engine.DIBPadBytes = IJL_DIB_PAD_BYTES(jpeg_engine.DIBWidth, jpeg_engine.DIBChannels);

		int jpeg_size = (jpeg_engine.DIBWidth * jpeg_engine.DIBChannels + jpeg_engine.DIBPadBytes) * jpeg_engine.DIBHeight;

		byte* dib_data = new byte[ jpeg_size ];
		jpeg_engine.DIBBytes = dib_data;

		if( ijlRead( &jpeg_engine, IJL_JBUFF_READWHOLEIMAGE) != IJL_OK )
		{		
			AUTO_DELETE( dib_data );
			ijlFree( &jpeg_engine );
			return false;
		}
		ijlFree( &jpeg_engine );

		try
		{
			switch ( jpeg_engine.DIBColor )
			{
			case IJL_BGR:
				{
					switch ( tex_format )
					{
					case texture_format::US_TF_A1R5G5B5:
						{
							out_buffer = AUTO_ALIGNED_ALLOC_MEMORY( jpeg_engine.DIBWidth * jpeg_engine.DIBHeight * sizeof(ushort), 16);
							out_width  = jpeg_engine.DIBWidth;
							out_height = jpeg_engine.DIBHeight;

							byte* v_in_ptr	= dib_data;
							ushort* v_out_ptr = NULL;

							int v_dib_pad	= IJL_DIB_PAD_BYTES(jpeg_engine.DIBWidth, 3);
							int v_in_pitch	= jpeg_engine.DIBWidth * 3 + v_dib_pad;
							int v_out_pitch = jpeg_engine.DIBWidth * 2;

							for(int ni = 0; ni < jpeg_engine.DIBHeight; ni++)
							{
								v_in_ptr  = (byte*)(dib_data + v_in_pitch * ni);
								v_out_ptr = (ushort*)( out_buffer + v_out_pitch * ni );

								for(int nj = 0; nj < jpeg_engine.DIBWidth; nj++)
								{
									*(v_out_ptr) = 0x8000 
										| ushort(v_in_ptr[2])>>3<<10 
										| ushort(v_in_ptr[1])>>3<<5 
										| ushort(v_in_ptr[0])>>3 ;

									v_in_ptr  += 3;
									v_out_ptr += 1;
								}
							}
						}
						break;

					case texture_format::US_TF_A8R8G8B8:
						{
							int ob_size = jpeg_engine.DIBWidth * jpeg_engine.DIBHeight * sizeof(int);
							out_buffer = (byte*)( _aligned_malloc( ob_size, 16 ) );
							out_width  = jpeg_engine.DIBWidth;
							out_height = jpeg_engine.DIBHeight;			

							byte* v_in_ptr	= dib_data;
							byte* v_out_ptr = NULL;

							int v_dib_pad	= IJL_DIB_PAD_BYTES(jpeg_engine.DIBWidth, 3);
							int v_in_pitch	= jpeg_engine.DIBWidth * 3 + v_dib_pad;
							int v_out_pitch = jpeg_engine.DIBWidth * 4;

							for(int ni = 0; ni < jpeg_engine.DIBHeight; ni++)
							{
								v_in_ptr  = (byte*)(dib_data + v_in_pitch * ni);
								v_out_ptr = (byte*)( out_buffer + v_out_pitch * ni );

								for(int nj = 0; nj < jpeg_engine.DIBWidth; nj++)
								{
									v_out_ptr[0] = v_in_ptr[0];	
									v_out_ptr[1] = v_in_ptr[1];	
									v_out_ptr[2] = v_in_ptr[2];
									v_out_ptr[3] = 0xff;
									v_in_ptr  += 3;
									v_out_ptr += 4;
								}
							}
						}
						break;
					case texture_format::US_TF_R8G8B8:
						{
							int v_size = jpeg_engine.DIBWidth * jpeg_engine.DIBHeight * 3;
							out_buffer = AUTO_ALIGNED_ALLOC_MEMORY( v_size, 16 );
							out_width  = jpeg_engine.DIBWidth;
							out_height = jpeg_engine.DIBHeight;

							memcpy( out_buffer, dib_data, v_size );
						}
						break;
					default:
						break;
					}
				}
				break;

			default:
				{

				}
				break;
			}
		}
		catch ( ... )
		{
			int nn = 0;
		}

		if( dib_data == NULL )
		{
			ijlFree( &jpeg_engine );
			return NULL;
		}
		AUTO_DELETE_ARRAY( dib_data );

		return (out_buffer != NULL );
	}

	bool jpeg_loader::release( byte* buffer )
	{
		if ( buffer == NULL ) return false;
		CoTaskMemFree( buffer );
		return true;
	}

	bool jpeg_loader::resize( byte* in_buffer, long in_width, long in_height , byte* out_buffer, long out_width, long out_height , long texture_format )
	{
		if ( in_buffer == NULL || out_buffer == NULL ) false;

		bool is_sample_scale = false;

		if ( (in_width == out_width) && (in_height == out_height) ) is_sample_scale = true;

		float v_scale_x = ((float)in_width)/((float)out_width);
		float v_scale_y = ((float)in_width)/((float)out_height);

		int v_in_pos = 0, v_out_pos = 0, v_row = 0, v_col = 0;
		float v_temp_row = 0.0f, v_temp_col = 0.0f, v_f_row = 0.0f, v_f_col = 0.0f;

		int v_red = 0, v_green = 0, v_blue = 0;
		for( int ni = 0; ni < out_height; ni++ )
		{
			v_temp_row = (ni + 0.5f) * v_scale_y - 0.5f;
			v_row = (int)v_temp_row;
			v_f_row = v_temp_row - v_row;
			for( int nj = 0; nj < out_width; nj++ )
			{
				v_temp_col = (nj + 0.5f ) * v_scale_x - 0.5f;
				v_col = (int)v_temp_col;
				v_f_col = v_temp_col - v_col;
				v_in_pos = ( v_row * in_width + v_col ) * 3;
				v_out_pos = ( ni * out_width + nj ) * 3;

				out_buffer[v_out_pos]	= ( 1.0-v_f_row)*(1.0-v_f_col)*in_buffer[v_in_pos] + (1.0-v_f_row)*v_f_col*in_buffer[v_in_pos+3] + v_f_row*(1.0-v_f_col)*in_buffer[v_in_pos+in_width*3] + v_f_row*v_f_col*in_buffer[v_in_pos+in_width*3+3];
				out_buffer[v_out_pos+1] = ( 1.0-v_f_row)*(1.0-v_f_col)*in_buffer[v_in_pos+1] + (1.0-v_f_row)*v_f_col*in_buffer[v_in_pos+3+1] + v_f_row*(1.0-v_f_col)*in_buffer[v_in_pos+in_width*3+1] + v_f_row*v_f_col*in_buffer[v_in_pos+in_width*3+3+1];
				out_buffer[v_out_pos+2] = ( 1.0-v_f_row)*(1.0-v_f_col)*in_buffer[v_in_pos+2] + (1.0-v_f_row)*v_f_col*in_buffer[v_in_pos+3+2] + v_f_row*(1.0-v_f_col)*in_buffer[v_in_pos+in_width+2] + v_f_row*v_f_col*in_buffer[v_in_pos+in_width*3+3+2];
			}
		}
		return true;
	}

	bool jpeg_loader::resample( byte* in_buffer, long in_width, long in_height, byte*& out_buffer, long out_width, long out_height, long texture_format )
	{
		int pixel_strike = 0;
		int pixel_format = 0;
		switch( texture_format )
		{
		case texture_format::US_TF_A8R8G8B8:
			pixel_strike = 4;
			pixel_format = PixelFormat32bppARGB;
			break;
		}

		int in_size = (in_width*in_height)*pixel_strike;
		Bitmap* t_bmp = new Bitmap( in_width, in_height, pixel_format );
		BitmapData bmp_data;
		t_bmp->LockBits( NULL, ImageLockModeRead, pixel_format, &bmp_data );
		memcpy(bmp_data.Scan0, in_buffer, in_size );
		t_bmp->UnlockBits(&bmp_data);

		Bitmap* new_bmp = new Bitmap( out_width, out_height );
		Rect rect( 0, 0, out_width, out_height );
		Graphics* t_gph = Graphics::FromImage(new_bmp);
		t_gph->DrawImage( t_bmp, rect );

		AUTO_DELETE( t_bmp );

		int out_size = (out_width*out_height)*pixel_strike;
		out_buffer = AUTO_ALIGNED_ALLOC_MEMORY( out_size, 16);
		new_bmp->LockBits( NULL, ImageLockModeRead, pixel_format, &bmp_data );
		memcpy( out_buffer, bmp_data.Scan0, out_size );
		new_bmp->UnlockBits(&bmp_data);

		AUTO_DELETE( new_bmp );
		//t_gph->ReleaseHDC( t_gph->GetHDC() );
		AUTO_DELETE( t_gph );
				
		return true;
	}

	bool jpeg_loader::get_image_size_exp( int in_row, int in_col, int& row_exp, int& col_exp )
	{
		/* col check */
		int src_col = in_col;
		int col_n = 0 , col_flag = 0;
		while ( (src_col>>1) > 0 )
		{
			col_n++;

			if ( src_col%2 != 0 )
			{
				col_flag = col_flag | 0x0001;
			}

			src_col = src_col>>1;
		}

		/* row check */
		int src_row = in_row;
		int row_n = 0, row_flag = 0;
		while ( (src_row>>1) > 0 )
		{
			row_n++;

			if ( src_row%2 != 0 )
			{
				row_flag = row_flag | 0x0001;
			}

			src_row = src_row>>1;
		}

		row_exp = row_n;
		col_exp = col_n;

		if ( col_flag == 0x0001 || row_flag == 0x0001 )
		{
			return false;
		}

		return true;

	}

	bool jpeg_loader::get_nearest_exp_size( int in_row, int in_col, float v_ref, int& out_row, int& out_col )
	{
		/* col check */
		int src_col = in_col;
		int col_n = 0 , col_flag = 0;
		while ( (src_col>>1) > 0 )
		{
			col_n++;

			if ( src_col%2 != 0 )
			{
				col_flag = col_flag | 0x0001;
			}

			src_col = src_col>>1;
		}

		/* row check */
		int src_row = in_row;
		int row_n = 0, row_flag = 0;
		while ( (src_row>>1) > 0 )
		{
			row_n++;

			if ( src_row%2 != 0 )
			{
				row_flag = row_flag | 0x0001;
			}

			src_row = src_row>>1;
		}

		/* calculate return value */
		bool b_ret = true;

		long dv_point = 0;
		if ( col_flag == 0x0001 )
		{
			out_col = 1<<col_n;
			long low = out_col * ( v_ref + 1.0 );
			if ( in_col > low )
			{
				out_col = out_col<<1;
			}

			b_ret = false;
		}
		else
		{
			out_col = in_col;
		}

		if ( row_flag == 0x0001 )
		{
			out_row = 1<<row_n;
			long low = out_row * ( v_ref + 1.0 );
			if ( in_row > low )
			{
				out_row = out_row<<1;
			}

			b_ret = false;
		}
		else
		{
			out_row = in_row;
		}

		return b_ret;
	}

	long jpeg_loader::calculate_mipmap_size( int in_row, int in_col, int mip_level, int t_fmt )
	{
		long v_size = 0;
		switch( t_fmt )
		{
		case texture_format::US_TF_A8R8G8B8:
		case texture_format::US_TF_DXT5:
			{
				for ( int i = 0; i < mip_level; i++ )
				{
					v_size += (in_row>>i) * (in_col>>i);
				}
			}
			break;
		case texture_format::US_TF_R8G8B8:
			break;
		case texture_format::US_TF_A1R5G5B5:
			break;
		case texture_format::US_TF_R5G6B5:
			break;
		default:
			break;
		}
		
		return v_size;
	}

	bool jpeg_loader::get_mipmap( byte* in_buffer, long in_width, long in_height, byte* out_buffer, int v_level, long texture_format )
	{
		if ( v_level == 0 ) return true;
		if ( in_buffer == NULL || out_buffer == NULL ) return false;

		int out_width = in_width>>v_level;
		int out_height = in_height>>v_level;
		if ( out_height < 4 || out_width < 4 ) return false;

		int v_scale = 1<<v_level;
		int v_row = 0, v_col = 0;
		int v_in_pos_up = 0, v_in_pos_down = 0, v_out_pos = 0;

		for( int ni = 0; ni < out_height; ni++ )
		{
			v_row = (ni+0.5)*v_scale;
			for( int nj = 0; nj < out_width; nj++ )
			{
				v_col = (nj+0.5)*v_scale;

				v_in_pos_up = (v_row*in_width+v_col)<<2;
				v_in_pos_down = ((v_row-1)*in_width+v_col)<<2;

				v_out_pos = (ni*out_width+nj)<<2;

				out_buffer[v_out_pos]	= 0.25*in_buffer[v_in_pos_up]   + 0.25*in_buffer[v_in_pos_up-4] + 0.25*in_buffer[v_in_pos_down]   + 0.25*in_buffer[v_in_pos_down-4];
				out_buffer[v_out_pos+1] = 0.25*in_buffer[v_in_pos_up+1] + 0.25*in_buffer[v_in_pos_up-3] + 0.25*in_buffer[v_in_pos_down+1] + 0.25*in_buffer[v_in_pos_down-3];
				out_buffer[v_out_pos+2] = 0.25*in_buffer[v_in_pos_up+2] + 0.25*in_buffer[v_in_pos_up-2] + 0.25*in_buffer[v_in_pos_down+2] + 0.25*in_buffer[v_in_pos_down-2];
				out_buffer[v_out_pos+3] = 0.25*in_buffer[v_in_pos_up+3] + 0.25*in_buffer[v_in_pos_up-1] + 0.25*in_buffer[v_in_pos_down+3] + 0.25*in_buffer[v_in_pos_down-1];
				//out_buffer[v_out_pos]	= in_buffer[v_in_pos_up]>>2   + in_buffer[v_in_pos_up-4]>>2 + in_buffer[v_in_pos_down]>>2   + in_buffer[v_in_pos_down-4]>>2;
				//out_buffer[v_out_pos+1] = in_buffer[v_in_pos_up+1]>>2 + in_buffer[v_in_pos_up-3]>>2 + in_buffer[v_in_pos_down+1]>>2 + in_buffer[v_in_pos_down-3]>>2;
				//out_buffer[v_out_pos+2] = in_buffer[v_in_pos_up+2]>>2 + in_buffer[v_in_pos_up-2]>>2 + in_buffer[v_in_pos_down+2]>>2 + in_buffer[v_in_pos_down-2]>>2;
				//out_buffer[v_out_pos+3] = 0xff ;//in_buffer[v_in_pos_up+3]>>2 + in_buffer[v_in_pos_up-1]>>2 + in_buffer[v_in_pos_down+3]>>2 + in_buffer[v_in_pos_down-1]>>2;
			}
		}

		return true;
	}

	bool jpeg_loader::load( byte* in_buffer, int buffer_size, byte*& out_buffer, int& out_width, int& out_height, int tex_format, int v_mip_level )
	{
		

		return true;
	}
	

	void jpeg_loader::save_as_bmp( byte* in_buffer, int v_row, int v_col, LPCTSTR str_file )
	{
		BITMAPINFOHEADER bmpInfoHeader = {0};

		// Set the size 
		bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);

		// Bit count
		bmpInfoHeader.biBitCount = 32;

		// Use all colors
		bmpInfoHeader.biClrImportant = 0;

		// Use as many colors according to bits per pixel
		bmpInfoHeader.biClrUsed = 0;

		// Store as un Compressed
		bmpInfoHeader.biCompression = BI_RGB;

		// Set the height in pixels
		bmpInfoHeader.biHeight = v_row;

		// Width of the Image in pixels
		bmpInfoHeader.biWidth = v_col;

		// Default number of planes
		bmpInfoHeader.biPlanes = 1; 

		// Calculate the image size in bytes
		bmpInfoHeader.biSizeImage = v_row * v_col * 4;

		BITMAPFILEHEADER bfh = {0}; 
		// This value should be values of BM letters i.e 0¡Á4D42 
		// 0¡Á4D = M 0¡Á42 = B storing in reverse order to match with endian 
		/* or 
		bfh.bfType = ¡®B¡¯+(¡¯M¡¯ << 8); 
		// <<8 used to shift ¡®M¡¯ to end 
		*/ 
		bfh.bfType=0x4D42;

		// Offset to the RGBQUAD 
		bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

		// Total size of image including size of headers 
		bfh.bfSize = bfh.bfOffBits + bmpInfoHeader.biSizeImage; 

		// Create the file in disk to write 
		HANDLE hFile = CreateFile( str_file,GENERIC_WRITE, 0,NULL, 

			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL );

		if( !hFile ) // return if error opening file 
		{ 
			return; 
		} 

		DWORD dwWritten = 0;

		// Write the File header 
		WriteFile( hFile, &bfh, sizeof(bfh), &dwWritten , NULL ); 

		// Write the bitmap info header 
		WriteFile( hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwWritten, NULL ); 

		// Write the RGB Data
		WriteFile( hFile, in_buffer, bmpInfoHeader.biSizeImage, &dwWritten, NULL );
				
		// Close the file handle 
		CloseHandle( hFile ); 
	}
}