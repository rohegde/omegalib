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
#include "omega.h"
#include "omega/scene.h"
#include "omega/ui.h"

#include "omega/Plane.h"
#include "omega/Ray.h"

using namespace omega;
using namespace omega::scene;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerClient: public ApplicationClient, IUIEventHandler
{
public:
	MeshViewerClient(Application* app): ApplicationClient(app), myGpu(NULL), myActiveNode(NULL) {}

	virtual void initialize();
	virtual bool handleEvent(const InputEvent& evt);
	virtual void handleUIEvent(const UIEvent& evt);
	virtual void update(const UpdateContext& context);
	virtual void draw(const DrawContext& context);

private:
	void addObject(Mesh* m, const Vector3f& position);

private:
	// Managers
	GpuManager* myGpu;
	TextureManager* myTexMng;
	FontManager* myFontMng;
	SceneManager* mySceneManager;
	MeshManager* myMeshManager;
	EffectManager* myEffectManager;
	UIManager myUI;

	// Active object.
	SceneNode* myActiveNode;

	Vector3f myStartPosition;
	bool myMoving;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new MeshViewerClient(this); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::initialize()
{
	myFontMng = new FontManager();
	myTexMng = new TextureManager();

	//myUI = new UIManager();
	//myUI->initialize();

	myGpu = new GpuManager();
	myGpu->initialize();

	mySceneManager = new SceneManager(myGpu);
	myEffectManager = new EffectManager(myGpu);
	myMeshManager = new MeshManager(myGpu, myEffectManager);

	myMeshManager->loadMesh("screwdriver", "../../data/meshes/screwdriver.ply", MeshManager::MeshFormatPly);

	Mesh* mesh = myMeshManager->getMesh("screwdriver");
	//myTestDrawable->setPrimitiveType(SimplePrimitive::SolidTeapot);
	//myTestDrawable->setEffect(new Effect());
	//myTestDrawable->getEffect()->setColor(0.3, 0.8, 0.3);

	mySceneManager->initialize();

	addObject(mesh, Vector3f(0, 0.2, -0.5f));
	addObject(mesh, Vector3f(0, -0.2, -0.5f));
	addObject(mesh, Vector3f(0, 0, -0.5f));

	//mySceneManager->getRootNode()->setScale(0.1);

	// Setup data and parameters for the agent render program
	//myAgentRenderer = new GpuProgram(myGpu);
	//myAgentRenderer->initialize();

	//myNumTouches = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MeshViewerClient::handleEvent(const InputEvent& evt)
{
	switch(evt.serviceType)
	{
	case InputService::Pointer:
		if(evt.type == InputEvent::Down)
		{
			myMoving = true;

			RaySceneQuery query(mySceneManager);

			Vector3f origin;
			Vector3f direction;
			GfxUtils::getViewRay(evt.position[0], evt.position[1], &origin, &direction);

			Ray ray(origin, direction);
			query.setRay(ray);

			const SceneQueryResultList& rl = query.execute();
			if(rl.size() > 0)
			{
				if(myActiveNode != NULL)
				{
					myActiveNode->setBoundingBoxVisible(false);
				}
				myActiveNode = rl.front().node;
				myActiveNode->setBoundingBoxVisible(true);
				myStartPosition = myActiveNode->getPosition();
			}
			else
			{
				if(myActiveNode != NULL)
				{
					myActiveNode->setBoundingBoxVisible(false);
				}
				myActiveNode = NULL;
			}

			//printf("orig: %f %f %f, dir %f %f %f numresults: %d\n", origin[0], origin[1], origin[2], 
			//	direction[0], direction[1], direction[2], rl.size());
		}
		else if(evt.type == InputEvent::Up)
		{
			if(myActiveNode != NULL)
			{
				myActiveNode->setBoundingBoxVisible(false);
			}
			myActiveNode = NULL;
		}
		else if(evt.type == InputEvent::Move)
		{
			if(myActiveNode != NULL)
			{
				Vector3f origin;
				Vector3f direction;
				GfxUtils::getViewRay(evt.position[0], evt.position[1], &origin, &direction);

				const Vector3f& oldPos = myActiveNode->getPosition();
				
				float l = (oldPos[2] - origin[2]) / direction[2];
				float tx = origin[0] + l * direction[0];
				float ty = origin[1] + l * direction[1];

				Vector3f newPos = Vector3f(tx, ty, oldPos[2]);

				myActiveNode->setPosition(newPos);
			}
		}
		//mouseX = evt.position[0] / 100;
		//mouseY = evt.position[1] / 100;
		break;
	case InputService::Touch:
		//myTouchX[myNumTouches] = evt.position.x();
		//myTouchY[myNumTouches] = evt.position.y();
	break;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::addObject(Mesh* m, const Vector3f& position)
{
	SceneNode* node = new SceneNode(mySceneManager);
	node->addDrawable(m);
	node->setPosition(position);
	node->setSelectable(true);
	mySceneManager->getRootNode()->addChild(node);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::handleUIEvent(const UIEvent& evt)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::update(const UpdateContext& context)
{
	//mySceneManager->getRootNode()->setRotation(mouseX, mouseY, 0);
	//myUI->update(context);

	//if(myRotate)
	//{
	//	myRotateX += (myMouseX - myLastMouseX);
	//	myRotateY += (myMouseY - myLastMouseY);
	//	//myLightPos->setFloatValue(myMouseX / context.viewportWidth, myMouseY / context.viewportHeight);
	//}

	//for(int j = 0; j < myNumTouches; j++)
	//{
	//	Vector3f mouseRayOrigin;
	//	Vector3f mouseRayDirection;
	//	GfxUtils::getViewRay(myTouchX[j], myTouchY[j], &mouseRayOrigin, &mouseRayDirection);

	//	myInteractorData[j].x = mouseRayOrigin[0];
	//	myInteractorData[j].y = mouseRayOrigin[1];
	//	myInteractorData[j].z = mouseRayOrigin[2];
	//	myInteractorData[j].dx = mouseRayDirection[0];
	//	myInteractorData[j].dy = mouseRayDirection[1];
	//	myInteractorData[j].dz = mouseRayDirection[2];
	//}

	//myNumTouches = 0;

	//myLastMouseX = myMouseX;
	//myLastMouseY = myMouseY;
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

		GfxUtils::setLightingEnabled(false);
		GfxUtils::setLightEnabled(0, true);
		GfxUtils::setLightColor(0, Color(1.0, 1.0, 1.0));
		GfxUtils::setLightPosition(0, Vector3f(0, 1, 0));

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
