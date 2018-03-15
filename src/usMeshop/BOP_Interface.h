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

#ifndef BOP_INTERFACE_H
#define BOP_INTERFACE_H

 //#include "stdafx.h"


#include "BOP_BSPTree.h"
#include "BOP_Mesh.h"
#include "BOP_Face2Face.h"
#include "BOP_Merge.h"
#include "BOP_Merge2.h"
#include "BOP_Chrono.h"

#include <usUtil/us_matrix4.h>


namespace uniscope_globe
{

	typedef enum EnumBoolOpState { BOP_OK, BOP_NO_SOLID, BOP_ERROR } BoolOpState;
	typedef enum EnumBoolOpType { BOP_INTERSECTION, BOP_UNION, BOP_DIFFERENCE } BoolOpType;

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
		bool       invertMeshB);


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
		BOP_Index       origFace);

	/**
	* Preprocess to filter no collisioned faces.
	* @param meshC Input & Output mesh data
	* @param faces Faces list to test
	* @param bsp BSP tree used to filter
	*/
	void BOP_meshFilter(BOP_Mesh* meshC, BOP_Faces* faces, BOP_BSPTree* bsp);

	/**
	* Pre-process to filter no collisioned faces.
	* @param meshC Input & Output mesh data
	* @param faces Faces list to test
	* @param bsp BSP tree used to filter
	* @param inverted determines if the object is inverted
	*/
	void BOP_simplifiedMeshFilter(BOP_Mesh* meshC, BOP_Faces* faces, BOP_BSPTree* bsp, bool inverted);

	/**
	* Process to classify the mesh faces using a bsp tree.
	* @param meshC Input & Output mesh data
	* @param faces Faces list to classify
	* @param bsp BSP tree used to face classify
	*/
	void BOP_meshClassify(BOP_Mesh* meshC, BOP_Faces* faces, BOP_BSPTree* bsp);


	//// hardware mesh about /////////////////////////////////////////////////



	/**
	* Adds mesh information into destination mesh.
	* @param mesh input/output mesh, destination for the new mesh data
	* @param meshFacesId output mesh faces, contains an added faces list
	* @param inMesh
	* @param inverted if TRUE adding inverted faces, non-inverted otherwise
	*/
	void BOP_addMesh(BOP_Mesh* mesh, BOP_Faces* meshFacesId, int& origin_face_base,
		d3d9_hardware_mesh_buffer* inMesh, matrix_4d* inMatrix, bool invert);

	void BOP_combineVertex(d3d9_hardware_mesh_buffer* inMesh, matrix_4d* inMatrix, vector<vector_3d>& outVers, vector<int>& outTris);


	/**
	* Exports a BOP_Mesh to a BSP_CSGMesh.
	* @param mesh Input mesh
	* @param invert if TRUE export with inverted faces, no inverted otherwise
	* @return the corresponding new BSP_CSGMesh
	*/
	void BOP_exportMesh(BOP_Mesh* inputMesh, bool invert, d3d9_hardware_mesh_buffer* outputMesh, int face_range);



	//// interface ///////////////////////////////////////////////////////////


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
		bool                         closeWithB = true);
}

#endif
