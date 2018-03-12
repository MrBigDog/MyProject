///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "Geo3DML_SDK/GM_XML/Geo3DProject.h"
#include "Geo3DML_SDK/GM_XML/GeoModel.h"
#include "Geo3DML_SDK/GM_XML/Geo3DMap.h"
#include "Geo3DML_SDK/GM_XML/Geo3DLayer.h"
#include "Geo3DML_SDK/GM_XML/Geo3DStyle.h"
#include "Geo3DML_SDK/GM_XML/FeatureStyle.h"
#include "Geo3DML_SDK/GM_XML/GeoSymbolizer.h"
#include "Geo3DML_SDK/GM_XML/FeatureClass.h"
#include "Geo3DML_SDK/GM_XML/FeatureRelation.h"
#include "Geo3DML_SDK/GM_XML/GeologicFeature.h"
#include "Geo3DML_SDK/GM_XML/MappedFeature.h"
#include "Geo3DML_SDK/vtkExtending/vtkObject.h"
#include "Geo3DML_SDK/vtkExtending/GMPolyData.h"
#include "Geo3DML_SDK/vtkExtending/GMUnstructuredGrid.h"
#include "Geo3DML_SDK/vtkExtending/vtkCellArray.h"
#include "Geo3DML_SDK/vtkExtending/vtkPointData.h"
#include "Geo3DML_SDK/vtkExtending/vtkDoubleArray.h"
#include "Geo3DML_SDK/vtkExtending/vtkFloatArray.h"
#include "Geo3DML_SDK/GMXMLFileIO\Geo3DProjectXMLReader.h"
#include "lib_gfx/us_d3d9_vertex_declear.h"
#include "us_geo3d_filter.h"

namespace uniscope_globe
{

	//-是否调整YZ的方向
	void FlipYZ(double* xx)
	{
		//std::swap(xx[0], xx[1]);
		//std::swap(xx[1], xx[2]);
		double dTemp = xx[2];
		xx[2] = xx[1];
		xx[1] = dTemp;
	}

	void processPolyData(GMPolyData* poly_data, d3d9_hardware_mesh* in_hardware_mesh, se::GeoSymbolizer* gs)
	{

		if (poly_data && poly_data->GetGeometryType() == 5)
		{
			vtkPoints* points = poly_data->GetPoints();
			int vCount = points->GetNumberOfPoints();
			in_hardware_mesh->m_num_of_vertices = vCount * 2;
			in_hardware_mesh->m_vertex_buffer_size = in_hardware_mesh->m_vertex_stride * in_hardware_mesh->m_num_of_vertices;
			in_hardware_mesh->m_vertex_buffer = new unsigned char[in_hardware_mesh->m_vertex_buffer_size];

			// vertex
			int vertex_offset = 0;
			axis_aligned_box<double> box;
			for (ulong i = 0; i < vCount; i++)
			{
				double xx[3];
				points->GetPoint(i, xx);
				FlipYZ(xx);
				vector_3f v(xx[0], xx[1], xx[2]);
				box.combine(vector_3d(xx));
				in_hardware_mesh->set_vector3(vertex_offset, i, &v);
				in_hardware_mesh->set_vector3(vertex_offset, i + vCount, &v);
			}

			in_hardware_mesh->m_mesh_aabb.set_extent(box.get_min(), box.get_max());
			in_hardware_mesh->set_bound_box(in_hardware_mesh->m_mesh_aabb);

			vtkCellArray* line = poly_data->GetPolys();
			long tCount = line->GetNumberOfCells();
			// 渲染时，去掉背面剔除，这样就不用传入正反两个三角面了 [1/4/2018 mahc]
			in_hardware_mesh->m_num_of_triangles = tCount ;

			in_hardware_mesh->m_32bit_index = true;
			in_hardware_mesh->m_index_buffer = new ulong[in_hardware_mesh->m_num_of_triangles * 3];
			in_hardware_mesh->m_index_buffer_size = sizeof(ulong) * in_hardware_mesh->m_num_of_triangles * 3;
			ulong* v_buffer = (ulong*)in_hardware_mesh->m_index_buffer;
			int v_off = 0;
			for (int i = 0; i < tCount * 4; i = i + 4)
			{
				vtkIdType counta;
				vtkIdType *pts;
				line->GetCell(i, counta, pts);

				v_buffer[v_off] = pts[0];
				v_buffer[v_off + 1] = pts[1];
				v_buffer[v_off + 2] = pts[2];
				v_off += 3;

				//v_buffer[v_off] = pts[0] + vCount;
				//v_buffer[v_off + 1] = pts[2] + vCount;
				//v_buffer[v_off + 2] = pts[1] + vCount;
				//v_off += 3;
			}


			// 计算法线 [1/5/2018 mahc]
			std::vector<vector_3f> vNormals;
			vNormals.resize(in_hardware_mesh->m_num_of_vertices, vector_3f());
			for (ulong i = 0; i < in_hardware_mesh->m_num_of_triangles; ++i)
			{
				int u1 = v_buffer[i * 3], u2 = v_buffer[i * 3 + 1], u3 = v_buffer[i * 3 + 2];
				vector_3f p1 = in_hardware_mesh->get_vector3(0, u1);
				vector_3f p2 = in_hardware_mesh->get_vector3(0, u2);
				vector_3f p3 = in_hardware_mesh->get_vector3(0, u3);
				vector_3f vNor = (p2 - p1).cross(p3 - p2);
				vNormals[u1] += vNor;
				vNormals[u2] += vNor;
				vNormals[u3] += vNor;
			}
			vertex_offset += 3 * sizeof(float);
			for (ulong i = 0; i < vNormals.size(); ++i)
			{
				vNormals[i].normalize();
				in_hardware_mesh->set_vector3(vertex_offset, i, &vNormals[i]);
			}

			// 设置颜色 [1/8/2018 mahc]
			vertex_offset += 3 * sizeof(float);
			ulong color = RGB(gs->DiffuseColor[0] * 255, gs->DiffuseColor[1] * 255, gs->DiffuseColor[2] * 255);
			for (ulong i = 0; i < in_hardware_mesh->m_num_of_vertices; ++i)
			{
				in_hardware_mesh->set_ulong(vertex_offset, i, color);
			}
			

		}
	}


	void constructOneFace(int n0, int n1, int n2, int n3, ulong* v_buffer,ulong& v_off)
	{
		v_buffer[v_off] = n0;
		v_buffer[v_off + 1] = n2;
		v_buffer[v_off + 2] = n1;

		v_off += 3;
		v_buffer[v_off] = n2;
		v_buffer[v_off + 1] = n0;
		v_buffer[v_off + 2] = n3;
		v_off += 3;

		//v_buffer[v_off] = n0;
		//v_buffer[v_off + 1] = n1;
		//v_buffer[v_off + 2] = n2;

		//v_off += 3;
		//v_buffer[v_off] = n2;
		//v_buffer[v_off + 1] = n3;
		//v_buffer[v_off + 2] = n0;
		//v_off += 3;
	}

	void processGMUnstructuredGrid(GMUnstructuredGrid *grid, d3d9_hardware_mesh* in_hardware_mesh)
	{
		vtkPoints* points = grid->GetPoints();

		int vCount = points->GetNumberOfPoints();
		in_hardware_mesh->m_num_of_vertices = vCount;
		in_hardware_mesh->m_vertex_buffer_size = in_hardware_mesh->m_vertex_stride * in_hardware_mesh->m_num_of_vertices;
		in_hardware_mesh->m_vertex_buffer = new unsigned char[in_hardware_mesh->m_vertex_buffer_size];
		int vertex_offset = 0;
		axis_aligned_box<double> box;
		for (int i = 0; i < vCount; i++)
		{
			double xx[3];
			points->GetPoint(i, xx);
			FlipYZ(xx);
			vector_3f v(xx[0], xx[1], xx[2]);
			box.combine(vector_3d(xx));
			in_hardware_mesh->set_vector3(vertex_offset, i, &v);
		}

		in_hardware_mesh->m_mesh_aabb.set_extent(box.get_min(), box.get_max());
		in_hardware_mesh->set_bound_box(in_hardware_mesh->m_mesh_aabb);

		vtkCellArray* line = grid->GetCells();
		long tCount = grid->GetNumberOfCells();
		ulong* v_buffer = NULL;
		if (grid->GetGeometryType() == 10)
		{
			in_hardware_mesh->m_num_of_triangles = tCount * 4*2;
			in_hardware_mesh->m_32bit_index = true;
			in_hardware_mesh->m_index_buffer = new ulong[in_hardware_mesh->m_num_of_triangles * 3];
			in_hardware_mesh->m_index_buffer_size = sizeof(ulong) * in_hardware_mesh->m_num_of_triangles * 3;
			v_buffer = (ulong*)in_hardware_mesh->m_index_buffer;
			ulong v_off = 0;
			for (int i = 0; i < tCount * 5; i = i + 5)
			{
				vtkIdType counta;
				vtkIdType *pts;
				line->GetCell(i, counta, pts);

				if (counta != 4)
					continue;

				v_buffer[v_off] = pts[0];
				v_buffer[v_off + 1] = pts[1];
				v_buffer[v_off + 2] = pts[2];
				v_off += 3;

				v_buffer[v_off] = pts[3];
				v_buffer[v_off + 1] = pts[2];
				v_buffer[v_off + 2] = pts[1];
				v_off += 3;

				v_buffer[v_off] = pts[3];
				v_buffer[v_off + 1] = pts[0];
				v_buffer[v_off + 2] = pts[1];
				v_off += 3;

				v_buffer[v_off] = pts[0];
				v_buffer[v_off + 1] = pts[2];
				v_buffer[v_off + 2] = pts[3];
				v_off += 3;

				///
				v_buffer[v_off] = pts[0];
				v_buffer[v_off + 1] = pts[2];
				v_buffer[v_off + 2] = pts[1];
				v_off += 3;

				v_buffer[v_off] = pts[3];
				v_buffer[v_off + 1] = pts[1];
				v_buffer[v_off + 2] = pts[2];
				v_off += 3;

				v_buffer[v_off] = pts[3];
				v_buffer[v_off + 1] = pts[1];
				v_buffer[v_off + 2] = pts[0];
				v_off += 3;

				v_buffer[v_off] = pts[0];
				v_buffer[v_off + 1] = pts[3];
				v_buffer[v_off + 2] = pts[2];
				v_off += 3;
			}

		}
		else if (grid->GetGeometryType() == 12)
		{
			in_hardware_mesh->m_num_of_triangles = tCount * 12;
			in_hardware_mesh->m_32bit_index = true;
			in_hardware_mesh->m_index_buffer = new ulong[in_hardware_mesh->m_num_of_triangles * 3];
			in_hardware_mesh->m_index_buffer_size = sizeof(ulong) * in_hardware_mesh->m_num_of_triangles * 3;
			v_buffer = (ulong*)in_hardware_mesh->m_index_buffer;

			ulong v_off = 0;
			for (int i = 0; i < tCount * 9; i = i + 9)
			{
				vtkIdType counta;
				vtkIdType *pts;
				line->GetCell(i, counta, pts);

				if (counta != 8)
					continue;

				// 一共6个面，每个面构造两个三角形 [1/7/2018 mahc]
				constructOneFace(pts[0], pts[1], pts[2], pts[3],v_buffer,v_off);//top
				constructOneFace(pts[4], pts[5], pts[6], pts[7], v_buffer, v_off);//bottom

				constructOneFace(pts[0], pts[1], pts[5], pts[4], v_buffer, v_off);//left
				constructOneFace(pts[3], pts[2], pts[6], pts[7], v_buffer, v_off);//right

				constructOneFace(pts[0], pts[3], pts[7], pts[4], v_buffer, v_off);//front
				constructOneFace(pts[1], pts[2], pts[6], pts[5], v_buffer, v_off);//back
			}
		}

		if (v_buffer)
		{
			// 计算法线 [1/5/2018 mahc]
			std::vector<vector_3f> vNormals;
			vNormals.resize(in_hardware_mesh->m_num_of_vertices, vector_3f());
			for (ulong i = 0; i < in_hardware_mesh->m_num_of_triangles; ++i)
			{
				int u1 = v_buffer[i * 3], u2 = v_buffer[i * 3 + 1], u3 = v_buffer[i * 3 + 2];
				vector_3f p1 = in_hardware_mesh->get_vector3(0, u1);
				vector_3f p2 = in_hardware_mesh->get_vector3(0, u2);
				vector_3f p3 = in_hardware_mesh->get_vector3(0, u3);
				vector_3f vNor = (p2 - p1).cross(p3 - p2);
				vNormals[u1] += vNor;
				vNormals[u2] += vNor;
				vNormals[u3] += vNor;
			}
			vertex_offset += 3 * sizeof(float);
			for (ulong i = 0; i < vNormals.size(); ++i)
			{
				vNormals[i].normalize();
				in_hardware_mesh->set_vector3(vertex_offset, i, &vNormals[i]);
			}

			// 设置颜色 [1/8/2018 mahc]
			vertex_offset += 3 * sizeof(float);
			int num1 = grid->GetPointData()->GetNumberOfArrays();
			if (num1 > 0)
			{
				vtkDataArray * vPointData = grid->GetPointData()->GetArray(0);
				std::vector<double> vPropData;
				double dMin = 1E10, dMax = -1E10;
				if (0 == strcmp(vPointData->GetClassName(), "vtkDoubleArray"))
				{
					vtkDoubleArray* dataA = (vtkDoubleArray*)vPointData;
					vPropData.resize(dataA->GetNumberOfTuples()*dataA->GetNumberOfComponents());
					for (ulong i = 0; i<dataA->GetNumberOfTuples()*dataA->GetNumberOfComponents(); ++i)
					{
						double fValue = dataA->GetValue(i);
						vPropData[i] = fValue;
						dMin = (fValue < dMin) ? fValue : dMin;
						dMax = (fValue > dMax) ? fValue : dMax;

					}
				}
				else if (0 == strcmp(vPointData->GetClassName(), "vtkFloatArray"))
				{
					vtkFloatArray* dataA = (vtkFloatArray*)vPointData;
					vPropData.resize(dataA->GetNumberOfTuples()*dataA->GetNumberOfComponents());
					for (ulong i = 0; i < dataA->GetNumberOfTuples()*dataA->GetNumberOfComponents(); ++i)
					{
						double fValue = dataA->GetValue(i);
						vPropData[i] = fValue;
						dMin = (fValue < dMin) ? fValue : dMin;
						dMax = (fValue > dMax) ? fValue : dMax;
					}
				}




				int nAllColorCount = 0x00FFFFFF - 0x0000FFFF;
				double dStep = nAllColorCount/(dMax - dMin) ;
				for (ulong i = 0; i < vPropData.size(); ++i)
				{
					double fValue = vPropData[i];	
					in_hardware_mesh->set_ulong(vertex_offset, i, 0x0000FFFF + ulong((fValue - dMin)*dStep));
				}
			}
		}

	}


	void geo3d_filter::construct_single_mesh(document_base* in_doc, d3d9_hardware_mesh* in_hardware_mesh, gmml::GeologicFeature* pFC, se::GeoSymbolizer* gs)
	{
		in_hardware_mesh->set_name(string_ext::to_wstring(pFC->getName()).c_str());
		
		in_hardware_mesh->set_fvf(position_normal_color::fvf);
		in_hardware_mesh->set_stride(position_normal_color::stride);

		
		// vertex [1/4/2018 mahc]
		gmml::MappedFeature* mapped_feature = pFC->GetMappedFeature(0);
		vtkObject* pShape = mapped_feature->GetShape();

		std::string strShapeName = pShape->GetClassName();



		if (strShapeName == "GMPolyData")
		{
			GMPolyData* poly_data = (GMPolyData*)pShape;
			processPolyData(poly_data, in_hardware_mesh,gs);
		}
		else if ("GMUnstructuredGrid" == strShapeName)
		{
			GMUnstructuredGrid *grid = (GMUnstructuredGrid *)pShape;
			processGMUnstructuredGrid(grid, in_hardware_mesh);
		}

		resource_manager<ustring>* v_texture_manager = in_doc->get_common_manager_group()->get_texture_manager(L"district_texture");
		in_hardware_mesh->set_texture_manager(v_texture_manager);
	}

	void geo3d_filter::readModelGeoData(gmml::GeoModel* pModel, std::map<std::string, std::map<std::string, gmml::GeologicFeature*>>& geoData)
	{
		int fc_count = pModel->GetGeoFeatureClassCount();
		for (int i = 0; i < fc_count; ++i)
		{
			gmml::GeologicFeatureClass* pFCls = pModel->GetGeoFeatureClass(i);
			int f_count = pFCls->GetGeologicFeatureCount();
			std::map<std::string, gmml::GeologicFeature*>& lstNames = geoData.insert(std::make_pair(pFCls->getID(), std::map<std::string, gmml::GeologicFeature*>())).first->second;
			for (int j = 0; j < f_count; ++j)
			{
				gmml::GeologicFeature* pFC = pFCls->GetGeologicFeature(j);
				//if (pFC->GetMappedFeatureCount() < 1)
				//{
				//	continue;
				//}
				lstNames.insert(std::make_pair(pFC->getName(), pFC));
			}
		}
	}
	vtkPoints* getPoints(vtkObject* pShape)
	{
		std::string strShapeName = pShape->GetClassName();
		vtkPoints* points = nullptr;
		if (strShapeName == "GMPolyData")
		{
			GMPolyData* poly_data = (GMPolyData*)pShape;
			points = poly_data->GetPoints();
		}
		else if ("GMUnstructuredGrid" == strShapeName)
		{
			GMUnstructuredGrid *grid = (GMUnstructuredGrid *)pShape;
			points = grid->GetPoints();
		}
		return points;
	}

	// 将整个模型移动到中心点 [1/25/2018 mahc]
	void geo3d_filter::MoveWholeModelToZero(std::map<std::string, std::map<std::string, gmml::GeologicFeature*>>& geoData)
	{
		// 将模型向中心点移动 [1/25/2018 mahc]
		axis_aligned_box_d box;
		for (auto it = geoData.begin(); it != geoData.end(); ++it)
		{
			for (auto itFC = it->second.begin(); itFC != it->second.end(); ++itFC)
			{
				gmml::GeologicFeature* pFC = itFC->second;
				if (pFC->GetMappedFeatureCount() < 1)
				{
					continue;
				}
				vtkPoints* points = getPoints(pFC->GetMappedFeature(0)->GetShape());
				int vCount = points->GetNumberOfPoints();
				for (int i = 0; i < vCount; i++)
				{
					double xx[3];
					points->GetPoint(i, xx);
					FlipYZ(xx);
					box.combine(vector_3d(xx));
				}
			}
		}
		vector_3d v_cen = box.center();
		box.make_invalid();
		for (auto it = geoData.begin(); it != geoData.end(); ++it)
		{
			for (auto itFC = it->second.begin(); itFC != it->second.end(); ++itFC)
			{
				gmml::GeologicFeature* pFC = itFC->second;
				if (pFC->GetMappedFeatureCount() < 1)
				{
					continue;
				}
				vtkPoints* points = getPoints(pFC->GetMappedFeature(0)->GetShape());
				int vCount = points->GetNumberOfPoints();
				for (int i = 0; i < vCount; i++)
				{
					double xx[3];
					points->GetPoint(i, xx);
					FlipYZ(xx);
					vector_3d v(xx);
					v -= v_cen;
					points->SetPoint(i, v.x, v.y, v.z);
				}
			}
		}

	}

	object_base* geo3d_filter::parse(raw_buffer* in_buffer)
	{
		//square_buffer* buf = (square_buffer*)in_buffer;
		//ustring strXmlPath = buf->get_path();
		//document_base* doc = buf->get_document();
		//return parse(strXmlPath, L"", doc);
		return NULL;
	}

	void geo3d_filter::parse(ustring strXmlPath, ustring strLayer, document_base* doc, std::list<object_base*>& lstObjs)
	{
		Geo3DProjectXMLReader xmlreader;

		gmml::Geo3DProject* project = NULL;

		if (!xmlreader.ReadFile(string_ext::from_wstring(strXmlPath).c_str(), ""))
		{
			return;
		}


		project = (gmml::Geo3DProject*)xmlreader.GetGeoProject();

		int model_count = project->GetGeoModelCount();
		if (model_count < 1) return;
		// 先解析第一个 [1/3/2018 mahc]
		gmml::GeoModel* pModel = project->GetGeoModel(0);

		// 获取map信息 [1/24/2018 mahc]
		// 需要需要map中layer下的信息，在Model中找对应的几何体 [1/24/2018 mahc]

		std::map<std::string, std::map<std::string, gmml::GeologicFeature*>> geoData;
		readModelGeoData(pModel, geoData);
		std::map<std::string, gmml::FeatureRelation*> relData;
		{
			// 读取关系 [1/26/2018 mahc]
			int nCount = pModel->GetGeoFeatureRelationCount();
			for (int i = 0; i < nCount; ++i)
			{
				gmml::FeatureRelation* pRel = pModel->GetGeoFeatureRelation(i);
				relData.insert(std::make_pair(pRel->getSourceRole(), pRel));
			}
		}
		MoveWholeModelToZero(geoData);

		for (int i = 0; i < project->GetGeo3DMapCount(); ++i)
		{
			gmml::Geo3DMap* pMap = project->GetGeo3DMap(i);
			int layercount = pMap->GetGeo3DLayerCount();
			for (int j = 0; j < layercount; ++j)
			{
				gmml::Geo3DLayer* layer = pMap->GetLayer(j);
				if (!layer) continue;
				if (!strLayer.empty() && layer->getName() != string_ext::from_wstring(strLayer))
				{
					continue;
				}

				gmml::GeologicFeatureClass* fc = layer->GetFeatureClass();
				//与Model中的数据对应
				auto itFeatureClass = geoData.find(fc->getID());
				if (itFeatureClass == geoData.end())
				{
					continue;
				}

				gmml::Geo3DStyle* style = layer->GetStyle();
				int ftsNum = style->GetFeatureTypeStyleCount();
				for (int ftsi = 0; ftsi < ftsNum; ++ftsi)
				{
					se::FeatureType3DStyle* fts = style->GetFeatureTypeStyle(ftsi);
					std::string name = fts->getFeatureTypeName();
					int rulNum = fts->getRuleCount();
					if (rulNum<1)
					{
						continue;
					}

					for (int ruli = 0; ruli < rulNum; ++ruli)
					{
						se::FeatureStyleRule* rule = fts->getRule(ruli);
						auto itFeature = itFeatureClass->second.find(rule->Literal);

						if (itFeature != itFeatureClass->second.end())
						{
							gmml::GeologicFeature* pFC = itFeature->second;
							se::GeoSymbolizer* gs = rule->GetSymbolizer();
							d3d9_hardware_mesh* v_sub_mesh = new d3d9_hardware_mesh();
							v_sub_mesh->m_path = strXmlPath;
							construct_single_mesh(doc, v_sub_mesh, pFC, gs);
							v_sub_mesh->set_name(string_ext::to_wstring(layer->getName() + rule->Literal).c_str());
							//v_new_mesh->m_root_frame = nullptr;
							//v_new_mesh->m_fvf = v_sub_mesh->m_fvf;

							//box.combine(v_sub_mesh->m_mesh_aabb);
							//v_new_mesh->m_mesh_frame_map.insert(make_pair(v_sub_mesh, nullptr));
							lstObjs.push_back(v_sub_mesh);
							continue;
						}

						auto itRel = relData.find(rule->Literal);
						if (itRel == relData.end())
						{
							continue;
						}

						d3d9_complex_hardware_mesh* v_new_mesh = new d3d9_complex_hardware_mesh();
						v_new_mesh->set_name(string_ext::to_wstring(rule->Literal).c_str());
						axis_aligned_box<double> box;
						for (int iRel = 0; iRel < itRel->second->GetTargetCount(); ++iRel)
						{
							std::string str = itRel->second->getTargetRole(iRel);
							auto itFeature = itFeatureClass->second.find(str);
							gmml::GeologicFeature* pFC = NULL;
							if (itFeature == itFeatureClass->second.end())
							{
								for (auto it = geoData.begin(); it != geoData.end(); ++it)
								{
									itFeature = it->second.find(str);
									if (itFeature != it->second.end())
									{
										pFC = itFeature->second;
										break;
									}
								}
							}
							else
							{
								pFC = itFeature->second;
							}


							if (NULL != pFC)
							{
								se::GeoSymbolizer* gs = rule->GetSymbolizer();
								d3d9_hardware_mesh* v_sub_mesh = new d3d9_hardware_mesh();
								v_sub_mesh->m_path = strXmlPath;
								construct_single_mesh(doc, v_sub_mesh, pFC, gs);
								v_sub_mesh->set_name(string_ext::to_wstring(layer->getName() + rule->Literal).c_str());
								v_new_mesh->m_root_frame = nullptr;
								v_new_mesh->m_fvf = v_sub_mesh->m_fvf;

								box.combine(v_sub_mesh->m_mesh_aabb);
								v_new_mesh->m_mesh_frame_map.insert(make_pair(v_sub_mesh, nullptr));
								continue;
							}
						}
						if (v_new_mesh->m_mesh_frame_map.empty())
						{
							AUTO_DELETE(v_new_mesh);
							continue;
						}
						v_new_mesh->set_bound_box(box);
						lstObjs.push_back(v_new_mesh);
					}

				}
			}

		}

		// 需要释放 [1/3/2018 mahc]
		AUTO_DELETE(project);
		//return dynamic_cast<hardware_mesh*>(v_new_mesh);
	}

}