/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Nightfield is an application that tests most of omegalib & omega features.
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
	GpuProgram* myAgentBehavior;
	GpuProgram* myAgentUpdate;
	GpuProgram* myAgentRenderer;

	// Gpu data
	VertexBuffer* myAgentBuffer;
	GpuConstant* myDt;
	GpuConstant* myNumAgents;
	GpuConstant* myGroupId;
	GpuConstant* myTotGroups;
	GpuConstant* myCenter;
	GpuBuffer* myInteractorBuffer;
	GpuConstant* myNumInteractors;
	GpuConstant* myLightPos;
	// Gpu simulation constants
	GpuConstant* myAvoidanceDist;
	GpuConstant* myCoordinationDist;
	GpuConstant* myFriction;

	// Interactors.
	InteractorRay myInteractorData[NightfieldServer::MaxInteractors];

	// Textures
	TextureManager* myTexMng;
	Texture* myGlowTexture;

	// User interface stuff.
	FontManager* myFontMng;
	NightfieldUI* myUI;

	bool myRotate;
	float myRotateX;
	float myRotateY;

	float myMouseX;
	float myMouseY;
	float myLastMouseX;
	float myLastMouseY;

	// Touch info
	float myTouchX[NightfieldServer::MaxInteractors];
	float myTouchY[NightfieldServer::MaxInteractors];
	int myNumTouches;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NightfieldApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new NightfieldClient(this); }
	virtual ApplicationServer* createServer() { return new NightfieldServer(this); }
};

#endif