#include <gwPipeline/PipelineMeshGenerateHelper.h>
#include <gwPipeline/PipelineGenerateParameter.h>
#include <gwPipeline/PipelineGenerateHelper.h>

#include <assert.h>

namespace gwPipeline
{
	osg::Geometry* PipelineMeshGenerateHelper::createEncloseCircularTube( 
		const osg::Vec3d& s_point, const osg::Vec3d& e_point
		, const osg::Vec3d& s_dir, const osg::Vec3d& e_dir
		, double v_radius, float texScl/*, PplMaterialEntry& v_mat_entry*/ )
	{
		// 计算辅助参数;
		osg::Vec3d tube_direction = (e_point-s_point); 
		float tube_length = tube_direction.length();
		tube_direction.normalize();

		osg::Matrixd v_mat = PipelineGenerateHelper::generateTbnMatrix( tube_direction, s_point );

		// 生成圆柱顶点坐标, 顺时针;
		osg::ref_ptr<osg::Vec3Array> v_vertex_template = new osg::Vec3Array;
		v_vertex_template->reserve(CYCLE_SEGMENT+1);
		createCircleClockD( v_vertex_template, osg::Vec3d(0.0, -1.0, 0.0), v_radius, CYCLE_SEGMENT );
		v_vertex_template->push_back((*v_vertex_template)[0]);

		int v_vertex_size = CYCLE_SEGMENT_PLUS1*2;
		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array; va->reserve(v_vertex_size);
		osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array; na->reserve(v_vertex_size);
		osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array; ta->reserve(v_vertex_size);

		float u_step = 1.0f*2.0f*osg::PI*v_radius / float(CYCLE_SEGMENT)*texScl;
		float v_step = tube_length*texScl;

		osg::Plane v_start_plane( s_dir, s_point );
		osg::Plane v_end_plane(e_dir, e_point);
		for ( int nj = 0; nj < CYCLE_SEGMENT_PLUS1; nj++ )
		{
			{
				// vertex 
				osg::Vec3d vec_tmp_1 = (*v_vertex_template)[nj];
				vec_tmp_1 = vec_tmp_1 * v_mat;
				osg::Vec3d plane_normal = v_start_plane.getNormal();
				double cos_angle = plane_normal*tube_direction;
				double dis = v_start_plane.distance(vec_tmp_1);
				double len = dis/cos_angle;
				osg::Vec3d v_vec = vec_tmp_1 - tube_direction*len;//v_vec是v_start_plane与ray的交点
				assert(!v_vec.isNaN());
				va->push_back( v_vec );

				//normal 
				osg::Vec3d v_normal = v_vec-s_point;
				v_normal.normalize();
				assert(!v_normal.isNaN());
				na->push_back(v_normal);

				//texture coord
				osg::Vec2 v_texture_coord = osg::Vec2(u_step*nj, 0.0);
				assert(!v_texture_coord.isNaN());

				ta->push_back(v_texture_coord);
			}
			{
				// vertex 
				osg::Vec3d vec_tmp_1 = (*v_vertex_template)[nj];
				vec_tmp_1 = vec_tmp_1 * v_mat;
				osg::Vec3d plane_normal = v_end_plane.getNormal();
				double cos_angle = plane_normal*tube_direction;
				double dis = v_end_plane.distance(vec_tmp_1);
				double len = dis/cos_angle;
				osg::Vec3d v_vec = vec_tmp_1 - tube_direction*len;//v_vec是v_end_plane与ray的交点
				assert(!v_vec.isNaN());
				va->push_back( v_vec );

				//normal 
				osg::Vec3d v_normal = v_vec-e_point;
				v_normal.normalize();
				assert(!v_normal.isNaN());
				na->push_back(v_normal);

				//texture coord
				osg::Vec2 v_texture_coord = osg::Vec2(u_step*nj, v_step);
				assert(!v_texture_coord.isNaN());
				ta->push_back(v_texture_coord);
			}
		}

		osg::ref_ptr<osg::Geometry> v_geom = new osg::Geometry;
		v_geom->setVertexArray(va);
		v_geom->setNormalArray(na);
		v_geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
		v_geom->setTexCoordArray(0, ta);
		v_geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, va->size()));

		return v_geom.release();
	}

	osg::Geometry* PipelineMeshGenerateHelper::create_enclose_square_tube(const osg::Vec3d& s_point, const osg::Vec3d& e_point, 
		const osg::Vec3d& s_dir, const osg::Vec3d& e_dir, double v_width, double v_height, float texScl)
	{
		// 计算辅助参数;
		osg::Vec3d tube_direction = e_point-s_point;
		float tube_length = tube_direction.length();
		tube_direction.normalize();

		osg::Matrixd v_mat = PipelineGenerateHelper::generateTbnMatrix( tube_direction, s_point );

		int vertexSize = BLOCK_SEGMENT_PLUS1*2;
		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
		va->reserve(vertexSize);
		osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
		na->reserve(vertexSize);
		osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array;
		ta->reserve(vertexSize);

		// 生成圆柱顶点坐标, 顺时针;
		osg::ref_ptr<osg::Vec3Array> v_vertex_template = new osg::Vec3Array;
		createRectangleD( v_vertex_template, osg::Vec3d( 0.0, -1.0, 0.0 ), v_width, v_height );
		//createRectangleD( v_vertex_template + v_off, osg::Vec3d( 0.0, -1.0, 0.0 ), v_width, v_height );

		//float uv_step = 1.0f / float(BLOCK_SEGMENT);
		float u_step = 1.0f*2.0f*(v_width+v_height) / float(BLOCK_SEGMENT)*texScl;
		float v_step = tube_length*texScl;
		// 计算精确坐标;
		osg::Plane v_start_plane(s_dir, s_point);
		osg::Plane v_end_plane( e_dir, e_point );
		for ( int nj = 0; nj < BLOCK_SEGMENT_PLUS1; nj++ )
		{
			{
				// vertex 
				osg::Vec3d vec_tmp_1 = (*v_vertex_template)[nj];
				vec_tmp_1 = vec_tmp_1 * v_mat;
				osg::Vec3d plane_normal = v_start_plane.getNormal();
				double cos_angle = plane_normal*tube_direction;
				double dis = v_start_plane.distance(vec_tmp_1);
				double len = dis/cos_angle;
				osg::Vec3d v_vec = vec_tmp_1 - tube_direction*len;//v_vec是v_start_plane与ray的交点
				assert(!v_vec.isNaN());
				va->push_back( v_vec );

				//normal 
				osg::Vec3d v_normal = vec_tmp_1-s_point;
				v_normal.normalize();
				assert(!v_normal.isNaN());
				na->push_back(v_normal);

				//texture coord
				osg::Vec2 v_texture_coord = osg::Vec2(u_step*nj, 0.0);
				assert(!v_texture_coord.isNaN());
				ta->push_back(v_texture_coord);
			}
			{
				// vertex 
				osg::Vec3d vec_tmp_1 = (*v_vertex_template)[nj];
				vec_tmp_1 = vec_tmp_1 * v_mat;
				osg::Vec3d plane_normal = v_end_plane.getNormal();
				double cos_angle = plane_normal*tube_direction;
				double dis = v_end_plane.distance(vec_tmp_1);
				double len = dis/cos_angle;
				osg::Vec3d v_vec = vec_tmp_1 - tube_direction*len;//v_vec是v_end_plane与ray的交点
				assert(!v_vec.isNaN());
				va->push_back( v_vec );

				//normal 
				osg::Vec3d v_normal = vec_tmp_1-s_point;
				v_normal.normalize();
				assert(!v_normal.isNaN());
				na->push_back(v_normal);

				//texture coord
				osg::Vec2 v_texture_coord = osg::Vec2(u_step*nj, v_step);
				assert(!v_texture_coord.isNaN());
				ta->push_back(v_texture_coord);
			}
		}

		osg::ref_ptr<osg::Geometry> v_geom = new osg::Geometry;
		v_geom->setVertexArray(va);
		v_geom->setNormalArray(na);
		v_geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
		v_geom->setTexCoordArray(0, ta);
		v_geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, va->size()));

		return v_geom.release();
	}


	void PipelineMeshGenerateHelper::createCircleClockD( osg::Vec3Array* outCirclePoints, const osg::Vec3d& inCenterPoint, double inRadiusRadians, int inSegment )
	{
		double v_x = 0.0, v_z = 0.0;
		double v_y = inCenterPoint.y();

		double v_angle = 0.0;
		double v_angle_step = osg::PI*2.0 / inSegment;
		for( int ni = 0; ni < inSegment; ni++ )
		{
			v_x = inCenterPoint.x() + std::sin( v_angle ) * inRadiusRadians;
			v_z = inCenterPoint.z() + std::cos( v_angle ) * inRadiusRadians;

			outCirclePoints->push_back(osg::Vec3d( v_x, v_y, v_z ));

			v_angle += v_angle_step;
		}
	}

	void PipelineMeshGenerateHelper::createRectangleD( osg::Vec3Array* out_rec_points, const osg::Vec3d& in_center_point, double width, double height )
	{
		out_rec_points->push_back(osg::Vec3d(in_center_point.x()-width*0.5, in_center_point.y(), in_center_point.z()+height*0.5));
		out_rec_points->push_back(osg::Vec3d(in_center_point.x()+width*0.5, in_center_point.y(), in_center_point.z()+height*0.5));
		out_rec_points->push_back(osg::Vec3d(in_center_point.x()+width*0.5, in_center_point.y(), in_center_point.z()-height*0.5));
		out_rec_points->push_back(osg::Vec3d(in_center_point.x()-width*0.5, in_center_point.y(), in_center_point.z()-height*0.5));
		out_rec_points->push_back((*out_rec_points)[0]);
	}
}
