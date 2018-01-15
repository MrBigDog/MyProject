#ifndef GWUTIL_NODEUTIL_H
#define GWUTIL_NODEUTIL_H 1

#include <gwUtil/Export.h>
#include <Compiler.h>
#include <osgDB/Input>
#include <osgDB/Output>
#include <osgDB/Registry>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Version>
#include <osg/ClipPlane>

namespace gwUtil
{
	inline GWUTIL_EXPORT bool isGeodeEqual(const osg::Geode* geode1, const osg::Geode* geode2);
	inline GWUTIL_EXPORT void addAlphaBlendFunc(osg::StateSet* stateset, GLenum source = GL_SRC_ALPHA, GLenum destination = GL_ONE_MINUS_SRC_ALPHA);
	inline GWUTIL_EXPORT bool hasMaterial(osg::StateSet* stateset);

	///得到一个group中的第一个geode 递归的方式;
	inline GWUTIL_EXPORT osg::Geode* getGeode(osg::Group* group);

	inline GWUTIL_EXPORT osg::Vec3 transformBillboardGeometry(osg::Geometry* geometry, osg::Matrix mat);

	inline GWUTIL_EXPORT std::string getNodePathString(const osg::Node* node);

	inline GWUTIL_EXPORT bool glDumpWindow(const char *filename, int win_width, int win_height);

	class BFSNodeVisitor : public osg::NodeVisitor
	{
	public:
		BFSNodeVisitor(osg::NodeVisitor::TraversalMode tm = osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
			: osg::NodeVisitor(tm)
		{}

		virtual void reset() { _pendingNodes.clear(); }
		virtual void apply(osg::Node& node) { traverseBFS(node); }

	protected:
		virtual ~BFSNodeVisitor() {}

		virtual void traverseBFS(osg::Node& node)
		{
			osg::Group* group = node.asGroup();
			if (!group) return;

			for (unsigned int i = 0; i < group->getNumChildren(); ++i)
			{
				_pendingNodes.push_back(group->getChild(i));
			}
			while (_pendingNodes.size() > 0)
			{
				osg::Node* node = _pendingNodes.front();
				_pendingNodes.pop_front();
				node->accept(*this);
			}
		}
		std::deque<osg::Node*> _pendingNodes;
	};

	/////跟osg::ClipNode类似，但是用的方式不同.
	//class ClipGroup : public osg::Group
	//{
	//public:
	//	ClipGroup();
	//	ClipGroup(const ClipGroup&, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
	//	//META_Node(simgear, ClipGroup);

	//	virtual osg::BoundingSphere computeBound() const;

	//	void addClipPlane(unsigned num, const osg::Vec2d& p0, const osg::Vec2d& p1);
	//	void setDrawArea(const osg::Vec2d& lowerLeft, const osg::Vec2d& upperRight);
	//	void setDrawArea(const osg::Vec2d& bottomLeft,
	//		const osg::Vec2d& topLeft,
	//		const osg::Vec2d& bottomRight,
	//		const osg::Vec2d& topRight);

	//	void addClipPlane(osg::ClipPlane* clipPlnae);

	//protected:
	//	class CullCallback;
	//	class ClipRenderBin;
	//	struct ClipBinRegistrar;
	//	std::vector<osg::ref_ptr<osg::ClipPlane> > mClipPlanes;
	//};

	class GWUTIL_EXPORT NodeAndDrawableVisitor : public osg::NodeVisitor
	{
	public:
		NodeAndDrawableVisitor(osg::NodeVisitor::TraversalMode tm);
		NodeAndDrawableVisitor(osg::NodeVisitor::VisitorType type, osg::NodeVisitor::TraversalMode tm);
		virtual ~NodeAndDrawableVisitor();
		using osg::NodeVisitor::apply;
		virtual void apply(osg::Node& node);
		/** Visit a Drawable node. Note that you cannot write an apply()
		method with an argument that is a subclass of Drawable and expect
		it to be called, because this visitor can't add the double dispatch
		machinery of NodeVisitor to the existing OSG Drawable subclasses.
		*/
		virtual void apply(osg::Drawable& drawable);
		// hides NodeVisitor::traverse
		void traverse(osg::Node& node);
	};

	class GWUTIL_EXPORT UpdateOnceNodeCallback : public osg::NodeCallback
	{
	public:
		UpdateOnceNodeCallback() {}
		UpdateOnceNodeCallback(const UpdateOnceNodeCallback& nc, const osg::CopyOp& copyop)
			: osg::NodeCallback(nc, copyop)
		{ }

		virtual void doUpdate(osg::Node* node, osg::NodeVisitor* nv);

		///Do not override; use doUpdate instead!
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
	};

	//////////////////////////////////////////////////////////////////////////
	class GWUTIL_EXPORT StateAttributeVisitor : public osg::NodeVisitor
	{
	public:
		StateAttributeVisitor();
		virtual void apply(osg::StateSet::RefAttributePair&);
		virtual void apply(osg::StateSet::AttributeList& attrList);
		virtual void apply(osg::StateSet* stateSet);
		virtual void apply(osg::Node& node);
		virtual void apply(osg::Geode& node);
	};

	//////////////////////////////////////////////////////////////////////////
	/*绘制类似于呼吸灯或者机场跑道灯的效果;*/
	class GWUTIL_EXPORT VasiDrawable : public osg::Drawable
	{
		struct LightData;
	public:
		VasiDrawable(const VasiDrawable&, const osg::CopyOp&);
		VasiDrawable(const osg::Vec4f& red = osg::Vec4f(1, 0, 0, 1),
			const osg::Vec4f& white = osg::Vec4f(1, 1, 1, 1));

		/// Add a red/white switching light pointing into the direction that
		/// is computed to point in about the given normal with the given
		/// azimut angle upwards. The up direction is the world up direction
		/// that defines the horizontal plane.
		void addLight(const osg::Vec3f& position, const osg::Vec3f& normal,
			const osg::Vec3f& up, float azimutDeg);

		/// add a red/white switching light pointing towards normal
		/// at the given position with the given up vector. The up direction
		/// is the world up direction that defines the horizontal plane.
		void addLight(const osg::Vec3f& position, const osg::Vec3f& normal,
			const osg::Vec3f& up);

		virtual void drawImplementation(osg::RenderInfo& renderInfo) const;
		virtual osg::BoundingBox
#if OSG_VERSION_LESS_THAN(3,3,2)
			computeBound()
#else
			computeBoundingBox()
#endif
			const;

	private:
		osg::Vec4f getColor(float angleDeg) const;
		void draw(const osg::Vec3f& eyePoint, const LightData& light) const;

		std::vector<LightData> _lights;
		osg::Vec4f _red;
		osg::Vec4f _white;
	};


	//////////////////////////////////////////////////////////////////////////
	class GWUTIL_EXPORT ShaderGeometry : public osg::Drawable
	{
	public:
		ShaderGeometry() : varieties(1)
		{
			setSupportsDisplayList(false);
		}

		ShaderGeometry(int v) : varieties(v)
		{
			setSupportsDisplayList(false);
		}

		/** Copy constructor using CopyOp to manage deep vs shallow copy.*/
		ShaderGeometry(const ShaderGeometry& ShaderGeometry, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
			: osg::Drawable(ShaderGeometry, copyop)
		{}

		virtual void drawImplementation(osg::RenderInfo& renderInfo) const;

		virtual osg::BoundingBox
#if OSG_VERSION_LESS_THAN(3,3,2)
			computeBound()
#else
			computeBoundingBox()
#endif
			const;

		void setGeometry(osg::Geometry* geometry)
		{
			_geometry = geometry;
		}

		void addObject(const osg::Vec3& position, float scale, int texture_index);

		osg::ref_ptr<osg::Geometry> _geometry;

		int varieties;
		osg::ref_ptr<osg::Vec4Array> _posScaleArray;
		osg::ref_ptr<osg::FloatArray> _vertexAttribArray;

	protected:

		virtual ~ShaderGeometry() {}
	};
}
#endif // GWUTIL_NODEUTIL_H
