#ifndef _US_POLYGON_ALGORITHM_H_
#define _US_POLYGON_ALGORITHM_H_

#include <usUtil\Export.h>
#include <usUtil\us_vector3.h>

using namespace std;

namespace uniscope_globe
{
	enum us_bool_type
	{
		intersection,
		_union,
		difference
	};
	typedef enum tag_us_polygon_relationship
	{
		equal,		// poly1 和 poly2 完全相同
		contain,	// poly1 包含 poly2
		within,		// poly2 包含 poly1
		intersect,	// poly1 和 poly2 相交
		touch,		// poly1 和 poly2 相邻，边界有公共部分
		disjoint	// poly1 和 poly2 相离
	}us_polygon_relationship;

	enum us_coordinate_unit
	{
		meter = 0,
		degree = meter + 1
	};

	class USUTIL_EXPORT us_polygon_algorithm
	{

	public:
		us_polygon_algorithm(void);
		~us_polygon_algorithm(void);
	public:
		// 删除冗余点。
		void CollapseVertex(coordinates_array& ring);

		// 调整方向。
		// to_left_hand: 调整为左手(顺时针)向。false反之。
		// z_is_height: ring的z坐标值代表高度方向。false则y代表高度。
		void AdjustRingDirection(coordinates_array& ring, bool to_left_hand, bool z_is_height);

		// 判断单点与多边形环的关系；
		// 返回值： 在内：1；  在外：-1；  在线上：0；
		int VectorInRing(vector_3d vec, coordinates_array ring, double epsl);

		// 判断单点与多边形(可能有内环)的关系；
		// 返回值： 在内：1；  在外：-1；  在线上：0；
		int VectorInPoly(vector_3d vec, vector<coordinates_array> poly, double epsl);

		bool IsPolygonCCW(coordinates_array in_poly, bool z_is_height);
		void polygon_to_inner(vector<coordinates_array> inPoly, vector<coordinates_array>& outPolyInner, bool z_is_height, bool clear_height);

		void RingClip(coordinates_array& ring1, coordinates_array& ring2, double epsl);

		bool LineLineClip(vector_3d v1, vector_3d v2, vector_3d p1, vector_3d p2, double epsl,
			coordinates_array& insert_v, coordinates_array& insert_p);

		void SortByDist(coordinates_array& vers, vector_3d ver_start);
		double GetFirstPointHeight(vector<coordinates_array> poly);


		// 判断ring1与ring2的关系；ring1与ring2均是单环。
		void RingsRelationship(coordinates_array ring1, coordinates_array ring2, double epsl, us_polygon_relationship& out_relate);

		// 判断ring与poly的关系，poly可能包含内环
		// 与外环相等返回Equal。与内环相等返回Touch。
		void RingPolyRelationship(coordinates_array ring, coordinates_part_array poly, us_coordinate_unit unit, us_polygon_relationship& out_relate);

	public:
		bool polys_bool_operation(vector<coordinates_array> polygon1, vector<coordinates_array> polygon2, us_coordinate_unit unit, us_bool_type type, vector<coordinates_array>& out_polygon);
		bool polys_relationship(vector<coordinates_array> polygon1, vector<coordinates_array> polygon2, us_coordinate_unit unit, us_polygon_relationship *outRelationship);
		void sub_bool_operation(vector<coordinates_array> polygon1, vector<coordinates_array> polygon2, us_coordinate_unit unit, us_bool_type type, vector<coordinates_array>& out_polygon);
	};
}

#endif // _US_POLYGON_ALGORITHM_H_
