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
#ifndef __FLOCK_H__
#define __FLOCK_H__

#include <omega.h>
#include <omegaToolkit.h>

using namespace omega;
using namespace omegaToolkit;

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Preset
{
	float avoidanceDist;
	float friction;
	float coordinationDist;
	bool useAdditiveAlpha;
	String flockImage;
	bool useFog;
	bool drawSpeedVectors;
	float speedVectorScale;
	Color speedVectorColor;
	String fragmentShader;
	bool hasFragmentShader;
	float pointSize;
	int trailSize;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Settings
{
	Settings();
	void load(const Setting& s);
	void loadPreset(Preset* p, const Setting& s);

	float minAvoidanceDist;
	float maxAvoidanceDist;
	float minCoordinationDist;
	float maxCoordinationDist;
	float minFriction;
	float maxFriction;
	int numAgents;
	Vector3f areaMin;
	Vector3f areaMax;
	Vector3f center;

	Vector<Preset*> presets;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Agent
{
	float x, y, z; // Position
	float vx, vy, vz; // Velocity
	float s1, s2; // State values
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct FlockAffector
{
	float x, y, z; // origin
	float rx, ry, rz; // Radii (for now we assume as sphere, so only rx is used)
	float f1, f2; // Affector parameters;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class Flock: public RenderableSceneObject
{
public:
static const int MaxAffectors = 32;

public:
	Flock() {}
	virtual Renderable* createRenderable();
	void setup(Settings* settings);

	void initialize();
	void initializeCL();
	virtual void update(const UpdateContext& context);
	void updateAgentsCPU(const UpdateContext& context);
	void updateAgentsGPU(const UpdateContext& context);

	const AlignedBox3* getBoundingBox();
	bool hasBoundingBox();

	ImageData* getAgentImage() { return myAgentImage; }
	Settings* getSettings() { return mySettings; }
	Preset* getCurrentPreset() 
	{ 
		return myCurrentPreset; 
	}

	FlockAffector* getAffector(int index) { return &myAffectorData[index]; }
	void setActiveAffectors(int value) { myActiveAffectors = value; }
	int getActiveAffectors() { return myActiveAffectors; }

	Agent* getAgents() { return myAgents; }

private:
	// Application settings.
	Settings* mySettings;
	Preset* myCurrentPreset;

	AlignedBox3 myBoundingBox;

	ImageData* myAgentImage;

	// Gpu used for computing
	GpuManager* myComputeGpu;
	VertexBuffer* myAgentComputeBuffer;

	// Current simulation preset;

	GpuProgram* myAgentBehavior;
	GpuProgram* myAgentUpdate;

	// Agent CPU buffer
	Agent* myAgents;

	// Gpu data
	GpuConstant* myDt;
	GpuConstant* myNumAgents;
	GpuConstant* myCenter;
	GpuBuffer* myInteractorBuffer;
	GpuConstant* myNumInteractors;
	// Gpu simulation constants
	GpuConstant* myAvoidanceDist;
	GpuConstant* myCoordinationDist;
	GpuConstant* myFriction;
	
	GpuProgramParams myAgentUpdateParams;
	GpuProgramParams myAgentBehaviorParams;
	ComputeStageOptions myAgentUpdateOptions;
	ComputeStageOptions myAgentBehaviorOptions;

	// Affectors.
	int myActiveAffectors;
	FlockAffector myAffectorData[MaxAffectors];
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class FlockRenderable: public SceneRenderable
{
public:
	FlockRenderable(Flock* owner): myOwner(owner) {}

	virtual void initialize();
	virtual void dispose();
	virtual void refresh();
	virtual void draw(RenderState* state);
	virtual void drawTrails(RenderState* state);
	virtual void drawPoints(RenderState* state);

private:
	Flock* myOwner;

	GpuProgram* myAgentRenderer;

	// Gpu data
	VertexBuffer* myAgentBuffer;
	GpuConstant* myLightPos;
	
	GpuProgramParams myAgentRenderParams;
	RenderStageOptions myAgentRenderOptions;

	Texture* myAgentTexture;

	// Trail data.
	float* myTrailData;
	int myTrailDataSize;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
inline const AlignedBox3* Flock::getBoundingBox()
{
	return &myBoundingBox;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Flock::hasBoundingBox()
{
	return false;
}

#endif