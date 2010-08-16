/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Test executable
 *********************************************************************************************************************/
#include "omega.h"
#include "outk.h"

using namespace omega;
using namespace outk::gfx;
using namespace outk::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Settings
{
	Settings():
		numAgents(3000),
		totGroups(5),
		areaMin(Vector3f(-0.5, 0.5, -1.0)),
		areaMax(Vector3f(0.8, 2.0, -3.0))
		{}

	int totGroups;
	int numAgents;
	Vector3f areaMin;
	Vector3f areaMax;
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
	// These parameters determine a ray in the
	float x, y; // Origin on the z = 0 plane
	float dx, dy; // x, y increment for unitary z increment.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NightfieldApplication: public Application, IUIEventHandler
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	NightfieldApplication()
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void initialize()
	{
		myFontMng.createFont("arial30", "../../data/fonts/Arial.ttf", 22);
		myFont = myFontMng.getFont("arial30");

		myTexMng = new TextureManager();
		myTexMng->loadTexture("glow", "../../data/images/glow2.png");

		myGlowTexture = myTexMng->getTexture("glow");

		myUI.setEventHandler(this);
		myUI.setDefaultFont(myFont);

		WidgetFactory* wf = myUI.getWidgetFactory();

		Box* b1 = wf->createBox("box1", myUI.getRootWidget(), Box::LayoutHorizontal);
		b1->setUserMoveEnabled(true);
		b1->setDebugModeEnabled(true);
		b1->setPosition(Vector2f(100, 100));
		b1->setSize(Vector2f(400, 100));

		Button* btn1 = wf->createButton("L1", b1);
		btn1->setText("Hello Button!");

		myAgentBuffer = NULL;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void initializeWindow()
	{
		myGpu = new GpuManager();
		myGpu->initialize();

		std::vector<String> shaderNames;
		shaderNames.push_back("behavior");
		shaderNames.push_back("update");
		myGpu->loadComputeShaders("../../data/shaders/agentsim.cl", shaderNames);

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

		myInteractorBuffer = new GpuBuffer(myGpu);
		myInteractorBuffer->initialize(MaxInteractors * sizeof(InteractorRay), sizeof(InteractorRay));

		myNumInteractors = new GpuConstant();
		myNumInteractors->setIntValue(0);

		// Setup data and parameters for the agent behavior program
		myAgentBehavior = new GpuProgram(myGpu);
		myAgentBehavior->setComputeShader(myGpu->getComputeShader("behavior"));
		myAgentBehavior->setInput(0, myAgentBuffer);
		myAgentBehavior->setInput(1, myDt);
		myAgentBehavior->setInput(2, myNumAgents);
		myAgentBehavior->setInput(3, myTotGroups);
		myAgentBehavior->setInput(4, myGroupId);
		myAgentBehavior->setInput(5, myNumInteractors);
		myAgentBehavior->setInput(6, myInteractorBuffer);
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
		myAgentRenderer->setInput(0, myAgentBuffer);
		myAgentRenderer->setNumRenderItems(mySettings.numAgents);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void draw3D(const DrawContext& context)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		static int groupId = 0;

		//const float lightPos[] = { 0.0f, 1.6f, 0.0f, 0.0f };
		const float lightPos[] = { 0, 1.8, 0, 1.0f };
		const float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		const float lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
		glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0 );
		glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

		glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse );


		glEnable(GL_FOG);

		const float fogCol[] = { 0.1f, 0.1f, 1.0f };
		glFogfv( GL_FOG_COLOR, fogCol );
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, 1);
		glFogf(GL_FOG_END, 3);

		glColor3f(1.0, 1.0, 1.0);

		glDisable(GL_LIGHTING);

		myGroupId->setIntValue(groupId);

		groupId++;
		if(groupId == mySettings.totGroups) groupId = 0;

		//glTranslatef(0.2, 1.5, -2);
		//glRotatef(myMouseX, 0, 1, 0);
		//glRotatef(myMouseY, 1, 0, 0);
		//glTranslatef(-0.2, -1.5, 2);

		if(context.frameNum < 200)
		{
			myDt->setFloatValue(0.04f);
			myAgentBehavior->run();
			myAgentUpdate->run();

			char txt[256];
			sprintf(txt, "Initializing simulation: %d%%", context.frameNum / 2);
			GfxUtils::beginOverlayMode(context);
			GfxUtils::drawText(0, 10, txt);
			GfxUtils::endOverlayMode();
		}
		else
		{
			GLdouble modelview[16];
			GLdouble projection[16];
			GLint viewport[4];

			glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
			glGetDoublev( GL_PROJECTION_MATRIX, projection );
			glGetIntegerv( GL_VIEWPORT, viewport );

			double mx, my, mz;
			gluUnProject(myMouseX, viewport[3] - myMouseY, 0.01f, modelview, projection, viewport, &mx, &my, &mz);

			glEnable(GL_TEXTURE_2D);
			glEnable(GL_POINT_SPRITE);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, myGlowTexture->getGLTexture());
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			myDt->setFloatValue(0.005f);
			myNumInteractors->setIntValue(0);

			myAgentBehavior->run();
			myAgentUpdate->run();


			glPointSize(64);
			myAgentRenderer->run(GpuProgram::PrimPoints);

			glDisable(GL_BLEND);

			glEnable(GL_DEPTH_TEST);
		}

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void draw2D(const DrawContext& context)
	{
		GfxUtils::beginOverlayMode(context);

		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		myUI.draw(context);

		GfxUtils::endOverlayMode();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void draw(const DrawContext& context)
	{
		switch(context.layer)
		{
			case 0:
				draw3D(context);
				break;
			case 1:
				draw2D(context);
				break;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void update(const UpdateContext& context)
	{
		myUI.update(context);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool handleEvent(const InputEvent& evt)
	{
		switch(evt.serviceType)
		{
		case InputService::Touch:
		case InputService::Pointer:
			myUI.processInputEvent(evt);
			myMouseX = evt.position.x();
			myMouseY = evt.position.y();
		break;

		case InputService::Mocap:
		{
		}
		}
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void handleUIEvent(const UIEvent& evt)
	{
		printf("click yay!\n");
	}

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

	GpuBuffer* myInteractorBuffer;
	GpuConstant* myNumInteractors;

	// Textures
	TextureManager* myTexMng;
	Texture* myGlowTexture;

	// User interface stuff.
	UIManager myUI;
	FontManager myFontMng;
	Font* myFont;

	float myMouseX;
	float myMouseY;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	ologopen("nightfield-log.txt");

	Config* cfg = new Config(argv[1]);

	SystemManager* sys = SystemManager::instance();
	sys->setup(cfg);

	NightfieldApplication app;
	sys->setApplication(&app);

	sys->initialize();
	Observer* obs = sys->getDisplaySystem()->getObserver(0);
	obs->update(Vector3f(0, 1.6f, 0), Vector3f(0, 0, 0));

	sys->run();

	sys->cleanup();

	ologclose();
}
