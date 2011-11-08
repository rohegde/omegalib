/*******************************************************************************/
/*
 * Author: Ing. Jose Larios Delgado.
 * Project:  glsl shaders implementation with OpenSceneGraph.
 * Institutions: DGSCA, UNAM, Virtual Reality Department.
 * Description: Parallax Map for a directional light with soft shadow.   
 */
/*******************************************************************************/
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
#include <osg/Group>
#include <osg/Notify>
#include <osg/Program>
#include <osg/Shader>
#include <osg/Uniform>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>

#include <osg/Material>

#include <osg/PositionAttitudeTransform>
#include <osg/Texture2D>
#include <osg/ShapeDrawable>
#include <osgUtil/TangentSpaceGenerator>

#include <osgShadow/ShadowedScene>
#include <osgShadow/SoftShadowMap>
#include <osgUtil/Optimizer>

#include <osg/NodeVisitor>
#include <iostream>

#include "findgeometry.h"

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <oengine.h>
#include <oosg.h>

using namespace omega;
using namespace oengine;
using namespace oosg;

static osg::Timer_t old_tick, new_tick; 
static double delta;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LoadShaderSource( osg::Shader* shader, const std::string& fileName )
{
	String path;
	DataManager::findFile(fileName, path);
    if( path.length() != 0 )
    {
        shader->loadShaderSourceFromFile( path.c_str() );
    }
    else
    {
        osg::notify(osg::WARN) << "File \"" << fileName << "\" not found." << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OsgParallax: public EngineServer
{
public:
	OsgParallax(Application* app): EngineServer(app) {}
	virtual void initialize();
	virtual void update(const UpdateContext& context);
	void handleEvent(const Event& evt);

private:
	OsgModule* myOsg;
	SceneNode* mySceneNode;
	SceneNode* myLightNode;
	osg::Light* myLight2;
	DefaultMouseInteractor* myMouseInteractor;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OsgParallax::initialize()
{
	EngineServer::initialize();
	myOsg = new OsgModule();
	myOsg->initialize(this);

	osg::Program *illProgram;
	osg::Shader *illVertObj;
	osg::Shader *illFragObj;

    // construct the viewer.
	osgViewer::Viewer *viewer = new osgViewer::Viewer();
	osg::Group *root = new osg::Group();

	osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
    shadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
    shadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);
	
	String path;
	DataManager::findFile("osg/paralax.osg", path);
	osg::Node *cqNode = osgDB::readNodeFile(path);

	osgUtil::Optimizer optimizer;

	osg::PositionAttitudeTransform* pat = new osg::PositionAttitudeTransform();
	pat->addChild(cqNode);

	pat->setScale(osg::Vec3(0.02f, 0.02f, 0.02f));
	pat->setAttitude(osg::Quat(
		-Math::Pi / 2, osg::Vec3d(1, 0, 0),
		0, osg::Vec3d(0, 1, 0),
		0, osg::Vec3d(0, 0, 1)
		));

	//optimizer.optimize(pat,	osgUtil::Optimizer::FLATTEN_STATIC_TRANSFORMS |
	//							osgUtil::Optimizer::REMOVE_REDUNDANT_NODES |
	//							osgUtil::Optimizer::DEFAULT_OPTIMIZATIONS);

	shadowedScene->addChild(pat);
	root->addChild(shadowedScene);

	findGeoVisitor findNode;
	root->accept(findNode);

	myOsg->setRootNode(root);

	//std::cout<<findNode.geolist.size()<<std::endl;

	//------------------------------------------------------------------
	//---------------------------------------------------------------------

	for(unsigned int i = 0; i < findNode.geolist.size(); i++ )
	{
		
		osg::StateSet *terrainState = findNode.geolist.at(i)->getOrCreateStateSet();
		terrainState->addUniform( new osg::Uniform("colorMap", 0) );
	
		terrainState->addUniform( new osg::Uniform("normalMap", 1) );
	
		terrainState->addUniform( new osg::Uniform("heightMap", 3) );
	

	
		osg::Material *material = new osg::Material();
		material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.9f, 0.9f, 0.9f, 1.0f));
		material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.9f, 0.9f, 0.9f, 1.0f));
		material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.2f, 0.2f, 0.2f, 1.0f));
		material->setShininess(osg::Material::FRONT_AND_BACK, 4.0f);
		terrainState->setAttribute(material, osg::StateAttribute::ON);	


		illProgram = new osg::Program;
		illProgram->setName( "Parallax" );
		illVertObj = new osg::Shader( osg::Shader::VERTEX );
		illFragObj = new osg::Shader( osg::Shader::FRAGMENT );
		illProgram->addShader( illFragObj );
		illProgram->addShader( illVertObj );
		terrainState->setAttributeAndModes(illProgram, osg::StateAttribute::ON);


		LoadShaderSource( illVertObj, "shaders/parallaxv4.vert" );
		LoadShaderSource( illFragObj, "shaders/parallaxv4.frag" );

		osgUtil::TangentSpaceGenerator* tsg = new osgUtil::TangentSpaceGenerator();
	
		tsg->generate(findNode.geolist.at(i), 3);
	
		osg::Vec4Array* a_tangent = tsg->getTangentArray();
		//osg::Vec4Array* a_binormal = tsg->getBinormalArray();
	
		// the relevant part for using "setVertexAttribArray"
		findNode.geolist.at(i)->setVertexAttribArray (6, a_tangent);
		//findNode.geolist.at(i)->setVertexAttribArray (7, a_binormal);
		findNode.geolist.at(i)->setVertexAttribBinding (6, osg::Geometry::BIND_PER_VERTEX);
		//findNode.geolist.at(i)->setVertexAttribBinding (7, osg::Geometry::BIND_PER_VERTEX);
		illProgram->addBindAttribLocation ("Tangent", 6);
		//illProgram->addBindAttribLocation ("binormal", 7);

		//std::cout<<tmpGeo->getNumVertexAttribArrays()<<std::endl;

		
	}
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------

	osg::ref_ptr<osgShadow::SoftShadowMap> sm = new osgShadow::SoftShadowMap;
	//sm->addShader(ssmFrag);
	//sm->addShader(ssmVert);
	sm->setTextureSize(osg::Vec2s(512, 512));
	sm->setAmbientBias(osg::Vec2(0.8f, 0.8f));
	sm->setTextureUnit(4);
	sm->setJitterTextureUnit(5);
	//sm->setSoftnessWidth(0.01);

    shadowedScene->setShadowTechnique(sm.get());

	/*osg::StateSet *rootState = new osg::StateSet();
	root->setStateSet(rootState);*/

	osg::StateSet *sState = shadowedScene->getOrCreateStateSet();

	/*osg::Uniform* sbias   = new osg::Uniform( "scaleBias", osg::Vec2(0.034f, 0.004f));
	rootState->addUniform( sbias );*/
	
	myLight2 = new osg::Light;
    myLight2->setLightNum(0);
    myLight2->setPosition(osg::Vec4(0.0, 282.8427, 282.8427, 0.0));
    myLight2->setAmbient(osg::Vec4(0.2f,0.2f,0.2f,1.0f));
    myLight2->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	myLight2->setSpecular(osg::Vec4(0.8f,0.8f,0.8f,1.0f));
    /*myLight2->setConstantAttenuation(1.0f);
    myLight2->setLinearAttenuation(2.0f/70.0f);
    myLight2->setQuadraticAttenuation(2.0f/osg::square(70.0f));*/

    osg::LightSource* lightS2 = new osg::LightSource;    
    lightS2->setLight(myLight2);
    lightS2->setLocalStateSetModes(osg::StateAttribute::ON); 

    lightS2->setStateSetModes(*sState,osg::StateAttribute::ON);
	shadowedScene->addChild(lightS2);

	myLightNode = new SceneNode(this);
	//myLightNode->scale(0.1f, 0.1f, 0.1f);
	myLightNode->addObject(new Box());

	getScene(0)->addChild(myLightNode);

	myMouseInteractor = new DefaultMouseInteractor();
	myMouseInteractor->setSceneNode(myLightNode);
	myMouseInteractor->setRotateButtonFlag(Event::Middle);
	myMouseInteractor->setMoveButtonFlag(Event::Right);
	addActor(myMouseInteractor);

	getDefaultCamera()->focusOn(getScene(0));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OsgParallax::update(const UpdateContext& context)
{
	EngineServer::update(context);
	myOsg->update(context);
	Vector3f pos = myLightNode->getPosition();
	myLight2->setPosition(osg::Vec4(pos[0], pos[1], pos[2], 1.0f));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgParallax::handleEvent(const Event& evt) 
{
	EngineServer::handleEvent(evt);
	if(evt.isKeyDown('l'))
    {
		Vector3f pos = getDefaultCamera()->getPosition();
		myLightNode->setPosition(pos);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	EngineApplication<OsgParallax> app;
	const char* cfgName = "system/desktop.cfg";
	if(argc == 2) cfgName = argv[1];
	omain(app, cfgName, "osgparallax.log", new FilesystemDataSource(OMEGA_DATA_PATH));
	return 0;
}
