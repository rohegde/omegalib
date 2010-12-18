/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 *********************************************************************************************************************/
#include "MeshViewerApplication.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::initialize()
{
	myFontMng = new FontManager();

	myTexMng = new TextureManager();

	myUI = new MeshViewerUI(myTexMng, myFontMng);
	myUI->initialize();

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
void MeshViewerClient::update(const UpdateContext& context)
{
	mySceneManager->getRootNode()->setRotation(mouseX, mouseY, 0);
	myUI->update(context);

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
