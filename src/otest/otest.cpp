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

using namespace omega;
using namespace omega::gfx;

void drawSolidTeapot(GLdouble scale);
void drawWireTeapot(GLdouble scale);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TestApplication: public Application
{
public:
	virtual void Draw(int layer)
	{
		static float rx;
		static float ry;
		static float rz;

		int av = GetInputManager()->GetAvailableEvents();
		int ls = GetInputManager()->GetDroppedEvents();
		if(av != 0)
		{
			InputEvent evt;
			GetInputManager()->GetEvents(&evt, 1);
		
			rx = evt.rx;
			ry = evt.ry;
			rz = evt.rz;
		}

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		const float lightPos[] = { 0.0f, 1.0f, 1.0f, 0.0f };
		glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

		const float lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient );

		glRotated(rx, 1, 0, 0);
		glRotated(ry + 3.14f / 2, 0, 1, 0);
		glRotated(rz, 0, 0, 1);

		drawSolidTeapot(0.3f);

		glDisable(GL_LIGHTING);
		//glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GfxUtils::Begin2DCoords();

		glColor4f(1.0, 1.0, 1.0, 1.0);
		GfxUtils::Print(10, 20, "Hello World!\nFrom OmegaLib!!!", GfxUtils::Helvetica18);

		GfxUtils::End2DCoords();

		glEnable(GL_LIGHTING);
		//glEnable(GL_LIGHTING);

		//printf("available: %d    lost: %d    dt: %f\n", av, ls, dt);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	OM_INIT_EQUALIZER_LOG();

	SystemManager* sys = SystemManager::GetInstance();

	Config* cfg = new Config(argc, argv);

	cfg->SetDisplayConfig("--eq-config ../../data/eqc/4-window.all.eqc");

	sys->Setup(cfg);

	TestApplication app;
	sys->SetApplication(&app);

	//sys->SetDisplaySystem(new EqualizerDisplaySystem());
	sys->SetDisplaySystem(new GLUTDisplaySystem());
	sys->GetInputManager()->AddService(new MouseService());
	//sys->GetInputManager()->AddService(new TrackIRService());

	sys->GetDisplaySystem()->SetLayerEnabled(0, "default", true);

	sys->Initialize();
	sys->Run();

	sys->Cleanup();
}
