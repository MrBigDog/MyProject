#include "us_polygon_algorithm.h"
#include <usUtil\us_vector2.h>
#include <usUtil\us_math.h>
#include <usUtil\us_common_file.h>
#include <usUtil\us_base_algorithm.h>

#include <winerror.h>

namespace uniscope_globe
{
	struct vector_with_relationship_ex
	{
		vector_3d vec_position;
		int point_poly_relation; // -1 or 0 or 1
		int origin_poly; // 1 or 2;
		vector_with_relationship_ex* ptr_pre;
		vector_with_relationship_ex* ptr_nxt;

		vector_with_relationship_ex()
		{
			point_poly_relation = -1;
			origin_poly = 1;
			ptr_pre = NULL;
			ptr_nxt = NULL;
		}
	};
	us_polygon_algorithm::us_polygon_algorithm(void)
	{
	}

	us_polygon_algorithm::~us_polygon_algorithm(void)
	{
	}

	void us_polygon_algorithm::CollapseVertex(coordinates_array& ring)
	{
		if (ring.size() < 2)
		{
			return;
		}

		coordinates_array temp_ring;
		temp_ring.push_back(ring[0]);

		for (int i = 1; i < ring.size(); i++)
		{
			if (i != 1 && ring[i] == ring[0])
			{
				break;
			}
			if (ring[i] != ring[i - 1])
			{
				temp_ring.push_back(ring[i]);
			}
		}

		ring.clear();
		ring = temp_ring;
	}

	bool us_polygon_algorithm::IsPolygonCCW(coordinates_array in_poly, bool z_is_height)
	{
		CollapseVertex(in_poly);
		int max_ni = 0;
		double max_value = -math_d::s_max_real;
		std::vector<vector_2d> tri_array;
		int side_num = in_poly.size();
		for (int ni = 0; ni < side_num; ni++)
		{
			if (z_is_height)
			{
				tri_array.push_back(vector_2d(in_poly[ni].x, in_poly[ni].y));
			}
			else
			{
				tri_array.push_back(vector_2d(in_poly[ni].x, in_poly[ni].z));
			}
			if (tri_array[ni].y > max_value || (tri_array[ni].y == max_value && tri_array[ni].x > tri_array[max_ni].x))
			{
				max_ni = ni;
				max_value = tri_array[ni].y;
			}
		}

		vector_2d prev_pos, next_pos, curr_pos;
		curr_pos = tri_array[max_ni];
		if (max_ni == 0) prev_pos = tri_array[side_num - 1];
		else prev_pos = tri_array[max_ni - 1];

		if (max_ni == side_num - 1) next_pos = tri_array[0];
		else next_pos = tri_array[max_ni + 1];

		prev_pos = prev_pos - curr_pos;
		prev_pos.normalize();
		prev_pos = prev_pos + curr_pos;
		next_pos = next_pos - curr_pos;
		next_pos.normalize();
		next_pos = next_pos + curr_pos;

		return (prev_pos.x < next_pos.x); // 顺时针（左手）为true；
	}

	void us_polygon_algorithm::AdjustRingDirection(coordinates_array& ring, bool to_left_hand, bool z_is_height)
	{
		int side_num = ring.size();

		if (IsPolygonCCW(ring, z_is_height) != to_left_hand)
		{
			coordinates_array new_ring = ring;
			for (int i = 0; i < side_num; i++)
			{
				ring[i] = new_ring[side_num - 1 - i];
			}
		}
	}

	int us_polygon_algorithm::VectorInRing(uniscope_globe::vector_3d vec, uniscope_globe::coordinates_array ring, double epsl)
	{
		double epsl_dbl = epsl * epsl;
		int rela = -1;

		int vers_num = ring.size();
		for (int i = 0; i < vers_num; i++)
		{
			vector_3d p1 = ring[i];
			vector_3d p2 = ring[(i + 1) % vers_num];

			// 向x正方向扫描
			// 处理y重复的情况
			double dist_p1y = math_d::fabs_(vec.y - p1.y);
			double dist_p2y = math_d::fabs_(vec.y - p2.y);
			if (dist_p1y < epsl && dist_p2y < epsl)
			{
				if ((vec.x - p1.x) * (vec.x - p2.x) <= 0)
				{
					rela = 0;
					break;
				}
			}
			else if (dist_p1y < epsl)
			{
				if (math_d::fabs_(vec.x - p1.x) < epsl)
				{
					rela = 0;
					break;
				}
				if (vec.x - p1.x < epsl && p1.y < p2.y)
				{
					rela *= -1;
				}
			}
			else if (dist_p2y < epsl)
			{
				if (math_d::fabs_(vec.x - p2.x) < epsl)
				{
					rela = 0;
					break;
				}
				// 仅判断下断点
				if (vec.x - p2.x < epsl && p2.y < p1.y)
				{
					rela *= -1;
				}
			}
			else if (vec.y < p2.y)
			{
				//p2 above ray
				if (vec.y > p1.y)
				{
					//p1 on or blew ray
					double val1 = (vec.y - p1.y) * (p2.x - p1.x);
					double val2 = (vec.x - p1.x) * (p2.y - p1.y);
					double val = val1 - val2;

					if (math_d::fabs_(val) < epsl_dbl)
					{
						rela = 0;
						break;
					}
					else if (val > 0)
					{
						rela *= -1;
					}
				}
			}
			else if (vec.y < p1.y)
			{
				double val1 = (vec.y - p1.y) * (p2.x - p1.x);
				double val2 = (vec.x - p1.x) * (p2.y - p1.y);

				double val = val1 - val2;

				if (math_d::fabs_(val) < epsl_dbl)
				{
					rela = 0;
					break;
				}
				else if (val < 0)
				{
					rela *= -1;
				}
			}
		}

		return rela;
	}

	int us_polygon_algorithm::VectorInPoly(vector_3d vec, vector<coordinates_array> poly, double epsl)
	{
		int rela_outring = VectorInRing(vec, poly[0], epsl);
		if (rela_outring != 1)
		{
			return rela_outring;
		}
		for (int i = 1; i < poly.size(); i++)
		{
			int rela = VectorInRing(vec, poly[i], epsl);
			if (rela == 0)
			{
				return rela;
			}
			if (rela == 1)
			{
				return -1;
			}
		}
		return rela_outring;
	}



	void us_polygon_algorithm::RingClip(coordinates_array& ring1, coordinates_array& ring2, double epsl)
	{
		vector_3d v1, v2, p1, p2;
		coordinates_array insert_v, insert_p, insert_v_all;
		for (int i = 0; i < ring1.size(); i++)
		{
			v1 = ring1[i];
			v2 = ring1[(i + 1) % ring1.size()];

			for (int j = 0; j < ring2.size(); j++)
			{
				p1 = ring2[j];
				p2 = ring2[(j + 1) % ring2.size()];

				if (LineLineClip(v1, v2, p1, p2, epsl, insert_v, insert_p))
				{
					// p: 插入点
					for (int k = 0; k < insert_p.size(); k++)
					{
						ring2.insert(ring2.begin() + j + 1, insert_p[k]);
						j++;
					}
					// v: 加入待插入列表；
					for (int k = 0; k < insert_v.size(); k++)
					{
						insert_v_all.push_back(insert_v[k]);
					}
				}
			}

			// insert_v_all排序；
			SortByDist(insert_v_all, v1);
			CollapseVertex(insert_v_all);
			for (int k = 0; k < insert_v_all.size(); k++)
			{
				ring1.insert(ring1.begin() + i + 1, insert_v_all[k]);
				i++;
			}
			insert_v_all.clear();
		}
	}
	void us_polygon_algorithm::SortByDist(coordinates_array& vers, vector_3d ver_start)
	{
		if (vers.size() <= 1) return;

		double distMin = 0.0, distCurr = 0.0;
		coordinates_array versTemp = vers;
		vers.clear();
		while (versTemp.size() > 0)
		{
			int index = 0;
			for (int i = 0; i < versTemp.size(); i++)
			{
				if (i == 0)
				{
					distMin = distCurr = (versTemp[i] - ver_start).length();
					index = 0;
				}
				else if (i > 0)
				{
					distCurr = (versTemp[i] - ver_start).length();
					if (distMin > distCurr)
					{
						distMin = distCurr;
						index = i;
					}
				}
			}
			vers.push_back(versTemp[index]);
			versTemp.erase(versTemp.begin() + index);
		}

	}

	bool us_polygon_algorithm::LineLineClip(vector_3d v1, vector_3d v2, vector_3d p1, vector_3d p2, double epsl,
		coordinates_array& insert_v, coordinates_array& insert_p)
	{
		insert_v.clear();
		insert_p.clear();

		int p1_v1v2 = us_base_algorithm<double>::judge_side(p1, v1, v2, epsl);
		int p2_v1v2 = us_base_algorithm<double>::judge_side(p2, v1, v2, epsl);

		if (p1_v1v2 == p2_v1v2)
		{
			//  不相交；
			if (p1_v1v2 != 0) return false;

			// 共线
			double dis_p1v1 = (p1 - v1).length();
			double dis_p1v2 = (p1 - v2).length();
			double dis_p2v1 = (p2 - v1).length();
			double dis_p2v2 = (p2 - v2).length();
			double dis_v1v2 = (v1 - v2).length();
			double dis_p1p2 = (p1 - p2).length();

			// 判断 p1,p2 是否落入 v1,v2 间
			bool p1_in = (dis_p1v1 > epsl && dis_p1v2 > epsl && fabs(dis_p1v1 + dis_p1v2 - dis_v1v2) < epsl);
			bool p2_in = (dis_p2v1 > epsl && dis_p2v2 > epsl && fabs(dis_p2v1 + dis_p2v2 - dis_v1v2) < epsl);
			if (p1_in && p2_in)
			{
				if (dis_p1v1 < dis_p2v1)
				{
					insert_v.push_back(p1);
					insert_v.push_back(p2);
				}
				else
				{
					insert_v.push_back(p2);
					insert_v.push_back(p1);
				}
			}
			else if (p1_in)
			{
				insert_v.push_back(p1);
			}
			else if (p2_in)
			{
				insert_v.push_back(p2);
			}
			// 判断 v1,v2 是否落入 p1,p2 间
			if (insert_v.size() == 2)
			{
				return true;
			}
			bool v1_in = (dis_p1v1 > epsl && dis_p2v1 > epsl && fabs(dis_p1v1 + dis_p2v1 - dis_p1p2) < epsl);
			bool v2_in = (dis_p1v2 > epsl && dis_p2v2 > epsl && fabs(dis_p1v2 + dis_p2v2 - dis_p1p2) < epsl);
			if (v1_in && v2_in)
			{
				if (dis_p1v1 < dis_p1v2)
				{
					insert_p.push_back(v1);
					insert_p.push_back(v2);
				}
				else
				{
					insert_p.push_back(v2);
					insert_p.push_back(v1);
				}
			}
			else if (p1_in)
			{
				insert_v.push_back(p1);
			}
			else if (p2_in)
			{
				insert_v.push_back(p2);
			}

			if (insert_p.size() + insert_v.size() > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		int v1_p1p2 = us_base_algorithm<double>::judge_side(v1, p1, p2, epsl);
		int v2_p1p2 = us_base_algorithm<double>::judge_side(v2, p1, p2, epsl);

		if (v1_p1p2 == v2_p1p2)
		{
			return false; // 共线已排除，仅有不相交情况；
		}

		int zero_num = 0;
		int zero_index = -1; // 用于仅有一个点在对边线上的情况；

		if (p1_v1v2 == 0)
		{
			zero_num++;
			zero_index = 1;
		}
		if (p2_v1v2 == 0)
		{
			zero_num++;
			zero_index = 2;
		}
		if (v1_p1p2 == 0)
		{
			zero_num++;
			zero_index = 3;
		}
		if (v2_p1p2 == 0)
		{
			zero_num++;
			zero_index = 4;
		}

		if (zero_num == 0) // 求交点并加入两个列表；
		{
			// 相交
			double denom = (p2.y - p1.y) * (v2.x - v1.x) - (p2.x - p1.x) * (v2.y - v1.y);
			double nume_a = (p2.x - p1.x) * (v1.y - p1.y) - (p2.y - p1.y) * (v1.x - p1.x);
			double nume_b = (v2.x - v1.x) * (v1.y - p1.y) - (v2.y - v1.y) * (v1.x - p1.x);

			double v_ua = nume_a / denom;
			double v_ub = nume_b / denom;

			vector_3d vec_cross;
			vec_cross.x = v1.x + v_ua * (v2.x - v1.x);
			vec_cross.y = v1.y + v_ua * (v2.y - v1.y);
			vec_cross.z = p1.z + v_ub * (p2.z - p1.z);

			insert_v.push_back(vec_cross);
			insert_p.push_back(vec_cross);
			return true;
		}
		if (zero_num == 1) // 该端点加入到对方列表
		{
			switch (zero_index)
			{
			case 1:
				insert_v.push_back(p1);
				break;
			case 2:
				insert_v.push_back(p2);
				break;
			case 3:
				insert_p.push_back(v1);
				break;
			case 4:
				insert_p.push_back(v2);
				break;
			default:
				break;
			}
			return true;
		}
		return false;
	}


	double us_polygon_algorithm::GetFirstPointHeight(vector<coordinates_array> poly)
	{
		long ring_count = poly.size();
		if (ring_count == 0) return 0;
		coordinates_array ring = poly[0];
		long ver_count = ring.size();
		if (ver_count == 0) return 0;

		double z = ring[0].z;
		return z;
	}
	void us_polygon_algorithm::RingsRelationship(coordinates_array ring1, coordinates_array ring2, double epsl, us_polygon_relationship& out_relate)
	{
		// box比较；
		vector_3d min_poly1 = vector_3d(DBL_MAX, DBL_MAX, DBL_MAX);
		vector_3d max_poly1 = vector_3d(DBL_MIN, DBL_MIN, DBL_MIN);
		int ring1_num = ring1.size();
		for (int i = 0; i < ring1_num; i++)
		{
			min_poly1.x = MIN(min_poly1.x, ring1[i].x);
			min_poly1.y = MIN(min_poly1.y, ring1[i].y);
			min_poly1.z = MIN(min_poly1.z, ring1[i].z);

			max_poly1.x = MAX(max_poly1.x, ring1[i].x);
			max_poly1.y = MAX(max_poly1.y, ring1[i].y);
			max_poly1.z = MAX(max_poly1.z, ring1[i].z);
		}

		vector_3d min_poly2 = vector_3d(DBL_MAX, DBL_MAX, DBL_MAX);
		vector_3d max_poly2 = vector_3d(DBL_MIN, DBL_MIN, DBL_MIN);

		int ring2_num = ring2.size();
		for (int i = 0; i < ring2_num; i++)
		{
			min_poly2.x = MIN(min_poly2.x, ring2[i].x);
			min_poly2.y = MIN(min_poly2.y, ring2[i].y);
			min_poly2.z = MIN(min_poly2.z, ring2[i].z);

			max_poly2.x = MAX(max_poly2.x, ring2[i].x);
			max_poly2.y = MAX(max_poly2.y, ring2[i].y);
			max_poly2.z = MAX(max_poly2.z, ring2[i].z);
		}

		if (min_poly1.x > max_poly2.x || max_poly1.x < min_poly2.x || min_poly1.y > max_poly2.y || max_poly1.y < min_poly2.y)
		{
			out_relate = us_polygon_relationship::disjoint;
			return;
		}

		// ring1点与ring2的关系，内/外/边个数
		int inner_num = 0;
		int outer_num = 0;
		int edge_num = 0;
		for (int i = 0; i < ring1_num; i++)
		{
			int vec_ring_rela = VectorInRing(ring1[i], ring2, epsl);
			if (vec_ring_rela == 0)
			{
				edge_num++;
			}
			else if (vec_ring_rela == 1)
			{
				inner_num++;
			}
			else
			{
				outer_num++;
			}
		}


		// 内外都有：
		if (inner_num > 0 && outer_num > 0)
		{
			out_relate = us_polygon_relationship::intersect;
			return;
		}


		// line line intersection
		vector_3d vec_in_ring1_a, vec_in_ring1_b;
		vector_3d vec_in_ring2_a, vec_in_ring2_b;
		for (int i = 0; i < ring1_num; i++)
		{
			vec_in_ring1_a = ring1[i];
			vec_in_ring1_b = ring1[(i + 1) % ring1_num];
			for (int j = 0; j < ring2_num; j++)
			{
				vec_in_ring2_a = ring2[j];
				vec_in_ring2_b = ring2[(j + 1) % ring2_num];
				vector_3d vec_cross;
				if (us_base_algorithm<double>::line_line_intersect(vec_in_ring1_a, vec_in_ring1_b, vec_in_ring2_a, vec_in_ring2_b, vec_cross, epsl) == line_line_ratationship::INTERSECT)
				{
					out_relate = us_polygon_relationship::intersect;
					return;
				}
			}
		}

		double epsl_dbl = epsl * epsl;
		if (inner_num == 0 && outer_num == 0)
		{
			bool be_equal = false;
			if (ring1_num == ring2_num)
			{
				// 找相同的点
				int first_equal_index = -1;
				for (int i = 0; i < ring2_num; i++)
				{
					if (math_d::fabs_((ring2[i] - ring1[0]).length()) < epsl_dbl)
					{
						first_equal_index = i;
						break;
					}
				}
				// 判断是否都相等
				if (first_equal_index != -1)
				{
					int i = 0;
					for (; i < ring1_num; i++)
					{
						if (math_d::fabs_((ring2[(i + first_equal_index) % ring2_num] - ring1[i]).length()) > epsl_dbl)
						{
							break;
						}
					}
					if (i == ring1_num)
					{
						be_equal = true;
					}
				}
			}

			// 顶点都相等:
			if (be_equal)
			{
				out_relate = us_polygon_relationship::equal;
				return;
			}
			else
			{
				// 判断是within还是touch:
				for (int i = 0; i < ring1_num; i++)
				{
					vector_3d vec_pre = ring1[i];
					vector_3d vec_nxt = ring1[(i + 1) % ring1_num];
					vector_3d vec_center = (vec_pre + vec_nxt) / 2;

					int pnt_ring_rela = VectorInRing(vec_center, ring2, epsl);
					if (pnt_ring_rela == 1)
					{
						out_relate = us_polygon_relationship::within;
						return;
					}
					else if (pnt_ring_rela == -1)
					{
						out_relate = us_polygon_relationship::touch;
						return;
					}
				}
				out_relate = us_polygon_relationship::within;
				return;
			}
		}
		if (outer_num == 0)
		{
			out_relate = us_polygon_relationship::within;
			return;
		}
		if (inner_num == 0)
		{
			// ring2任意点是否在ring1内
			for (int i = 0; i < ring2_num; i++)
			{
				int vec_ring_rela = VectorInRing(ring2[i], ring1, epsl);
				// 在内:包含
				if (vec_ring_rela == 1)
				{
					out_relate = us_polygon_relationship::contain;
					return;
				}
				if (vec_ring_rela == -1)
				{
					break;
				}
			}
			if (edge_num == 0)
			{
				out_relate = us_polygon_relationship::disjoint;
				return;
			}
			out_relate = us_polygon_relationship::touch;
			return;
		}
		out_relate = us_polygon_relationship::disjoint;
		return;
	}


	void us_polygon_algorithm::RingPolyRelationship(coordinates_array ring, coordinates_part_array poly, us_coordinate_unit unit, us_polygon_relationship& out_relate)
	{
		// 外环关系
		double epsl = (unit == us_coordinate_unit::degree) ? 0.00000001 : 0.0001;
		RingsRelationship(ring, poly[0], epsl, out_relate);
		// Equal: Equal
		// Disjoint: Disjoint
		// Intersect: Intersect
		// Contain: Contain
		// Touch: Touch
		if (out_relate != us_polygon_relationship::within)
		{
			return;
		}

		// Within: 判断内环
		for (int i = 1; i < poly.size(); i++)
		{
			us_polygon_relationship rela_subring;
			RingsRelationship(ring, poly[i], epsl, rela_subring);
			// Equal: Touch
			if (rela_subring == us_polygon_relationship::equal)
			{
				out_relate = us_polygon_relationship::touch;
				return;
			}
			// Intersect: Intersect;
			// Contain: Intersect;
			else if (rela_subring == us_polygon_relationship::intersect || rela_subring == us_polygon_relationship::contain)
			{
				out_relate = us_polygon_relationship::intersect;
				return;
			}
			else if (rela_subring == us_polygon_relationship::within)
			{
				// Touch or Disjoint
				bool be_touch = false;
				for (int j = 0; j < ring.size(); j++)
				{
					if (VectorInRing(ring[j], poly[i], epsl) == 0)
					{
						be_touch = true;
						break;
					}
				}
				if (be_touch)
				{
					out_relate = us_polygon_relationship::touch;
					return;
				}
				else
				{
					out_relate = us_polygon_relationship::disjoint;
					return;
				}
			}
			// Disjoint & Touch: 继续判断和其他内环的关系；
		}
		out_relate = us_polygon_relationship::within;
		return;
	}



	bool us_polygon_algorithm::polys_bool_operation(vector<coordinates_array> polygon1, vector<coordinates_array> polygon2, us_coordinate_unit unit, us_bool_type type, vector<coordinates_array>& out_polygon)
	{
		// relationship
		us_polygon_relationship rela;
		polys_relationship(polygon1, polygon2, unit, &rela);

		// disjoint
		if (rela == us_polygon_relationship::disjoint)
		{
			// union: poly1, poly2 分别加入 collection
			if (type == us_bool_type::_union)
			{
				out_polygon = polygon1;
				for (int ni = 0; ni < polygon2.size(); ni++)
				{
					out_polygon.push_back(polygon2[ni]);
				}
				return true;
			}
			// intersection: null
			else if (type == us_bool_type::intersection)
			{
				return false;
			}
			// difference: poly1
			else
			{
				out_polygon = polygon1;
			}
			return true;
		}

		// Equal
		else if (rela == us_polygon_relationship::equal)
		{
			// union: poly1
			if (type == us_bool_type::_union)
			{
				out_polygon = polygon1;
				return true;
			}
			// intersection: poly1
			else if (type == us_bool_type::intersection)
			{
				out_polygon = polygon1;
				return true;
			}
			// difference: null
			else
			{
				return false;
			}
		}

		// Within
		else if (rela == us_polygon_relationship::within)
		{
			// union: poly2
			if (type == us_bool_type::_union)
			{
				out_polygon = polygon2;
				return true;
			}
			// intersection: poly1
			else if (type == us_bool_type::intersection)
			{
				out_polygon = polygon1;
				return true;
			}
			// difference: null
			else
			{
				return false;
			}
		}

		// Contian
		else if (rela == us_polygon_relationship::contain)
		{
			// union: poly1;
			if (type == us_bool_type::_union)
			{
				out_polygon = polygon1;
				return true;
			}
			// intersection: poly2;
			else if (type == us_bool_type::intersection)
			{
				out_polygon = polygon2;
				return true;
			}
			// difference: poly2 的外环 作为 poly1 的内环； poly2如果有内环则加入 Collection；
			else
			{
				sub_bool_operation(polygon1, polygon2, unit, type, out_polygon);
				return true;
			}
		}

		// Touch
		else if (rela == us_polygon_relationship::touch)
		{
			// union: poly1 + poly2;
			if (type == us_bool_type::_union)
			{
				sub_bool_operation(polygon1, polygon2, unit, type, out_polygon);
				return true;
			}
			// intersection: null;
			else if (type == us_bool_type::intersection)
			{
				return false;
			}
			// difference: poly1
			else
			{
				out_polygon = polygon1;
				return true;
			}
		}

		// Intersect
		else if (rela == us_polygon_relationship::intersect)
		{
			sub_bool_operation(polygon1, polygon2, unit, type, out_polygon);
			if (out_polygon.size() == 1)
			{
				if (out_polygon[0].size() != 0)
				{
					int n_count = out_polygon[0].size();
					if (out_polygon[0][0] != out_polygon[0][n_count - 1])
						out_polygon[0].push_back(out_polygon[0][0]);
				}
			}
			return true;
		}
	}


	// interface /////////////////////////////////////////////////////////////
	bool us_polygon_algorithm::polys_relationship(vector<coordinates_array> polygon1, vector<coordinates_array> polygon2, us_coordinate_unit unit, us_polygon_relationship *outRelationship)
	{
		// 前提: 默认输入多边形没有自相交没有冗余点。
		*outRelationship = us_polygon_relationship::disjoint;
		if (polygon1.size() == 0 || polygon2.size() == 0) return false;

		/*LONG*/long ring_num_1 = polygon1.size();
		/*LONG*/long ring_num_2 = polygon2.size();

		if (ring_num_1 < 1 || ring_num_2 < 1) return false;

		vector<coordinates_array> innerPoly1;
		vector<coordinates_array> innerPoly2;
		polygon_to_inner(polygon1, innerPoly1, true, true);
		polygon_to_inner(polygon2, innerPoly2, true, true);

		// 删除无效点；
		for (int i = 0; i < innerPoly1.size(); i++)
		{
			CollapseVertex(innerPoly1[i]);
		}
		for (int i = 0; i < innerPoly2.size(); i++)
		{
			CollapseVertex(innerPoly2[i]);
		}

		// 计算poly1的外环与poly2的关系；
		us_polygon_relationship outring_rela;
		RingPolyRelationship(innerPoly1[0], innerPoly2, unit, outring_rela);
		// 	USPolygonRelationship outring_rela = RingsRelationship( innerPoly1[0], innerPoly2[0], unit );

			//////////////////////////////////////////////////////////////////////////
			// Disjoint, Intersect and Touch
		if (outring_rela == us_polygon_relationship::disjoint || outring_rela == us_polygon_relationship::intersect || outring_rela == us_polygon_relationship::touch)
		{
			*outRelationship = outring_rela;
			return S_OK;
		}

		us_polygon_relationship inner_rela;
		//////////////////////////////////////////////////////////////////////////
		// Equal
		if (outring_rela == us_polygon_relationship::equal)
		{
			// 计算所有ring1_poly2,ring2_poly1; 可能有四种结果
			// 首先判断是否Equal
			if (ring_num_1 == ring_num_2)
			{
				int first_equal_index = -1;
				// 寻找第一个Equal的内环
				for (int i = 1; i < ring_num_2; i++)
				{
					RingsRelationship(innerPoly1[1], innerPoly2[i], unit, inner_rela);
					if (inner_rela == us_polygon_relationship::equal)
					{
						first_equal_index = i - 1;
						break;
					}
					// Intersect：直接返回；
					else if (inner_rela == us_polygon_relationship::intersect)
					{
						*outRelationship = us_polygon_relationship::intersect;
						return S_OK;
					}
				}
				// 找到相等的：判断其他内环是否相等；
				if (first_equal_index != -1)
				{
					bool be_all_equal = true;
					for (int i = 2; i < ring_num_1; i++)
					{
						coordinates_array inner_ring2 = (i + first_equal_index > ring_num_2 - 1) ? innerPoly2[(i + first_equal_index) % ring_num_2 + 1] : innerPoly2[i + first_equal_index];
						RingsRelationship(innerPoly1[i], inner_ring2, unit, inner_rela);
						if (inner_rela != us_polygon_relationship::equal)
						{
							be_all_equal = false;
							break;
						}
					}
					// 都相等：返回
					if (be_all_equal)
					{
						*outRelationship = us_polygon_relationship::equal;
						return S_OK;
					}
				}
				// 否则：继续判断；
			}
			// Within: ring2_poly1均Touch或者Disjoint(没有Within和Intersect)
			bool be_intersect = false;
			for (int i = 1; i < ring_num_2; i++)
			{
				RingPolyRelationship(innerPoly2[i], innerPoly1, unit, inner_rela);
				if (inner_rela == us_polygon_relationship::within || inner_rela == us_polygon_relationship::intersect)
				{
					be_intersect = true;
					break;
				}
			}
			if (be_intersect == false)
			{
				*outRelationship = us_polygon_relationship::within;
				return S_OK;
			}
			// Contain: ring1_poly2均Touch或者Disjoint(没有Within和Intersect)
			be_intersect = false;
			for (int i = 1; i < ring_num_1; i++)
			{
				RingPolyRelationship(innerPoly1[i], innerPoly2, unit, inner_rela);
				if (inner_rela == us_polygon_relationship::within || inner_rela == us_polygon_relationship::intersect)
				{
					be_intersect = true;
					break;
				}
			}
			if (be_intersect == false)
			{
				*outRelationship = us_polygon_relationship::contain;
				return S_OK;
			}
			// Intersect: 其他情况
			*outRelationship = us_polygon_relationship::intersect;
			return S_OK;
		}

		//////////////////////////////////////////////////////////////////////////
		// Within
		if (outring_rela == us_polygon_relationship::within)
		{
			// Within: ring2_poly1均Touch或者Disjoint(没有Within和Intersect)
			bool be_intersect = false;
			for (int i = 1; i < ring_num_2; i++)
			{
				RingPolyRelationship(innerPoly2[i], innerPoly1, unit, inner_rela);
				if (inner_rela == us_polygon_relationship::within || inner_rela == us_polygon_relationship::intersect)
				{
					be_intersect = true;
					break;
				}
			}
			if (be_intersect == false)
			{
				*outRelationship = us_polygon_relationship::within;
				return S_OK;
			}

			*outRelationship = us_polygon_relationship::intersect;
			return S_OK;
		}

		//////////////////////////////////////////////////////////////////////////
		// Contain
		if (outring_rela == us_polygon_relationship::contain)
		{
			// Contain: ring1_poly2均Touch或者Disjoint(没有Within和Intersect)
			bool be_intersect = false;
			for (int i = 1; i < ring_num_1; i++)
			{
				RingPolyRelationship(innerPoly1[i], innerPoly2, unit, inner_rela);
				if (inner_rela == us_polygon_relationship::within || inner_rela == us_polygon_relationship::intersect)
				{
					be_intersect = true;
					break;
				}
			}
			if (be_intersect == false)
			{
				*outRelationship = us_polygon_relationship::contain;
				return S_OK;
			}

			*outRelationship = us_polygon_relationship::intersect;
			return S_OK;
		}
		return S_OK;
	}

	void us_polygon_algorithm::sub_bool_operation(vector<coordinates_array> polygon1, vector<coordinates_array> polygon2, us_coordinate_unit unit, us_bool_type type, vector<coordinates_array>& out_polygon)
	{
		// Set invert flags depending on boolean operation type:
		// INTERSECTION: A^B = and(A,B)
		// UNION:        A|B = not(and(not(A),not(B)))
		// DIFFERENCE:   A-B = and(A,not(B))
		bool invert1 = (type == us_bool_type::_union);
		bool invert2 = (type != us_bool_type::intersection);
		bool invertOut = (type == us_bool_type::_union);


		//////////////////////////////////////////////////////////////////////////
		vector<coordinates_array> innerPoly1;
		vector<coordinates_array> innerPoly2;
		polygon_to_inner(polygon1, innerPoly1, true, true);
		polygon_to_inner(polygon2, innerPoly2, true, true);

		// 调整顺序；
		bool ring1_dir = !invert1;
		CollapseVertex(innerPoly1[0]);
		AdjustRingDirection(innerPoly1[0], ring1_dir, true);
		for (int i = 1; i < innerPoly1.size(); i++)
		{
			CollapseVertex(innerPoly1[i]);
			AdjustRingDirection(innerPoly1[i], !ring1_dir, true);
		}
		bool ring2_dir = !invert2;
		CollapseVertex(innerPoly2[0]);
		AdjustRingDirection(innerPoly2[0], ring2_dir, true);
		for (int i = 1; i < innerPoly2.size(); i++)
		{
			CollapseVertex(innerPoly2[i]);
			AdjustRingDirection(innerPoly2[i], !ring2_dir, true);
		}

		// Clip所有边；
		double epsl = (unit == us_coordinate_unit::degree) ? 0.00000001 : 0.0001;
		for (int i = 0; i < innerPoly1.size(); i++)
		{
			coordinates_array ring1 = innerPoly1[i];
			for (int j = 0; j < innerPoly2.size(); j++)
			{
				coordinates_array ring2 = innerPoly2[j];
				RingClip(ring1, ring2, epsl);
				innerPoly2[j] = ring2;
			}
			innerPoly1[i] = ring1;
		}

		//////////////////////////////////////////////////////////////////////////
		int point_num1 = 0;
		for (int i = 0; i < innerPoly1.size(); i++)
		{
			point_num1 += innerPoly1[i].size();
		}
		int point_num2 = 0;
		for (int i = 0; i < innerPoly2.size(); i++)
		{
			point_num2 += innerPoly2[i].size();
		}

		// 得到所有顶点的关系:in/out/clip； 
		int index_begin = 0;
		vector<vector_with_relationship_ex> points1_with_rela;
		points1_with_rela.resize(point_num1);
		for (int i = 0; i < innerPoly1.size(); i++)
		{
			coordinates_array ring = innerPoly1[i];
			int point_num = ring.size();
			for (int j = 0; j < point_num; j++)
			{
				points1_with_rela[index_begin + j].origin_poly = 1;
				points1_with_rela[index_begin + j].vec_position = ring[j];
				points1_with_rela[index_begin + j].point_poly_relation = VectorInPoly(ring[j], innerPoly2, epsl);
				if (invert2)
				{
					points1_with_rela[index_begin + j].point_poly_relation *= -1;
				}

				points1_with_rela[(j + point_num - 1) % point_num + index_begin].ptr_nxt = &(points1_with_rela[index_begin + j]);
				points1_with_rela[(j + point_num + 1) % point_num + index_begin].ptr_pre = &(points1_with_rela[index_begin + j]);
			}
			index_begin += point_num;
		}

		index_begin = 0;
		vector<vector_with_relationship_ex> points2_with_rela;
		points2_with_rela.resize(point_num2);
		for (int i = 0; i < innerPoly2.size(); i++)
		{
			coordinates_array ring = innerPoly2[i];
			int point_num = ring.size();
			for (int j = 0; j < point_num; j++)
			{
				points2_with_rela[index_begin + j].origin_poly = 2;
				points2_with_rela[index_begin + j].vec_position = ring[j];
				points2_with_rela[index_begin + j].point_poly_relation = VectorInPoly(ring[j], innerPoly1, epsl);
				if (invert1)
				{
					points2_with_rela[index_begin + j].point_poly_relation *= -1;
				}

				points2_with_rela[(j + point_num - 1) % point_num + index_begin].ptr_nxt = &(points2_with_rela[index_begin + j]);
				points2_with_rela[(j + point_num + 1) % point_num + index_begin].ptr_pre = &(points2_with_rela[index_begin + j]);
			}
			index_begin += point_num;

		}

		// 处理邻接关系，删除所有out，得到有效的点；
		vector<vector_with_relationship_ex*> valid_points_ptr;
		vector<int> clip_index1, clip_index2;

		for (int i = 0; i < point_num1; i++)
		{
			bool be_valid = true;
			// out: invalid
			if (points1_with_rela[i].point_poly_relation == -1)
			{
				be_valid = false;
			}
			// rela == clip时: 前后点状态相同则改为和前后点相同的rela；
			else if (points1_with_rela[i].point_poly_relation == 0)
			{
				vector_3d vec_curr = points1_with_rela[i].vec_position;
				vector_3d vec_pre = points1_with_rela[(i + point_num1 - 1) % point_num1].vec_position;
				vector_3d vec_nxt = points1_with_rela[(i + point_num1 + 1) % point_num1].vec_position;

				vector_3d vec_center_pre = (vec_pre + vec_curr) / 2;
				vector_3d vec_center_nxt = (vec_nxt + vec_curr) / 2;

				int rela_center_pre = VectorInPoly(vec_center_pre, innerPoly2, epsl);
				int rela_center_nxt = VectorInPoly(vec_center_nxt, innerPoly2, epsl);
				if (invert2)
				{
					rela_center_pre *= -1;
					rela_center_nxt *= -1;
				}
				if (rela_center_pre == rela_center_nxt)
				{
					points1_with_rela[i].point_poly_relation = rela_center_pre;
					if (rela_center_pre != 1)
					{
						be_valid = false;
					}
				}
				else
				{
					clip_index1.push_back(i);
				}
			}
			// valid: 填写pre，nxt；加入valid列表;
			if (be_valid)
			{
				// 			points1_with_rela[(i + point_num1 - 1) % point_num1].ptr_nxt = &(points1_with_rela[i]);
				// 			points1_with_rela[(i + point_num1 + 1) % point_num1].ptr_pre = &(points1_with_rela[i]);
				valid_points_ptr.push_back(&(points1_with_rela[i]));
			}
			else
			{
				if (points1_with_rela[i].ptr_pre != NULL) points1_with_rela[i].ptr_pre->ptr_nxt = NULL;
				if (points1_with_rela[i].ptr_nxt != NULL) points1_with_rela[i].ptr_nxt->ptr_pre = NULL;
			}
		}
		for (int i = 0; i < point_num2; i++)
		{
			bool be_valid = true;
			// out: invalid
			if (points2_with_rela[i].point_poly_relation == -1)
			{
				be_valid = false;
			}
			// rela == clip时: 前后点状态相同则改为和前后点相同的rela；
			if (points2_with_rela[i].point_poly_relation == 0)
			{
				vector_3d vec_curr = points2_with_rela[i].vec_position;
				vector_3d vec_pre = points2_with_rela[(i + point_num2 - 1) % point_num2].vec_position;
				vector_3d vec_nxt = points2_with_rela[(i + point_num2 + 1) % point_num2].vec_position;

				vector_3d vec_center_pre = (vec_pre + vec_curr) / 2;
				vector_3d vec_center_nxt = (vec_nxt + vec_curr) / 2;

				int rela_center_pre = VectorInPoly(vec_center_pre, innerPoly1, epsl);
				int rela_center_nxt = VectorInPoly(vec_center_nxt, innerPoly1, epsl);
				if (invert1)
				{
					rela_center_pre *= -1;
					rela_center_nxt *= -1;
				}
				if (rela_center_pre == rela_center_nxt)
				{
					points2_with_rela[i].point_poly_relation = rela_center_pre;

					if (rela_center_pre != 1)
					{
						be_valid = false;
					}
				}
				else
				{
					clip_index2.push_back(i);
				}
			}
			// valid: 填写pre，nxt；加入valid列表;
			if (be_valid)
			{
				// 			points2_with_rela[(i + point_num2 - 1) % point_num2].ptr_nxt = &(points2_with_rela[i]);
				// 			points2_with_rela[(i + point_num2 + 1) % point_num2].ptr_pre = &(points2_with_rela[i]);
				valid_points_ptr.push_back(&(points2_with_rela[i]));
			}
			else
			{
				if (points2_with_rela[i].ptr_pre != NULL) points2_with_rela[i].ptr_pre->ptr_nxt = NULL;
				if (points2_with_rela[i].ptr_nxt != NULL) points2_with_rela[i].ptr_nxt->ptr_pre = NULL;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		vector<coordinates_array> out_rings;
		vector<vector_with_relationship_ex*>::iterator itr = valid_points_ptr.begin();

		while (valid_points_ptr.size() > 0)
		{
			coordinates_array out_ring;

			// 从第一个点开始
			vector_with_relationship_ex* begin_ptr = valid_points_ptr[0];
			vector_with_relationship_ex* curr_ptr = begin_ptr;

			while (valid_points_ptr.size() > 0)
			{
				// 加入
				out_ring.push_back(curr_ptr->vec_position);
				// 从列表中删除
				itr = std::find(valid_points_ptr.begin(), valid_points_ptr.end(), curr_ptr);
				if (itr == valid_points_ptr.end())
				{
					break;
				}
				else
				{
					if ((*itr)->ptr_pre != NULL) (*itr)->ptr_pre->ptr_nxt = NULL;
					if ((*itr)->ptr_nxt != NULL) (*itr)->ptr_nxt->ptr_pre = NULL;
					valid_points_ptr.erase(itr);
				}

				// 取nxt点
				vector_with_relationship_ex* nxt_ptr = NULL;
				// clip
				if (curr_ptr->point_poly_relation == 0 && !(out_ring.size() == 1 && curr_ptr->ptr_nxt != NULL/* && curr_ptr->ptr_nxt->point_poly_relation == 1 */))
				{
					vector_with_relationship_ex* clip_ptr = NULL;
					// 寻找另一多边形中对应的点
					vector_3d temp;
					if (curr_ptr->origin_poly == 1)
					{
						for (int j = 0; j < clip_index2.size(); j++)
						{
							temp = points2_with_rela[clip_index2[j]].vec_position;
							if ((temp - curr_ptr->vec_position).length() < epsl)
							{
								clip_ptr = &(points2_with_rela[clip_index2[j]]);
								break;
							}
						}
					}
					else
					{
						for (int j = 0; j < clip_index1.size(); j++)
						{
							temp = points1_with_rela[clip_index1[j]].vec_position;
							if ((temp - curr_ptr->vec_position).length() < epsl)
							{
								clip_ptr = &(points1_with_rela[clip_index1[j]]);
								break;
							}
						}
					}

					if (clip_ptr != NULL)
					{
						// 列表中删除对应的clip点；
						itr = std::find(valid_points_ptr.begin(), valid_points_ptr.end(), clip_ptr);
						if (itr != valid_points_ptr.end())
						{
							if ((*itr)->ptr_pre != NULL) (*itr)->ptr_pre->ptr_nxt = NULL;
							if ((*itr)->ptr_nxt != NULL) (*itr)->ptr_nxt->ptr_pre = NULL;
							valid_points_ptr.erase(itr);
						}
						if (out_ring.size() == 1 && clip_ptr->ptr_nxt == NULL)
						{
							begin_ptr = clip_ptr;
						}
						else
						{
							// 找到则更新为curr点；
							curr_ptr = clip_ptr;
						}

					}
				}
				nxt_ptr = curr_ptr->ptr_nxt;

				// nxt为空或nxt == begin则跳出循环；
				if (nxt_ptr == NULL)
				{
					break;
				}
				if (nxt_ptr == begin_ptr)
				{
					itr = std::find(valid_points_ptr.begin(), valid_points_ptr.end(), nxt_ptr);
					if (itr != valid_points_ptr.end())
					{
						if ((*itr)->ptr_pre != NULL) (*itr)->ptr_pre->ptr_nxt = NULL;
						if ((*itr)->ptr_nxt != NULL) (*itr)->ptr_nxt->ptr_pre = NULL;
						valid_points_ptr.erase(itr);
					}
					break;
				}
				curr_ptr = nxt_ptr;
			}
			if (out_ring.size() > 2)
			{
				out_rings.push_back(out_ring);
			}
		}


		if (invertOut)
		{
			for (int i = 0; i < out_rings.size(); i++)
			{
				coordinates_array new_ring = out_rings[i];
				int pnt_num = new_ring.size();
				for (int j = 0; j < pnt_num; j++)
				{
					out_rings[i][j] = new_ring[pnt_num - 1 - j];
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 判断方向

		vector<int> out_ring_lhs, out_ring_rhs;
		for (int i = 0; i < out_rings.size(); i++)
		{
			if (IsPolygonCCW(out_rings[i], true)) // ccw: 顺时针，左手 -_-！
			{
				out_ring_lhs.push_back(i);
			}
			else
			{
				out_ring_rhs.push_back(i);
			}
		}

		double v_height = GetFirstPointHeight(polygon1);

		// 正向：添加新的polygon；
		//CComObject<CUSPolygonCollection>* outCollection = NULL;
		//CComObject<CUSPolygonCollection>::CreateInstance( &outCollection );

		for (int i = 0; i < out_ring_lhs.size(); i++)
		{
			coordinates_array out_ring = out_rings[out_ring_lhs[i]];
			out_polygon.push_back(out_ring);
			//// create ring
			//CComObject<CUSVector3s>* outRing = NULL;
			//CComObject<CUSVector3s>::CreateInstance( &outRing );
			//for ( int j = 0; j < out_ring.size(); j ++ )
			//{
			//	outRing->Add( out_ring[j].x, out_ring[j].y, v_height );
			//}

			//IUSVector3s* ptrOutRing = NULL;
			//outRing->QueryInterface( &ptrOutRing );

			//// create poly
			//CComObject<CUSPolygon>* outPoly = NULL;
			//CComObject<CUSPolygon>::CreateInstance( &outPoly );

			//outPoly->AddRing( ptrOutRing );

			//IUSPolygon* ptrOutPoly = NULL;
			//outPoly->QueryInterface( &ptrOutPoly );

			//// add to collection
			//outCollection->Add( ptrOutPoly );
		}
		// 反向：判断是谁的子。加入到对应的polygon作为子环。
		for (int i = 0; i < out_ring_rhs.size(); i++)
		{
			coordinates_array out_ring = out_rings[out_ring_rhs[i]];
			// create ring
			//CComObject<CUSVector3s>* outRing = NULL;
			//CComObject<CUSVector3s>::CreateInstance( &outRing );
			//for ( int j = 0; j < out_ring.size(); j ++ )
			//{
			//	outRing->Add( out_ring[j].x, out_ring[j].y, v_height );
			//}

			// get parent index;
			int parentIndex = -1;
			for (int j = 0; j < out_ring_lhs.size(); j++)
			{
				if (VectorInRing(out_ring[0], out_rings[out_ring_lhs[j]], epsl) == 1)
				{
					parentIndex = j;
					break;
				}
			}
			if (parentIndex == -1)
			{
				continue;
			}

			//// get parent poly
			//IUSPolygon* ptrParentPoly = NULL;
			//outCollection->get_Items( parentIndex, &ptrParentPoly );

			//// add to poly
			//ptrParentPoly->AddRing( outRing );
			out_polygon.push_back(out_ring);
		}
		//outCollection->QueryInterface( outPolyCollection );
	}

	void us_polygon_algorithm::polygon_to_inner(vector<coordinates_array> inPoly, vector<coordinates_array>& outPolyInner, bool z_is_height, bool clear_height)
	{
		long ring_count = inPoly.size();
		for (int i = 0; i < ring_count; i++)
		{

			coordinates_array inRing = inPoly[i];
			if (inRing.size() == 0)
			{
				continue;
			}
			long vec_count = inRing.size();

			coordinates_array ring;
			vector_3d vec;
			for (int j = 0; j < vec_count; j++)
			{
				vec = inRing[j];
				if (z_is_height == false)
				{
					double temp = vec.y;
					vec.y = vec.z;
					vec.z = temp;
				}
				if (clear_height)
				{
					vec.z = 0;
				}
				ring.push_back(vec);
			}
			outPolyInner.push_back(ring);
		}
	}
}
