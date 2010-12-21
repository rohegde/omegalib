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

using namespace omega;
using namespace omega::scene;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerClient: public ApplicationClient, IUIEventHandler
{
public:
	MeshViewerClient(Application* app): ApplicationClient(app), myGpu(NULL) {}

	virtual void initialize();
	virtual bool handleEvent(const InputEvent& evt);
	virtual void handleUIEvent(const UIEvent& evt);
	virtual void update(const UpdateContext& context);
	virtual void draw(const DrawContext& context);

private:
	// Managers
	GpuManager* myGpu;
	TextureManager* myTexMng;
	FontManager* myFontMng;
	SceneManager* mySceneManager;
	UIManager myUI;

	SimplePrimitive* myTestDrawable;

	int mouseX;
	int mouseY;
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

	myTestDrawable = new SimplePrimitive();
	myTestDrawable->setPrimitiveType(SimplePrimitive::SolidTeapot);
	myTestDrawable->setEffect(new Effect());
	myTestDrawable->getEffect()->setColor(0.3, 0.8, 0.3);

	mySceneManager->getRootNode()->addDrawable(myTestDrawable);
	mySceneManager->getRootNode()->setScale(0.1f);

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
		mouseX = evt.position.x();
		mouseY = evt.position.y();
		//if(evt.type == InputEvent::Down)
		//{
		//	myRotate = true;
		//}
		//else if(evt.type == InputEvent::Up)
		//{
		//	myRotate = false;
		//}
		break;
	case InputService::Touch:
		//myTouchX[myNumTouches] = evt.position.x();
		//myTouchY[myNumTouches] = evt.position.y();
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
	mySceneManager->getRootNode()->setRotation(mouseX, mouseY, 0);
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
		glEnable(GL_LIGHTING);
		//glDisable(GL_FOG);

		GfxUtils::setLightingEnabled(true);
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
	ologopen("meshviewer-log.txt");

	Config* cfg = new Config(argv[1]);

	SystemManager* sys = SystemManager::instance();
	sys->setup(cfg);

	MeshViewerApplication app;
	sys->setApplication(&app);
	sys->initialize();
	sys->run();

	sys->cleanup();

	ologclose();
}
