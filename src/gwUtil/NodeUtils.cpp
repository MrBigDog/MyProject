#include <gwUtil/NodeUtils.h>
#include <gwUtil/Math.h>

#include <osgUtil/RenderBin>
#include <osgUtil/CullVisitor>
#include <osgDB/WriteFile>
#include <osg/Math>
#include <osg/Quat>
#include <osg/BlendFunc>


namespace gwUtil
{
	bool isGeodeEqual(const osg::Geode* geode1, const osg::Geode* geode2)
	{
		if (!geode1 || !geode2) return false;

		if (geode1->getNumDrawables() != geode2->getNumDrawables())
			return false;

		for (unsigned int i = 0; i < geode1->getNumDrawables(); i++)
		{
			const osg::Geometry* geom1 = geode1->getDrawable(i)->asGeometry();
			const osg::Geometry* geom2 = geode2->getDrawable(i)->asGeometry();

			const osg::Vec3Array* coords1 = dynamic_cast<const osg::Vec3Array*>(geom1->getVertexArray());
			const osg::Vec3Array* coords2 = dynamic_cast<const osg::Vec3Array*>(geom2->getVertexArray());
			if (coords1 && coords2)
			{
				int size1 = coords1->size();
				int size2 = coords2->size();
				if (coords1->size() != coords2->size())
				{
					return false;
				}
				for (unsigned int j = 0; j < coords1->size(); j++)
				{
					if (!isVec3Equal((*coords1)[j], (*coords2)[j]))
					{
						return false;
					}
				}
			}

			if (geom1->getNumTexCoordArrays() != geom1->getNumTexCoordArrays())
			{
				return false;
			}

			for (unsigned int units = 0; units < geom1->getNumTexCoordArrays(); units++)
			{
				const osg::Vec2Array* tcoords1 = dynamic_cast<const osg::Vec2Array*>(geom1->getTexCoordArray(units));
				const osg::Vec2Array* tcoords2 = dynamic_cast<const osg::Vec2Array*>(geom2->getTexCoordArray(units));
				if (tcoords1 && tcoords2)
				{
					if (tcoords1->size() != tcoords2->size())
					{
						return false;
					}
					for (unsigned int j = 0; j < tcoords1->size(); j++)
					{
						if (!isVec2Equal((*tcoords1)[j], (*tcoords2)[j]))
						{
							return false;
						}
					}
				}
			}
		}
		return true;
	}

	osg::Geode* getGeode(osg::Group* group)
	{
		if (!group) return NULL;

		for (unsigned int i = 0; i < group->getNumChildren(); i++)
		{
			osg::Node* node = group->getChild(i);
			osg::Geode* geode = dynamic_cast<osg::Geode*>(node);
			if (geode) return geode;

			osg::Group *childGroup = dynamic_cast<osg::Group*>(node);
			if (childGroup) geode = getGeode(childGroup);
			if (geode) return geode;
		}
		return NULL;
	}

	void addAlphaBlendFunc(osg::StateSet* stateset, GLenum source, GLenum destination)
	{
		osg::ref_ptr<osg::BlendFunc> osgBlendFunc = new osg::BlendFunc();
		osgBlendFunc->setFunction(source, destination);
		stateset->setAttribute(osgBlendFunc.get());
		stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
		stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	}

	bool hasMaterial(osg::StateSet* stateset)
	{
		return stateset && stateset->getAttribute(osg::StateAttribute::MATERIAL);
	}

	/**
	* This method will transform the given geometry's vertices
	* and normals by the given matrix.
	*/
	osg::Vec3 transformBillboardGeometry(osg::Geometry* geometry, osg::Matrix mat)
	{
		// Rotate and scale every vertex to absolute coordinates.
		// The geometry will still be positioned in its local coordinate system.
		osg::Matrix cMat(mat);
		cMat.setTrans(0, 0, 0);
		osg::Vec3Array* coords = static_cast<osg::Vec3Array*>(geometry->getVertexArray());
		if (coords)
		{
			int numCoords = coords->getNumElements();
			for (int i = 0; i < numCoords; i++)
			{
				osg::Vec3 coord = (*coords)[i];
				coord = cMat.preMult(coord);
				(*coords)[i].set(coord.x(), coord.y(), coord.z());
			}
		}

		// Transform every normal to absolute coordinates.
		osg::Matrix nMat(mat);
		nMat.setTrans(0, 0, 0);
		osg::Vec3Array* normals = (osg::Vec3Array*)geometry->getNormalArray();
		if (normals)
		{
			int numNormals = normals->getNumElements();
			for (int i = 0; i < numNormals; i++)
			{
				osg::Vec3 normal = (*normals)[i];
				normal = nMat.preMult(normal);
				normal.normalize();
				(*normals)[i].set(normal.x(), normal.y(), normal.z());
			}
		}
		return mat.getTrans();
	}

	std::string getNodePathString(const osg::Node* node)
	{
		if (!node) return "(nullptr)";

		std::string str;
		const osg::NodePathList& paths = node->getParentalNodePaths();
		for (size_t i = 0; i < paths.size(); ++i)
		{
			if (!str.empty()) str += ":";

			const osg::NodePath& path = paths.at(i);
			for (size_t j = 0; j < path.size(); ++j)
			{
				const osg::Node* node = path[j];
				str += "/'" + node->getName() + "'";
			}
		}
		return str;
	}

	bool glDumpWindow(const char * filename, int win_width, int win_height)
	{
		osg::ref_ptr<osg::Image> img(new osg::Image);
		img->readPixels(0, 0, win_width, win_height, GL_RGB, GL_UNSIGNED_BYTE);
		return osgDB::writeImageFile(*img, filename);
	}

	/************************************************************************/
	/* The implement of NodeAndDrawableVisitor                              */
	/************************************************************************/
	NodeAndDrawableVisitor::NodeAndDrawableVisitor(osg::NodeVisitor::TraversalMode tm)
		: osg::NodeVisitor(tm)
	{ }

	NodeAndDrawableVisitor::NodeAndDrawableVisitor(osg::NodeVisitor::VisitorType type, osg::NodeVisitor::TraversalMode tm)
		: osg::NodeVisitor(type, tm)
	{ }

	NodeAndDrawableVisitor::~NodeAndDrawableVisitor()
	{ }

	void NodeAndDrawableVisitor::apply(osg::Node& node)
	{
		traverse(node);
	}

	void NodeAndDrawableVisitor::apply(osg::Drawable& Drawable)
	{ }

	void NodeAndDrawableVisitor::traverse(osg::Node& node)
	{
		TraversalMode tm = getTraversalMode();
		if (tm == TRAVERSE_NONE)
		{
			return;
		}
		else if (tm == TRAVERSE_PARENTS)
		{
			osg::NodeVisitor::traverse(node);
			return;
		}
		osg::Geode* geode = dynamic_cast<osg::Geode*>(&node);
		if (geode)
		{
			unsigned numDrawables = geode->getNumDrawables();
			for (unsigned i = 0; i < numDrawables; ++i)
			{
				apply(*geode->getDrawable(i));
			}
		}
		else
		{
			osg::NodeVisitor::traverse(node);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void UpdateOnceNodeCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::ref_ptr<UpdateOnceNodeCallback> prevent_premature_deletion = this;
		doUpdate(node, nv);
		node->removeUpdateCallback(this);
		// The callback could be deleted now.
	}

	void UpdateOnceNodeCallback::doUpdate(osg::Node* node, osg::NodeVisitor* nv)
	{
		traverse(node, nv);
	}


	//////////////////////////////////////////////////////////////////////////
	StateAttributeVisitor::StateAttributeVisitor()
		: osg::NodeVisitor(osg::NodeVisitor::NODE_VISITOR, osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{ }

	void StateAttributeVisitor::apply(osg::StateSet::RefAttributePair&)
	{ }

	void StateAttributeVisitor::apply(osg::StateSet::AttributeList& attrList)
	{
		osg::StateSet::AttributeList::iterator i;
		i = attrList.begin();
		while (i != attrList.end())
		{
			apply(i->second);
			++i;
		}
	}

	void StateAttributeVisitor::apply(osg::StateSet* stateSet)
	{
		if (!stateSet) return;
		apply(stateSet->getAttributeList());
	}

	void StateAttributeVisitor::apply(osg::Node& node)
	{
		apply(node.getStateSet());
		traverse(node);
	}

	void StateAttributeVisitor::apply(osg::Geode& node)
	{
		unsigned nDrawables = node.getNumDrawables();
		for (unsigned i = 0; i < nDrawables; ++i)
		{
			apply(node.getDrawable(i)->getStateSet());
		}
		apply(node.getStateSet());
		traverse(node);
	}

	//////////////////////////////////////////////////////////////////////////
	struct VasiDrawable::LightData
	{
		LightData(const osg::Vec3f& p, const osg::Vec3f& n, const osg::Vec3f& up)
			: position(p)
			, normal(n)
			//, horizontal(normalize(cross(up, n)))
			//, normalCrossHorizontal(normalize(cross(n, horizontal)))
		{
			horizontal = up^n;
			horizontal.normalize();
			normalCrossHorizontal = n^horizontal;
			normalCrossHorizontal.normalize();
		}

		void draw(const osg::Vec4f& color) const
		{
			glBegin(GL_POINTS);
			glColor4fv(color.ptr()/*color.data()*/);
			glNormal3fv(normal.ptr()/*normal.data()*/);
			glVertex3fv(position.ptr()/*position.data()*/);
			glEnd();
		}

		osg::Vec3f position;
		osg::Vec3f normal;
		osg::Vec3f horizontal;
		osg::Vec3f normalCrossHorizontal;
	};

	VasiDrawable::VasiDrawable(const VasiDrawable& vd, const osg::CopyOp&)
		: _lights(vd._lights)
		, _red(vd._red)
		, _white(vd._white)
	{
		setUseDisplayList(false);
		setSupportsDisplayList(false);
	}

	VasiDrawable::VasiDrawable(const osg::Vec4f& red, const osg::Vec4f& white)
		: _red(red)
		, _white(white)
	{
		setUseDisplayList(false);
		setSupportsDisplayList(false);
	}

	void VasiDrawable::addLight(const osg::Vec3f& position, const osg::Vec3f& normal,
		const osg::Vec3f& up, float azimutDeg)
	{

		osg::Vec3f horizontal(up^ normal);
		horizontal.normalize();

		osg::Vec3f zeroGlideSlope = ((horizontal^ up));
		zeroGlideSlope.normalize();

		//SGQuatf rotation = SGQuatf::fromAngleAxisDeg(azimutDeg, horizontal);
		//osg::Vec3f azimutGlideSlope = rotation.transform(zeroGlideSlope);

		osg::Quat rotation(azimutDeg, horizontal);
		osg::Vec3f azimutGlideSlope = rotation*zeroGlideSlope;

		addLight(position, azimutGlideSlope, up);
	}

	void VasiDrawable::addLight(const osg::Vec3f& position, const osg::Vec3f& normal, const osg::Vec3f& up)
	{
		_lights.push_back(LightData(position, normal, up));
	}

	void VasiDrawable::drawImplementation(osg::RenderInfo& renderInfo) const
	{
		// Make sure we have the current state set
		//   renderInfo.getState()->apply();

		// Retrieve the eye point in local coords
		osg::Matrix m;
		m.invert(renderInfo.getState()->getModelViewMatrix());
		osg::Vec3f eyePoint(/*toSG*/(m.preMult(osg::Vec3(0, 0, 0))));

		// paint the points
		for (unsigned i = 0; i < _lights.size(); ++i)
		{
			draw(eyePoint, _lights[i]);
		}
	}

	osg::BoundingBox
#if OSG_VERSION_LESS_THAN(3,3,2)
		VasiDrawable::computeBound()
#else
		VasiDrawable::computeBoundingBox()
#endif
		const
	{
		osg::BoundingBox bb;
		for (unsigned i = 0; i < _lights.size(); ++i)
		{
			bb.expandBy(/*toOsg*/(_lights[i].position));
		}

		// blow up to avoid being victim to small feature culling ...
		bb.expandBy(bb._min - osg::Vec3(1, 1, 1));
		bb.expandBy(bb._max + osg::Vec3(1, 1, 1));
		return bb;
	}

	osg::Vec4f VasiDrawable::getColor(float angleDeg) const
	{
		float transDeg = 0.05f;
		if (angleDeg < -transDeg)
		{
			return _red;
		}
		else if (angleDeg < transDeg)
		{
			float fac = angleDeg*0.5f / transDeg + 0.5f;
			return _red + /*fac**/(_white - _red)*fac;
		}
		else
		{
			return _white;
		}
	}

	void VasiDrawable::draw(const osg::Vec3f& eyePoint, const LightData& light) const
	{
		// vector pointing from the light position to the eye
		osg::Vec3f lightToEye = eyePoint - light.position;

		// dont' draw, we are behind it
		if ((lightToEye * light.normal) < GWLimits<float>::min())
		{
			return;
		}


		// Now project the eye point vector into the plane defined by the
		// glideslope direction and the up direction
		osg::Vec3f projLightToEye = lightToEye - light.horizontal*(lightToEye*light.horizontal);

		// dont' draw, if we are to near, looks like we are already behind
		float sqrProjLightToEyeLength = (projLightToEye* projLightToEye);
		if (sqrProjLightToEyeLength < 1e-3*1e-3)
		{
			return;
		}

		// the scalar product of the glide slope up direction with the eye vector
		float dotProd = /*dot*/(projLightToEye/*,*/* light.normalCrossHorizontal);
		float sinAngle = dotProd / sqrt(sqrProjLightToEyeLength);
		if (sinAngle < -1) sinAngle = -1;
		if (1 < sinAngle) sinAngle = 1;

		float angleDeg = osg::RadiansToDegrees(asin(sinAngle));// SGMiscf::rad2deg(asin(sinAngle));
		light.draw(getColor(angleDeg));
	}


	//////////////////////////////////////////////////////////////////////////
	/*The implement of ShaderGeometry*/
	//////////////////////////////////////////////////////////////////////////
	void ShaderGeometry::addObject(const osg::Vec3& position, float scale, int texture_index)
	{
		if (!_posScaleArray.valid())
		{
			_posScaleArray = new osg::Vec4Array();
			_vertexAttribArray = new osg::FloatArray();
		}
		_posScaleArray->push_back(osg::Vec4(position, scale));
		_vertexAttribArray->push_back((float)texture_index / varieties);
		dirtyBound();
	}

	void ShaderGeometry::drawImplementation(osg::RenderInfo& renderInfo) const
	{
		osg::State& state = *renderInfo.getState();
#if OSG_VERSION_LESS_THAN(3,3,4)
		const Extensions* extensions = getExtensions(state.getContextID(), true);
#else
		const osg::GLExtensions* extensions = osg::GLExtensions::Get(state.getContextID(), true);
#endif
		osg::Vec4Array::const_iterator citer = _posScaleArray->begin();
		osg::Vec4Array::const_iterator cend = _posScaleArray->end();
		osg::FloatArray::const_iterator viter = _vertexAttribArray->begin();
		for (; citer != cend; ++citer, ++viter)
		{
			const osg::Vec4& color = *citer;
			const float attrib = *viter;
			glColor4fv(color.ptr());
			extensions->glVertexAttrib1f(1, attrib);
			_geometry->draw(renderInfo);
		}
	}

	osg::BoundingBox
#if OSG_VERSION_LESS_THAN(3,3,2)
		ShaderGeometry::computeBound()
#else
		ShaderGeometry::computeBoundingBox()
#endif
		const
	{
		const osg::BoundingBox& geom_box =
#if OSG_VERSION_LESS_THAN(3,3,2)
			_geometry->getBound();
#else
			_geometry->getBoundingBox();
#endif

		osg::BoundingBox bb;
		const osg::Vec4Array* posScales = _posScaleArray.get();
		if (!posScales)
			return bb;
		//    size_t numPosScales = posScales->size();
		for (osg::Vec4Array::const_iterator iter = _posScaleArray->begin(), e = _posScaleArray->end(); iter != e; ++iter)
		{
			const osg::Vec4& posScale = *iter;
			const float scale = posScale.w();
			const osg::Vec3 pos(posScale.x(), posScale.y(), posScale.z());
			for (unsigned j = 0; j < 7; ++j)
			{
				bb.expandBy(geom_box.corner(j) * scale + pos);
			}

		}
		return bb;
	}

	bool ShaderGeometry_readLocalData(osg::Object& obj, osgDB::Input& fr)
	{
		bool iteratorAdvanced = false;

		ShaderGeometry& geom = static_cast<ShaderGeometry&>(obj);

		if ((fr[0].matchWord("geometry")))
		{
			++fr;
			iteratorAdvanced = true;
			osg::Geometry* drawable = dynamic_cast<osg::Geometry*>(fr.readDrawable());
			if (drawable)
			{
				geom._geometry = drawable;
			}
		}
		//    int capacity = 0;
		if (fr.matchSequence("posScale %i {"))
		{
			int entry = fr[1].getNoNestedBrackets();
			int capacity;
			fr[1].getInt(capacity);
			osg::Vec4Array* posScale = new osg::Vec4Array;
			posScale->reserve(capacity);
			fr += 3;
			while (!fr.eof() && fr[0].getNoNestedBrackets() > entry)
			{
				osg::Vec4 v;
				if (fr[0].getFloat(v.x()) && fr[1].getFloat(v.y())
					&& fr[2].getFloat(v.z()) && fr[3].getFloat(v.w()))
				{
					fr += 4;
					posScale->push_back(v);
				}
				else ++fr;
			}
			++fr;
			geom._posScaleArray = posScale;
		}
		if (fr.matchSequence("variety %i {"))
		{
			int entry = fr[1].getNoNestedBrackets();
			int capacity;
			fr[1].getInt(capacity);
			osg::FloatArray* variety = new osg::FloatArray;
			variety->reserve(capacity);
			fr += 3;
			while (!fr.eof() && fr[0].getNoNestedBrackets() > entry)
			{
				float val;
				if (fr[0].getFloat(val))
				{
					++fr;
					variety->push_back(val);
				}
				else ++fr;
			}
			++fr;
			geom._vertexAttribArray = variety;
		}

		return iteratorAdvanced;
	}

	bool ShaderGeometry_writeLocalData(const osg::Object& obj, osgDB::Output& fw)
	{
		const ShaderGeometry& geom = static_cast<const ShaderGeometry&>(obj);

		fw.indent() << "geometry" << std::endl;
		fw.writeObject(*geom._geometry);
		if (geom._posScaleArray.valid())
		{
			fw.indent() << "posScale " << geom._posScaleArray->size() << " {\n";
			fw.moveIn();
			for (osg::Vec4Array::const_iterator iter = geom._posScaleArray->begin();
				iter != geom._posScaleArray->end();
				++iter)
			{
				fw.indent() << iter->x() << " " << iter->y() << " " << iter->z() << " "
					<< iter->w() << "\n";
			}
			fw.moveOut();
			fw.indent() << "}\n";
		}
		if (geom._vertexAttribArray.valid())
		{
			fw.indent() << "variety" << geom._vertexAttribArray->size() << " {\n";
			fw.moveIn();
			for (osg::FloatArray::const_iterator iter = geom._vertexAttribArray->begin();
				iter != geom._vertexAttribArray->end();
				++iter)
			{
				fw.indent() << *iter << "\n";
			}
			fw.moveOut();
			fw.indent() << "}\n";
		}
		return true;
	}

	osgDB::RegisterDotOsgWrapperProxy shaderGeometryProxy
	(
		new ShaderGeometry,
		"ShaderGeometry",
		"Object Drawable ShaderGeometry",
		&ShaderGeometry_readLocalData,
		&ShaderGeometry_writeLocalData
	);
}