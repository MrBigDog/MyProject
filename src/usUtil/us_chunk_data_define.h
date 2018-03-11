///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_chunk_data_define.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : chunk_data_define class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_DATA_DEFINE_H_
#define _US_DATA_DEFINE_H_

#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
#define ACCURATE 30000

	/// 
	/// 基础数据类型枚举
	/// 
	enum USBaseDataType
	{
		US_BASEDATA_GRID = 0,		// 0 地形（GRID）
		US_BASEDATA_TIN,			// 1 地形（TIN）
		US_BASEDATA_DOM,			// 2 影像
		US_BASEDATA_MODEL,			// 3 模型
		US_BASEDATA_TEXTURE,		// 4 模型纹理
		US_BASEDATA_POI,			// 5 POI
		US_BASEDATA_POLYLINE,		// 6 矢量线
		US_BASEDATA_POLYGON,		// 7 矢量面
		US_BASEDATA_POISEARCH,		// 8 查询用POI
		US_BASEDATA_BOX,			// 9 用于楼块查询的box数据
	};

	/// 
	/// 基础数据的数据段类型枚举
	/// 
	enum USBaseDataSectionType
	{
		US_SECTION_GRID = 0,		// 0 GRID		地形格网
		US_SECTION_VERTEX3D,		// 1 Vertex3D	顶点序列（带高程）
		US_SECTION_VERTEX2D,		// 2 Vertex2D	顶点序列（无高程）
		US_SECTION_TRIINDEX,		// 3 TriIndex	三角面索引
		US_SECTION_LINEINDEX,		// 4 LineIndex	矢量线索引
		US_SECTION_DOM,				// 5 DOM		影像JPG部分
		US_SECTION_MASK,			// 6 Mask		影像Mask部分
		US_SECTION_POI,				// 7 POI		显示用POI
	};


	/// 
	/// 基础数据块文件头结构
	/// 
	struct USBaseDataHead
	{
		short version;
		short datatype;				// USBaseDataType
	};

	/// 
	/// 基础数据块数据段头结构
	/// 
	struct USBaseDataSectionHead
	{
		short sectiontype;			// USBaseDataSectionType;
		long  datanum;
		long  datalength;
	};

	/// 
	/// 基础数据块数据段头扩展结构：GRID或DOM、Mask段的段头
	/// 
	struct USTerraSectionHeadEx
	{
		short gridsize;				// GRID或者DOM的块大小
	};

	/// 
	/// 基础数据块数据段头扩展结构：带高程的vertex段头
	/// 
	struct USVertex3dSectionHeadEx
	{
		float maxeluvation;			// 最大高程值
		float mineluvation;			// 最小高程值
		short accurate;				// 切分精度
	};

	/// 
	/// 基础数据块数据段头扩展结构：无高程的vertex段头
	/// 
	struct USVertex2dSectionHeadEx
	{
		short accurate;				// 切分精度
	};

	struct USVertex3d
	{
		short x;
		short y;
		short z;
	};

	struct USVertexGeo3d
	{
		double x;
		double y;
		double z;
	};

	struct us_vertex2d
	{
		short x;
		short y;
	};

	struct USVertexGeo2d
	{
		double x;
		double y;
	};

	struct USTriangleIndex
	{
		short v0;
		short v1;
		short v2;
	};

	struct USLineIndex

	{
		short lineLengh;
	};

	struct USPOIData
	{
		double pos[2];			// 经纬度
		char   text[80];		// 文字，限定在80个字符之内（shp的字符字段最大允许值）
		char   discription[80];	// 描述
	};

	struct  USPOIAttr
	{
		short	length;			// poi所占字节数；
		short	inneroffset;	// 文字在POI内的偏移；
	};

	struct USPOIIndex
	{
		interior_index  poiID;			// poi所在块的ID；
		unsigned long   offset;			// poi在块内的偏移；
		USPOIAttr		poiAttr;		// poi所占字节和文字在POI内的偏移
	};


	struct USBoxQueryData
	{
		double dEast, dWest, dSouth, dNorth;	// 楼块的东西南北box值；
		double dHeight;							// 楼块的高度；
		char*  text;							// 文字，限定在80个字符之内
	};

	/// 
	/// 编译错误信息枚举
	/// 
	enum CompileError
	{
		US_ERROR_OK = 0,			// 没有错误；

		US_ERROR_NOSRCFILE,			// 源文件不存在；
		US_ERROR_FAILEDDESTPATH,	// 路径创建失败；
		US_ERROR_FAILEDSAVE,		// 保存blk文件失败；
		US_ERROR_FAILEDLOAD,		// 打开blk文件失败；
		US_ERROR_NOBLK,				// blk文件不存在；

		US_ERROR_OPENSHP,			// 打开shp文件失败；
		US_ERROR_OPENDBF,			// 打开dbf文件失败；
		US_ERROR_SHPDBFUNMATCH,		// Shp文件和dbf文件内容不匹配；
		US_ERROR_VECNOTPOLYLINE,	// 编译矢量线时：原始数据不是矢量线类型；
		US_ERROR_VECNOTPOLYGON,		// 编译矢量面时：原始数据不是矢量面类型；
		US_ERROR_VECNOTPOI,			// POI编译：原始数据不是点数据类型；

		US_ERROR_NOINDEXFILE,		// POI的index文件不存在；
		US_ERROR_LOADINDEX,			// 打开poi的index文件失败；
		US_ERROR_SAVEINDEX,			// 写poi的index文件失败；
	};

}

#endif // _US_DATA_DEFINE_H_