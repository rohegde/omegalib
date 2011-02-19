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
#include "NightfieldApplication.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Settings::Settings():
	numAgents(3000),
	totGroups(2),
	areaMin(Vector3f(-0.4, 0.7, -1.4)),
	areaMax(Vector3f(0.8, 1.9, -2.6)),
	center(0.2f, 1.3f, -2),

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NightfieldServer::initialize()
{
	myCurrentPreset = mySettings.presets[0];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NightfieldServer::update(const UpdateContext& context)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool NightfieldServer::handleEvent(const InputEvent& evt)
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
void NightfieldClient::initialize()
{
	myCurrentPreset = mySettings.presets[0];

	myFontMng = new FontManager();

	myTexMng = new TextureManager();
	myTexMng->loadTexture("glow", "../../data/images/glow2.png");

	myGlowTexture = myTexMng->getTexture("glow");

	myUI = new NightfieldUI(myTexMng, myFontMng);
	myUI->initialize();

	myAgentBuffer = NULL;

	myRotate = false;

	myGpu = new GpuManager();
	myGpu->initialize();

	std::vector<String> shaderNames;
	shaderNames.push_back("behavior");
	shaderNames.push_back("update");
	myGpu->loadComputeShaders("../../data/shaders/agentsim.cl", shaderNames);

	myGpu->loadFragmentShader("smoke", "../../data/shaders/smoke.frag");

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
		agentData[i].x = minx + Math::rnd() * dx;
		agentData[i].y = miny + Math::rnd() * dy;
		agentData[i].z = minz + Math::rnd() * dz;
		agentData[i].vx = 0;
		agentData[i].vy = 0;
		agentData[i].vz = 0;
	}

	// Create the gpu buffers and constants.
	int bufSize = mySettings.numAgents * sizeof(Agent);

	myAgentBuffer = new VertexBuffer(myGpu);
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
	myInteractorBuffer = new GpuBuffer(myGpu);
	myInteractorBuffer->initialize(NightfieldServer::MaxInteractors * sizeof(InteractorRay), sizeof(InteractorRay), NULL, GpuBuffer::BufferFlagsCLNative);

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
	myAgentBehavior = new GpuProgram(myGpu);
	myAgentBehavior->setComputeShader(myGpu->getComputeShader("behavior"));
	myAgentBehavior->setInput(0, myAgentBuffer);
	myAgentBehavior->setInput(1, myDt);
	myAgentBehavior->setInput(2, myCenter);
	myAgentBehavior->setInput(3, myNumAgents);
	myAgentBehavior->setInput(4, myTotGroups);
	myAgentBehavior->setInput(5, myGroupId);
	myAgentBehavior->setInput(6, myNumInteractors);
	myAgentBehavior->setInput(7, myInteractorBuffer);
	myAgentBehavior->setInput(8, myAvoidanceDist);
	myAgentBehavior->setInput(9, myCoordinationDist);
	myAgentBehavior->setInput(10, myFriction);

	myAgentBehavior->setComputeDimensions(1);
	myAgentBehavior->setLocalComputeThreads(0, 100);
	myAgentBehavior->setGlobalComputeThreads(0, mySettings.numAgents / mySettings.totGroups);

	// Setup data and parameters for the agent update program
	myAgentUpdate = new GpuProgram(myGpu);
	myAgentUpdate->setComputeShader(myGpu->getComputeShader("update"));
	myAgentUpdate->setInput(0, myAgentBuffer);
	myAgentUpdate->setInput(1, myDt);
	myAgentUpdate->setComputeDimensions(1);
	myAgentUpdate->setLocalComputeThreads(0, 100);
	myAgentUpdate->setGlobalComputeThreads(0, mySettings.numAgents);
	myAgentUpdate->setNumRenderItems(mySettings.numAgents);

	// Setup data and parameters for the agent render program
	myAgentRenderer = new GpuProgram(myGpu);
	//myAgentRenderer->setFragmentShader(myGpu->getFragmentShader("smoke"));
	myAgentRenderer->setInput(0, myAgentBuffer);
	//myAgentRenderer->setInput(1, myLightPos);
	myAgentRenderer->setNumRenderItems(mySettings.numAgents);
	myAgentRenderer->initialize();

	myNumTouches = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NightfieldClient::update(const UpdateContext& context)
{
	myUI->update(context);

	if(myRotate)
	{
		myRotateX += (myMouseX - myLastMouseX);
		myRotateY += (myMouseY - myLastMouseY);
		//myLightPos->setFloatValue(myMouseX / context.viewportWidth, myMouseY / context.viewportHeight);
	}

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
	myNumInteractors->setIntValue(myNumTouches);
	myInteractorBuffer->setData(myInteractorData);

	myNumTouches = 0;

	myDt->setFloatValue(0.005f);

	static int groupId = 0;
	myGroupId->setIntValue(groupId);
	groupId++;
	if(groupId == mySettings.totGroups) groupId = 0;

	// Set simulation parameters.
	myCoordinationDist->setFloatValue(myCurrentPreset.coordinationDist);
	myAvoidanceDist->setFloatValue(myCurrentPreset.avoidanceDist);
	myFriction->setFloatValue(myCurrentPreset.friction);

	//myAgentBehavior->run();
	//myAgentUpdate->run();

	myLastMouseX = myMouseX;
	myLastMouseY = myMouseY;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NightfieldClient::draw(const DrawContext& context)
{
	if(context.layer == 1)
	{
		// Draw UI.
	}
	else
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
			glFogf(GL_FOG_START, 1);
			glFogf(GL_FOG_END, 3);
		}
		//else
		{
			glEnable(GL_FOG);
		}

		glColor3f(1.0, 1.0, 1.0);

		glTranslatef(mySettings.center[0], mySettings.center[1], mySettings.center[2]);
		glRotatef(myRotateX / 3, 0, 1, 0);
		glRotatef(myRotateY / 3, 1, 0, 0);
		glTranslatef(-mySettings.center[0], -mySettings.center[1], -mySettings.center[2]);

		GfxUtils::beginOverlayMode(context);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		GfxUtils::drawVGradient(Vector2i(0, context.viewportHeight - 100), Vector2i(context.viewportWidth, 100), Color(0, 0, 0), Color(50, 50, 60), 0.3);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		GfxUtils::endOverlayMode();

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_POINT_SPRITE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myGlowTexture->getGLTexture());
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		int ps = context.viewportHeight < context.viewportWidth ? context.viewportHeight / 8 : context.viewportWidth / 8;
		glPointSize(ps);

		myAgentRenderer->run(GpuProgram::PrimPoints);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}
