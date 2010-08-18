/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Test executable
 *********************************************************************************************************************/
#ifndef __NIGHTFIELD_APPLICATION_H__
#define __NIGHTFIELD_APPLICATION_H__

#include "NightfieldUI.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Settings
{
	Settings():
		numAgents(3000),
		totGroups(2),
		areaMin(Vector3f(-0.5, 0.5, -1.0)),
		areaMax(Vector3f(0.8, 2.0, -3.0)),
		center(0.2f, 1.5f, -2)
		{}

	int totGroups;
	int numAgents;
	Vector3f areaMin;
	Vector3f areaMax;
	Vector3f center;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Agent
{
	float x, y, z; // Position
	float vx, vy, vz; // Velocity
	float s1, s2; // State values
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct InteractorRay
{
	float x, y, z; // Ray origin
	float dx, dy, dz; // Ray direction
	float s1, s2; // Additional values for alignment, I can use them to store additional properties about the ray.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NightfieldApplication: public Application
{
public:
	NightfieldApplication();
	virtual void initialize();
	virtual void initializeWindow();
	void draw3D(const DrawContext& context);
	void draw2D(const DrawContext& context);
	virtual void draw(const DrawContext& context);
	virtual void update(const UpdateContext& context);
	virtual bool handleEvent(const InputEvent& evt);

private:
	static const int MaxInteractors = 32;

	// Application settings.
	Settings mySettings;

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

	// Interactors.
	InteractorRay myInteractorData[MaxInteractors];

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
	float myTouchX[MaxInteractors];
	float myTouchY[MaxInteractors];
	int myNumTouches;
};

#endif