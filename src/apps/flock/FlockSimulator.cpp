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
#include "FlockSimulator.h"

bool sUseOpenCL = true;

///////////////////////////////////////////////////////////////////////////////////////////////////
FlockSettings::FlockSettings():
    numAgents(200),
    areaMin(Vector3f(-1.0f, 0.0f, -1.4f)),
    areaMax(Vector3f(1.0f, 2.0f, -2.6f)),
    center(0.0f, 2.0f, 0.0f),

    minAvoidanceDist(0), maxAvoidanceDist(1),
    minCoordinationDist(0), maxCoordinationDist(1),
    minFriction(0), maxFriction(1)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockSettings::load(const Setting& s)
{
    const Setting& sPreset = s["preset"];

    Preset* p = new Preset();
    loadPreset(p, sPreset);
    presets.push_back(p);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockSettings::loadPreset(Preset* p, const Setting& s)
{
    p->friction = s["friction"];
    p->avoidanceDist = s["avoidanceDist"];
    p->coordinationDist = s["coordinationDist"];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockSimulator::setup(FlockSettings* settings)
{
	mySettings = settings;
	myCurrentPreset = mySettings->presets[0];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockSimulator::initialize()
{
	// Setup the agent buffer.
	myAgents = new Agent[mySettings->numAgents];

	float minx = mySettings->areaMin[0];
	float miny = mySettings->areaMin[1];
	float minz = mySettings->areaMin[2];

	float dx = mySettings->areaMax[0] - mySettings->areaMin[0];
	float dy = mySettings->areaMax[1] - mySettings->areaMin[1];
	float dz = mySettings->areaMax[2] - mySettings->areaMin[2];

	// Place the agents at initial random positions.
	for(int i = 0; i < mySettings->numAgents; i++)
	{
		myAgents[i].x = minx + Math::rangeRandom(0, 1) * dx;
		myAgents[i].y = miny + Math::rangeRandom(0, 1) * dy;
		myAgents[i].z = minz + Math::rangeRandom(0, 1) * dz;
		myAgents[i].vx = 0;
		myAgents[i].vy = 0;
		myAgents[i].vz = 0;
	}

	if(sUseOpenCL) initializeCL();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockSimulator::initializeCL()
{
	// Create the compute gpu.
	myComputeGpu = new GpuManager();
	myComputeGpu->setInitFlags(GpuManager::InitCL);
	myComputeGpu->initialize();

	myAgentComputeBuffer = new VertexBuffer(myComputeGpu);
	int bufSize = mySettings->numAgents * sizeof(Agent);
	myAgentComputeBuffer->initialize(bufSize, sizeof(Agent), myAgents);

	Vector<String> shaderNames;
	shaderNames.push_back("behavior");
	shaderNames.push_back("update");
	myComputeGpu->loadComputeShaders("shaders/agentsim.cl", shaderNames);

	myCenter = new GpuConstant();
	myCenter->setFloatValue(mySettings->center[0], mySettings->center[1], mySettings->center[2], 0);

	myDt = new GpuConstant();
	myAvoidanceDist = new GpuConstant();
	myCoordinationDist = new GpuConstant();
	myFriction = new GpuConstant();

	// Create a native OpenCL buffer storing interactor information.
	myInteractorBuffer = new GpuBuffer(myComputeGpu);
	myInteractorBuffer->initialize(MaxAffectors * sizeof(FlockAffector), sizeof(FlockAffector), NULL);

	myNumInteractors = new GpuConstant();
	myNumInteractors->setIntValue(0);

	myNumAgents = new GpuConstant();
	myNumAgents->setIntValue(mySettings->numAgents);

	// Setup data and parameters for the agent behavior program
	myAgentBehavior = new GpuProgram(myComputeGpu);
	myAgentBehavior->setComputeShader(myComputeGpu->getComputeShader("behavior"));
	myAgentBehaviorParams.setParam(0, myAgentComputeBuffer);
	myAgentBehaviorParams.setParam(1, myDt);
	myAgentBehaviorParams.setParam(2, myCenter);
	myAgentBehaviorParams.setParam(3, myNumAgents);
	myAgentBehaviorParams.setParam(4, myNumInteractors);
	myAgentBehaviorParams.setParam(5, myInteractorBuffer);
	myAgentBehaviorParams.setParam(6, myAvoidanceDist);
	myAgentBehaviorParams.setParam(7, myCoordinationDist);
	myAgentBehaviorParams.setParam(8, myFriction);
	myAgentBehaviorOptions.dimensions = 1;
	myAgentBehaviorOptions.localThreads[0] = 100;
	myAgentBehaviorOptions.globalThreads[0] = mySettings->numAgents;

	// Setup data and parameters for the agent update program
	myAgentUpdate = new GpuProgram(myComputeGpu);
	myAgentUpdate->setComputeShader(myComputeGpu->getComputeShader("update"));
	myAgentUpdateParams.setParam(0, myAgentComputeBuffer);
	myAgentUpdateParams.setParam(1, myDt);
	myAgentUpdateOptions.dimensions = 1;
	myAgentUpdateOptions.localThreads[0] = 100;
	myAgentUpdateOptions.globalThreads[0] = mySettings->numAgents;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockSimulator::update(const UpdateContext& context)
{
	if(sUseOpenCL)
	{
		// Default path: run the flock simulation on the GPU
		updateAgentsGPU(context);
	}
	else
	{
		// Fallback path: run a simplified simulation on the CPU.
		updateAgentsCPU(context);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockSimulator::updateAgentsGPU(const UpdateContext& context)
{
	// Write the interactor buffer, storing positions and other parameters of 3d objects
	// influencing the flock.
	myNumInteractors->setIntValue(myActiveAffectors);
	myInteractorBuffer->write(myAffectorData, 0, MaxAffectors * sizeof(FlockAffector));

	// Update simulation parameters.
	myDt->setFloatValue(context.dt);
	myCoordinationDist->setFloatValue(myCurrentPreset->coordinationDist);
	myAvoidanceDist->setFloatValue(myCurrentPreset->avoidanceDist);
	myFriction->setFloatValue(myCurrentPreset->friction);
	myCenter->setFloatValue(mySettings->center[0], mySettings->center[1], mySettings->center[2], 0);

	// Run the simulation
	myAgentBehavior->runComputeStage(myAgentBehaviorOptions, &myAgentBehaviorParams);
	myAgentUpdate->runComputeStage(myAgentUpdateOptions, &myAgentUpdateParams);
	
	// Read back data from the compute buffer.
	myAgentComputeBuffer->read(myAgents, 0, mySettings->numAgents * sizeof(Agent));
}


///////////////////////////////////////////////////////////////////////////////////////////////////// 
// Fallback code to simulate the flock on the cpu.
void FlockSimulator::updateAgentsCPU(const UpdateContext& context)
{
	float scale = 0.5f;
	Vector3f center = Vector3f::Zero();
	// Compute behavior
	for(int i = 0; i < mySettings->numAgents; i++)
	{
		// Compute attraction vector.
		Vector3f pos(myAgents[i].x, myAgents[i].y, myAgents[i].z);
		Vector3f vel(myAgents[i].vx, myAgents[i].vy, myAgents[i].vz);

		Vector3f attractDir =  center - pos;
		attractDir.normalize();
	
		// Compute avoidance vector.
		Vector3f avoidDir(0, 0, 0);
		Vector3f coordDir(0, 0, 0);
		for(int j = 0; j < mySettings->numAgents; j++)
		{
			if(j != i)
			{
				Vector3f pos2(myAgents[j].x, myAgents[j].y, myAgents[j].z);
				Vector3f dv = pos2 - pos;
				float l = dv.norm();
				if(l < myCurrentPreset->avoidanceDist)
				{
					dv.normalize();
					avoidDir -= dv;
				}
				if(l < myCurrentPreset->coordinationDist)
				{
					coordDir += vel;
				}
			}
		}
	
		if(coordDir.norm() > 0) coordDir.normalize();
		Vector3f dir = attractDir + avoidDir + coordDir;
		dir.normalize();
	
		int friction = myCurrentPreset->friction / (context.dt * scale);
		vel = (vel * friction + dir) / (friction + 1);
		//vel = dir;
		if(vel.norm() > 0) vel.normalize();

		myAgents[i].vx = vel[0];
		myAgents[i].vy = vel[1];
		myAgents[i].vz = vel[2];
	}
	// Update position
	for(int i = 0; i < mySettings->numAgents; i++)
	{
		myAgents[i].x += myAgents[i].vx * context.dt * scale;
		myAgents[i].y += myAgents[i].vy * context.dt * scale;
		myAgents[i].z += myAgents[i].vz * context.dt* scale;
	}
}

