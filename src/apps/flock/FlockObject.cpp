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
#include "FlockObject.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
FlockObject::FlockObject(SceneManager* scene, FlockSimulator* simulator, const String& agentModelName, const String& name):
	Entity(scene),
		mySimulator(simulator),
		myFlockRoot(NULL)
{
	oassert(simulator != NULL);

	FlockSettings* s = mySimulator->getSettings();

	ModelAsset* agentModel = scene->getModel(agentModelName);

	myFlockRoot = new osg::Group();
	myAgentNodes = new osg::PositionAttitudeTransform*[s->numAgents];
	for(int i = 0; i < s->numAgents; i++)
	{
		myAgentNodes[i] = new osg::PositionAttitudeTransform();
		if(agentModel != NULL)
		{
			myAgentNodes[i]->addChild(agentModel->nodes[0]);
		}
		myFlockRoot->addChild(myAgentNodes[i]);
	}

	if(name != "") setName(name);

	initialize(myFlockRoot);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockObject::update(const UpdateContext& context)
{
	FlockSettings* s = mySimulator->getSettings();
	Agent* agent = mySimulator->getAgents();
	for(int i = 0; i < s->numAgents; i++)
	{
		myAgentNodes[i]->setPosition(osg::Vec3d(agent[i].x, agent[i].y, agent[i].z));
		Quaternion q = Math::buildRotation(-Vector3f::UnitX(), Vector3f(agent[i].vx, agent[i].vy, agent[i].vz), Vector3f::UnitY());
		myAgentNodes[i]->setAttitude(osg::Quat(q.x(), q.y(), q.z(), q.w()));
	}
}
