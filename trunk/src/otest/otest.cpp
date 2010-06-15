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

		static float x = 0;
		static float y = 0;

		int av = GetInputManager()->GetAvailableEvents();
		int ls = GetInputManager()->GetDroppedEvents();
		if(av != 0)
		{
			InputEvent evts[16];
			GetInputManager()->GetEvents(evts, 16);
		
			for( int evtNum = 0; evtNum < av; evtNum++)
			{
				InputEvent &evt = evts[evtNum];
				switch(evt.source)
				{
				case InputEvent::Touch:
				case InputEvent::Pointer:
					x = evt.x;
					y = evt.y;
					//printf("touchin x:%f, y:%f\n", x ,y);
					break;

				case InputEvent::Mocap:
					rx = evt.rx;
					ry = evt.ry;
					rz = evt.rz;
					//printf("head rx:%f, ry:%f, rz:%f \n", rx, ry, rz);
					//printf("     x:%f, y:%f, z:%f \n", evt.x, evt.y, evt.z);
					break;

				default: break;
				}
			}
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

		glColor4f (1.0f, 0.2f, 0.2f, 1.0f);
		glPointSize (5.0);
		glBegin(GL_POINTS);
			glVertex2f( x, y);    // lower left vertex
		glEnd();
		

		GfxUtils::End2DCoords();

		glEnable(GL_LIGHTING);
		//glEnable(GL_LIGHTING);

		//printf("available: %d    lost: %d    dt: %f\n", av, ls, dt);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	// TODO: Logging does not work with visual studio 2010, apparently because of incompatibilities betwen VC9 and VC10 standard
	// libraries (Equalizer included binaries are compiled with VC9 as for now).
	OM_INIT_EQUALIZER_LOG();

	SystemManager* sys = SystemManager::GetInstance();

	Config* cfg = new Config("../../data/test.cfg");
	cfg->SetDisplayConfig("--eq-config ../../data/eqc/test.eqc");

	cfg->Load();

	///////////////////////////////////////////////////////////////////////////////////////////////
	// EXAMPLE: HOW TO READ CONFIGURATION VALUES.
	printf("Reading float: %f\n", cfg->GetValue("config/floatVal", 0.0f));
	printf("Reading string: %s\n", cfg->GetValue("config/stringVal", "DEFAULT"));
	printf("Reading dafault string: %s\n", cfg->GetValue("config/NOTTHERE", "DEFAULT"));

	std::vector<int> v;
	cfg->GetArray("config/subsection/intArray", v);
	printf("Reading vector: size is %d\n", v.size());
	///////////////////////////////////////////////////////////////////////////////////////////////


	sys->Setup(cfg);

	TestApplication app;
	sys->SetApplication(&app);

	//sys->SetDisplaySystem(new EqualizerDisplaySystem());
	sys->SetDisplaySystem(new GLUTDisplaySystem());
	sys->GetInputManager()->AddService(new MouseService());
	//sys->GetInputManager()->AddService(new MoCapService());
	//sys->GetInputManager()->AddService(new TrackIRService());
	//sys->GetInputManager()->AddService(new PQService());

	sys->GetDisplaySystem()->SetLayerEnabled(0, "default", true);

	sys->Initialize();
	sys->Run();

	sys->Cleanup();
}
