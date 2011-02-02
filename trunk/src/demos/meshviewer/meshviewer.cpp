/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#include "meshviewer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::initialize()
{
	myFontMng = new FontManager();
	myTexMng = new TextureManager();

	myUI = new MeshViewerUI();
	myUI->initialize(this);

	myGpu = new GpuManager();
	myGpu->initialize();

	mySceneManager = new SceneManager(myGpu);
	myEffectManager = new EffectManager(myGpu);
	myMeshManager = new MeshManager(myGpu, myEffectManager);

	myMeshManager->loadMesh("screwdriver", "../../data/meshes/screwdriver.ply", MeshManager::MeshFormatPly);
	myMeshManager->loadMesh("arm", "../../data/meshes/rockerArm.ply", MeshManager::MeshFormatPly);

	Mesh* mesh1 = myMeshManager->getMesh("screwdriver");
	Mesh* mesh2 = myMeshManager->getMesh("arm");

	mySceneManager->initialize();

	addEntity(mesh1, Vector3f(0, 0.1, 0.0f));
	addEntity(mesh1, Vector3f(0, -0.1, 0.0f));
	addEntity(mesh2, Vector3f(0, 0, 0.0f));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::addEntity(Mesh* m, const Vector3f& position)
{
	Entity* e = new Entity(mySceneManager, m, position);
	myEntities.push_back(e);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MeshViewerClient::handleEvent(const InputEvent& evt)
{
	switch(evt.serviceType)
	{
	case InputService::Pointer:
		if(evt.type == InputEvent::Down)
		{
			// Select objects.
			Ray ray = GfxUtils::getViewRay(Vector2f(evt.position[0], evt.position[1]));

			VectorIterator<std::list<Entity*> > it(myEntities.begin(), myEntities.end());
			while(it.hasMoreElements())
			{
				Entity* e = it.getNext();
				Vector3f handlePos;
				if(e->hit(ray, &handlePos))
				{
					e->activate(handlePos);
					if(myActiveEntity != NULL) myActiveEntity->deactivate();
					myActiveEntity = e;
					break;
				}
			}
		}
		else if(evt.type == InputEvent::Up)
		{
			// Deselect objects.
			if(myActiveEntity != NULL)
			{
				myActiveEntity->deactivate();
			}
			myActiveEntity = NULL;
		}
		else if(evt.type == InputEvent::Move)
		{
			// Manipulate object, if one is active.
			if(myActiveEntity != NULL)
			{
				Ray ray = GfxUtils::getViewRay(Vector2f(evt.position[0], evt.position[1]));

				if((evt.flags & InputEvent::Left) == InputEvent::Left)
				{
					myActiveEntity->manipulate(Entity::Move, ray);
				}
				else if((evt.flags & InputEvent::Right) == InputEvent::Right)
				{
					myActiveEntity->manipulate(Entity::Rotate, ray);
				}
				else if((evt.flags & InputEvent::Middle) == InputEvent::Middle)
				{
					myActiveEntity->manipulate(Entity::Scale, ray);
				}

			}
		}
		break;
	case InputService::Touch:
	break;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::handleUIEvent(const UIEvent& evt)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::update(const UpdateContext& context)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::draw(const DrawContext& context)
{
	if(context.layer == 1)
	{
		// Draw UI.
	}
	else
	{
		// We don't use lighting for this application.
		//glDisable(GL_FOG);

		//GfxUtils::setLightingEnabled(false);
		//GfxUtils::setLightEnabled(0, true);
		//GfxUtils::setLightColor(0, Color(1.0, 1.0, 1.0));
		//GfxUtils::setLightPosition(0, Vector3f(0, 1, 0));

		glEnable(GL_DEPTH_TEST);

		mySceneManager->draw();

		//glEnable(GL_TEXTURE_2D);
		//glActiveTexture(GL_TEXTURE0);

		//glDisable(GL_BLEND);
		//glEnable(GL_DEPTH_TEST);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
void main(int argc, char** argv)
{
	MeshViewerApplication app;
	omain(app, argv[1], "meshviewer-log.txt");
}
