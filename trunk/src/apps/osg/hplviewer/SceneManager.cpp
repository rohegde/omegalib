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
#include "SceneManager.h"

#include <osgDB/Archive>
#include <osgDB/ReadFile>

using namespace omega;
using namespace oengine;
using namespace hpl;

SceneManager* SceneManager::mysInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SceneManager::SceneManager()
{
	mysInstance = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneManager::findResource(const String& name, String& outPath)
{
	if(DataManager::findFile(name, outPath)) return true;
	if(DataManager::findFile("../" + name, outPath)) return true;
	if(DataManager::findFile("../wall/" + name, outPath)) return true;
	if(DataManager::findFile("../ceiling/" + name, outPath)) return true;
	if(DataManager::findFile("../pillar/" + name, outPath)) return true;
	if(DataManager::findFile("../special/" + name, outPath)) return true;
	if(DataManager::findFile("../floor/" + name, outPath)) return true;
	if(DataManager::findFile("../stairs/" + name, outPath)) return true;
	if(DataManager::findFile("../../decals/" + name, outPath)) return true;
	if(DataManager::findFile("../../debris/hole/" + name, outPath)) return true;
	if(DataManager::findFile("../../debris/rock/" + name, outPath)) return true;
	if(DataManager::findFile("../../debris/wood/" + name, outPath)) return true;
	if(DataManager::findFile("../../castlebase/ceiling/" + name, outPath)) return true;
	if(DataManager::findFile("../cellarbase/floor/" + name, outPath)) return true;
	if(DataManager::findFile("../../particles/materials/" + name, outPath)) return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Texture2D* SceneManager::getTexture(const String& name)
{
	// If texture has been loaded already return it.
	if(myTextures.find(name) != myTextures.end())
	{
		return myTextures[name];
	}

	// Split the path and get the file name.
	String path;
	String filename;
	String extension;
	StringUtils::splitFullFilename(name, filename, extension, path);

	if(findResource(filename + "." + extension, path))
	{
		//ofmsg("Loading texture file %1%", %filename);

        osg::ref_ptr< osg::Image > image;

		image = osgDB::readRefImageFile(path);

        if ( image.valid() )
        {
            osg::Texture2D* texture = new osg::Texture2D( image.get() );
            osg::Texture::WrapMode textureWrapMode;
            textureWrapMode = osg::Texture::REPEAT;

            texture->setWrap(osg::Texture2D::WRAP_R, textureWrapMode);
            texture->setWrap(osg::Texture2D::WRAP_S, textureWrapMode);
            texture->setWrap(osg::Texture2D::WRAP_T, textureWrapMode);

			myTextures[name] = texture;
			return texture;
        }
		else
		{
			ofwarn("Image not valid: %1%", %filename);
		}
	}
	else
	{
		ofwarn("Could not find texture file %1%", %filename);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::loadShader(osg::Shader* shader, const String& name)
{
	String path;
	if(DataManager::findFile(name, path))
	{
		//ofmsg("Loading shader file %1%", %name);
	    shader->loadShaderSourceFromFile(path);
	}
	else
	{
		ofwarn("Could not find shader file %1%", %name);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Program* SceneManager::getProgram(const String& name, const String& vertexShaderName, const String& fragmentShaderName)
{
	// If program has been loaded already return it.
	if(myPrograms.find(name) != myPrograms.end())
	{
		return myPrograms[name];
	}

	osg::Program* prog = new osg::Program();
	prog->setName(name);

	osg::Shader* vs = new osg::Shader( osg::Shader::VERTEX );
	osg::Shader* fs = new osg::Shader( osg::Shader::FRAGMENT );
	prog->addShader(vs);
	prog->addShader(fs);

	// Load shader sources.
	loadShader(vs, vertexShaderName);
	loadShader(fs, fragmentShaderName);

	myPrograms[name] = prog;

	return prog;
}
