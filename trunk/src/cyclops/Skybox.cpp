/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#include "cyclops/Skybox.h"
#include "cyclops/SceneManager.h"
#include "cyclops/EffectNode.h"

#include <osg/Depth>
#include <osg/ShapeDrawable>
#include <osg/TexEnv>
#include <osg/TexMat>
#include <osg/PolygonMode>
#include <osgDB/ReadFile>
#include <osgwTools/Shapes.h>

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MoveSkyWithEyePointTransform : public osg::Transform
{
public:
	float pitch;
public:

    /** Get the transformation matrix which moves from local coords to world coords.*/
    virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const 
    {
        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
        if (cv)
        {
            osg::Vec3 eyePointLocal = cv->getEyeLocal();
            matrix.preMultTranslate(eyePointLocal);
			matrix.preMultRotate(osg::Quat(pitch, osg::Vec3(1, 0, 0)));
        }
        return true;
    }

    /** Get the transformation matrix which moves from world coords to local coords.*/
    virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
    {
        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
        if (cv)
        {
            osg::Vec3 eyePointLocal = cv->getEyeLocal();
            matrix.postMultTranslate(-eyePointLocal);
	    }
        return true;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Skybox::mysPreallocBlock[3];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Skybox::setPreallocBlock(int blockId, DrawContext::Eye eye)
{
	mysPreallocBlock[eye] = blockId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Skybox::Skybox():
	myTexture(NULL), myNode(NULL), myRootStateSet(NULL)
{
	// Allocate and delete MoveSkyWithEyePointTransform, since we can't use Ref<> inside the Skybox header 
	// (because MoveSkyWithEyePointTransform is defined within this file)
	myTransform = new MoveSkyWithEyePointTransform();

	// Init prealloc block ids to -1 (to disable usage of prealloc blocks)
	mysPreallocBlock[DrawContext::EyeCyclop] = -1;
	mysPreallocBlock[DrawContext::EyeLeft] = -1;
	mysPreallocBlock[DrawContext::EyeRight] = -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Skybox::~Skybox()
{
	delete myTransform;
	myTransform = NULL;
}
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Skybox::initialize(osg::StateSet* rootStateSet)
{
	//if(myTexture != NULL && myNode == NULL)
	{
		myRootStateSet = rootStateSet;
		myNode = createSkyBox();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Skybox::loadPano(const String& panoName)
{
	myType = Pano;

	// Change prealloc block state
	int prevBlock = ImageUtils::getLoadPreallocatedBlock();
	ImageUtils::setLoadPreallocatedBlock(mysPreallocBlock[DrawContext::EyeCyclop]);

	osg::Image* pano = osgDB::readImageFile(panoName);

	// Reset prealloc block state
	ImageUtils::setLoadPreallocatedBlock(prevBlock);

	if(pano)
	{
		osg::Texture2D* panoTex = new osg::Texture2D();
		panoTex->setImage(pano);
		myTexture = panoTex;
		updateSkyBox();
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Skybox::loadStereoPano(const String& panoName, const String& extension)
{
	myType = StereoPano;

	// Change prealloc block state
	int prevBlock = ImageUtils::getLoadPreallocatedBlock();

	ImageUtils::setLoadPreallocatedBlock(mysPreallocBlock[DrawContext::EyeLeft]);
	String fileName = ostr(panoName, %"L");
	osg::Image* panoL = osgDB::readImageFile(ostr("%1%.%2%", %fileName %extension));

	ImageUtils::setLoadPreallocatedBlock(mysPreallocBlock[DrawContext::EyeRight]);
	fileName = ostr(panoName, %"R");
	osg::Image* panoR = osgDB::readImageFile(ostr("%1%.%2%", %fileName %extension));

	// Reset prealloc block state
	ImageUtils::setLoadPreallocatedBlock(prevBlock);

	if(panoL && panoR)
	{
		osg::Texture2D* panoTexL = new osg::Texture2D();
		panoTexL->setImage(panoL);
		myTexture = panoTexL;

		osg::Texture2D* panoTexR = new osg::Texture2D();
		panoTexR->setImage(panoR);
		myTextureR = panoTexR;

		updateSkyBox();
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Skybox::loadCubeMap(const String& cubemapDir, const String& extension)
{
	myType = CubeMap;

    Ref<osg::TextureCubeMap> cubemap = new osg::TextureCubeMap;

	Ref<osg::Image> imagePosX = osgDB::readImageFile(cubemapDir + "/posx." + extension);
    Ref<osg::Image> imageNegX = osgDB::readImageFile(cubemapDir + "/negx." + extension);
    Ref<osg::Image> imagePosY = osgDB::readImageFile(cubemapDir + "/negy." + extension);
    Ref<osg::Image> imageNegY = osgDB::readImageFile(cubemapDir + "/posy." + extension);
    Ref<osg::Image> imagePosZ = osgDB::readImageFile(cubemapDir + "/posz." + extension);
    Ref<osg::Image> imageNegZ = osgDB::readImageFile(cubemapDir + "/negz." + extension);

    if (!imagePosX.isNull() && !imageNegX.isNull() && 
		!imagePosY.isNull() && !imageNegY.isNull() && 
		!imagePosZ.isNull() && !imageNegZ.isNull())
    {
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, imagePosX.get());
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, imageNegX.get());
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, imagePosY.get());
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, imageNegY.get());
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, imagePosZ.get());
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, imageNegZ.get());

        cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
        cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

        cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
        cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

		myTexture = cubemap;

		updateSkyBox();

		return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Node* Skybox::createSkyBox()
{
	//if(myTexture != NULL)
	{
		osg::StateSet* stateset = new osg::StateSet();

		// Uncomment to draw wireframe skybox.
		//osg::PolygonMode* pm = new osg::PolygonMode();
		//pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		//stateset->setAttribute(pm);

		stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
		stateset->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );
		stateset->setMode( GL_TEXTURE_2D, osg::StateAttribute::ON );

		// clear the depth to the far plane.
		Ref<osg::Depth> depth = new osg::Depth;
		depth->setFunction(osg::Depth::ALWAYS);
		depth->setRange(1.0,1.0);   
		stateset->setAttributeAndModes(depth, osg::StateAttribute::ON );

		stateset->setRenderBinDetails(-1,"RenderBin");

		
		osg::Geometry* geometry = osgwTools::makeAltAzSphere(10.0, 24, 24);
		//osg::Geometry* geometry = osgwTools::makeGeodesicSphere(0.1f, 8);
		//osg::Geometry* geometry = osgwTools::makeBox(osg::Vec3(0.1f, 0.1f, 0.1f));

		myGeode = new osg::Geode;		
		myGeode->setCullingActive(false);
		myGeode->setStateSet(stateset);
		myGeode->addDrawable(geometry);

		myTransform->setCullingActive(false);
		myTransform->addChild(myGeode);

		osg::ClearNode* clearNode = new osg::ClearNode;
		clearNode->setClearColor(osg::Vec4(0.0, 1.0, 1.0, 1.0));
		clearNode->addChild(myTransform);

		updateSkyBox();

		return clearNode;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Skybox::updateSkyBox()
{
	if(myTexture != NULL && myGeode != NULL)
	{
		omsg("Updating Skybox");

		SceneManager* sm = SceneManager::instance();
		ProgramAsset* cubeMapProgram = NULL;
		if(myType == CubeMap)
		{
			cubeMapProgram = sm->getProgram(
				"skybox-cube", 
				"cyclops/common/Skybox.vert", 
				"cyclops/common/Skybox.frag");
		}
		else if(myType == Pano)
		{
			cubeMapProgram = sm->getProgram(
				"skybox-pano", 
				"cyclops/common/Skybox-pano.vert", 
				"cyclops/common/Skybox-pano.frag");
		}
		else if(myType == StereoPano)
		{
			cubeMapProgram = sm->getProgram(
				"skybox-stereopano", 
				"cyclops/common/Skybox-pano.vert", 
				"cyclops/common/Skybox-stereopano.frag");
		}

		osg::StateSet* stateset = myGeode->getOrCreateStateSet();
		if(cubeMapProgram != NULL)
		{
			stateset->setAttributeAndModes(cubeMapProgram->program, osg::StateAttribute::ON);
		}

		// Setup the root state set to apply the environment map to scene objects.
		// Use Texture stage 3, 6 for the environment map (0-2 reserved for object textures, 4,5 reserved for shadow maps)
		myRootStateSet->setTextureAttributeAndModes(3, myTexture, osg::StateAttribute::ON);
		if(myType == StereoPano)
		{
			myRootStateSet->setTextureAttributeAndModes(6, myTextureR, osg::StateAttribute::ON);
		}

		if(myTextureUniform != NULL)
		{
			myRootStateSet->removeUniform(myTextureUniform);
		}

		if(myType == CubeMap)
		{
			myRootStateSet->addUniform( new osg::Uniform("unif_CubeMap", 3) );
		}
		else if(myType == Pano)
		{
			myRootStateSet->addUniform( new osg::Uniform("unif_PanoMap", 3) );
		}
		else if(myType == StereoPano)
		{
			myRootStateSet->addUniform( new osg::Uniform("unif_PanoMapL", 3) );
			myRootStateSet->addUniform( new osg::Uniform("unif_PanoMapR", 6) );
		}

		if(myType == CubeMap)
		{
			myTransform->pitch = 0.0f;
		}
		else if(myType == Pano || myType == StereoPano)
		{
			myTransform->pitch = -Math::Pi / 2;
		}
	}
}
