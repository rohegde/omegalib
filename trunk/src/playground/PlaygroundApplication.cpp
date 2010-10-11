/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Playground is a simple application designed to test event and gesture handling in a multithreaded / distributed 
 * application.
 *********************************************************************************************************************/
#include "PlaygroundApplication.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaygroundServer::initialize()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaygroundServer::update(const UpdateContext& context)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PlaygroundServer::handleEvent(const InputEvent& evt)
{
/*
	if(!myUI->handleEvent(evt))
	{
		switch(evt.serviceType)
		{
		case InputService::Pointer:
			myMouseX = evt.position.x();
			myMouseY = evt.position.y();
			if(evt.type == InputEvent::Down)
			{
				myRotate = true;
			}
			else if(evt.type == InputEvent::Up)
			{
				myRotate = false;
			}
			break;
		case InputService::Touch:
			myTouchX[myNumTouches] = evt.position.x();
			myTouchY[myNumTouches] = evt.position.y();
			myNumTouches++;
		break;

		case InputService::Mocap:
		{
		}
		}
		return true;
	}
*/
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaygroundClient::initialize()
{
	myFontMng = new FontManager();

	myTexMng = new TextureManager();
	myTexMng->loadTexture("glow", "../../data/images/glow2.png");

	myUI = new PlaygroundUI(myTexMng, myFontMng);
	myUI->initialize();

	myGpu = new GpuManager();
	myGpu->initialize();

	// Setup data and parameters for the agent render program
	//myAgentRenderer = new GpuProgram(myGpu);
	//myAgentRenderer->initialize();

	myNumTouches = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaygroundClient::update(const UpdateContext& context)
{
	myUI->update(context);

	//if(myRotate)
	//{
	//	myRotateX += (myMouseX - myLastMouseX);
	//	myRotateY += (myMouseY - myLastMouseY);
	//	//myLightPos->setFloatValue(myMouseX / context.viewportWidth, myMouseY / context.viewportHeight);
	//}

	for(int j = 0; j < myNumTouches; j++)
	{
		Vector3f mouseRayOrigin;
		Vector3f mouseRayDirection;
		GfxUtils::getViewRay(myTouchX[j], myTouchY[j], &mouseRayOrigin, &mouseRayDirection);

		myInteractorData[j].x = mouseRayOrigin[0];
		myInteractorData[j].y = mouseRayOrigin[1];
		myInteractorData[j].z = mouseRayOrigin[2];
		myInteractorData[j].dx = mouseRayDirection[0];
		myInteractorData[j].dy = mouseRayDirection[1];
		myInteractorData[j].dz = mouseRayDirection[2];
	}

	myNumTouches = 0;

	//myLastMouseX = myMouseX;
	//myLastMouseY = myMouseY;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaygroundClient::draw(const DrawContext& context)
{
	if(context.layer == 1)
	{
		// Draw UI.
	}
	else
	{
		// We don't use lighting for this application.
		//glDisable(GL_LIGHTING);
		//glDisable(GL_FOG);

		//glColor3f(1.0, 1.0, 1.0);

		GfxUtils::beginOverlayMode(context);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		GfxUtils::drawVGradient(Vector2i(0, context.viewportHeight - 100), Vector2i(context.viewportWidth, 100), Color(0, 0, 0), Color(50, 50, 60), 0.3);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		GfxUtils::endOverlayMode();

		//glEnable(GL_TEXTURE_2D);
		//glEnable(GL_POINT_SPRITE);
		//glActiveTexture(GL_TEXTURE0);

		//glDisable(GL_BLEND);
		//glEnable(GL_DEPTH_TEST);
	}
}
