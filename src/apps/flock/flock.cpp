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
#include <omega.h>
#include <cyclops.h>
#include <omegaToolkit.h>
#include <omegaVtk.h>

#include<osg/PositionAttitudeTransform>

using namespace omega;
using namespace omegaToolkit;
using namespace omegaToolkit::ui;
using namespace cyclops;

bool sUseOpenCL = true;

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Preset
{
	float avoidanceDist;
	float friction;
	float coordinationDist;
	//bool useAdditiveAlpha;
	//String flockImage;
	//bool useFog;
	//bool drawSpeedVectors;
	//float speedVectorScale;
	//Color speedVectorColor;
	//String fragmentShader;
	//bool hasFragmentShader;
	//float pointSize;
	//int trailSize;
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
class Flock
{
public:
static const int MaxAffectors = 32;

public:
	Flock() {}
	void setup(Settings* settings);

	void initialize();
	void initializeCL();
	virtual void update(const UpdateContext& context);
	void updateAgentsCPU(const UpdateContext& context);
	void updateAgentsGPU(const UpdateContext& context);

	const AlignedBox3* getBoundingBox();
	bool hasBoundingBox();

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
class FlockApplication: public ServerModule
{
public:
	FlockApplication();

	virtual void initialize();
	virtual void update(const UpdateContext&);
	virtual void handleEvent(const Event& evt);
private:
	SceneManager* mySceneManager;
	Flock* myFlock;
	Settings mySettings;

	osg::PositionAttitudeTransform** myAgentNodes;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
Settings::Settings():
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
void Settings::load(const Setting& s)
{
    const Setting& sPreset = s["preset"];

    Preset* p = new Preset();
    loadPreset(p, sPreset);
    presets.push_back(p);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::loadPreset(Preset* p, const Setting& s)
{
    p->friction = s["friction"];
    p->avoidanceDist = s["avoidanceDist"];
    p->coordinationDist = s["coordinationDist"];
    //p->flockImage = String((const char*)s["flockImage"]);
    //if(s.exists("fragmentShader"))
    //{
    //    p->fragmentShader = String((const char*)s["fragmentShader"]);
    //    p->hasFragmentShader = true;
    //}
    //else
    //{
    //    p->hasFragmentShader = false;
    //}
    //p->useAdditiveAlpha = s["useAddictiveAlpha"];
    //p->drawSpeedVectors = s["drawSpeedVectors"];

    //if(s.exists("pointSize"))
    //{
    //    p->pointSize = s["pointSize"];
    //}
    //else
    //{
    //    p->pointSize = 32;
    //}

    //if(s.exists("trailSize"))
    //{
    //    p->trailSize = s["trailSize"];
    //}
    //else
    //{
    //    p->trailSize = 16;
    //}

    //p->speedVectorScale = s["speedVectorScale"];

    //p->useFog = s["useFog"];


    //Setting& sc = s["speedVectorColor"];
    //p->speedVectorColor = Color(sc[0], sc[1], sc[2], sc[3]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Flock::setup(Settings* settings)
{
	mySettings = settings;
	myCurrentPreset = mySettings->presets[0];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Flock::initialize()
{
	//myAgentImage = ImageUtils::loadImage(myCurrentPreset->flockImage);

	// Setup the agent buffer.
	myAgents = new Agent[mySettings->numAgents];

	float minx = mySettings->areaMin[0];
	float miny = mySettings->areaMin[1];
	float minz = mySettings->areaMin[2];

	float dx = mySettings->areaMax[0] - mySettings->areaMin[0];
	float dy = mySettings->areaMax[1] - mySettings->areaMin[1];
	float dz = mySettings->areaMax[2] - mySettings->areaMin[2];

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
void Flock::initializeCL()
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
void Flock::update(const UpdateContext& context)
{
	if(sUseOpenCL)
	{
		updateAgentsGPU(context);
	}
	else
	{
		updateAgentsCPU(context);
	}
	// Force the flock renderables to refresh.
	//refresh();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Flock::updateAgentsGPU(const UpdateContext& context)
{
	myNumInteractors->setIntValue(myActiveAffectors);
	myInteractorBuffer->write(myAffectorData, 0, MaxAffectors * sizeof(FlockAffector));

	myDt->setFloatValue(context.dt);

	// Set simulation parameters.
	myCoordinationDist->setFloatValue(myCurrentPreset->coordinationDist);
	myAvoidanceDist->setFloatValue(myCurrentPreset->avoidanceDist);
	myFriction->setFloatValue(myCurrentPreset->friction);

	myCenter->setFloatValue(mySettings->center[0], mySettings->center[1], mySettings->center[2], 0);

	myAgentBehavior->runComputeStage(myAgentBehaviorOptions, &myAgentBehaviorParams);
	myAgentUpdate->runComputeStage(myAgentUpdateOptions, &myAgentUpdateParams);
	
	// Read back data from the compute buffer.
	myAgentComputeBuffer->read(myAgents, 0, mySettings->numAgents * sizeof(Agent));
}


///////////////////////////////////////////////////////////////////////////////////////////////////// 
// Fallback code to simulate the flock on the cpu.
void Flock::updateAgentsCPU(const UpdateContext& context)
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

///////////////////////////////////////////////////////////////////////////////////////////////////
FlockApplication::FlockApplication()
{
    Config* cfg = getServer()->getSystemManager()->getAppConfig();
    if(cfg->exists("config"))
    {
        Setting& sCfg = cfg->lookup("config");
        mySettings.load(sCfg);
    }

	myFlock = new Flock();
	myFlock->setup(&mySettings);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockApplication::initialize()
{
#ifdef OMEGA_USE_PYTHON
	omegaToolkitPythonApiInit();
	cyclopsPythonApiInit();
#endif

	myFlock->initialize();

	// Create and initialize the cyclops scene manager.
	// If a scene file is specified in the application config file, the scene manager will
	// load it during initialization.
	mySceneManager = new SceneManager();
	mySceneManager->doInitialize(getServer());
	ModuleServices::addModule(mySceneManager);

	ModelAsset* agentModel = mySceneManager->getModelAsset(0);
	myAgentNodes = new osg::PositionAttitudeTransform*[mySettings.numAgents];
	for(int i = 0; i < mySettings.numAgents; i++)
	{
		myAgentNodes[i] = new osg::PositionAttitudeTransform();
		if(agentModel != NULL)
		{
			myAgentNodes[i]->addChild(agentModel->nodes[0]);
		}
		mySceneManager->addNode(myAgentNodes[i]);
	}

	// Setup lighting
	cyclops::Light* l = mySceneManager->getLight(0);
	l->enabled = true;
	l->position = Vector3f(0, 10, 0);
	l->color = Color(1.0f, 1.0f, 0.7f);
	l->ambient = Color(0.1f, 0.1f, 0.1f);
	mySceneManager->setMainLight(l);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockApplication::update(const UpdateContext& context)
{
	myFlock->update(context);
	Agent* agent = myFlock->getAgents();
	for(int i = 0; i < mySettings.numAgents; i++)
	{
		myAgentNodes[i]->setPosition(osg::Vec3d(agent[i].x, agent[i].y, agent[i].z));
		Quaternion q = Math::buildRotation(-Vector3f::UnitX(), Vector3f(agent[i].vx, agent[i].vy, agent[i].vz), Vector3f::UnitY());
		myAgentNodes[i]->setAttitude(osg::Quat(q.x(), q.y(), q.z(), q.w()));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockApplication::handleEvent(const Event& evt)
{
	//if(evt.getServiceType() == Service::UI) 
	//{
	//	handleUIEvent(evt);
	//}
	//else
	//{
	//	EngineClient::handleEvent(evt);
	//}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	Application<FlockApplication> app("flock");
	return omain(app, argc, argv);
}
