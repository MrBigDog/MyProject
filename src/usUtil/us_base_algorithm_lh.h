//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_base_algorithm_lh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 一些非常通用的计算，例如线段相交，点与三角形/多边形的关系之类；
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_BASE_ALGORITHM_LH_H_
#define _US_BASE_ALGORITHM_LH_H_

namespace uniscope_globe
{
	template<typename T>
	class us_base_algorithm_lh
	{
	public:
		us_base_algorithm_lh(void) {  }

		virtual ~us_base_algorithm_lh() {  }

	public:

		//
		// 判断点与线段的位置：1：左边CCW；0 ：线上；-1：右边，CW；
		//
		static int judge_side(const vector3<T>& point_judge, const vector3<T>& section_point1, const vector3<T>& section_point2, double epsilon = 0.001);

		//
		// 判断点与线段的位置(通过构成的三角形面积判断)
		//
		static int judge_side_by_area(const vector3<T>& point_judge, const vector3<T>& section_point1, const vector3<T>& section_point2, double epsilon);

		//
		// 判断点与线段的位置(通过夹角判断)
		//
		static int judge_side_by_angle(const vector3<T>& point_judge, const vector3<T>& section_point1, const vector3<T>& section_point2, double epsilon);

		//
		// 简要判断是否相交
		//
		static bool line_line_intersect_simp(const vector3<T> v1, const vector3<T> v2, const vector3<T> p1, const vector3<T> p2);

		// 
		// 在XY二维平面判断两线段相交关系，如果相交，给出p1p2上的交点；
		// 
		static line_line_ratationship line_line_intersect(const vector3<T> v1, const vector3<T> v2, const vector3<T> p1, const vector3<T> p2, vector3<T>& intersect_point);

		// 
		// 简要判断点是否在三角形内，与边角点重合的情况默认为true
		// 
		static bool point_in_triangle_simp(const vector3<T> P, const vector3<T> A, const vector3<T> B, const vector3<T> C);

		// 
		// 在XY平面内判断点与三角形关系
		// 
		static point_triangle_ralationship point_in_triangle(const vector3<T> P, const vector3<T> A, const vector3<T> B, const vector3<T> C);

		//
		// 判断点和多边形关系；
		// 
		static bool point_in_polygon(vector3<T> pos, vector3<T>* polygon_buffer, ulong polygon_size);
	};

	//// judge base //////////////////////////////////////////////////////////
	template<typename T>
	int us_base_algorithm_lh<T>::judge_side(const vector3<T>& point_judge, const vector3<T>& section_point1, const vector3<T>& section_point2, double epsilon)
	{
		return judge_side_by_angle(point_judge, section_point1, section_point2, epsilon);
		// 		return judge_side_by_area( point_judge, section_point1, section_point2, epsilon );
	}

	template<typename T>
	int us_base_algorithm_lh<T>::judge_side_by_area(const vector3<T>& point_judge, const vector3<T>& section_point1, const vector3<T>& section_point2, double epsilon)
	{
		double v_lf_sum1 = (point_judge.z - section_point1.z) * (point_judge.x - section_point2.x);
		double v_lf_sum2 = (point_judge.z - section_point2.z) * (point_judge.x - section_point1.x);
		double v_f_diff = v_lf_sum1 - v_lf_sum2;

		if (math_d::fabs_(v_f_diff) < epsilon)
		{
			return 0;
		}
		if (v_f_diff > 0)
		{
			return -1;
		}
		return 1;
	}

	template<typename T>
	int us_base_algorithm_lh<T>::judge_side_by_angle(const vector3<T>& point_judge, const vector3<T>& section_point1, const vector3<T>& section_point2, double epsilon)
	{

		vector3<T> line_vec = section_point2 - section_point1;
		line_vec.y = 0.0;
		line_vec.normalize();

		vector3<T> test_vec = point_judge - section_point1;
		test_vec.y = 0.0;
		double r = test_vec.length();
		test_vec.normalize();

		double val = math<double>::acos_(vector3<T>::dot(line_vec, test_vec));

		double dist = r * math<double>::sin_(val);

		if (dist < epsilon)
		{
			return 0;
		}

		vector3<T> cross = vector3<T>::cross(test_vec, line_vec);
		double test_dot = vector3<T>::dot(cross, vector3<T>::s_unit_y);
		if (test_dot < 0)
		{
			return -1;
		}

		return 1;
	}

	//// simple judgement ////////////////////////////////////////////////////
	template<typename T>
	bool us_base_algorithm_lh<T>::line_line_intersect_simp(const uniscope_globe::vector3<T> v1, const uniscope_globe::vector3<T> v2, const uniscope_globe::vector3<T> p1, const uniscope_globe::vector3<T> p2)
	{
		int p1_v1v2 = judge_side(p1, v1, v2);
		int p2_v1v2 = judge_side(p2, v1, v2);

		if (p1_v1v2 == 0)
		{
			if (v1.x != v2.x && (p1.x - v1.x) * (p1.x - v2.x) > 0)
			{
				return false;
			}
			if (v1.z != v2.z && (p1.z - v1.z) * (p1.z - v2.z) > 0)
			{
				return false;
			}
			return true;
		}
		if (p2_v1v2 == 0)
		{
			if (v1.x != v2.x && (p2.x - v1.x) * (p2.x - v2.x) > 0)
			{
				return false;
			}
			if (v1.z != v2.z && (p2.z - v1.z) * (p2.z - v2.z) > 0)
			{
				return false;
			}
			return true;
		}

		if (p1_v1v2 == p2_v1v2)
		{
			return false;
		}

		int v1_p1p2 = judge_side(v1, p1, p2);
		int v2_p1p2 = judge_side(v2, p1, p2);

		if (v1_p1p2 == v2_p1p2)
		{
			return false;
		}
		return true;
	}

	template<typename T>
	bool us_base_algorithm_lh<T>::point_in_triangle_simp(const vector3<T> P, const vector3<T> A, const vector3<T> B, const vector3<T> C)
	{
		if ((P.x > A.x && P.x > B.x && P.x > C.x)
			|| (P.x < A.x && P.x < B.x && P.x < C.x)
			|| (P.z > A.z && P.z > B.z && P.z > C.z)
			|| (P.z < A.z && P.z < B.z && P.z < C.z))
		{
			return false;
		}


		int C_AB = judge_side(C, A, B);
		int A_BC = judge_side(A, B, C);
		int B_AC = judge_side(B, A, C);

		int P_AB = judge_side(P, A, B);
		int P_BC = judge_side(P, B, C);
		int P_AC = judge_side(P, A, C);

		if ((P_AB != 0 && P_AB != C_AB) ||
			(P_BC != 0 && P_BC != A_BC) ||
			(P_AC != 0 && P_AC != B_AC))
		{
			return false;
		}

		return true;
	}

	//// relationship ////////////////////////////////////////////////////////
	template<typename T>
	line_line_ratationship us_base_algorithm_lh<T>::line_line_intersect(const vector3<T> v1, const vector3<T> v2, const vector3<T> p1, const vector3<T> p2, vector3<T>& intersect_point)
	{
		int p1_v1v2 = judge_side(p1, v1, v2);
		int p2_v1v2 = judge_side(p2, v1, v2);

		if (p1_v1v2 == 0)
		{
			if (v1.x != v2.x && (p1.x - v1.x) * (p1.x - v2.x) > 0)
			{
				return line_line_ratationship::NOT_INTERSECT;
			}
			if (v1.z != v2.z && (p1.z - v1.z) * (p1.z - v2.z) > 0)
			{
				return line_line_ratationship::NOT_INTERSECT;
			}
			intersect_point = p1;
			return line_line_ratationship::INTERSECT_POINT_C;
		}
		if (p2_v1v2 == 0)
		{
			if (v1.x != v2.x && (p2.x - v1.x) * (p2.x - v2.x) > 0)
			{
				return line_line_ratationship::NOT_INTERSECT;
			}
			if (v1.z != v2.z && (p2.z - v1.z) * (p2.z - v2.z) > 0)
			{
				return line_line_ratationship::NOT_INTERSECT;
			}
			intersect_point = p2;
			return line_line_ratationship::INTERSECT_POINT_D;
		}

		if (p1_v1v2 == p2_v1v2)
		{
			return line_line_ratationship::NOT_INTERSECT;
		}

		int v1_p1p2 = judge_side(v1, p1, p2);
		int v2_p1p2 = judge_side(v2, p1, p2);

		if (v1_p1p2 == v2_p1p2)
		{
			return line_line_ratationship::NOT_INTERSECT;
		}

		// 相交
		double denom = (p2.z - p1.z) * (v2.x - v1.x) - (p2.x - p1.x) * (v2.z - v1.z);
		double nume_a = (p2.x - p1.x) * (v1.z - p1.z) - (p2.z - p1.z) * (v1.x - p1.x);
		double nume_b = (v2.x - v1.x) * (v1.z - p1.z) - (v2.z - v1.z) * (v1.x - p1.x);

		double v_ua = nume_a / denom;
		double v_ub = nume_b / denom;

		if (v1_p1p2 == 0)
		{
			intersect_point = v1;
			intersect_point.y = p1.y + v_ub * (p2.y - p1.y);
			return line_line_ratationship::INTERSECT_POINT_A;
		}
		if (v2_p1p2 == 0)
		{
			intersect_point = v2;
			intersect_point.y = p1.y + v_ub * (p2.y - p1.y);
			return line_line_ratationship::INTERSECT_POINT_B;
		}

		intersect_point.x = v1.x + v_ua * (v2.x - v1.x);
		intersect_point.z = v1.z + v_ua * (v2.z - v1.z);
		intersect_point.y = p1.y + v_ub * (p2.y - p1.y);

		return line_line_ratationship::INTERSECT;
	}

	template<typename T>
	point_triangle_ralationship us_base_algorithm_lh<T>::point_in_triangle(const vector3<T> P, const vector3<T> A, const vector3<T> B, const vector3<T> C)
	{
		if ((P.x > A.x && P.x > B.x && P.x > C.x)
			|| (P.x < A.x && P.x < B.x && P.x < C.x)
			|| (P.z > A.z && P.z > B.z && P.z > C.z)
			|| (P.z < A.y && P.z < B.z && P.z < C.z))
		{
			return point_triangle_ralationship::EXTERIOR;
		}


		int C_AB = judge_side(C, A, B);
		int A_BC = judge_side(A, B, C);
		int B_AC = judge_side(B, A, C);

		int P_AB = judge_side(P, A, B);
		int P_BC = judge_side(P, B, C);
		int P_AC = judge_side(P, A, C);

		if ((P_AB != 0 && P_AB != C_AB) ||
			(P_BC != 0 && P_BC != A_BC) ||
			(P_AC != 0 && P_AC != B_AC))
		{
			return point_triangle_ralationship::EXTERIOR;
		}

		if (P_AB != 0 && P_BC != 0 && P_AC != 0)
		{
			return point_triangle_ralationship::INTERIOR;
		}

		if (P_AB == 0 && P_AC == 0)
		{
			return point_triangle_ralationship::VERTEX_A;
		}
		else if (P_AB == 0 && P_BC == 0)
		{
			return point_triangle_ralationship::VERTEX_B;
		}
		else if (P_AC == 0 && P_BC == 0)
		{
			return point_triangle_ralationship::VERTEX_C;
		}

		if (P_AB == 0)
		{
			return point_triangle_ralationship::BOUNDARY_AB;
		}
		else if (P_BC == 0)
		{
			return point_triangle_ralationship::BOUNDARY_BC;
		}
		else if (P_AC == 0)
		{
			return point_triangle_ralationship::BOUNDARY_AC;
		}

		return point_triangle_ralationship::UNKNOWN;
	}

	template<typename T>
	bool us_base_algorithm_lh<T>::point_in_polygon(vector3<T> pos, vector3<T>* polygon_buffer, ulong polygon_size)
	{
		bool inside = false;

		for (int i = 0; i < polygon_size; i++)
		{
			vector3<T> p1 = polygon_buffer[(i + polygon_size) % polygon_size];
			vector3<T> p2 = polygon_buffer[(i + 1 + polygon_size) % polygon_size];

			// 向x正方向扫描

			if (pos.z < p2.z)
			{
				//p1 above ray
				if (pos.z >= p1.z)
				{
					//p1 on or blew ray
					double val1 = (pos.z - p1.z) * (p2.x - p1.x);
					double val2 = (pos.x - p1.x) * (p2.z - p1.z);

					if (val1 > val2)
					{
						inside = !inside;
					}

				}
			}
			else if (pos.z < p1.z)
			{
				double val1 = (pos.z - p1.z) * (p2.x - p1.x);
				double val2 = (pos.x - p1.x) * (p2.z - p1.z);

				if (val1 < val2)
				{
					inside = !inside;
				}
			}
		}

		return inside;
	}


	//////////////////////////////////////////////////////////////////////////

} // uniscope_globe

#endif // _US_BASE_ALGORITHM_LH_H_
