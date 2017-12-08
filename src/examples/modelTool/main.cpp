#include <osgEarth/URI>
#include <osgEarth/Config>
#include <osgEarth/IOTypes>
#include <osgEarth/StringUtils>

#include <osg/Node>
#include <osg/Array>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgViewer/Viewer>

#include "wykobi/wykobi.hpp"
#include "wykobi/wykobi_algorithm.hpp"

int IncludeChinese(const char *str)
{
	int nRet = 0;
	char c;
	while (c = *str++)
	{
		//如果字符高位为1且下一字符高位也是1则有中文字符
		if ((c & 0x80) && (*str & 0x80))
		{
			nRet = 1;
			break;
		}
	}
	return nRet;
}


class TextureAndImageVisitor : public osg::NodeVisitor
{
public:
	TextureAndImageVisitor() : osg::NodeVisitor()
	{
		setNodeMaskOverride(~0L);
		setTraversalMode(TRAVERSE_ALL_CHILDREN);

		index = 0;
		headstr = "BigDog";
	}
	virtual ~TextureAndImageVisitor() { }

public:
	/** Visits a texture and, by default, all its components images */
	virtual void apply(osg::Texture& texture)
	{
		texture.setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		texture.setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
		texture.setWrap(osg::Texture::WRAP_R, osg::Texture::REPEAT);
		texture.setBorderWidth(2);
		texture.dirtyTextureParameters();
		//for (unsigned k = 0; k < texture.getNumImages(); ++k)
		//{
		//	osg::Image* image = texture.getImage(k);
		//	if (image)
		//	{
		//		apply(*image);
		//	}
		//}
	}

	/** Visits an image inside a texture */
	virtual void apply(osg::Image& image)
	{
		std::string filename = image.getFileName();
		std::string osgbFilename = osgDB::getNameLessExtension(filename);
		if (!_nameMap[osgbFilename].empty())
		{
			image.setFileName(_nameMap[osgbFilename] + ".osgb");
		}
		else
		{
			if (IncludeChinese(osgbFilename.c_str()))
			{
				std::stringstream ss;
				ss << index;
				std::string newname = headstr + ss.str();
				image.setFileName(newname + ".osgb");
				index++;
				_nameMap[newname] = newname;
			}
			else
			{
				std::string newname = headstr + osgbFilename;
				image.setFileName(newname + ".osgb");
				_nameMap[newname] = newname;
			}
		}

	}

public: // osg::NodeVisitor
	virtual void apply(osg::Node& node)
	{
		if (node.getStateSet())
		{
			apply(*node.getStateSet());
		}
		traverse(node);
	}


	virtual void apply(osg::Geode& geode)
	{
		if (geode.getStateSet())
		{
			apply(*geode.getStateSet());
		}
		for (unsigned i = 0; i < geode.getNumDrawables(); ++i)
		{
			apply(*geode.getDrawable(i));
		}
	}
	virtual void apply(osg::Drawable& drawable)
	{
		if (drawable.getStateSet())
		{
			apply(*drawable.getStateSet());
		}
	}

	virtual void apply(osg::StateSet& stateSet)
	{
		osg::StateSet::TextureAttributeList& a = stateSet.getTextureAttributeList();
		for (osg::StateSet::TextureAttributeList::iterator i = a.begin(); i != a.end(); ++i)
		{
			osg::StateSet::AttributeList& b = *i;
			for (osg::StateSet::AttributeList::iterator j = b.begin(); j != b.end(); ++j)
			{
				osg::StateAttribute* sa = j->second.first.get();
				if (sa)
				{
					osg::Texture* tex = dynamic_cast<osg::Texture*>(sa);
					if (tex)
					{
						apply(*tex);
					}
				}
			}
		}
	}

private:
	std::string headstr;
	int index;

	std::map<std::string, std::string> _nameMap;
};

//
//bool isGeodeEqual(const osg::Geode* geode1, const osg::Geode* geode2, bool checkTexCoord, bool checkNormal, bool checkColor, bool checkStateset)
//{
//	if (!geode1 || !geode2) return false;
//
//	if (geode1->getNumDrawables() != geode2->getNumDrawables())
//		return false;
//
//	for (unsigned int i = 0; i < geode1->getNumDrawables(); i++)
//	{
//		const osg::Geometry* geom1 = geode1->getDrawable(i)->asGeometry();
//		const osg::Geometry* geom2 = geode2->getDrawable(i)->asGeometry();
//
//		//vertex
//		const osg::Vec3Array* coords1 = dynamic_cast<const osg::Vec3Array*>(geom1->getVertexArray());
//		const osg::Vec3Array* coords2 = dynamic_cast<const osg::Vec3Array*>(geom2->getVertexArray());
//		if (!gwUtil::isVec3ArrayEqual(coords1, coords2))
//		{
//			return false;
//		}
//		if (checkTexCoord)
//		{
//			//texture_coord
//			if (geom1->getNumTexCoordArrays() != geom1->getNumTexCoordArrays()) return false;
//			for (unsigned int units = 0; units < geom1->getNumTexCoordArrays(); units++)
//			{
//				const osg::Vec2Array* tcoords1 = dynamic_cast<const osg::Vec2Array*>(geom1->getTexCoordArray(units));
//				const osg::Vec2Array* tcoords2 = dynamic_cast<const osg::Vec2Array*>(geom2->getTexCoordArray(units));
//				if (!gwUtil::isVec2ArrayEqual(tcoords1, tcoords2))
//				{
//					return false;
//				}
//			}
//		}
//		//color
//		if (checkColor)
//		{
//			const osg::Vec4Array* colorva1 = dynamic_cast<const osg::Vec4Array*>(geom1->getColorArray());
//			const osg::Vec4Array* colorva2 = dynamic_cast<const osg::Vec4Array*>(geom2->getColorArray());
//			if (!gwUtil::isVec4ArrayEqual(colorva1, colorva2))
//			{
//				return false;
//			}
//		}
//		if (checkNormal)
//		{
//			//normal
//			const osg::Vec3Array* normalv1 = dynamic_cast<const osg::Vec3Array*>(geom1->getColorArray());
//			const osg::Vec3Array* normalv2 = dynamic_cast<const osg::Vec3Array*>(geom2->getColorArray());
//			if (!gwUtil::isVec3ArrayEqual(normalv1, normalv2))
//			{
//				return false;
//			}
//		}
//		if (checkStateset)
//		{
//			//state_set.
//		}
//	}
//	return true;
//}
//
//
//class NodeOptimizer : public osg::NodeVisitor
//{
//public:
//	NodeOptimizer(const std::string& path)
//		: osg::NodeVisitor()
//		, _desPath(path)
//	{
//		setNodeMaskOverride(~0L);
//		setTraversalMode(TRAVERSE_ALL_CHILDREN);
//
//		_opts = new osgDB::Options("WriteImageHint=WriteOut Compressor=zlib");//
//	}
//
//	virtual ~NodeOptimizer() { }
//
//public:
//	virtual void apply(osg::Geode& geode)
//	{
//		std::string filename = geode.getName() + ".osgb";
//
//		std::string respath = osgDB::concatPaths(_desPath, filename);
//
//		osgDB::writeNodeFile(geode, respath, _opts);
//	}
//
//private:
//	std::string _desPath;
//	osg::ref_ptr<osgDB::Options> _opts;
//
//	//typedef	std::vector<osg::ref_ptr<osg::Geode> > GeodeVector;
//	//GeodeVector _geodeVector;
//};





class DirectoryVisitor
{
	//private:
	//	struct PointPara : public osg::Referenced
	//	{
	//		PointPara(const osg::Vec3d& pos, const osg::Matrixd& mat)
	//			:_pos(pos), _mat(mat)
	//		{}
	//		osg::Vec3d _pos;
	//		osg::Matrixd _mat;
	//	};
	//	typedef std::map<std::string, osg::ref_ptr<PointPara> > ModelPositionMap;

public:
	DirectoryVisitor::DirectoryVisitor(const std::string& path)
		: _path(path)
		//, _root(new osg::Group)
		//, modelNum(0)
		//, writeNum(0)
	{
		_opts = new osgDB::Options("WriteImageHint=WriteOut Compressor=zlib");//

		//initConfig("E:/LFSJN/LFSJ/building.xml", _buildingConfig);
		//initConfig("E:/LFSJN/LFSJ/ground.xml", _groundConfig);
		//initConfig("E:/LFSJN/LFSJ/road.xml", _roadConfig);
	}

	//void DirectoryVisitor::initConfig(const std::string& xmlfile, ModelPositionMap& pointmap)
	//{
	//	osgEarth::Config buildingConfig;
	//	readConfigFromXml(xmlfile, buildingConfig);
	//	const osgEarth::Config* buildingModelclass = buildingConfig.find("modelpointclass");
	//	if (buildingModelclass)
	//	{
	//		const osgEarth::ConfigSet& models = buildingModelclass->children("modelpoint");
	//		for (osgEarth::ConfigSet::const_iterator it = models.begin(); it != models.end(); ++it)
	//		{
	//			std::string name = it->value("modelname");

	//			double posX = osgEarth::as(it->value("locationx"), 0.0);
	//			//posX -= 39473449.4322129;
	//			double posY = osgEarth::as(it->value("locationy"), 0.0);
	//			//posY -= 4377659.46563995;
	//			double posZ = osgEarth::as(it->value("locationz"), 0.0);

	//			std::string matStr = it->value("matrix3");
	//			std::vector<std::string> StringVector;
	//			osgEarth::StringTokenizer st(matStr, StringVector, ",");

	//			osg::Matrixd matrix;
	//			matrix(0, 0) = osgEarth::as(StringVector[0], 0.0);
	//			matrix(0, 1) = osgEarth::as(StringVector[1], 0.0);
	//			matrix(0, 2) = osgEarth::as(StringVector[2], 0.0);
	//			matrix(0, 3) = 0.0;
	//			matrix(1, 0) = osgEarth::as(StringVector[3], 0.0);
	//			matrix(1, 1) = osgEarth::as(StringVector[4], 0.0);
	//			matrix(1, 2) = osgEarth::as(StringVector[5], 0.0);
	//			matrix(1, 3) = 0.0;
	//			matrix(2, 0) = osgEarth::as(StringVector[6], 0.0);
	//			matrix(2, 1) = osgEarth::as(StringVector[7], 0.0);
	//			matrix(2, 2) = osgEarth::as(StringVector[8], 0.0);
	//			matrix(2, 3) = 0.0;
	//			matrix(3, 0) = 0.0;
	//			matrix(3, 1) = 0.0;
	//			matrix(3, 2) = 0.0;
	//			matrix(3, 3) = 1.0;

	//			osg::ref_ptr<PointPara> pp = new PointPara(osg::Vec3d(posX, posY, posZ), matrix);
	//			pointmap[name] = pp;
	//		}
	//	}
	//}

public:
	void DirectoryVisitor::traverse(const std::string& path)
	{
		if (osgDB::fileType(path) == osgDB::DIRECTORY)
		{
			if (handleDir(path))
			{
				osgDB::DirectoryContents files = osgDB::getDirectoryContents(path);
				for (osgDB::DirectoryContents::const_iterator f = files.begin(); f != files.end(); ++f)
				{
					if (f->compare(".") == 0 || f->compare("..") == 0)
						continue;

					std::string filepath = osgDB::concatPaths(path, *f);
					traverse(filepath);
				}
			}
		}
		else if (osgDB::fileType(path) == osgDB::REGULAR_FILE)
		{
			handleFile(path);
		}
	}

	//void writeTo(const std::string& name)
	//{
	//	TextureAndImageVisitor tai;
	//	_root->accept(tai);

	//	osgDB::writeNodeFile(*_root, osgDB::concatPaths(_path, name));
	//}

	//osg::Node* getResult() { return _root; }

//private:
//	void readConfigFromXml(const std::string xmlfile, osgEarth::Config& arg)
//	{
//		osgEarth::ReadResult r = osgEarth::URI(xmlfile).readString();
//		if (r.failed())
//		{
//			return;
//		}
//		std::stringstream buf(r.getString());
//		arg.fromXML(buf);
//	}


	void handleFile(const std::string& filename)
	{
		std::string fileexten = osgDB::convertToLowerCase(osgDB::getFileExtension(filename));
		//if (fileexten == "dds")
		//{
		//	osg::ref_ptr<osg::Image> image = osgDB::readImageFile(filename);
		//	if (image)
		//	{
		//		std::string simplename = osgDB::getSimpleFileName(filename);
		//		std::string simplenamewioutext = osgDB::getNameLessExtension(simplename);
		//		std::string newname = osgDB::concatPaths(_path, simplenamewioutext + ".png");

		//		osgDB::writeImageFile(*image, newname);
		//	}
		//}
		if (fileexten == "osg")
		{
			osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(filename);
			if (node)
			{

				TextureAndImageVisitor tv;
				node->accept(tv);

				std::string simplename = osgDB::getSimpleFileName(filename);
				std::string simplenamewioutext = osgDB::getNameLessExtension(simplename);

				std::string newname = osgDB::concatPaths(_path, simplenamewioutext + ".osgb");
				osgDB::writeNodeFile(*node, newname, _opts/*, new osgDB::Options("extended3dsFilePaths")*/);
			}
		}
	}

	bool handleDir(const std::string& path)
	{
		return true;
	}

private:
	osg::ref_ptr<osgDB::Options> _opts;

	std::string _path;
};



#define SMALL_NUM   0.00000001 // anything that avoids division overflow
// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x() * (v).x() + (u).y() * (v).y() + (u).z() * (v).z())
#define norm(v)    sqrt(dot(v,v))  // norm = length of  vector
#define d(u,v)     norm(u-v)        // distance = norm of difference
#define abs(x)     ((x) >= 0 ? (x) : -(x))   //  absolute value


// dist3D_Segment_to_Segment(): get the 3D minimum distance between 2 segments
//    Input:  two 3D line segments S1 and S2
//    Return: the shortest distance between S1 and S2

struct Segment
{
	Segment(const osg::Vec3d& v0, const osg::Vec3d& v1)
		:P0(v0), P1(v1)
	{}

	osg::Vec3d P0, P1;
};


float dist3D_Segment_to_Segment(Segment S1, Segment S2)
{
	osg::Vec3d   u = S1.P1 - S1.P0;
	osg::Vec3d   v = S2.P1 - S2.P0;
	osg::Vec3d   w = S1.P0 - S2.P0;
	float    a = dot(u, u);         // always >= 0
	float    b = dot(u, v);
	float    c = dot(v, v);         // always >= 0
	float    d = dot(u, w);
	float    e = dot(v, w);
	float    D = a*c - b*b;        // always >= 0
	float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
	float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

								   // compute the line parameters of the two closest points
	if (D < SMALL_NUM) { // the lines are almost parallel
		sN = 0.0;         // force using point P0 on segment S1
		sD = 1.0;         // to prevent possible division by 0.0 later
		tN = e;
		tD = c;
	}
	else {                 // get the closest points on the infinite lines
		sN = (b*e - c*d);
		tN = (a*e - b*d);
		if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
		tN = 0.0;
		// recompute sc for this edge
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
		tN = tD;
		// recompute sc for this edge
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else {
			sN = (-d + b);
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	sc = (abs(sN) < SMALL_NUM ? 0.0 : sN / sD);
	tc = (abs(tN) < SMALL_NUM ? 0.0 : tN / tD);

	// get the difference of the two closest points
	osg::Vec3d dP = w + (u*sc) - (v*tc);  // =  S1(sc) - S2(tc)

	//osg::Vec3d r1 = w + (u*sc);
	osg::Vec3d r2 = (v*tc) - w;


	return norm(dP);   // return the closest distance
}


#define EPSILON FLT_EPSILON
// Clamp n to lie within the range [min, max]
float Clamp(float n, float min, float max)
{
	if (n < min) return min;
	if (n > max) return max;
	return n;
}
// Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
// S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared
// distance between between S1(s) and S2(t)
float ClosestPtSegmentSegment(const osg::Vec3d& p1, const osg::Vec3d& q1, const osg::Vec3d& p2, const osg::Vec3d& q2,
	float &s, float &t, osg::Vec3d &c1, osg::Vec3d &c2)
{
	osg::Vec3d d1 = q1 - p1; // Direction vector of segment S1
	osg::Vec3d d2 = q2 - p2; // Direction vector of segment S2
	osg::Vec3d r = p1 - p2;
	float a = d1* d1; // Squared length of segment S1, always nonnegative
	float e = d2* d2; // Squared length of segment S2, always nonnegative
	float f = d2* r;
	// Check if either or both segments degenerate into points
	if (a <= EPSILON && e <= EPSILON) {
		// Both segments degenerate into points
		s = t = 0.0f;
		c1 = p1;
		c2 = p2;
		return (c1 - c2)*(c1 - c2);
	}
	if (a <= EPSILON) {
		// First segment degenerates into a point
		s = 0.0f;
		t = f / e; // s = 0 => t = (b*s + f) / e = f / e
		t = Clamp(t, 0.0f, 1.0f);
	}
	else {
		float c = (d1* r);
		if (e <= EPSILON) {
			// Second segment degenerates into a point
			t = 0.0f;
			s = Clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
		}
		else {
			// The general nondegenerate case starts here
			float b = (d1* d2);
			float denom = a*e - b*b; // Always nonnegative
									 // If segments not parallel, compute closest point on L1 to L2 and
									 // clamp to segment S1. Else pick arbitrary s (here 0)
			if (denom != 0.0f)
			{
				s = Clamp((b*f - c*e) / denom, 0.0f, 1.0f);
			}
			else s = 0.0f;
			// Compute point on L2 closest to S1(s) using
			// t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
			t = (b*s + f) / e;
			// If t in [0,1] done. Else clamp t, recompute s for the new value
			// of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
			// and clamp s to [0, 1]
			if (t < 0.0f) {
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t > 1.0f) {
				t = 1.0f;
				s = Clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}
	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;


	return (c1 - c2)*(c1 - c2);
}


double getClockwiseAngle11(const osg::Vec3d& v1, const osg::Vec3d& v2, const osg::Vec3d& n)
{
	osg::Quat rot;
	rot.makeRotate(v1, v2);

	double angle;
	rot.getRotate(angle, osg::Vec3d(n));

	double f = (v1^v2)*n;

	if (f <= 0)
		return angle;
	else
		return 2.0*osg::PI - angle;

	return angle;
}



class NodeAndDrawableVisitor : public osg::NodeVisitor
{
public:
	NodeAndDrawableVisitor(osg::NodeVisitor::TraversalMode tm)
		: osg::NodeVisitor(tm)
	{ }
	NodeAndDrawableVisitor(osg::NodeVisitor::VisitorType type, osg::NodeVisitor::TraversalMode tm)
		: osg::NodeVisitor(type, tm)
	{ }

	virtual ~NodeAndDrawableVisitor() {}
	using osg::NodeVisitor::apply;
	virtual void apply(osg::Node& node)
	{
		traverse(node);
	}

	virtual void apply(osg::Drawable& drawable)
	{
		osg::Geometry* geom = dynamic_cast<osg::Geometry*>(&drawable);
		if (!geom)
		{
			return;
		}
		int ps = geom->getNumPrimitiveSets();
	}

	void traverse(osg::Node& node)
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
};



int main()
{
	TextureAndImageVisitor ndv/*(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)*/;

	osg::ref_ptr<osg::Node> node1 = osgDB::readNodeFile("E:/DATA/qxmx/qxmx/textureTest/Tile_1321013303332233301_L23_00033300.osgb");
	osg::ref_ptr<osg::Node> node2 = osgDB::readNodeFile("E:/DATA/qxmx/qxmx/textureTest/Tile_1321013303332233301_L23_00033200.osgb");
	osg::ref_ptr<osg::Node> node3 = osgDB::readNodeFile("E:/DATA/qxmx/qxmx/textureTest/Tile_1321013303332233301_L23_00033100.osgb");
	osg::ref_ptr<osg::Node> node4 = osgDB::readNodeFile("E:/DATA/qxmx/qxmx/textureTest/Tile_1321013303332233301_L23_00033000.osgb");
	osg::ref_ptr<osg::Node> node5 = osgDB::readNodeFile("E:/DATA/qxmx/qxmx/textureTest/Tile_1321013303332233301_L23_00032300.osgb");
	osg::ref_ptr<osg::Node> node6 = osgDB::readNodeFile("E:/DATA/qxmx/qxmx/textureTest/Tile_1321013303332233301_L23_00032200.osgb");
	osg::ref_ptr<osg::Node> node7 = osgDB::readNodeFile("E:/DATA/qxmx/qxmx/textureTest/Tile_1321013303332233301_L23_00032100.osgb");

	osg::ref_ptr<osg::Group> root = new osg::Group;
	//root->addChild(node1);
	//root->addChild(node2);
	//root->addChild(node3);
	//root->addChild(node4);
	root->addChild(node5);
	root->addChild(node6);
	//root->addChild(node7);

	osgDB::writeNodeFile(*root, "E:/DATA/qxmx/qxmx/textureTest/1/root.3ds");


	osgDB::writeNodeFile(*node1, "E:/DATA/qxmx/qxmx/textureTest/1/Tile_1321013303332233301_L23_00033300.3ds");
	osgDB::writeNodeFile(*node2, "E:/DATA/qxmx/qxmx/textureTest/2/Tile_1321013303332233301_L23_00033200.3ds");
	osgDB::writeNodeFile(*node3, "E:/DATA/qxmx/qxmx/textureTest/3/Tile_1321013303332233301_L23_00033100.3ds");
	osgDB::writeNodeFile(*node4, "E:/DATA/qxmx/qxmx/textureTest/4/Tile_1321013303332233301_L23_00033000.3ds");
	osgDB::writeNodeFile(*node5, "E:/DATA/qxmx/qxmx/textureTest/5/Tile_1321013303332233301_L23_00032300.3ds");
	osgDB::writeNodeFile(*node6, "E:/DATA/qxmx/qxmx/textureTest/6/Tile_1321013303332233301_L23_00032200.3ds");
	osgDB::writeNodeFile(*node7, "E:/DATA/qxmx/qxmx/textureTest/7/Tile_1321013303332233301_L23_00032100.3ds");

	//node1->accept(ndv);
	//node2->accept(ndv);

	//osg::Vec3d v1 = osg::Vec3d(0, 1, 0);
	//osg::Vec3d v2 = osg::Vec3d(1, 0, 0);

	//double angle = getClockwiseAngle11(v1, v2, osg::Vec3d(0, 0, 1));

	////osg::Quat rot;
	////rot.makeRotate_original(v1, v2);
	////double angle;
	////rot.getRotate(angle, osg::Vec3d(0, 0, -1));

	//double dangle = osg::RadiansToDegrees(angle);

	//double ddddd = angle;


	//wykobi::point3d<double> p1;
	//p1(0) = 281.07714843750000;
	//p1(1) = -1.0724452733993530;
	//p1(2) = -11.501375198364258;
	//wykobi::point3d<double> p2;
	//p2(0) = 281.07714843750000;
	//p2(1) = -1.0724452733993530;
	//p2(2) = -3.8611783981323242;
	//wykobi::segment3d s1;
	//s1[0] = p1;
	//s1[1] = p2;

	//wykobi::point3d<double> p3;
	//p3(0) = 281.07714843750000;
	//p3(1) = 18.303804397583008;
	//p3(2) = -11.546002388000488;
	//wykobi::point3d<double> p4;
	//p4(0) = 281.07714843750000;
	//p4(1) = -1.0724468231201172;
	//p4(2) = -11.501372337341309;
	//wykobi::segment3d s2;
	//s2[0] = p3;
	//s2[1] = p4;

	//osg::Vec3d d1 = osg::Vec3d(p2(0) - p1(0), p2(1) - p1(1), p2(2) - p1(2));
	//osg::Vec3d d2 = osg::Vec3d(p4(0) - p3(0), p4(1) - p3(1), p4(2) - p3(2));

	//std::vector<wykobi::segment3d> segment_list;
	//segment_list.push_back(s1);
	//segment_list.push_back(s2);

	//std::vector<wykobi::point3d<double> > intersection_list;

	//wykobi::algorithm::naive_group_intersections<wykobi::segment3d >
	//	(
	//		segment_list.begin(),
	//		segment_list.end(),
	//		std::back_inserter(intersection_list)
	//		);
	//osg::Vec3d p1(0, 0, 0);
	//osg::Vec3d p2(1, 0, 0);
	//osg::Vec3d q1(2, 2, 2);
	//osg::Vec3d q2(0.3, 0, 0);

	//float s, t;
	//osg::Vec3d c1, c2;
	//ClosestPtSegmentSegment(p1, p2, q1, q2, s, t, c1, c2);

	//Segment s1(osg::Vec3d(0, 0, 0), osg::Vec3d(1, 0, 0));
	//Segment s2(osg::Vec3d(2, 2, 2), osg::Vec3d(0.3, 0, 0));

	//osg::Vec3d p;
	//float dis = dist3D_Segment_to_Segment(s1, s2);


	//DirectoryVisitor dv("E:/LFSJN/LFSJ_OSGB_Z");
	//dv.traverse("E:/LFSJN/LFSJ");


	osgViewer::Viewer viewer;
	//viewer.setUpViewInWindow(20, 1200, 20, 1000);
	viewer.setSceneData(root);
	viewer.realize();

	viewer.run();
	return 0;
}
