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
#include "cyclops/SkyBox.h"
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
SkyBox::SkyBox():
	myTexture(NULL), myNode(NULL), myRootStateSet(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SkyBox::initialize(osg::StateSet* rootStateSet)
{
	if(myTexture != NULL && myNode == NULL)
	{
		myRootStateSet = rootStateSet;
		myNode = createSkyBox();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SkyBox::loadPano(const String& panoName)
{
	myType = Pano;
	osg::Image* pano = osgDB::readImageFile(panoName);
	if(pano)
	{
		osg::Texture2D* panoTex = new osg::Texture2D();
		panoTex->setImage(pano);
		myTexture = panoTex;
       myTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        myTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
        myTexture->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

        myTexture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
        myTexture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);		
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SkyBox::loadCubeMap(const String& cubemapDir, const String& extension)
{
	myType = CubeMap;

    osg::TextureCubeMap* cubemap = new osg::TextureCubeMap;

    osg::Image* imagePosX = osgDB::readImageFile(cubemapDir + "/posx." + extension);
    osg::Image* imageNegX = osgDB::readImageFile(cubemapDir + "/negx." + extension);
    osg::Image* imagePosY = osgDB::readImageFile(cubemapDir + "/negy." + extension);
    osg::Image* imageNegY = osgDB::readImageFile(cubemapDir + "/posy." + extension);
    osg::Image* imagePosZ = osgDB::readImageFile(cubemapDir + "/posz." + extension);
    osg::Image* imageNegZ = osgDB::readImageFile(cubemapDir + "/negz." + extension);

    if (imagePosX && imageNegX && imagePosY && imageNegY && imagePosZ && imageNegZ)
    {
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, imagePosX);
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, imageNegX);
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, imagePosY);
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, imageNegY);
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, imagePosZ);
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, imageNegZ);

        cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
        cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

        cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
        cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

		myTexture = cubemap;

		return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Node* SkyBox::createSkyBox()
{
	if(myTexture != NULL)
	{
		osg::StateSet* stateset = new osg::StateSet();

		osg::TexEnv* te = new osg::TexEnv;
		//te->setMode(osg::TexEnv::REPLACE);
		//stateset->setTextureAttributeAndModes(0, te, osg::StateAttribute::ON);

		osg::TexGen *tg = new osg::TexGen;
		//tg->setMode(osg::TexGen::SPHERE_MAP);
		//tg->setMode(osg::TexGen::EYE_LINEAR);
		//stateset->setTextureAttributeAndModes(0, tg, osg::StateAttribute::ON);
		
		// Uncomment to draw wireframe skybox.
		//osg::PolygonMode* pm = new osg::PolygonMode();
		//pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		//stateset->setAttribute(pm);

		// Setup the root state set to apply the environment map to scene objects.
		// Use Texture stage 3 for the environment map (0-2 reserved for object textures, 4,5 reserved for shadow maps)
		myRootStateSet->setTextureAttributeAndModes(3, myTexture, osg::StateAttribute::ON);
		if(myType == CubeMap)
		{
			myRootStateSet->addUniform( new osg::Uniform("unif_CubeMap", 3) );
		}
		else if(myType == Pano)
		{
			myRootStateSet->addUniform( new osg::Uniform("unif_PanoMap", 3) );
		}

		//stateset->setTextureAttributeAndModes(0, myCubeMap, osg::StateAttribute::ON);

		stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
		stateset->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );
		stateset->setMode( GL_TEXTURE_2D, osg::StateAttribute::ON );

		// clear the depth to the far plane.
		osg::Depth* depth = new osg::Depth;
		depth->setFunction(osg::Depth::ALWAYS);
		depth->setRange(1.0,1.0);   
		stateset->setAttributeAndModes(depth, osg::StateAttribute::ON );

		stateset->setRenderBinDetails(-1,"RenderBin");

		
		osg::Geometry* geometry = osgwTools::makeAltAzSphere(10.0, 24, 24);
		//osg::Geometry* geometry = osgwTools::makeGeodesicSphere(0.1f, 8);
		//osg::Geometry* geometry = osgwTools::makeBox(osg::Vec3(0.1f, 0.1f, 0.1f));

		osg::Geode* geode = new osg::Geode;
		geode->setCullingActive(false);
		geode->setStateSet( stateset );
		geode->addDrawable(geometry);


		ProgramAsset* cubeMapProgram = NULL;
		if(myType == CubeMap)
		{
			cubeMapProgram = SceneManager::instance()->getProgram(
				"skybox-cube", 
				"cyclops/common/SkyBox.vert", 
				"cyclops/common/SkyBox.frag");
		}
		else if(myType == Pano)
		{
			cubeMapProgram = SceneManager::instance()->getProgram(
				"skybox-pano", 
				"cyclops/common/SkyBox-pano.vert", 
				"cyclops/common/SkyBox-pano.frag");
		}

		if(cubeMapProgram != NULL)
		{
			stateset->setAttributeAndModes(cubeMapProgram->program, osg::StateAttribute::ON);
		}

		MoveSkyWithEyePointTransform* transform = new MoveSkyWithEyePointTransform;
		if(myType == CubeMap)
		{
			transform->pitch = 0.0f;
		}
		else if(myType == Pano)
		{
			transform->pitch = -Math::Pi / 2;
		}

		transform->setCullingActive(false);
		transform->addChild(geode);

		osg::ClearNode* clearNode = new osg::ClearNode;
		clearNode->setClearColor(osg::Vec4(0.0, 1.0, 1.0, 1.0));
		clearNode->addChild(transform);
		return clearNode;
	}
	return NULL;
}
