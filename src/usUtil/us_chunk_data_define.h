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
	/// ������������ö��
	/// 
	enum USBaseDataType
	{
		US_BASEDATA_GRID = 0,		// 0 ���Σ�GRID��
		US_BASEDATA_TIN,			// 1 ���Σ�TIN��
		US_BASEDATA_DOM,			// 2 Ӱ��
		US_BASEDATA_MODEL,			// 3 ģ��
		US_BASEDATA_TEXTURE,		// 4 ģ������
		US_BASEDATA_POI,			// 5 POI
		US_BASEDATA_POLYLINE,		// 6 ʸ����
		US_BASEDATA_POLYGON,		// 7 ʸ����
		US_BASEDATA_POISEARCH,		// 8 ��ѯ��POI
		US_BASEDATA_BOX,			// 9 ����¥���ѯ��box����
	};

	/// 
	/// �������ݵ����ݶ�����ö��
	/// 
	enum USBaseDataSectionType
	{
		US_SECTION_GRID = 0,		// 0 GRID		���θ���
		US_SECTION_VERTEX3D,		// 1 Vertex3D	�������У����̣߳�
		US_SECTION_VERTEX2D,		// 2 Vertex2D	�������У��޸̣߳�
		US_SECTION_TRIINDEX,		// 3 TriIndex	����������
		US_SECTION_LINEINDEX,		// 4 LineIndex	ʸ��������
		US_SECTION_DOM,				// 5 DOM		Ӱ��JPG����
		US_SECTION_MASK,			// 6 Mask		Ӱ��Mask����
		US_SECTION_POI,				// 7 POI		��ʾ��POI
	};


	/// 
	/// �������ݿ��ļ�ͷ�ṹ
	/// 
	struct USBaseDataHead
	{
		short version;
		short datatype;				// USBaseDataType
	};

	/// 
	/// �������ݿ����ݶ�ͷ�ṹ
	/// 
	struct USBaseDataSectionHead
	{
		short sectiontype;			// USBaseDataSectionType;
		long  datanum;
		long  datalength;
	};

	/// 
	/// �������ݿ����ݶ�ͷ��չ�ṹ��GRID��DOM��Mask�εĶ�ͷ
	/// 
	struct USTerraSectionHeadEx
	{
		short gridsize;				// GRID����DOM�Ŀ��С
	};

	/// 
	/// �������ݿ����ݶ�ͷ��չ�ṹ�����̵߳�vertex��ͷ
	/// 
	struct USVertex3dSectionHeadEx
	{
		float maxeluvation;			// ���߳�ֵ
		float mineluvation;			// ��С�߳�ֵ
		short accurate;				// �з־���
	};

	/// 
	/// �������ݿ����ݶ�ͷ��չ�ṹ���޸̵߳�vertex��ͷ
	/// 
	struct USVertex2dSectionHeadEx
	{
		short accurate;				// �з־���
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
		double pos[2];			// ��γ��
		char   text[80];		// ���֣��޶���80���ַ�֮�ڣ�shp���ַ��ֶ��������ֵ��
		char   discription[80];	// ����
	};

	struct  USPOIAttr
	{
		short	length;			// poi��ռ�ֽ�����
		short	inneroffset;	// ������POI�ڵ�ƫ�ƣ�
	};

	struct USPOIIndex
	{
		interior_index  poiID;			// poi���ڿ��ID��
		unsigned long   offset;			// poi�ڿ��ڵ�ƫ�ƣ�
		USPOIAttr		poiAttr;		// poi��ռ�ֽں�������POI�ڵ�ƫ��
	};


	struct USBoxQueryData
	{
		double dEast, dWest, dSouth, dNorth;	// ¥��Ķ����ϱ�boxֵ��
		double dHeight;							// ¥��ĸ߶ȣ�
		char*  text;							// ���֣��޶���80���ַ�֮��
	};

	/// 
	/// ���������Ϣö��
	/// 
	enum CompileError
	{
		US_ERROR_OK = 0,			// û�д���

		US_ERROR_NOSRCFILE,			// Դ�ļ������ڣ�
		US_ERROR_FAILEDDESTPATH,	// ·������ʧ�ܣ�
		US_ERROR_FAILEDSAVE,		// ����blk�ļ�ʧ�ܣ�
		US_ERROR_FAILEDLOAD,		// ��blk�ļ�ʧ�ܣ�
		US_ERROR_NOBLK,				// blk�ļ������ڣ�

		US_ERROR_OPENSHP,			// ��shp�ļ�ʧ�ܣ�
		US_ERROR_OPENDBF,			// ��dbf�ļ�ʧ�ܣ�
		US_ERROR_SHPDBFUNMATCH,		// Shp�ļ���dbf�ļ����ݲ�ƥ�䣻
		US_ERROR_VECNOTPOLYLINE,	// ����ʸ����ʱ��ԭʼ���ݲ���ʸ�������ͣ�
		US_ERROR_VECNOTPOLYGON,		// ����ʸ����ʱ��ԭʼ���ݲ���ʸ�������ͣ�
		US_ERROR_VECNOTPOI,			// POI���룺ԭʼ���ݲ��ǵ��������ͣ�

		US_ERROR_NOINDEXFILE,		// POI��index�ļ������ڣ�
		US_ERROR_LOADINDEX,			// ��poi��index�ļ�ʧ�ܣ�
		US_ERROR_SAVEINDEX,			// дpoi��index�ļ�ʧ�ܣ�
	};

}

#endif // _US_DATA_DEFINE_H_