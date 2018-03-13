//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_matrix4.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : matrix4x4 template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_matrix4.h"

namespace uniscope_globe
{
#ifdef LINUX26
	template<> const matrix4<float> matrix4<float>::s_zero(
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f);

	template<> const matrix4<float> matrix4<float>::s_identity(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f);


	template<> const matrix4<double> matrix4<double>::s_zero(
		0.0,0.0,0.0,0.0,
		0.0,0.0,0.0,0.0,
		0.0,0.0,0.0,0.0,
		0.0,0.0,0.0,0.0);

	template<> const matrix4<double> matrix4<double>::s_identity(
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		0.0,0.0,0.0,1.0);

#endif //LINUX26

#ifndef WIN64
	void /*LIB_UTIL_API*/ multiply_matrix_4f_sse2( matrix_4f* pOut, const matrix_4f* pIn1, const matrix_4f* pIn2 )
	{
		_asm
		{
			mov edx, pIn2;			// 这时保存的是pIn2
			movups xmm4, [edx];		//pIn2的第1行
			movups xmm5, [edx+16];	//pIn2的第2行
			movups xmm6, [edx+32];	//pIn2的第3行
			movups xmm7, [edx+48];	//pIn2的第4行

			mov eax, pIn1;			// 这时保存的是pIn1
			mov edx, pOut;

			mov ecx, 4;				// 循环4次

LOOPIT:						        // 开始循环
			movss xmm0, [eax];		//xmm0 = pIn1->x
			shufps xmm0, xmm0, 0;	//洗牌xmm0 = pIn1->x, pIn1->x, pIn1->x, pIn1->x
			mulps xmm0, xmm4;

			movss xmm1, [eax+4];	//xmm1 = pIn1->y
			shufps xmm1, xmm1, 0;	//洗牌xmm1 = pIn1->y, pIn1->y, pIn1->y, pIn1->y
			mulps xmm1, xmm5;

			movss xmm2, [eax+8];	//xmm2 = pIn1->z
			shufps xmm2, xmm2, 0;	//洗牌xmm2 = pIn1->z, pIn1->z, pIn1->z, pIn1->z
			mulps xmm2, xmm6;

			movss xmm3, [eax+12];	//xmm3 = pIn1->w
			shufps xmm3, xmm3, 0;	//洗牌xmm3 = pIn1->w, pIn1->w, pIn1->w, pIn1->w
			mulps xmm3, xmm7;

			addps xmm0, xmm1;
			addps xmm2, xmm3;
			addps xmm0, xmm2;		//最终结果行保存在xmm0

			movups [edx], xmm0;		//将结果保存到pOut中
			add edx, 16;
			add eax, 16;			////作为变址用

			loop LOOPIT;
		}
	}
#endif // WIN32

#ifdef WIN64
#define SHUFFLE_PARAM(x, y, z, w) \
	((x) | ((y) << 2) | ((z) << 4) | ((w) << 6))

#define _mm_replicate_x_ps(v) \
	_mm_shuffle_ps((v), (v), SHUFFLE_PARAM(0, 0, 0, 0))
#define _mm_replicate_y_ps(v) \
	_mm_shuffle_ps((v), (v), SHUFFLE_PARAM(1, 1, 1, 1))
#define _mm_replicate_z_ps(v) \
	_mm_shuffle_ps((v), (v), SHUFFLE_PARAM(2, 2, 2, 2))
#define _mm_replicate_w_ps(v) \
	_mm_shuffle_ps((v), (v), SHUFFLE_PARAM(3, 3, 3, 3))

#define _mm_madd_ps(a, b, c) \
	_mm_add_ps(_mm_mul_ps((a), (b)), (c))

	// 64位矩阵相乘 sse算法， 使用下面的测试算法，32位下用时2.5秒左右，64下用时1.5秒左右 ，但下面的函数在32位下运行会崩溃，只能在64位下使用，所以使用WIN64宏控制 [8/9/2017 mahc]
	// 32位下崩溃原因，怀疑与16位字节对齐有关系 [8/9/2017 mahc]
	/*
	matrix_4f in1,in2,out;
	in1 = matrix_4f(1.1,1.2,1.3,1.4,2.1,2.2,2.3,2.4,3.1,3.2,3.3,3.4,4.1,4.2,4.3,4.4);
	in2 = matrix_4f(21.1,21.22,21.3,21.4,22.1,22.22,22.3,22.4,23.1,23.2,23.3,23.4,24.1,24.2,24.3,24.4);

	int i=0,iMax = 100000000;
	double dur;
	clock_t start,end;
	#ifndef WIN64
	start = clock();
	for (i=0;i<iMax;i++)
	{
	out.zero();
	uniscope_globe::multiply_matrix_4f_sse2(&out,&in1,&in2);
	}
	end = clock();
	dur = (double)(end - start);
	printf("Use Time1:%f\n",(dur/CLOCKS_PER_SEC));2.5秒s
	#endif

	#ifdef WIN64
	start = clock();
	for (i=0;i<iMax;i++)
	{
	out.zero();
	multiply_matrix_4f_sse2(&out,&in1,&in2);
	}

	end = clock();
	dur = (double)(end - start);
	printf("Use Time2:%f\n",(dur/CLOCKS_PER_SEC)); 1.5秒
	#endif

	*/
	void /*LIB_UTIL_API*/ multiply_matrix_4f_sse2( matrix_4f* pOut, const matrix_4f* pIn1, const matrix_4f* pIn2 )
	{

		__m128 m1_row_0 = _mm_setr_ps(pIn1->m[0], pIn1->m[1], pIn1->m[2], pIn1->m[3]);
		__m128 m1_row_1 = _mm_setr_ps(pIn1->m[4], pIn1->m[5], pIn1->m[6], pIn1->m[7]);
		__m128 m1_row_2 = _mm_setr_ps(pIn1->m[8], pIn1->m[9], pIn1->m[10], pIn1->m[11]);
		__m128 m1_row_3 = _mm_setr_ps(pIn1->m[12], pIn1->m[13], pIn1->m[14], pIn1->m[15]);
		__m128 m2_row_0 = _mm_setr_ps(pIn2->m[0], pIn2->m[1], pIn2->m[2], pIn2->m[3]);
		__m128 m2_row_1 = _mm_setr_ps(pIn2->m[4], pIn2->m[5], pIn2->m[6], pIn2->m[7]);
		__m128 m2_row_2 = _mm_setr_ps(pIn2->m[8], pIn2->m[9], pIn2->m[10], pIn2->m[11]);
		__m128 m2_row_3 = _mm_setr_ps(pIn2->m[12], pIn2->m[13], pIn2->m[14], pIn2->m[15]);
		__m128 out0;
		__m128 out1;
		__m128 out2;
		__m128 out3;
		out0 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_0));
		out1 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_1));
		out2 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_2));
		out3 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_3));
		out0 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_0), out0);
		out1 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_1), out1);
		out2 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_2), out2);
		out3 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_3), out3);
		out0 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_0), out0);
		out1 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_1), out1);
		out2 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_2), out2);
		out3 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_3), out3);
		out0 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_0), out0);
		out1 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_1), out1);
		out2 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_2), out2);
		out3 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_3), out3);
		// 在实际使用中，pOut的地址不能保证是16位对齐的 [8/22/2017 mahc]
		_mm_storeu_ps(pOut->m, out0);
		_mm_storeu_ps(&(pOut->m[4]), out1);
		_mm_storeu_ps(&(pOut->m[8]), out2);
		_mm_storeu_ps(&(pOut->m[12]), out3);

	}
#endif




	void /*LIB_UTIL_API*/ multiply_matrix_4d_sse2( matrix_4d* out_mat, matrix_4d* mat1, matrix_4d* mat2 )
	{

	}

	void /*LIB_UTIL_API*/ multiply_matrix_4f_c(matrix4<float>* pOut, const matrix4<float>* pIn1, const matrix4<float>* pIn2)
	{
		// [edx]   =   xmm0   *   xmm4    +    xmm1   *   xmm5    +    xmm2  *   xmm6   +    xmm3   *   xmm7
		pOut->m11 = pIn1->m11*pIn2->m11 + pIn1->m12*pIn2->m21 + pIn1->m13*pIn2->m31 + pIn1->m14*pIn2->m41;
		pOut->m12 = pIn1->m11*pIn2->m12 + pIn1->m12*pIn2->m22 + pIn1->m13*pIn2->m32 + pIn1->m14*pIn2->m42;
		pOut->m13 = pIn1->m11*pIn2->m13 + pIn1->m12*pIn2->m23 + pIn1->m13*pIn2->m33 + pIn1->m14*pIn2->m43;
		pOut->m14 = pIn1->m11*pIn2->m14 + pIn1->m12*pIn2->m24 + pIn1->m13*pIn2->m34 + pIn1->m14*pIn2->m44;

		pOut->m21 = pIn1->m21*pIn2->m11 + pIn1->m22*pIn2->m21 + pIn1->m23*pIn2->m31 + pIn1->m24*pIn2->m41;
		pOut->m22 = pIn1->m21*pIn2->m12 + pIn1->m22*pIn2->m22 + pIn1->m23*pIn2->m32 + pIn1->m24*pIn2->m42;
		pOut->m23 = pIn1->m21*pIn2->m13 + pIn1->m22*pIn2->m23 + pIn1->m23*pIn2->m33 + pIn1->m24*pIn2->m43;
		pOut->m24 = pIn1->m21*pIn2->m14 + pIn1->m22*pIn2->m24 + pIn1->m23*pIn2->m34 + pIn1->m24*pIn2->m44;

		pOut->m31 = pIn1->m31*pIn2->m11 + pIn1->m32*pIn2->m21 + pIn1->m33*pIn2->m31 + pIn1->m34*pIn2->m41;
		pOut->m32 = pIn1->m31*pIn2->m12 + pIn1->m32*pIn2->m22 + pIn1->m33*pIn2->m32 + pIn1->m34*pIn2->m42;
		pOut->m33 = pIn1->m31*pIn2->m13 + pIn1->m32*pIn2->m23 + pIn1->m33*pIn2->m33 + pIn1->m34*pIn2->m43;
		pOut->m34 = pIn1->m31*pIn2->m14 + pIn1->m32*pIn2->m24 + pIn1->m33*pIn2->m34 + pIn1->m34*pIn2->m44;

		pOut->m41 = pIn1->m41*pIn2->m11 + pIn1->m42*pIn2->m21 + pIn1->m43*pIn2->m31 + pIn1->m44*pIn2->m41;
		pOut->m42 = pIn1->m41*pIn2->m12 + pIn1->m42*pIn2->m22 + pIn1->m43*pIn2->m32 + pIn1->m44*pIn2->m42;
		pOut->m43 = pIn1->m41*pIn2->m13 + pIn1->m42*pIn2->m23 + pIn1->m43*pIn2->m33 + pIn1->m44*pIn2->m43;
		pOut->m44 = pIn1->m41*pIn2->m14 + pIn1->m42*pIn2->m24 + pIn1->m43*pIn2->m34 + pIn1->m44*pIn2->m44;

	}

	void /*LIB_UTIL_API*/ multiply_matrix_4d_c(matrix4<double>* pOut, const matrix4<double>* pIn1, const matrix4<double>* pIn2)
	{
		// [edx]   =   xmm0   *   xmm4    +    xmm1   *   xmm5    +    xmm2  *   xmm6   +    xmm3   *   xmm7
		pOut->m11 = pIn1->m11*pIn2->m11 + pIn1->m12*pIn2->m21 + pIn1->m13*pIn2->m31 + pIn1->m14*pIn2->m41;
		pOut->m12 = pIn1->m11*pIn2->m12 + pIn1->m12*pIn2->m22 + pIn1->m13*pIn2->m32 + pIn1->m14*pIn2->m42;
		pOut->m13 = pIn1->m11*pIn2->m13 + pIn1->m12*pIn2->m23 + pIn1->m13*pIn2->m33 + pIn1->m14*pIn2->m43;
		pOut->m14 = pIn1->m11*pIn2->m14 + pIn1->m12*pIn2->m24 + pIn1->m13*pIn2->m34 + pIn1->m14*pIn2->m44;

		pOut->m21 = pIn1->m21*pIn2->m11 + pIn1->m22*pIn2->m21 + pIn1->m23*pIn2->m31 + pIn1->m24*pIn2->m41;
		pOut->m22 = pIn1->m21*pIn2->m12 + pIn1->m22*pIn2->m22 + pIn1->m23*pIn2->m32 + pIn1->m24*pIn2->m42;
		pOut->m23 = pIn1->m21*pIn2->m13 + pIn1->m22*pIn2->m23 + pIn1->m23*pIn2->m33 + pIn1->m24*pIn2->m43;
		pOut->m24 = pIn1->m21*pIn2->m14 + pIn1->m22*pIn2->m24 + pIn1->m23*pIn2->m34 + pIn1->m24*pIn2->m44;

		pOut->m31 = pIn1->m31*pIn2->m11 + pIn1->m32*pIn2->m21 + pIn1->m33*pIn2->m31 + pIn1->m34*pIn2->m41;
		pOut->m32 = pIn1->m31*pIn2->m12 + pIn1->m32*pIn2->m22 + pIn1->m33*pIn2->m32 + pIn1->m34*pIn2->m42;
		pOut->m33 = pIn1->m31*pIn2->m13 + pIn1->m32*pIn2->m23 + pIn1->m33*pIn2->m33 + pIn1->m34*pIn2->m43;
		pOut->m34 = pIn1->m31*pIn2->m14 + pIn1->m32*pIn2->m24 + pIn1->m33*pIn2->m34 + pIn1->m34*pIn2->m44;

		pOut->m41 = pIn1->m41*pIn2->m11 + pIn1->m42*pIn2->m21 + pIn1->m43*pIn2->m31 + pIn1->m44*pIn2->m41;
		pOut->m42 = pIn1->m41*pIn2->m12 + pIn1->m42*pIn2->m22 + pIn1->m43*pIn2->m32 + pIn1->m44*pIn2->m42;
		pOut->m43 = pIn1->m41*pIn2->m13 + pIn1->m42*pIn2->m23 + pIn1->m43*pIn2->m33 + pIn1->m44*pIn2->m43;
		pOut->m44 = pIn1->m41*pIn2->m14 + pIn1->m42*pIn2->m24 + pIn1->m43*pIn2->m34 + pIn1->m44*pIn2->m44;

	}

	void /*LIB_UTIL_API*/ multiply_vector3_matrix_4f_c( vector_3f* pOut, const vector_3f* pVec, const matrix_4f* pMat )
	{
		pOut->x = pVec->x * pMat->m[0] + pVec->y * pMat->m[4] + pVec->z * pMat->m[8] +  pMat->m[12];
		pOut->y = pVec->x * pMat->m[1] + pVec->y * pMat->m[5] + pVec->z * pMat->m[9] +  pMat->m[13];
		pOut->z = pVec->x * pMat->m[2] + pVec->y * pMat->m[6] + pVec->z * pMat->m[10] + pMat->m[14];
	}

	void /*LIB_UTIL_API*/ multiply_vector3_matrix_4d_c( vector_3d* pOut, const vector_3d* pVec, const matrix_4d* pMat )
	{
		pOut->x = pVec->x * pMat->m[0] + pVec->y * pMat->m[4] + pVec->z * pMat->m[8] +  pMat->m[12];
		pOut->y = pVec->x * pMat->m[1] + pVec->y * pMat->m[5] + pVec->z * pMat->m[9] +  pMat->m[13];
		pOut->z = pVec->x * pMat->m[2] + pVec->y * pMat->m[6] + pVec->z * pMat->m[10] + pMat->m[14];
	}


} // namespace uniscope_globe
