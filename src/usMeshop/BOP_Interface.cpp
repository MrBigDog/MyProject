/**
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include "BOP_Interface.h"

namespace uniscope_globe
{
	/**
	* Computes the intersection boolean operation. Creates a new mesh resulting from 
	* an intersection of two meshes.
	* @param meshC Input & Output mesh
	* @param facesA Mesh A faces list
	* @param facesB Mesh B faces list
	* @param invertMeshA determines if object A is inverted
	* @param invertMeshB determines if object B is inverted
	* @return operation state: BOP_OK, BOP_NO_SOLID, BOP_ERROR
	*/
	BoolOpState BOP_intersectionBoolOp(BOP_Mesh*  meshC,
		BOP_Faces* facesA,
		BOP_Faces* facesB,
		bool       invertMeshA,
		bool       invertMeshB)
	{
#ifdef DEBUG
		BOP_Chrono chrono;
		float t = 0.0f;
		float c = 0.0f;
		chrono.start();  
		cout << "---" << endl;
#endif

		// Create BSPs trees for mesh A & B
		BOP_BSPTree bspA;
		bspA.addMesh(meshC, *facesA);

		BOP_BSPTree bspB;
		bspB.addMesh(meshC, *facesB);

#ifdef DEBUG
		c = chrono.stamp(); t += c;
		cout << "Create BSP     " << c << endl;	
#endif

		unsigned int numVertices = meshC->getNumVertexs();

		// mesh pre-filter
		BOP_simplifiedMeshFilter(meshC, facesA, &bspB, invertMeshB);
		if ((0.25*facesA->size()) > bspB.getDeep())
			BOP_meshFilter(meshC, facesA, &bspB);

		BOP_simplifiedMeshFilter(meshC, facesB, &bspA, invertMeshA);
		if ((0.25*facesB->size()) > bspA.getDeep())
			BOP_meshFilter(meshC, facesB, &bspA);

#ifdef DEBUG
		c = chrono.stamp(); t += c;
		cout << "mesh Filter    " << c << endl;	
#endif

		// Face 2 Face
		BOP_Face2Face(meshC,facesA,facesB);

#ifdef DEBUG
		c = chrono.stamp(); t += c;
		cout << "Face2Face      " << c << endl;
#endif

		// BSP classification
		BOP_meshClassify(meshC,facesA,&bspB);
		BOP_meshClassify(meshC,facesB,&bspA);

#ifdef DEBUG
		c = chrono.stamp(); t += c;
		cout << "Classification " << c << endl;
#endif

		// Process overlapped faces
		BOP_removeOverlappedFaces(meshC,facesA,facesB);

#ifdef DEBUG
		c = chrono.stamp(); t += c;
		cout << "Remove overlap " << c << endl;
#endif

		// Sew two meshes
		BOP_sew(meshC,facesA,facesB);

#ifdef DEBUG
		c = chrono.stamp(); t += c;
		cout << "Sew            " << c << endl;
#endif

		// Merge faces
#ifdef BOP_ORIG_MERGE
#ifndef BOP_NEW_MERGE
		BOP_Merge::getInstance().mergeFaces(meshC,numVertices);
#endif
#endif

		// #ifdef BOP_NEW_MERGE
		// #ifndef BOP_ORIG_MERGE
		// 	BOP_Merge2::getInstance().mergeFaces(meshC,numVertices);
		// #else
		// 	static int state = -1;
		// 	if (G.rt == 100) {
		// 		if( state != 1 ) {
		// 			cout << "Boolean code using old merge technique." << endl;
		// 			state = 1;
		// 		}
		// 		BOP_Merge::getInstance().mergeFaces(meshC,numVertices);
		// 	} else {
		// 		if( state != 0 ) {
		// 			cout << "Boolean code using new merge technique." << endl;
		// 			state = 0;
		// 		}
		// 		BOP_Merge2::getInstance().mergeFaces(meshC,numVertices);
		// 	}
		// #endif
		// #endif

#ifdef DEBUG
		c = chrono.stamp(); t += c;
		cout << "Merge faces    " << c << endl;
		cout << "Total          " << t << endl;
		// Test integrity
		meshC->testMesh();
#endif

		return BOP_OK;
	}

	/**
	* Preprocess to filter no collisioned faces.
	* @param meshC Input & Output mesh data
	* @param faces Faces list to test
	* @param bsp BSP tree used to filter
	*/
	void BOP_meshFilter(BOP_Mesh* meshC, BOP_Faces* faces, BOP_BSPTree* bsp)
	{
		BOP_IT_Faces it;
		BOP_TAG tag;

		it = faces->begin();
		while (it!=faces->end()) {
			BOP_Face *face = *it;
			MT_Point3 p1 = meshC->getVertex(face->getVertex(0))->getPoint();
			MT_Point3 p2 = meshC->getVertex(face->getVertex(1))->getPoint();
			MT_Point3 p3 = meshC->getVertex(face->getVertex(2))->getPoint();
			if ((tag = bsp->classifyFace(p1,p2,p3,face->getPlane()))==BOP_OUT||tag==OUTON) {
				face->setTAG(BROKEN);
				it = faces->erase(it);
			}
			else if (tag == BOP_IN) {
				it = faces->erase(it);
			}else{
				it++;
			}
		}
	}

	/**
	* Pre-process to filter no collisioned faces.
	* @param meshC Input & Output mesh data
	* @param faces Faces list to test
	* @param bsp BSP tree used to filter
	* @param inverted determines if the object is inverted
	*/
	void BOP_simplifiedMeshFilter(BOP_Mesh* meshC, BOP_Faces* faces, BOP_BSPTree* bsp, bool inverted)
	{
		BOP_IT_Faces it;

		it = faces->begin();
		while (it!=faces->end()) {
			BOP_Face *face = *it;
			MT_Point3 p1 = meshC->getVertex(face->getVertex(0))->getPoint();
			MT_Point3 p2 = meshC->getVertex(face->getVertex(1))->getPoint();
			MT_Point3 p3 = meshC->getVertex(face->getVertex(2))->getPoint();
			if (bsp->filterFace(p1,p2,p3,face)==BOP_OUT) {
				if (!inverted) face->setTAG(BROKEN);
				it = faces->erase(it);
			}
			else {
				it++;
			}
		}
	}

	/**
	* Process to classify the mesh faces using a bsp tree.
	* @param meshC Input & Output mesh data
	* @param faces Faces list to classify
	* @param bsp BSP tree used to face classify
	*/
	void BOP_meshClassify(BOP_Mesh* meshC, BOP_Faces* faces, BOP_BSPTree* bsp)
	{
		for(BOP_IT_Faces face=faces->begin();face!=faces->end();face++) {
			if ((*face)->getTAG()!=BROKEN) {
				MT_Point3 p1 = meshC->getVertex((*face)->getVertex(0))->getPoint();
				MT_Point3 p2 = meshC->getVertex((*face)->getVertex(1))->getPoint();
				MT_Point3 p3 = meshC->getVertex((*face)->getVertex(2))->getPoint();
				if (bsp->simplifiedClassifyFace(p1,p2,p3,(*face)->getPlane())!=BOP_IN) {
					(*face)->setTAG(BROKEN);
				}
			}
		}
	}

	/**
	* Returns a new mesh triangle.
	* @param meshC Input & Output mesh data
	* @param vertex1 first vertex of the new face
	* @param vertex2 second vertex of the new face
	* @param vertex3 third vertex of the new face
	* @param origFace identifier of the new face
	* @return new the new face
	*/
	BOP_Face3 *BOP_createFace3(BOP_Mesh* mesh, 
		BOP_Index       vertex1, 
		BOP_Index       vertex2, 
		BOP_Index       vertex3, 
		BOP_Index       origFace)
	{
		MT_Point3 p1 = mesh->getVertex(vertex1)->getPoint();
		MT_Point3 p2 = mesh->getVertex(vertex2)->getPoint();
		MT_Point3 p3 = mesh->getVertex(vertex3)->getPoint();
		MT_Plane3 plane(p1,p2,p3);

		return new BOP_Face3(vertex1, vertex2, vertex3, plane, origFace);
	}



	/**
	* Adds mesh information into destination mesh.
	* @param mesh input/output mesh, destination for the new mesh data
	* @param meshFacesId output mesh faces, contains an added faces list
	* @param inMesh 
	* @param inverted if TRUE adding inverted faces, non-inverted otherwise
	*/
	void BOP_addMesh(BOP_Mesh* mesh, BOP_Faces* meshFacesId, int& origin_face_base,
		d3d9_hardware_mesh_buffer* inMesh, matrix_4d* inMatrix, bool invert)
	{
		int indexOffset = mesh->getNumVertexs();

		vector<vector_3d> vers;
		vector<int> tris;

		BOP_combineVertex(inMesh, inMatrix, vers, tris);

		// 处理vertex
		int ver_num = vers.size();
		for (int i = 0; i < ver_num; i ++ )
		{
			MT_Point3 pos(vers[i].x, vers[i].y, vers[i].z);
			mesh->addVertex(pos);
		}

		// 处理index
		int tri_num = tris.size();
		BOP_Face3 *newface;
		for ( int i = 0; i < tri_num; i += 3 )
		{
			if (invert)
			{
				newface = BOP_createFace3(mesh, tris[i + 2] + indexOffset, tris[i + 1] + indexOffset, tris[i] + indexOffset, origin_face_base ++ );
			}
			else
			{
				newface = BOP_createFace3(mesh, tris[i] + indexOffset, tris[i + 1] + indexOffset, tris[i + 2] + indexOffset, origin_face_base ++ );
			}
			meshFacesId->push_back(newface);
			mesh->addFace(newface);
		}
	}

	void BOP_combineVertex(d3d9_hardware_mesh_buffer* inMesh, matrix_4d* inMatrix, vector<vector_3d>& outVers, vector<int>& outTris)
	{
		outVers.clear();
		outTris.clear();

		int ver_num = inMesh->get_num_of_vertices();
		int ver_stride = inMesh->m_vertex_stride;

		vector_3d vec;
		map<int, int> tri_map;
		for ( int i = 0; i < ver_num; i ++ )
		{
			float* ptr_vec = (float*)((char*)inMesh->m_vertex_buffer + i * ver_stride);
			vec.x = *(ptr_vec);
			vec.y = *(ptr_vec + 1);
			vec.z = *(ptr_vec + 2);

			if (inMatrix != NULL)
			{
				vec = vec * (*inMatrix);
			}

			int ver_num = outVers.size();
			for ( int j = 0; j < outVers.size(); j ++ )
			{
				if ( (outVers[j] - vec).length() < math_d::s_epsilon )
				{
					ver_num = j;
					break;
				}
			}

			if ( ver_num == outVers.size() )
			{
				outVers.push_back(vec);
			}
			tri_map.insert(make_pair(i, ver_num));
		}

		// 处理index
		int tri_num = inMesh->get_num_of_triangles();
		int ia, ib, ic;
		for ( int i = 0; i < tri_num; i ++ )
		{
			if( inMesh->m_32bit_index )
			{
				int* v_ptr = (int*)inMesh->m_index_buffer + i * 3;
				ia = *v_ptr;
				ib = *(v_ptr + 1);
				ic = *(v_ptr + 2);
			}
			else
			{
				short* v_ptr = (short*)inMesh->m_index_buffer + i * 3;
				ia = *v_ptr;
				ib = *(v_ptr + 1);
				ic = *(v_ptr + 2);
			}

			outTris.push_back(tri_map[ia]);
			outTris.push_back(tri_map[ib]);
			outTris.push_back(tri_map[ic]);
		}
	}

	/**
	* Exports a BOP_Mesh to a BSP_CSGMesh.
	* @param mesh Input mesh
	* @param invert if TRUE export with inverted faces, no inverted otherwise
	* @return the corresponding new BSP_CSGMesh
	*/
	void BOP_exportMesh(BOP_Mesh* mesh, bool invert, d3d9_hardware_mesh_buffer* outputMesh, int face_range)
	{
		//outputMesh = new d3d9_hardware_mesh();
		if ( outputMesh == NULL ) return;

		// vtx index dictionary, to translate indeces from input to output.
		map<int,unsigned int> dic;
		map<int,unsigned int>::iterator itDic;

		// Add a new face for each face in the input list
		BOP_Faces faces = mesh->getFaces();
		BOP_Vertexs vertexs = mesh->getVertexs();

		vector<int> tris;
		vector<vector_3f> vers;
		vector_3f vec;
		for (BOP_IT_Faces face = faces.begin(); face != faces.end(); face++) 
		{
			if ((*face)->getTAG()!=BROKEN && (*face)->getOriginalFace() < face_range)
			{
				if (invert) (*face)->invert();

				// 得到face的顶点
				vector<int> face_pnts;
				for (unsigned int pos=0;pos<(*face)->size();pos++) 
				{
					BOP_Index outVtxId;
					BOP_Index idVertex = (*face)->getVertex(pos);
					itDic = dic.find(idVertex);
					if (itDic == dic.end()) 
					{
						// The vertex isn't added yet
						outVtxId = vers.size();

						MT_Point3 mtPnt = mesh->getVertex(idVertex)->getPoint();
						vec.x = mtPnt.x();
						vec.y = mtPnt.y();
						vec.z = mtPnt.z();
						vers.push_back(vec);

						dic[idVertex] = outVtxId;
					}
					else 
					{
						// The vertex is added
						outVtxId = itDic->second;
					}
					face_pnts.push_back(outVtxId);
				}

				// 添加到三角面列表
				if (face_pnts.size() == 3)
				{
					tris.push_back(face_pnts[0]);
					tris.push_back(face_pnts[1]);
					tris.push_back(face_pnts[2]);
				}
				else if (face_pnts.size() == 4)
				{
					tris.push_back(face_pnts[0]);
					tris.push_back(face_pnts[1]);
					tris.push_back(face_pnts[2]);

					tris.push_back(face_pnts[0]);
					tris.push_back(face_pnts[2]);
					tris.push_back(face_pnts[3]);
				}
				else
				{
					continue;
				}
			}
		}

		// 组织输出的mesh
		outputMesh->set_fvf( position_color::fvf );
		outputMesh->set_stride( position_color::stride );

		outputMesh->m_num_of_vertices = vers.size();
		outputMesh->m_vertex_buffer_size = outputMesh->m_num_of_vertices * sizeof(position_color);
		outputMesh->m_vertex_buffer = new char[outputMesh->m_vertex_buffer_size];

		position_color* v_ver_buffer = (position_color*)outputMesh->m_vertex_buffer;


		for ( int i = 0; i < vers.size(); i ++ )
		{
			v_ver_buffer[i].x = vers[i].x;
			v_ver_buffer[i].y = vers[i].y;
			v_ver_buffer[i].z = vers[i].z;
			v_ver_buffer[i].color = 0xffff0000;
		}


		outputMesh->m_32bit_index = true;
		outputMesh->m_num_of_triangles = tris.size() / 3;
		outputMesh->m_index_buffer_size = outputMesh->m_num_of_triangles * 3 * sizeof(int);
		outputMesh->m_index_buffer = new char[outputMesh->m_index_buffer_size];
		int* v_index_buffer = (int*)outputMesh->m_index_buffer;

		for ( int i = 0; i < tris.size(); i ++ )
		{
			v_index_buffer[i] = tris[i];
		}
	}



	/**
	* Performs a generic booleam operation, the entry point for external modules.
	* @param opType Boolean operation type BOP_INTERSECTION, BOP_UNION, BOP_DIFFERENCE
	* @param outputMesh Output mesh, the final result (the object C)
	* @param inputMeshA 
	* @param inputMeshB 
	* @return operation state: BOP_OK, BOP_NO_SOLID, BOP_ERROR
	*/
	BoolOpState BOP_performBooleanOperation(BoolOpType                   opType,
		d3d9_hardware_mesh_buffer*   outputMesh,
		d3d9_hardware_mesh_buffer*   inputMeshA,
		matrix_4d*                   transformA,
		d3d9_hardware_mesh_buffer*   inputMeshB,
		matrix_4d*                   transformB,
		bool                         closeWithB)
	{
#ifdef DEBUG
		cout << "BEGIN BOP_performBooleanOperation" << endl;
#endif

		// Set invert flags depending on boolean operation type:
		// INTERSECTION: A^B = and(A,B)
		// UNION:        A|B = not(and(not(A),not(B)))
		// DIFFERENCE:   A-B = and(A,not(B))
		bool invertMeshA = (opType == BOP_UNION);
		bool invertMeshB = (opType != BOP_INTERSECTION);
		bool invertMeshC = (opType == BOP_UNION);

		// Faces list for both objects, used by boolean op.
		BOP_Faces meshAFacesId;
		BOP_Faces meshBFacesId;

		// Build C-mesh, the output mesh
		BOP_Mesh meshC;

		int origin_face_base = 0;
		// Add A-mesh into C-mesh
		BOP_addMesh(&meshC, &meshAFacesId, origin_face_base, inputMeshA, transformA, invertMeshA);

		int origin_face_begin_B = origin_face_base;
		// Add B-mesh into C-mesh
		BOP_addMesh(&meshC, &meshBFacesId, origin_face_base, inputMeshB, transformB, invertMeshB);

		// for now, allow operations on non-manifold (non-solid) meshes
#if 0
		if (!meshC.isClosedMesh())
			return BOP_NO_SOLID;
#endif

		// Perform the intersection boolean operation.
		BoolOpState result = BOP_intersectionBoolOp(&meshC, &meshAFacesId, &meshBFacesId, 
			invertMeshA, invertMeshB);

		// Invert the output mesh if is required
		if (closeWithB)
		{
			BOP_exportMesh(&meshC, invertMeshC, outputMesh, origin_face_base);
		}
		else
		{
			BOP_exportMesh(&meshC, invertMeshC, outputMesh, origin_face_begin_B);
		}

#ifdef DEBUG
		cout << "END BOP_performBooleanOperation" << endl;
#endif

		return result;
	}
}
