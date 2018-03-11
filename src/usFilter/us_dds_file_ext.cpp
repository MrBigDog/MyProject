///////////////////////////////////////////////////////////////////////////
//  这个文件只在64位下用，32位不用。
//  由于暂时没有找到Zlib和CXimage中的部分函数 [8/24/2017 mahc]
//  Zlib缺失函数
//  ZEXTERN void ZEXPORT gz_comp_mbr(const z_img_region* dst, const z_img_region* src);  缩放图形  使用http://blog.csdn.net/housisong/article/details/1452249 候思松的算法
//  ZEXTERN void ZEXPORT gz_comp_so( const unsigned char* in_buffer, int width, int height, unsigned char* out_buffer, int* out_size );    DXT1压缩算法  来自FastDXT，从OSG中扣出来的
//  ZEXTERN void ZEXPORT gz_comp_sf( const unsigned char* in_buffer, int width, int height, unsigned char* out_buffer, int* out_size );    DXT5压缩算法   FastDXT    其中ColorTo565函数使用IDA中的反编译的代码，否则海洋会变成沙漠色。
//  
//  CXImage缺失函数
//  unsigned GetTicks(CxImageMNG *pImg)   由IDA反编译重写
//  int GetDIBSize(CxImageMNG *pImg)      由IDA反编译重写
//  其他函数根据7.0版本使用新的函数，需要验证，由于目前断点没有进入mng解析的代码，所以无法验证正确性。
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_dds_file.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : dds_file class
//	Reference : 
//
/////////////////////////////////////////////////////////////////////////////
#include "us_dds_file.h"

namespace uniscope_globe
{
#ifdef WIN64

	//#define asm __asm
	#define must_inline inline 
	typedef unsigned char   UInt8;

	// 由IDA反编译重写 [8/24/2017 mahc]
	unsigned GetTicks(CxImageMNG *pImg)
	{
		return *((dword *)pImg + 110);
	}
	// 由IDA反编译重写 [8/24/2017 mahc]
	int GetDIBSize(CxImageMNG *pImg)
	{
		return *((dword *)pImg + 100) * *((dword *)pImg + 101);
	}
	//int GetDIBMapSize(CxImageMNG *pImg)
	//{
	//	return *((char *)pImg + 424);
	//}



	// 汇编实现，由于无法直接移植到64位中，暂不使用，
	//must_inline void  Bilinear_Fast_MMX(z_argb_32_s* PColor0,z_argb_32_s* PColor1,unsigned long u_8,unsigned long v_8,z_argb_32_s* result)
	//{
	//	asm
	//	{    
	//		MOVD      MM6,v_8
	//			MOVD      MM5,u_8
	//			mov       edx,PColor0
	//			mov       eax,PColor1
	//			PXOR      mm7,mm7

	//			MOVD         MM2,dword ptr [eax]  
	//		MOVD         MM0,dword ptr [eax+4]
	//		PUNPCKLWD    MM5,MM5
	//			PUNPCKLWD    MM6,MM6
	//			MOVD         MM3,dword ptr [edx]  
	//		MOVD         MM1,dword ptr [edx+4]
	//		PUNPCKLDQ    MM5,MM5 
	//			PUNPCKLBW    MM0,MM7
	//			PUNPCKLBW    MM1,MM7
	//			PUNPCKLBW    MM2,MM7
	//			PUNPCKLBW    MM3,MM7
	//			PSUBw        MM0,MM2
	//			PSUBw        MM1,MM3
	//			PSLLw        MM2,8
	//			PSLLw        MM3,8
	//			PMULlw       MM0,MM5
	//			PMULlw       MM1,MM5
	//			PUNPCKLDQ    MM6,MM6 
	//			PADDw        MM0,MM2
	//			PADDw        MM1,MM3

	//			PSRLw        MM0,8
	//			PSRLw        MM1,8
	//			PSUBw        MM0,MM1
	//			PSLLw        MM1,8
	//			PMULlw       MM0,MM6
	//			mov       eax,result
	//			PADDw        MM0,MM1

	//			PSRLw        MM0,8
	//			PACKUSwb     MM0,MM7
	//			movd      [eax],MM0 
	//			//emms
	//	}
	//}

	//图形图像处理－之－高质量的快速的图像缩放
	//HouSisong@GMail.com   2006255.12.13
	//可以到这里下载缩放算法的完整的可以编译的项目源代码:  http://blog.csdn.net/housisong/archive/2009/03/07/3967270.aspx  )
	must_inline void Bilinear_Fast_MMX(z_argb_32_s* PColor0,z_argb_32_s* PColor1,unsigned long u_8,unsigned long v_8,z_argb_32_s* result)
	{
		unsigned long pm3_8=(u_8*v_8)>>8;
		unsigned long pm2_8=u_8-pm3_8;
		unsigned long pm1_8=v_8-pm3_8;
		unsigned long pm0_8=256-pm1_8-pm2_8-pm3_8;

		unsigned long Color=*(unsigned long*)(PColor0);
		unsigned long BR=(Color & 0x00FF00FF)*pm0_8;
		unsigned long GA=((Color & 0xFF00FF00)>>8)*pm0_8;
		Color=((unsigned long*)(PColor0))[1];
		GA+=((Color & 0xFF00FF00)>>8)*pm2_8;
		BR+=(Color & 0x00FF00FF)*pm2_8;
		Color=*(unsigned long*)(PColor1);
		GA+=((Color & 0xFF00FF00)>>8)*pm1_8;
		BR+=(Color & 0x00FF00FF)*pm1_8;
		Color=((unsigned long*)(PColor1))[1];
		GA+=((Color & 0xFF00FF00)>>8)*pm3_8;
		BR+=(Color & 0x00FF00FF)*pm3_8;

		*(unsigned long*)(result)=(GA & 0xFF00FF00)|((BR & 0xFF00FF00)>>8);
	}

	//得到y行的颜色首地址
	inline z_argb_32_s* getLinePixels(const z_img_region_s* pic,const long y) 
	{ 
		return (z_argb_32_s*) ( ((UInt8*)pic->pdata) + pic->byte_width*y );
	}
	//访问(x,y)坐标处的颜色
	inline z_argb_32_s& pixels(const z_img_region_s* pic,const long x,const long y)
	{ 
		return getLinePixels(pic,y)[x]; 
	}


	//坐标边界饱和  如果(x,y)坐标在图片数据区外,(x,y)值会被设置到图片最近的边界内,并返回false(否则什么也不做,返回true) //警告! 图片区域不能为空
	inline bool clipToBorder(const z_img_region_s* pic,long& x, long& y)
	{
		bool isIn = true;
		if (x < 0) { 
			isIn = false; x = 0;
		} else if (x >= pic->width) { 
			isIn = false; x = pic->width - 1;
		}
		if (y < 0) {
			isIn = false; y = 0;
		} else if (y >= pic->height) {
			isIn = false; y = pic->height - 1;
		}
		return isIn;
	}
	//获取一个点的颜色,默认执行边界饱和测试  当坐标超出区域的时候返回的颜色为最近的边界上的颜色值并且其alpha通道置零  //警告! 图片区域不能为空 速度很慢 
	inline z_argb_32_s getPixelsBorder(const z_img_region_s* pic,long x, long y) 
	{
		bool isInPic = clipToBorder(pic,x,y);
		z_argb_32_s result = pixels(pic,x,y);
		if (!isInPic)
			result.a=0;
		return result;
	}

	void Bilinear_Border_MMX(const z_img_region_s* pic,const long x_16,const long y_16,z_argb_32_s* result)
	{
		long x=(x_16>>16);
		long y=(y_16>>16);
		unsigned long u_16=((unsigned short)(x_16));
		unsigned long v_16=((unsigned short)(y_16));

		z_argb_32_s pixel[4];
		pixel[0]=getPixelsBorder(pic,x,y);
		pixel[1]=getPixelsBorder(pic,x+1,y);
		pixel[2]=getPixelsBorder(pic,x,y+1);
		pixel[3]=getPixelsBorder(pic,x+1,y+1);

		Bilinear_Fast_MMX(&pixel[0],&pixel[2],u_16>>8,v_16>>8,result);
	}

	// 图形缩放 [8/24/2017 mahc]
	void gz_comp_mbr_ext(z_img_region_s* pDst,const z_img_region_s* pSrc)
	{
		if (  (0==pDst->width)||(0==pDst->height)
			||(0==pSrc->width)||(0==pSrc->height)) return;

		long xrIntFloat_16=((pSrc->width)<<16)/pDst->width+1; 
		long yrIntFloat_16=((pSrc->height)<<16)/pDst->height+1;
		const long csDErrorX=-(1<<15)+(xrIntFloat_16>>1);
		const long csDErrorY=-(1<<15)+(yrIntFloat_16>>1);

		long dst_width=pDst->width;

		//计算出需要特殊处理的边界
		long border_y0=-csDErrorY/yrIntFloat_16+1;              //y0+y*yr>=0; y0=csDErrorY => y>=-csDErrorY/yr
		if (border_y0>=pDst->height) border_y0=pDst->height;
		long border_x0=-csDErrorX/xrIntFloat_16+1;     
		if (border_x0>=pDst->width ) border_x0=pDst->width; 
		long border_y1=(((pSrc->height-2)<<16)-csDErrorY)/yrIntFloat_16+1; //y0+y*yr<=(height-2) => y<=(height-2-csDErrorY)/yr
		if (border_y1<border_y0) border_y1=border_y0;
		long border_x1=(((pSrc->width-2)<<16)-csDErrorX)/xrIntFloat_16+1; 
		if (border_x1<border_x0) border_x1=border_x0;

		z_argb_32_s* pDstLine=pDst->pdata;
		long Src_byte_width=pSrc->byte_width;
		long srcy_16=csDErrorY;
		long y;
		for (y=0;y<border_y0;++y)
		{
			long srcx_16=csDErrorX;
			for (long x=0;x<dst_width;++x)
			{
				Bilinear_Border_MMX(pSrc,srcx_16,srcy_16,&pDstLine[x]); //border
				srcx_16+=xrIntFloat_16;
			}
			srcy_16+=yrIntFloat_16;
			((UInt8*&)pDstLine)+=pDst->byte_width;
		}
		for (y=border_y0;y<border_y1;++y)
		{
			long srcx_16=csDErrorX;
			long x;
			for (x=0;x<border_x0;++x)
			{
				Bilinear_Border_MMX(pSrc,srcx_16,srcy_16,&pDstLine[x]);//border
				srcx_16+=xrIntFloat_16;
			}

			{
				unsigned long v_8=(srcy_16 & 0xFFFF)>>8;
				z_argb_32_s* PSrcLineColor= (z_argb_32_s*)((UInt8*)(pSrc->pdata)+Src_byte_width*(srcy_16>>16)) ;
				for (long x=border_x0;x<border_x1;++x)
				{
					z_argb_32_s* PColor0=&PSrcLineColor[srcx_16>>16];
					z_argb_32_s* PColor1=(z_argb_32_s*)((UInt8*)(PColor0)+Src_byte_width);
					Bilinear_Fast_MMX(PColor0,PColor1,(srcx_16 & 0xFFFF)>>8,v_8,&pDstLine[x]);
					srcx_16+=xrIntFloat_16;
				}
			}

			for (x=border_x1;x<dst_width;++x)
			{
				Bilinear_Border_MMX(pSrc,srcx_16,srcy_16,&pDstLine[x]);//border
				srcx_16+=xrIntFloat_16;
			}
			srcy_16+=yrIntFloat_16;
			((UInt8*&)pDstLine)+=pDst->byte_width;
		}
		for (y=border_y1;y<pDst->height;++y)
		{
			long srcx_16=csDErrorX;
			for (long x=0;x<dst_width;++x)
			{
				Bilinear_Border_MMX(pSrc,srcx_16,srcy_16,&pDstLine[x]); //border
				srcx_16+=xrIntFloat_16;
			}
			srcy_16+=yrIntFloat_16;
			((UInt8*&)pDstLine)+=pDst->byte_width;
		}
		//asm emms  整个函数，不使用mmx，无需释放
	}
#endif // WIN64
}

