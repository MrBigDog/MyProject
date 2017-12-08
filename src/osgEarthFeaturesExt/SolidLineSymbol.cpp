#include <osgEarthFeaturesExt/SolidLineSymbol.h>
#include <osg/Geometry>
#include <osg/Geode>

namespace oeFeaturesExt
{
	OSGEARTH_REGISTER_SIMPLE_SYMBOL(solid_line, SolidLineSymbol);

	namespace
	{
		//default cylinder
		osg::Geode* createDefaultUnitCylinder(int segmentNum = 24)
		{
			int vertexNum = segmentNum * 2 + 2;

			osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array; va->reserve(vertexNum);
			osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array; na->reserve(vertexNum);
			osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array; ta->reserve(vertexNum);
			osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array; ca->reserve(1);
			ca->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));

			double deltaAngle = osg::PI*2.0 / ((double)segmentNum);
			for (int i = 0; i < segmentNum + 1; ++i)
			{
				double vAngle = (double)i * deltaAngle;
				double vX = sin(vAngle);
				double vY = cos(vAngle);

				va->push_back(osg::Vec3d(vX, vY, 0.5));
				va->push_back(osg::Vec3d(vX, vY, -0.5));

				osg::Vec3d nor = osg::Vec3d(vX, vY, 0.0); nor.normalize();
				na->push_back(nor);
				na->push_back(nor);

				float vU = (float)i / (float)segmentNum;
				ta->push_back(osg::Vec2(vU, 0.0));
				ta->push_back(osg::Vec2(vU, 1.0));
			}

			osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
			geom->setUseVertexBufferObjects(true);
			geom->setUseDisplayList(false);
			geom->setVertexArray(va);
			geom->setTexCoordArray(0, ta);
			geom->setNormalArray(na, osg::Array::BIND_PER_VERTEX);
			geom->setColorArray(ca, osg::Array::BIND_OVERALL);
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, vertexNum));

			osg::ref_ptr<osg::Geode> geode = new osg::Geode;
			geode->addDrawable(geom);

			//osg::Image* image = osgDB::readImageFile("pipelineys.jpg");
			//if (image && image != NULL)
			//{
			//	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
			//	texture->setImage(image);

			//	if (texture)
			//	{
			//		geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
			//	}
			//}
			return geode.release();
		}
	}

	//---------------------------------------------------------------------------------------
	SolidLineSymbol::SolidLineSymbol(const SolidLineSymbol & rhs, const osg::CopyOp & copyop)
		: Symbol(rhs, copyop)
		, _unidSolidLineModel(rhs._unidSolidLineModel)
		, _radiusPropertyName(rhs._radiusPropertyName)
	{
		if (!_unidSolidLineModel)
		{
			_unidSolidLineModel = createDefaultUnitCylinder();
		}
	}

	SolidLineSymbol::SolidLineSymbol(const osgEarth::Config & conf)
		: Symbol(conf)
	{
		mergeConfig(conf);
	}

	osgEarth::Config SolidLineSymbol::getConfig() const
	{
		osgEarth::Config conf = osgEarth::Symbology::Symbol::getConfig();
		conf.key() = "solid_line";
		conf.addNonSerializable("SolidLineSymbol::Node", _unidSolidLineModel.get());
		return conf;
	}

	void SolidLineSymbol::mergeConfig(const osgEarth::Config & conf)
	{
		osgEarth::Symbology::Symbol::mergeConfig(conf);
		_unidSolidLineModel = conf.getNonSerializable<osg::Node>("SolidLineSymbol::Node");
	}

	void SolidLineSymbol::parseSLD(const osgEarth::Config & c, osgEarth::Symbology::Style & style)
	{

	}
}