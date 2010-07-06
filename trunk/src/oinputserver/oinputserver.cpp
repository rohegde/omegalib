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
#include "omega/input/MouseService.h"

using namespace omega;
using namespace omega::gfx;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TestApplication: public Application
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void draw3D(DrawContext& context)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		const float lightPos[] = { 0.0f, 0.6f, 1.0f, 0.0f };
		glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

		const float lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient );

		glEnable(GL_FOG);

		const float fogCol[] = { 0.0f, 0.0f, 0.0f };
		glFogfv( GL_FOG_COLOR, fogCol );
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, 1);
		glFogf(GL_FOG_END, 3);


		//glRotated(ry + 3.14f / 2, 0, 1, 0);
		//glRotated(rz, 0, 0, 1);
		//glRotated(rx, 1, 0, 0);

		static float i = 0;
		float zz = sin(i) * 0.4;
		float xx = cos(i * 1.2) * 0.3;
		float yy = cos(i * 0.7) * 0.1;
		i += 0.03;

		glPushMatrix();
		glTranslatef(x, y, -1.2 + z * 2);
		glRotated(i * 3, 0, 1, 0);
		GfxUtils::DrawSolidTeapot(0.1f);
		glPopMatrix();


		glPushMatrix();
		glTranslatef(0.3 + xx, 1.3 + yy, -2 + zz);
		glutSolidSphere(0.1f, 20, 20);
		glPopMatrix();

		glColor3f(0.3, 1.0, 0.3);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-0.4074, 1.16, -0.8128);
		glVertex3f( 0.6106, 1.16, -0.8128);
		glVertex3f(-0.4074, 1.16, -3.8128);
		glVertex3f( 0.6106, 1.16, -3.8128);
		glEnd();

		glColor3f(0.3, 1.0, 0.3);
		glBegin(GL_LINES);
		for(float y = 1.16; y < 1.8; y += 0.05)
		{
			glVertex3f(-0.4074, y, -0.8128);
			glVertex3f(-0.4074, y, -10.8128);
			glVertex3f( 0.6106, y, -0.8128);
			glVertex3f( 0.6106, y, -10.8128);
		}

		glEnd();
		glColor3f(0.3, 1.0, 0.3);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-0.4074, 1.8, -0.8128);
		glVertex3f( 0.6106, 1.8, -0.8128);
		glVertex3f(-0.4074, 1.8, -3.8128);
		glVertex3f( 0.6106, 1.8, -3.8128);
		glEnd();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void draw2D(DrawContext& context)
	{
		GfxUtils::BeginOverlayMode(context);

		glColor4f(1.0, 1.0, 1.0, 1.0);
		GfxUtils::DrawText(10, 20, "Hello World!\nFrom OmegaLib!!!", GfxUtils::Helvetica18);

		glColor4f (1.0f, 0.2f, 0.2f, 1.0f);
		glPointSize (5.0);
		glBegin(GL_POINTS);
			glVertex2f(x, y);    // lower left vertex
		glEnd();
		

		GfxUtils::EndOverlayMode();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void draw(DrawContext& context)
	{
		switch(context.layer)
		{
			case 0:
				draw3D(context);
				break;
			case 1:
				draw2D(context);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool handleEvent(const InputEvent& evt)
	{
		switch(evt.serviceType)
		{
		case InputService::Touch:
		case InputService::Pointer:
			x = evt.x;
			y = evt.y;					break;

		case InputService::Mocap:
		{

			//GfxUtils::BeginOverlayMode(context);

			//char str[1024];
			//glColor4f(1.0, 1.0, 0.4, 1.0);
			//sprintf(str, "head x:%.3f, y:%.3f, z:%.3f rx:%.3f, ry:%.3f, rz:%.3f", evt.x, evt.y, evt.z, evt.rx, evt.ry, evt.rz);
			//GfxUtils::DrawText(10, 20, str, GfxUtils::Helvetica12);

			//GfxUtils::EndOverlayMode();
			
			//evt.z = 0;
			if(evt.sourceId == 1)
			{
				Observer* obs = getDisplaySystem()->getObserver(0);
				obs->update(evt.x, evt.y, evt.z, evt.ry, evt.rx, evt.rz);
			}
			else if(evt.sourceId == 3)
			{
				if(evt.x != 0 || evt.y != 0)
				{
					x = evt.x;
					y = evt.y;
					z = evt.z;
				}
			}

			break;
		}

		default: break;
		}
		return true;
	}

private:
	float rx;
	float ry;
	float rz;

	float x;
	float y;
	float z;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	SystemManager* sys = SystemManager::instance();

	Config* cfg = new Config("../../data/test.cfg");
	//cfg->setDisplayConfig("--eq-config ../../data/eqc/test.eqc");
	cfg->load();

	sys->setup(cfg);

	TestApplication app;
	sys->setApplication(&app);

	sys->setDisplaySystem(new EqualizerDisplaySystem());
	//sys->setDisplaySystem(new GLUTDisplaySystem());
	//sys->getInputManager()->addService(new MoCapService());
	sys->getInputManager()->addService(new MouseService());
	//sys->getInputManager()->addService(new TrackIRService());
	//sys->getInputManager()->addService(new PQService());

	sys->initialize();

	sys->getDisplaySystem()->setLayerEnabled(0, "view3D", true);
	//sys->getDisplaySystem()->setLayerEnabled(1, "view2D", true);

	sys->run();

	sys->cleanup();
}
