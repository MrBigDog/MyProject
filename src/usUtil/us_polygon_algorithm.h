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
		equal,		// poly1 �� poly2 ��ȫ��ͬ
		contain,	// poly1 ���� poly2
		within,		// poly2 ���� poly1
		intersect,	// poly1 �� poly2 �ཻ
		touch,		// poly1 �� poly2 ���ڣ��߽��й�������
		disjoint	// poly1 �� poly2 ����
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
		// ɾ������㡣
		void CollapseVertex(coordinates_array& ring);

		// ��������
		// to_left_hand: ����Ϊ����(˳ʱ��)��false��֮��
		// z_is_height: ring��z����ֵ����߶ȷ���false��y����߶ȡ�
		void AdjustRingDirection(coordinates_array& ring, bool to_left_hand, bool z_is_height);

		// �жϵ��������λ��Ĺ�ϵ��
		// ����ֵ�� ���ڣ�1��  ���⣺-1��  �����ϣ�0��
		int VectorInRing(vector_3d vec, coordinates_array ring, double epsl);

		// �жϵ���������(�������ڻ�)�Ĺ�ϵ��
		// ����ֵ�� ���ڣ�1��  ���⣺-1��  �����ϣ�0��
		int VectorInPoly(vector_3d vec, vector<coordinates_array> poly, double epsl);

		bool IsPolygonCCW(coordinates_array in_poly, bool z_is_height);
		void polygon_to_inner(vector<coordinates_array> inPoly, vector<coordinates_array>& outPolyInner, bool z_is_height, bool clear_height);

		void RingClip(coordinates_array& ring1, coordinates_array& ring2, double epsl);

		bool LineLineClip(vector_3d v1, vector_3d v2, vector_3d p1, vector_3d p2, double epsl,
			coordinates_array& insert_v, coordinates_array& insert_p);

		void SortByDist(coordinates_array& vers, vector_3d ver_start);
		double GetFirstPointHeight(vector<coordinates_array> poly);


		// �ж�ring1��ring2�Ĺ�ϵ��ring1��ring2���ǵ�����
		void RingsRelationship(coordinates_array ring1, coordinates_array ring2, double epsl, us_polygon_relationship& out_relate);

		// �ж�ring��poly�Ĺ�ϵ��poly���ܰ����ڻ�
		// ���⻷��ȷ���Equal�����ڻ���ȷ���Touch��
		void RingPolyRelationship(coordinates_array ring, coordinates_part_array poly, us_coordinate_unit unit, us_polygon_relationship& out_relate);

	public:
		bool polys_bool_operation(vector<coordinates_array> polygon1, vector<coordinates_array> polygon2, us_coordinate_unit unit, us_bool_type type, vector<coordinates_array>& out_polygon);
		bool polys_relationship(vector<coordinates_array> polygon1, vector<coordinates_array> polygon2, us_coordinate_unit unit, us_polygon_relationship *outRelationship);
		void sub_bool_operation(vector<coordinates_array> polygon1, vector<coordinates_array> polygon2, us_coordinate_unit unit, us_bool_type type, vector<coordinates_array>& out_polygon);
	};
}

#endif // _US_POLYGON_ALGORITHM_H_
