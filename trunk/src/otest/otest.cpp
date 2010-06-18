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
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Draw3D(DrawContext& context)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		const float lightPos[] = { 0.0f, 1.0f, 1.0f, 0.0f };
		glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

		const float lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient );

		/*******************************************************************
		*	NOTE:
		*		Rotations must be performed in this ordering:
		*			First pitch(rx)
		*			Second roll(rz)
		*			Third yaw(ry)
		*		However, because of the way opengl parses the arguements
		*		they must be entered in code in reverse order as seen below.
		********************************************************************/
		glRotated(ry + 3.14f / 2, 0, 1, 0);
		glRotated(rz, 0, 0, 1);
		glRotated(rx, 1, 0, 0);

		drawSolidTeapot(0.3f);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Draw2D(DrawContext& context)
	{
		glDisable(GL_LIGHTING);
		//glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GfxUtils::Begin2DCoords(context);

		glColor4f(1.0, 1.0, 1.0, 1.0);
		GfxUtils::Print(10, 20, "Hello World!\nFrom OmegaLib!!!", GfxUtils::Helvetica18);

		glColor4f (1.0f, 0.2f, 0.2f, 1.0f);
		glPointSize (5.0);
		glBegin(GL_POINTS);
			glVertex2f(x, y);    // lower left vertex
		glEnd();
		

		GfxUtils::End2DCoords();

		glEnable(GL_LIGHTING);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void Draw(DrawContext& context)
	{
		double radToDegree = 180.0 / 3.14159265;
		int av = GetInputManager()->GetAvailableEvents();
		int ls = GetInputManager()->GetDroppedEvents();
		if(av != 0)
		{
			InputEvent evts[InputManager::MaxEvents];
			GetInputManager()->GetEvents(evts, InputManager::MaxEvents);
		
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
					if ( ( evt.id == 2 ) && ( evt.type == InputEvent::Trace ) )
					{
						rx = evt.rx * radToDegree;
						ry = evt.ry * radToDegree;
						rz = evt.rz * radToDegree;
						//printf("head rx:%f, ry:%f, rz:%f \n", rx, ry, rz);
						//printf("     x:%f, y:%f, z:%f \n", evt.x, evt.y, evt.z);
					}
					break;

				default: break;
				}
			}
		}

		//printf("Viewport: %d %d %d %d\n", context.viewportX, context.viewportY, context.viewportWidth, context.viewportHeight);

		switch(context.layer)
		{
			case 0:
				Draw3D(context);
				break;
			case 1:
				Draw2D(context);
		}
	}

private:
	float rx;
	float ry;
	float rz;

	float x;
	float y;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	// TODO: Logging does not work with visual studio 2010, apparently because of incompatibilities betwen VC9 and VC10 standard
	// libraries (Equalizer included binaries are compiled with VC9 as for now).
	OM_INIT_EQUALIZER_LOG();

	SystemManager* sys = SystemManager::GetInstance();

	Config* cfg = new Config("../../data/test.cfg");
	cfg->SetDisplayConfig("--eq-config ../../data/eqc/omegadesk.eqc");

	//cfg->Load();

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

	sys->SetDisplaySystem(new EqualizerDisplaySystem());
	//sys->SetDisplaySystem(new GLUTDisplaySystem());
	sys->GetInputManager()->AddService(new MoCapService());
	sys->GetInputManager()->AddService(new MouseService());
	//sys->GetInputManager()->AddService(new TrackIRService());
	//sys->GetInputManager()->AddService(new PQService());

	sys->Initialize();

	sys->GetDisplaySystem()->SetLayerEnabled(0, "view3D", true);
	sys->GetDisplaySystem()->SetLayerEnabled(1, "view2D", true);

	sys->Run();

	sys->Cleanup();
}
