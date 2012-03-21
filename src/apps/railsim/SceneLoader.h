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
#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <oengine.h>
#include <oosg.h>

#include <osg/Group>

#include "SceneManager.h"


using namespace omega;
using namespace oengine;
using namespace oosg;
using namespace osg;

namespace cyclops {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class SceneLoader
	{
	public:
		SceneLoader(TiXmlDocument& doc);

		// Loader interface
		void startLoading(SceneManager* sm);
		bool isLoadingComplete();
		int getLoadProgress();
		void loadStep();

		void loadAssets(TiXmlElement* xStaticObjectFiles, SceneManager::AssetType type);

		void createObjects(osg::Group* root, TiXmlElement* xStaticObjectFiles);
		void createPrimitives(osg::Group* root, TiXmlElement* xStaticObjectFiles);

		Vector3f readVector3f(TiXmlElement* elem, const String& attributeName);
		Vector2f readVector2f(TiXmlElement* elem, const String& attributeName);

		void initShading();

	private:
		osg::Node* createPlane(TiXmlElement* xPlane);

	private:
		TiXmlDocument& myDoc;
		SceneManager* mySceneManager;
	};
};

