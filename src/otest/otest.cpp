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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TestApplication: public Application
{
public:
	virtual void Draw(float dt)
	{
		static float t;

		t += dt;

		int av = GetInputManager()->GetAvailableEvents();
		int ls = GetInputManager()->GetDroppedEvents();
		if(av != 0)
		{
			InputEvent evt;
			GetInputManager()->GetEvents(&evt, 1);
		
			float rx = evt.x;
			float ry = evt.y;

			//glLoadIdentity();
			glRotated(rx, 0, 1, 0);
			glRotated(ry, 1, 0, 0);
		}

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		const float lightPos[] = { 0.0f, 1.0f, 1.0f, 0.0f };
		glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

		const float lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient );

		// rotate scene around the origin
		glRotatef( static_cast< float >( t ) * 4.0f, 1.0f, 0.5f, 0.25f );

		glutSolidTeapot(0.3f);

		//printf("available: %d    lost: %d    dt: %f\n", av, ls, dt);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	SystemManager* sys = SystemManager::GetInstance();

	Config* cfg = new Config(argc, argv);

	//cfg->SetDisplayConfig("--eq-config ../../data/eqc/layout.eqc");

	sys->Setup(cfg);

	TestApplication app;
	sys->SetApplication(&app);

	sys->SetDisplaySystem(new EqualizerDisplaySystem());
	//sys->SetDisplaySystem(new GLUTDisplaySystem());
	//sys->GetInputManager()->AddService(new GLUTMouseService());

	sys->Initialize();
	sys->Run();

	sys->Cleanup();
}
