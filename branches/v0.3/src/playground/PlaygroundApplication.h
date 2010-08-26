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
#ifndef __PLAYGROUND_APPLICATION_H__
#define __PLAYGROUND_APPLICATION_H__

#include "PlaygroundUI.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct InteractorRay
{
	float x, y, z; // Ray origin
	float dx, dy, dz; // Ray direction
	float s1, s2; // Additional values for alignment, I can use them to store additional properties about the ray.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PlaygroundServer: public ApplicationServer
{
public:
	static const int MaxInteractors = 32;

public:
	PlaygroundServer(Application* app): ApplicationServer(app) {}

	virtual void initialize();
	virtual void update(const UpdateContext& context);
	virtual bool handleEvent(const InputEvent& evt);

private:
	// Interactors.
	InteractorRay myInteractorData[MaxInteractors];

	bool myRotate;
	float myRotateX;
	float myRotateY;

	float myMouseX;
	float myMouseY;
	float myLastMouseX;
	float myLastMouseY;

	// Touch info
	float myTouchX[MaxInteractors];
	float myTouchY[MaxInteractors];
	int myNumTouches;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PlaygroundClient: public ApplicationClient
{
public:
	PlaygroundClient(Application* app): ApplicationClient(app), myGpu(NULL) {}

	virtual void initialize();
	virtual void update(const UpdateContext& context);
	virtual void draw(const DrawContext& context);

private:
	GpuManager* myGpu;
	// Interactors.
	InteractorRay myInteractorData[PlaygroundServer::MaxInteractors];

	// Textures
	TextureManager* myTexMng;
	Texture* myGlowTexture;

	// User interface stuff.
	FontManager* myFontMng;
	PlaygroundUI* myUI;

	// Touch info
	float myTouchX[PlaygroundServer::MaxInteractors];
	float myTouchY[PlaygroundServer::MaxInteractors];
	int myNumTouches;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PlaygroundApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new PlaygroundClient(this); }
	virtual ApplicationServer* createServer() { return new PlaygroundServer(this); }
};

#endif