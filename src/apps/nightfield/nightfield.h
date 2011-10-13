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
#ifndef __NIGHTFIELD_H__
#define __NIGHTFIELD_H__

#include "omega.h"
#include "oengine.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Preset
{
	float avoidanceDist;
	float friction;
	float coordinationDist;
	bool useFog;
	bool useAdditiveAlpha;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Settings
{
	Settings();

	float minAvoidanceDist;
	float maxAvoidanceDist;
	float minCoordinationDist;
	float maxCoordinationDist;
	float minFriction;
	float maxFriction;
	int totGroups;
	int numAgents;
	Vector3f areaMin;
	Vector3f areaMax;
	Vector3f center;

	std::vector<Preset> presets;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Agent
{
	float x, y, z; // Position
	float vx, vy, vz; // Velocity
	float s1, s2; // State values
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct InteractorRay
{
	float x, y, z; // Ray origin
	float dx, dy, dz; // Ray direction
	float s1, s2; // Additional values for alignment, I can use them to store additional properties about the ray.
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class NightfieldClient: public ApplicationClient
{
public:
	static const int MaxInteractors = 32;

public:
	NightfieldClient(ApplicationServer* server): ApplicationClient(server) {}

	virtual void initialize();
	void initializeCL();
	void initializeGL();
	virtual void update(const UpdateContext& context);
	virtual void draw(const DrawContext& context);
	void updateAgentsCPU(const UpdateContext& context);
	void updateAgentsGPU(const UpdateContext& context);
	//virtual bool handleEvent(const Event& evt, DrawContext& context);
	//virtual bool handleEvent(const Event& evt, UpdateContext& context);

private:
	// Application settings.
	Settings mySettings;

	// Gpu used for computing
	GpuManager* myComputeGpu;
	VertexBuffer* myAgentComputeBuffer;

	// Current simulation preset;
	Preset myCurrentPreset;

	GpuProgram* myAgentBehavior;
	GpuProgram* myAgentUpdate;
	GpuProgram* myAgentRenderer;

	// Agent CPU buffer
	Agent* myAgents;

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
	
	GpuProgramParams myAgentUpdateParams;
	GpuProgramParams myAgentBehaviorParams;
	GpuProgramParams myAgentRenderParams;
	ComputeStageOptions myAgentUpdateOptions;
	ComputeStageOptions myAgentBehaviorOptions;
	RenderStageOptions myAgentRenderOptions;

	// Interactors.
	InteractorRay myInteractorData[MaxInteractors];

	Texture* myGlowTexture;

	// User interface stuff.
	//FontManager* myFontMng;
	//NightfieldUI* myUI;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NightfieldApplication: public Application
{
public:
	virtual ApplicationClient* createClient(ApplicationServer* server) { return new NightfieldClient(server); }
};

#endif