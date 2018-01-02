//
//  Chunk.h
//  Voxel_Render
//
//  Created by Mitchell, George  AMRDEC/Simtech on 8/28/13.
//  Copyright (c) 2013 Mitchell, George  AMRDEC/Simtech. All rights reserved.
//

#ifndef __Voxel_Render__Chunk__
#define __Voxel_Render__Chunk__

#include "Voxel.h"
#include <osg/Vec3ui>
#include <Vector>
#include <iostream>

class Chunk : public osg::Group::Group 
{
public:
	Chunk(std::vector<osg::ref_ptr<osg::Node>>& voxels);
	~Chunk();
	
private:
	std::vector<osg::ref_ptr<osg::Node>> _voxels;
	osg::Vec3ui _dim;
};

#endif /* defined(__Voxel_Render__Chunk__) */
