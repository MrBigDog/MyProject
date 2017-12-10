#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Texture2D>
#include <osg/Light>

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgUtil/CullVisitor>

#ifdef _DEBUG
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgGAd.lib")
#pragma comment(lib, "osgUtild.lib")
#pragma comment(lib, "osgViewerd.lib")
#else
#pragma comment(lib, "osg.lib")
#pragma comment(lib, "osgDB.lib")
#pragma comment(lib, "osgGA.lib")
#pragma comment(lib, "osgUtil.lib")
#pragma comment(lib, "osgViewer.lib")
#endif // _DEBUG

static char* shadow_map_vert =
{
	"#version 120\n"
	"\n"
	//"uniform mat4 ShadowMatrix;\n"
	//"uniform mat4 osg_ViewMatrixInverse;\n"
	"\n"
	//"varying vec4 ShadowMapTexCoord;\n"
	"varying vec3 Normal, LightDirection;\n"
	"\n"
	"void main()\n"
	"{\n"
	"vec4 Position = gl_ModelViewMatrix * gl_Vertex;\n"
	//"ShadowMapTexCoord = ShadowMatrix * osg_ViewMatrixInverse* Position;\n"
	"Normal = gl_NormalMatrix * gl_Normal;\n"
	"LightDirection = gl_LightSource[0].position.xyz - Position.xyz;\n"
	"gl_FrontColor = gl_Color;\n"
	"gl_TexCoord[0] = gl_MultiTexCoord0;\n"
	"gl_Position = gl_ProjectionMatrix * Position;\n"
	"}\n"
};

static char* shadow_map_frag =
{
	"#version 120 \n"
	" \n"
	"uniform sampler2D Texture; \n"
	//"uniform sampler2D ShadowMap; \n"
	"uniform int Texturing; \n"
	" \n"
	//"varying vec4 ShadowMapTexCoord; \n"
	"varying vec3 Normal, LightDirection; \n"
	" \n"
	"void main() \n"
	"{ \n"
	"float LightDistance2 = dot(LightDirection, LightDirection); \n"
	"float LightDistance = sqrt(LightDistance2); \n"
	"float NdotLD = max(dot(normalize(Normal), LightDirection / LightDistance), 0.0);\n"
	"float Attenuation = gl_LightSource[0].constantAttenuation; \n"
	"Attenuation += gl_LightSource[0].linearAttenuation * LightDistance; \n"
	"Attenuation += gl_LightSource[0].quadraticAttenuation * LightDistance2; \n"
	////" NdotLD *= shadow2DProj(ShadowMap, ShadowMapTexCoord).r; \n"
	//"if(ShadowMapTexCoord.w > 0.0) \n"
	//"{ \n"
	//"vec3 ShadowMapTexCoordProj = ShadowMapTexCoord.xyz / ShadowMapTexCoord.w; \n"
	//" \n"
	//"if(ShadowMapTexCoordProj.x >= 0.0 && ShadowMapTexCoordProj.x < 1.0 && \n"
	//"ShadowMapTexCoordProj.y >= 0.0 && ShadowMapTexCoordProj.y < 1.0 && \n"
	//"ShadowMapTexCoordProj.z >= 0.0 && ShadowMapTexCoordProj.z < 1.0) \n"
	//"{ \n"
	//"const float b0 = 0.000001; \n"
	//"const float b1 = 0.01; \n"
	//"vec3 L = normalize(gl_LightSource[0].position.xyz); \n"
	//"vec3 N = normalize(Normal); \n"
	//"float costheta = clamp(dot(L,N), 0.0, 1.0); \n"
	//"float bias = b0*tan(acos(costheta)); \n"
	//" \n"
	//"if(texture2D(ShadowMap, ShadowMapTexCoordProj.xy).r <= ShadowMapTexCoordProj.z - bias) \n"
	//"{ \n"
	////"NdotLD = 0.0; \n"
	//"} \n"
	//"} \n"
	//"} \n"

	"gl_FragColor = gl_Color; \n"
	"if(Texturing == 1) gl_FragColor *= texture2D(Texture, gl_TexCoord[0].st); \n"
	"gl_FragColor.rgb *= (gl_LightSource[0].ambient.rgb + gl_LightSource[0].diffuse.rgb * NdotLD) / Attenuation;\n"
	"} \n"
};

osg::Texture2D* createTexture2D(std::string imageName)
{
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setImage(osgDB::readImageFile(imageName));
	texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::MIRROR);
	texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::MIRROR);
	texture->setResizeNonPowerOfTwoHint(false);

	return texture.release();
}

osg::Camera* createRtt(osg::Vec3 lightpos)
{
	osg::ref_ptr<osg::Camera> rttCamera = new osg::Camera;
	rttCamera->setReferenceFrame(osg::Camera::ABSOLUTE_RF_INHERIT_VIEWPOINT);
	rttCamera->setClearDepth(1.0);
	rttCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
	rttCamera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	rttCamera->setViewport(0, 0, 1024, 1024);
	rttCamera->setRenderOrder(osg::Camera::PRE_RENDER);
	rttCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	rttCamera->setImplicitBufferAttachmentMask(0, 0);

	rttCamera->setProjectionMatrix(osg::Matrix::ortho(-10, 10, -10, 10, -10, 20));
	rttCamera->setViewMatrixAsLookAt(lightpos, osg::Vec3(), osg::Vec3(0.0, 0.0, 1.0));

	return rttCamera.release();
}

osg::Geode* createBall(osg::Vec3 pos, float radius)
{
	osg::ref_ptr<osg::Sphere> ball = new osg::Sphere(pos, radius);
	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(ball);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(sd);
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, createTexture2D("cube.jpg"));

	return geode.release();
}

osg::Geode* createBox(osg::Vec3 pos, float width)
{
	osg::ref_ptr<osg::Box> box = new osg::Box(pos, width);
	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(box);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(sd);
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, createTexture2D("cube.jpg"));

	return geode.release();
}

osg::Geode* createPlane(float width)
{
	osg::ref_ptr<osg::Geometry> geom = createTexturedQuadGeometry(osg::Vec3(-width*0.5, -width*0.5, -1.0f), osg::Vec3(width, 0.0f, -1.0f), osg::Vec3(0.0, width, -1.0f));
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom);
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, createTexture2D("floor.jpg"));

	return geode.release();
}

osg::Group* createScene()
{
	osg::ref_ptr<osg::Group> group = new osg::Group;
	group->addChild(createPlane(200.0f));
	group->addChild(createBox(osg::Vec3(), 2.0f));
	group->addChild(createBox(osg::Vec3d(5.0f, 0.0f, 0.0f), 2.0f));
	group->addChild(createBall(osg::Vec3(-5.0f, 0.0f, 0.0f), 2.0f));

	return group.release();
}

osg::Texture2D* createShadowTexture()
{
	osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
	tex->setTextureSize(1024, 1024);
	tex->setInternalFormat(GL_DEPTH_COMPONENT);
	tex->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	tex->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	tex->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER);
	tex->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER);
	tex->setBorderColor(osg::Vec4(1, 1, 1, 1));

	return tex.release();
}

int main()
{
	osg::ref_ptr<osg::Node> scene = createScene();

	osg::Vec3 light_pos = osg::Vec3(0.0, 10.0, 10.0);
	osg::ref_ptr<osg::Geode> lightball = createBall(light_pos, 1.0);

	osg::ref_ptr<osg::Camera> rtt_camera = createRtt(light_pos);
	rtt_camera->addChild(scene);

	osg::Matrixf BiasMatrix =
		osg::Matrix::translate(1.0, 1.0, 1.0) *
		osg::Matrix::scale(0.5, 0.5, 0.5);

	osg::Matrixf light_project_matrix = rtt_camera->getProjectionMatrix();
	osg::Matrixf light_view_matrix = rtt_camera->getViewMatrix();
	osg::Matrixf shadow_matrix = light_view_matrix  * light_project_matrix * BiasMatrix;

	osg::ref_ptr<osg::Texture2D> shadow_texture = createShadowTexture();
	rtt_camera->attach(osg::Camera::DEPTH_BUFFER, shadow_texture);

	osg::ref_ptr<osg::Program> vp = new osg::Program;
	vp->addShader(new osg::Shader(osg::Shader::VERTEX, shadow_map_vert));
	vp->addShader(new osg::Shader(osg::Shader::FRAGMENT, shadow_map_frag));

	osg::ref_ptr<osg::Uniform> shadow_matrix_uniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "ShadowMatrix");
	shadow_matrix_uniform->set(shadow_matrix);

	osg::StateSet* ss = scene->getOrCreateStateSet();
	ss->setTextureAttributeAndModes(1, shadow_texture);
	ss->addUniform(shadow_matrix_uniform);
	ss->addUniform(new osg::Uniform("Texturing", 1));
	ss->addUniform(new osg::Uniform("Texture", 0));
	ss->addUniform(new osg::Uniform("ShadowMap", 1));
	ss->setAttributeAndModes(vp, osg::StateAttribute::ON);
	//ss->setMode(GL_LIGHT0,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
	//ss->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

	osg::ref_ptr<osg::Geode> quad = new osg::Geode;
	quad->addDrawable(createTexturedQuadGeometry(osg::Vec3(-10.0f, 10.0f, 0.0f), osg::Vec3(20.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 20.0f)));
	quad->getOrCreateStateSet()->setTextureAttributeAndModes(0, shadow_texture);

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(scene);
	root->addChild(rtt_camera);
	//root->addChild( quad );
	root->addChild(lightball);

	osg::ref_ptr<osg::Light> light = new osg::Light;
	light->setPosition(osg::Vec4(light_pos[0], light_pos[1], light_pos[2], 1.0));
	light->setDirection(light_pos);
	light->setAmbient(osg::Vec4(0.5, 0.5, 0.5, 1.0));
	light->setDiffuse(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	light->setLightNum(0);

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	viewer->setCameraManipulator(new osgGA::TrackballManipulator());
	viewer->setSceneData(root);
	viewer->setLight(light);
	viewer->setLightingMode(osg::View::SKY_LIGHT);
	viewer->getCamera()->setClearColor(osg::Vec4(0, 0, 0, 1));

	//while(!viewer->done())
	//{
	////osg::Matrixf BiasMatrix( 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f );
	////osg::Matrixf light_project_matrix = rtt_camera->getProjectionMatrix();
	////osg::Matrixf light_view_matrix = rtt_camera->getViewMatrix();
	////osg::Matrixf shadow_matrix =light_view_matrix  * light_project_matrix * BiasMatrix;

	////osg::Matrixf inverseMV;

	////shadow_matrix_uniform->set(/*inverseMV * */shadow_matrix );
	//viewer->frame();
	//}

	viewer->realize();
	viewer->run();

	return 0;
}