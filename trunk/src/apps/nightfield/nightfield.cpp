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
#include "nightfield.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
Settings::Settings():
	numAgents(1000),
	totGroups(2),
	areaMin(Vector3f(-0.4, 0.7, -1.4)),
	areaMax(Vector3f(0.8, 1.9, -2.6)),
	center(0.0f, 0.0f, 0.0),

	minAvoidanceDist(0), maxAvoidanceDist(1),
	minCoordinationDist(0), maxCoordinationDist(1),
	minFriction(0), maxFriction(1)
{
	Preset p;
	p.friction = 0.1f;
	p.avoidanceDist = 0.03f;
	p.coordinationDist = 0.15f;

	presets.push_back(p);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NightfieldClient::initialize()
{
	getGpu()->setInitFlags(GpuManager::InitCL | GpuManager::InitGL);
	ApplicationClient::initialize();

	myCurrentPreset = mySettings.presets[0];

	//myFontMng = new FontManager();

	ImageData img;
	if(ImageUtils::loadImage("images/glow2.png", &img))
	{
		myGlowTexture = getGpu()->createTexture(
		img.filename,
		img.width,
		img.height,
		img.data);
	}


	myAgentBuffer = NULL;

	Vector<String> shaderNames;
	shaderNames.push_back("behavior");
	shaderNames.push_back("update");
	getGpu()->loadComputeShaders("shaders/agentsim.cl", shaderNames);
	getGpu()->loadFragmentShader("smoke", "shaders/smoke.frag");

	// Setup the agent buffer.
	Agent* agentData = new Agent[mySettings.numAgents];

	float minx = mySettings.areaMin[0];
	float miny = mySettings.areaMin[1];
	float minz = mySettings.areaMin[2];

	float dx = mySettings.areaMax[0] - mySettings.areaMin[0];
	float dy = mySettings.areaMax[1] - mySettings.areaMin[1];
	float dz = mySettings.areaMax[2] - mySettings.areaMin[2];

	for(int i = 0; i < mySettings.numAgents; i++)
	{
		agentData[i].x = minx + Math::rangeRandom(0, 1) * dx;
		agentData[i].y = miny + Math::rangeRandom(0, 1) * dy;
		agentData[i].z = minz + Math::rangeRandom(0, 1) * dz;
		agentData[i].vx = 0;
		agentData[i].vy = 0;
		agentData[i].vz = 0;
	}

	// Create the gpu buffers and constants.
	int bufSize = mySettings.numAgents * sizeof(Agent);

	myAgentBuffer = new VertexBuffer(getGpu());
	myAgentBuffer->addAttribute(VertexAttribute(VertexAttribute::TargetPosition, VertexAttribute::TypeFloat, 0, 3));
	myAgentBuffer->initialize(bufSize, sizeof(Agent), agentData);

	delete[] agentData;

	myDt = new GpuConstant();

	myNumAgents = new GpuConstant();
	myNumAgents->setIntValue(mySettings.numAgents);

	myGroupId = new GpuConstant();

	myTotGroups = new GpuConstant();
	myTotGroups->setIntValue(mySettings.totGroups);

	// Create a native OpenCL buffer storing interactor information.
	myInteractorBuffer = new GpuBuffer(getGpu());
	myInteractorBuffer->initialize(MaxInteractors * sizeof(InteractorRay), sizeof(InteractorRay), NULL, GpuBuffer::BufferFlagsCLNative);

	myNumInteractors = new GpuConstant();
	myNumInteractors->setIntValue(0);

	myCenter = new GpuConstant();
	myCenter->setFloatValue(mySettings.center[0], mySettings.center[1], mySettings.center[2], 0);

	myLightPos = new GpuConstant();
	myLightPos->setName("lightpos");
	myLightPos->setFloatValue(1.0f, 1.0f);

	myAvoidanceDist = new GpuConstant();
	myCoordinationDist = new GpuConstant();
	myFriction = new GpuConstant();

	// Setup data and parameters for the agent behavior program
	myAgentBehavior = new GpuProgram(getGpu());
	myAgentBehavior->setComputeShader(getGpu()->getComputeShader("behavior"));
	myAgentBehaviorParams.setParam(0, myAgentBuffer);
	myAgentBehaviorParams.setParam(1, myDt);
	myAgentBehaviorParams.setParam(2, myCenter);
	myAgentBehaviorParams.setParam(3, myNumAgents);
	myAgentBehaviorParams.setParam(4, myTotGroups);
	myAgentBehaviorParams.setParam(5, myGroupId);
	myAgentBehaviorParams.setParam(6, myNumInteractors);
	myAgentBehaviorParams.setParam(7, myInteractorBuffer);
	myAgentBehaviorParams.setParam(8, myAvoidanceDist);
	myAgentBehaviorParams.setParam(9, myCoordinationDist);
	myAgentBehaviorParams.setParam(10, myFriction);
	myAgentBehaviorOptions.dimensions = 1;
	myAgentBehaviorOptions.localThreads[0] = 100;
	myAgentBehaviorOptions.globalThreads[0] = mySettings.numAgents / mySettings.totGroups;

	// Setup data and parameters for the agent update program
	myAgentUpdate = new GpuProgram(getGpu());
	myAgentUpdate->setComputeShader(getGpu()->getComputeShader("update"));
	myAgentUpdateParams.setParam(0, myAgentBuffer);
	myAgentUpdateParams.setParam(1, myDt);
	myAgentUpdateOptions.dimensions = 1;
	myAgentUpdateOptions.localThreads[0] = 100;
	myAgentUpdateOptions.globalThreads[0] = mySettings.numAgents;

	// Setup data and parameters for the agent render program
	myAgentRenderer = new GpuProgram(getGpu());
	//myAgentRenderer->setFragmentShader(myGpu->getFragmentShader("smoke"));
	myAgentRenderParams.setParam(0, myAgentBuffer);
	//myAgentRenderer->setInput(1, myLightPos);
	//myAgentRenderer->setNumRenderItems(mySettings.numAgents);
	myAgentRenderer->initialize();
	myAgentRenderOptions.items = mySettings.numAgents;
	myAgentRenderOptions.primType = RenderStageOptions::PrimPoints;

	//myNumTouches = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NightfieldClient::update(const UpdateContext& context)
{
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
	//myNumInteractors->setIntValue(myNumTouches);
	//myInteractorBuffer->setData(myInteractorData);

	myNumInteractors->setIntValue(0);
	//myInteractorBuffer->setData(myInteractorData);

	myNumInteractors->setIntValue(0);
	//myNumTouches = 0;

	myDt->setFloatValue(0.01);

	static int groupId = 0;
	myGroupId->setIntValue(groupId);
	groupId++;
	if(groupId == mySettings.totGroups) groupId = 0;

	// Set simulation parameters.
	myCoordinationDist->setFloatValue(myCurrentPreset.coordinationDist);
	myAvoidanceDist->setFloatValue(myCurrentPreset.avoidanceDist);
	myFriction->setFloatValue(myCurrentPreset.friction);

	myAgentBehavior->runComputeStage(myAgentBehaviorOptions, &myAgentBehaviorParams);
	myAgentUpdate->runComputeStage(myAgentUpdateOptions, &myAgentUpdateParams);

	//myLastMouseX = myMouseX;
	//myLastMouseY = myMouseY;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NightfieldClient::draw(const DrawContext& context)
{
	// We don't use lighting for this application.
	glDisable(GL_LIGHTING);
	glEnable(GL_FOG);

	//if(myCurrentPreset.useFog)
	{
		//glDisable(GL_FOG);
		const float fogCol[] = { 0.6f, 0.6f, 0.8f, 0.0f };
		glFogfv( GL_FOG_COLOR, fogCol );
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, 0);
		glFogf(GL_FOG_END, 2);
	}
	//else
	{
		glEnable(GL_FOG);
	}

	glColor3f(1.0, 1.0, 1.0);

	//glTranslatef(mySettings.center[0], mySettings.center[1], mySettings.center[2]);
	//glRotatef(myRotateX / 3, 0, 1, 0);
	//glRotatef(myRotateY / 3, 1, 0, 0);
	//glTranslatef(-mySettings.center[0], -mySettings.center[1], -mySettings.center[2]);

	//GfxUtils::beginOverlayMode(context);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);

	//GfxUtils::drawVGradient(Vector2i(0, context.viewportHeight - 100), Vector2i(context.viewportWidth, 100), Color(0, 0, 0), Color(50, 50, 60), 0.3);
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_LIGHTING);
	//GfxUtils::endOverlayMode();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_POINT_SPRITE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myGlowTexture->getGLTexture());
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//int ps = context.viewportHeight < context.viewportWidth ? context.viewportHeight / 8 : context.viewportWidth / 8;
	int ps = 32;
	glPointSize(ps);

	myAgentRenderer->runRenderStage(myAgentRenderOptions, &myAgentRenderParams);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	NightfieldApplication app;

	// Read config file name from command line or use default one.
	const char* cfgName = "nightfield.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "nightfield.log", new FilesystemDataSource(OMEGA_DATA_PATH));

	return 0;
}
