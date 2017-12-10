#ifndef GEOLOGICALOPERATORHELP_H
#define GEOLOGICALOPERATORHELP_H 1

//#include <gdal/gdal.h>
//#include <gdal/gdal_priv.h>
//#include <gdal/cpl_string.h>
//#include <gdal/ogrsf_frmts.h>

#include <osg/Array>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>

#define SLICE_CLOSED 1

#define PI2 6.28318530717958647692
#define GEO_EPSILOND 1.192092896e-05

template<typename VEC_TYPE = osg::Vec3d>
static VEC_TYPE normalize(const VEC_TYPE& v)
{
	VEC_TYPE n = v; n.normalize();
	return n;
}

static bool isVec3dEqual(const osg::Vec3d& p1, const osg::Vec3d& p2, double epsilon)
{
	if (!osg::equivalent(p1.x(), p2.x(), epsilon)) return false;
	if (!osg::equivalent(p1.y(), p2.y(), epsilon)) return false;
	if (!osg::equivalent(p1.z(), p2.z(), epsilon)) return false;
	return true;
}

static bool isVec3dParallel(const osg::Vec3d& p1, const osg::Vec3d& p2, double epsilon)
{
	osg::Vec3d np1 = normalize(p1);
	osg::Vec3d np2 = normalize(p2);

	if (!osg::equivalent(np1.x(), np2.x(), epsilon) &&
		!osg::equivalent(np1.x(), -np2.x(), epsilon)) return false;

	if (!osg::equivalent(np1.y(), np2.y(), epsilon) &&
		!osg::equivalent(np1.y(), -np2.y(), epsilon)) return false;

	if (!osg::equivalent(np1.z(), np2.z(), epsilon) &&
		!osg::equivalent(np1.z(), -np2.z(), epsilon)) return false;
	return true;
}

static bool isVec4Equal(const osg::Vec4& p1, const osg::Vec4& p2, float epsilon)
{
	if (!osg::equivalent(p1[0], p2[0], epsilon)) return false;
	if (!osg::equivalent(p1[1], p2[1], epsilon)) return false;
	if (!osg::equivalent(p1[2], p2[2], epsilon)) return false;
	if (!osg::equivalent(p1[3], p2[3], epsilon)) return false;
	return true;
}

static double getAngle(const osg::Vec3d& v1, const osg::Vec3d& v2, const osg::Vec3d& axis)
{
	osg::Quat rot;
	rot.makeRotate(v1, v2);

	double angele = 0.0;
	rot.getRotate(angele, osg::Vec3d(axis));

	return angele;
}

//得到顺时针方向的夹角;
static double getClockwiseAngle(const osg::Vec3d& v1, const osg::Vec3d& v2, const osg::Vec3d& n)
{
	if (isVec3dEqual(v1, osg::Vec3d(), GEO_EPSILOND) ||
		isVec3dEqual(v2, osg::Vec3d(), GEO_EPSILOND))
	{
		return 0.0;
	}

	osg::Vec3d vv1 = normalize(v1);
	osg::Vec3d vv2 = normalize(v2);
	if (isVec3dEqual(vv1, vv2, GEO_EPSILOND))
	{
		return 0.0;
	}
	if (isVec3dEqual(vv1, -vv2, GEO_EPSILOND))
	{
		return osg::PI;
	}

	osg::Quat rot;
	rot.makeRotate_original(v1, v2);

	double angle;
	osg::Vec3d rotAxis;
	rot.getRotate(angle, rotAxis);

	angle = rotAxis*n > 0 ? PI2 - angle : angle;

	return angle;
}

template<typename ARRAY_TYPE = osg::Vec3dArray>
static void removeRepeat(ARRAY_TYPE* va, double epsilon = GEO_EPSILOND)
{
	ARRAY_TYPE::iterator it, it1;
	for (it = ++va->begin(); it != va->end();)
	{
		bool isRepeat = false;
		for (it1 = va->begin(); it1 != it; ++it1)
		{
			if (isVec3dEqual(*it, *it1, epsilon))
			{
				isRepeat = true;
				break;
			}
		}
		if (isRepeat) it = va->erase(it);
		else it++;
	}
}


template<typename ARRAY_TYPE = osg::Vec3dArray>
static bool isArrayInClockwiseDirection(const ARRAY_TYPE * va, const osg::Vec3d & planenormal)
{
	//先找个凸点, 判断凸点处的方向, 就是多边形的方向;
	double farest = DBL_MIN;
	unsigned int farestIndex = 0;
	for (unsigned int i = 0; i < va->size(); ++i)
	{
		double r = (va->at(i)).length2();
		if (r < farest) continue;
		farest = r;
		farestIndex = i;
	}
	//凸点朝向;
	unsigned int pre = farestIndex == 0 ? va->size() - 1 : farestIndex - 1;
	unsigned int nex = farestIndex == va->size() - 1 ? 0 : farestIndex + 1;
	osg::Vec3d dir1 = normalize(va->at(farestIndex) - va->at(pre));
	osg::Vec3d dir2 = normalize(va->at(nex) - va->at(farestIndex));
	double f = (dir1^dir2)* planenormal;
	return f < 0;
}


template<typename ARRAY_TYPE, typename VEC_TYPE>
static VEC_TYPE getCenter(const ARRAY_TYPE* va)
{
	VEC_TYPE sum;
	for (ARRAY_TYPE::const_iterator it = va->begin(); it != va->end(); ++it)
	{
		sum += *it;
	}
	sum /= double(va->size());
	return sum;
}

struct GeologicalMaterial : public osg::Referenced
{
	GeologicalMaterial(const osg::Vec4& color, osg::Texture2D* texture)
		: _color(color)
		, _texture(texture)
	{}

	inline bool operator == (const GeologicalMaterial& gm)
	{
		return (_color == gm._color && _texture == gm._texture);
	}

	osg::Vec4 _color;
	osg::ref_ptr<osg::Texture2D> _texture;
};

//合并Geometry，计算纹理坐标、法线;
typedef std::vector<osg::ref_ptr<osg::Geode> > GeodeVector;
struct ColumnGeometryOptimizer :public osg::NodeVisitor
{
	ColumnGeometryOptimizer(const osg::Vec3d& wellCenter, const osg::Vec3d& dir, const osg::Vec3d&sidedir, double wellRadius, bool isOnEarth, unsigned segmentNum = 16)
		: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
		, _wellCenter(wellCenter)
		, _dir(normalize(dir))
		, _sideDir(normalize(sidedir))
		, _wellRadius(wellRadius)
		, _segmentNum(segmentNum)
		, _isOnEarth(isOnEarth)
	{
		_strideAngle = osg::PI / (double)segmentNum;
		_d = 2.0*wellRadius*sin(_strideAngle*0.5);

		_plane = osg::Plane(_dir, _wellCenter);
	}

	void apply(osg::Geode& geode);

private:
	//GeodeVector _geodeVector;

	bool _isOnEarth;
	double _wellRadius;
	osg::Vec3d _wellCenter;
	osg::Vec3d _dir;
	osg::Vec3d _sideDir;
	unsigned int _segmentNum;
	double _strideAngle;
	double _d;

	osg::Plane _plane;
};

struct PlaneGeometryOptimizer :public osg::NodeVisitor
{
	PlaneGeometryOptimizer(const osg::Vec3d& posOnPlane, const osg::Vec3d& normal, const osg::Vec3d& side)
		: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
		, _posOnPlane(posOnPlane)
		, _normal(normal)
		, _side(side)
	{

		_plane_t = osg::Plane(_side, _posOnPlane);
		_plane_s = osg::Plane(_side^_normal, _posOnPlane);
	}

	void apply(osg::Geode& geode);
	//osg::Node* optimize();

private:
	GeodeVector _geodeVector;
	osg::Vec3d _normal, _side, _posOnPlane;

	osg::Plane _plane_t, _plane_s;
};

static char* point_light_vert =
{
	"#version 120\n"
	"varying vec3 Normal, LightDirection;\n"
	"void main()\n"
	"{\n"
	"	vec4 Position = gl_ModelViewMatrix * gl_Vertex;\n"
	"	Normal = gl_NormalMatrix * gl_Normal;\n"
	"	LightDirection = gl_LightSource[0].position.xyz - Position.xyz;\n"
	"	gl_FrontColor = gl_Color;\n"
	"	gl_TexCoord[0] = gl_MultiTexCoord0;\n"
	"	gl_Position = gl_ProjectionMatrix * Position;\n"
	"}\n"
};

static char* point_light_frag =
{
	"#version 120 \n"
	"uniform sampler2D Texture; \n"
	"uniform int Texturing; \n"
	"varying vec3 Normal, LightDirection; \n"
	"void main() \n"
	"{ \n"
	"	float LightDistance2 = dot(LightDirection, LightDirection); \n"
	"	float LightDistance = sqrt(LightDistance2); \n"
	"	float NdotLD = max(dot(normalize(Normal), LightDirection / LightDistance), 0.0);\n"
	"	float Attenuation = gl_LightSource[0].constantAttenuation; \n"
	"	Attenuation += gl_LightSource[0].linearAttenuation * LightDistance; \n"
	"	Attenuation += gl_LightSource[0].quadraticAttenuation * LightDistance2; \n"
	"	gl_FragColor = gl_Color; \n"
	"	if(Texturing == 1) gl_FragColor *= texture2D(Texture, gl_TexCoord[0].st); \n"
	"	gl_FragColor.rgb *= (gl_LightSource[0].ambient.rgb + gl_LightSource[0].diffuse.rgb * NdotLD) / Attenuation;\n"
	"} \n"
};

static void addLightEffect(osg::StateSet* ss)
{
	osg::ref_ptr<osg::Program> vp = new osg::Program;
	vp->addShader(new osg::Shader(osg::Shader::VERTEX, point_light_vert));
	vp->addShader(new osg::Shader(osg::Shader::FRAGMENT, point_light_frag));
	ss->addUniform(new osg::Uniform("Texturing", 0));
	ss->addUniform(new osg::Uniform("Texture", 0));
	ss->setAttributeAndModes(vp, osg::StateAttribute::ON);
}

//-------------------------------------------------------------------------------------------------------

//#ifdef _DEBUG
//#pragma comment(lib, "gdal111d.lib")
//#else
//#pragma comment(lib, "gdal111.lib")
//#endif
//
//static bool convertGeomToShp(osg::Geometry* geom, const std::string& filename)
//{
//	if (!geom) return false;
//
//	osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
//	if (!va) return false;
//	if (va->empty()) return false;
//
//	CPLSetConfigOption("SHAPE_ENCODING", "");
//	OGRRegisterAll();//注册所有的驱动
//	//创建ESRI shp文件
//	char *pszDriverName = "ESRI Shapefile";
//	//调用对Shape文件读写的Driver
//	OGRSFDriver *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
//	if (!poDriver) return false;
//
//	OGRDataSource *shapeDataSource = poDriver->CreateDataSource(filename.c_str(), NULL);
//	if (!shapeDataSource) return false;
//
//	OGRFieldDefn layerId("ID", OFTInteger);
//	layerId.SetWidth(5);
//
//	OGRFieldDefn layerAttribuateFieldX("layer_attribuate", OFTString);
//	layerAttribuateFieldX.SetWidth(50);
//
//	std::string outShapName = filename;
//	std::string layerName = outShapName.substr(0, outShapName.length() - 4);
//	OGRLayer *poLayer = shapeDataSource->CreateLayer(layerName.c_str(), NULL, wkbLineString, NULL);
//	if (poLayer == NULL)
//	{
//		OGRDataSource::DestroyDataSource(shapeDataSource);
//		return false;
//	}
//	poLayer->CreateField(&layerId);
//	poLayer->CreateField(&layerAttribuateFieldX);
//
//	std::vector<osg::Vec3d> points; points.reserve(va->size());
//	for (osg::Vec3Array::const_iterator it = va->begin(); it != va->end(); ++it)
//	{
//		points.push_back(*it);
//	}
//
//	OGRLinearRing linearring;
//	for (std::vector<osg::Vec3d>::const_iterator it = points.begin(); it != points.end(); ++it)
//	{
//		linearring.addPoint(new OGRPoint((*it)[0], (*it)[1], (*it)[2]));
//	}
//
//	OGRFeature *poFeature;
//	poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
//	poFeature->SetField("ID", 0);
//	poFeature->SetField("layer_attribuate", "stone");
//	poFeature->SetGeometry(&linearring);
//	if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
//	{
//		printf("Failed to create feature in shapefile.\n");
//	}
//	OGRFeature::DestroyFeature(poFeature);
//	OGRDataSource::DestroyDataSource(shapeDataSource);
//
//	return true;
//}

#endif // GEOLOGICALOPERATORHELP_H