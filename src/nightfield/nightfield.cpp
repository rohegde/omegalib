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
		numAgents(5000),
		areaMin(Vector3f(-0.5, 0.5, -1.0)),
		areaMax(Vector3f(0.8, 2.0, -3.0))
		{}

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

		myGpu.initialize();

		myAgentProgram = new GpuProgram();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void initAgentData()
	{
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
		}

		int bufSize = mySettings.numAgents * sizeof(Agent);

		myAgentBuffer = new VertexBuffer();
		myAgentBuffer->initialize(bufSize, sizeof(Agent), agentData);

		delete[] agentData;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void draw3D(const DrawContext& context)
	{
		if(!myAgentBuffer) initAgentData();

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		//const float lightPos[] = { 0.0f, 1.6f, 0.0f, 0.0f };
		const float lightPos[] = { 0, 1.8, 0, 1.0f };
		const float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		const float lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
		glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0 );
		glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

		glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse );


		glEnable(GL_FOG);

		const float fogCol[] = { 0.0f, 0.0f, 0.0f };
		glFogfv( GL_FOG_COLOR, fogCol );
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, 1);
		glFogf(GL_FOG_END, 3);

		glColor3f(1.0, 1.0, 1.0);

		//glDisable(GL_LIGHTING);
		glPointSize(1);

		myAgentProgram->run(myAgentBuffer, NULL, GpuProgram::PrimPoints);
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
	// Application settings.
	Settings mySettings;

	GpuManager myGpu;
	VertexBuffer* myAgentBuffer;
	GpuProgram* myAgentProgram;

	// User interface stuff.
	UIManager myUI;
	FontManager myFontMng;
	Font* myFont;
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
