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
#include "omega/input/MoCapService.h"
#include "omega/input/PQService.h"
#include "omega/input/TrackIRService.h"

using namespace omega;
using namespace omega::gfx;

//#define LAPTOP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TestApplication: public Application
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Draw3D(DrawContext& context)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);



		//const float lightPos[] = { 0.0f, 1.6f, 0.0f, 0.0f };
		const float lightPos[] = { lx, ly, lz, 1.0f };
		glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

		const float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		const float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse );

		glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.6 );

		glEnable(GL_FOG);

		const float fogCol[] = { 0.0f, 0.0f, 0.0f };
		glFogfv( GL_FOG_COLOR, fogCol );
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, 1);
		glFogf(GL_FOG_END, 3);


		float i = (float)context.frameNum / 40;
		float zz = sin(i) * 0.7;
		float xx = cos(i * 1.2) * 0.3;
		float yy = 0.5f; //cos(i * 0.7) * 0.1;

		glColor3f(0.5, 0.4, 0.7);
		glColor3f(0.8, 1.0, 0.8);
		glPushMatrix();
		glTranslatef(-0.2, 0.5, -2.0);
		glRotated(i * 3, 0, 1, 0);
		GfxUtils::DrawSolidTeapot(0.1f);
		glPopMatrix();

		glColor3f(0.7, 0.4, 0.4);
		glPushMatrix();
		glTranslatef(0.0, 0.5, -0.7);
		GfxUtils::DrawSolidTeapot(0.1f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.3 + xx, 0.0 + yy, -1.6 + zz);
		glutSolidSphere(0.1f, 20, 20);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(mx, my, mz);
		glRotatef(ry , 0, 1, 0);
		glRotated(rz, 0, 0, 1);
		glRotated(rx, 1, 0, 0);
		glTranslatef(0, 0.1f, 0);
		GfxUtils::DrawSolidTeapot(0.05f);
		glPopMatrix();

		glColor3f(1.0, 1.0, 1.0);
		//DrawRoom(0.0f, 1.9f, -0.407f, 0.61f, 0, -3);
		DrawRoom(0.2f, 1.8f, -0.59f, 0.59f, 0, -3);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void DrawRoom(float bottom, float top, float left, float right, float nearEnd, float farEnd)
	{
		//glColor3f(0.3, 1.0, 0.3);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 1, 0);
		glVertex3f(left, bottom, nearEnd);
		glNormal3f(0, 1, 0);
		glVertex3f(right, bottom, nearEnd);
		glNormal3f(0, 1, 0);
		glVertex3f(left, bottom, farEnd);
		glNormal3f(0, 1, 0);
		glVertex3f(right, bottom, farEnd);
		glEnd();

		//glColor3f(0.3, 1.0, 0.3);
		glBegin(GL_LINES);
		for(float y = bottom; y < top; y += 0.05)
		{
			glVertex3f(left, y, nearEnd);
			glVertex3f(left, y, farEnd);
			glVertex3f(right, y, nearEnd);
			glVertex3f(right, y, farEnd);
		}

		glEnd();
		//glColor3f(0.3, 1.0, 0.3);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(left, top, nearEnd);
		glVertex3f(right, top, nearEnd);
		glVertex3f(left, top, farEnd);
		glVertex3f(right, top, farEnd);
		glEnd();

		//glColor3f(0.3, 1.0, 0.3);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(left, top, farEnd);
		glNormal3f(0, 0, 1);
		glVertex3f(right, top, farEnd);
		glNormal3f(0, 0, 1);
		glVertex3f(left, bottom, farEnd);
		glNormal3f(0, 0, 1);
		glVertex3f(right, bottom, farEnd);
		glEnd();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Draw2D(DrawContext& context)
	{
		GfxUtils::BeginOverlayMode(context);

		glColor4f(1.0, 1.0, 1.0, 1.0);
		GfxUtils::DrawText(10, 20, "Hello World!\nFrom OmegaLib!!!", GfxUtils::Helvetica18);

		glColor4f (1.0f, 0.2f, 0.2f, 1.0f);
		glPointSize (8.0);
		glBegin(GL_POINTS);
			glVertex2f(x, y);    // lower left vertex
		glEnd();
		

		GfxUtils::EndOverlayMode();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void Draw(DrawContext& context)
	{
		switch(context.layer)
		{
			case 0:
				Draw3D(context);
				break;
			case 1:
				Draw2D(context);
				break;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool HandleEvent(const InputEvent& evt)
	{
		double radToDegree = 180.0 / 3.14159265;

		switch(evt.serviceType)
		{
		case InputService::Touch:
		case InputService::Pointer:
			x = evt.x;
			y = evt.y;					
		break;

		case InputService::Mocap:
		{
			if(evt.sourceId == 1)
			{
				if(evt.x != 0 || evt.y != 0)
				{
					Observer* obs = GetDisplaySystem()->GetObserver(0);
					obs->Update(evt.x, evt.y, evt.z, evt.ry, evt.rx, evt.rz);
				}
			}
			else if(evt.sourceId == 3) //glove
			{
				if(evt.x != 0 || evt.y != 0)
				{
					lx = evt.x;
					ly = evt.y;
					lz = evt.z;
				}
			}
			else if(evt.sourceId ==2) //handheld
			{
				if(evt.x != 0 || evt.y != 0)
				{
					mx = evt.x;
					my = evt.y;
					mz = evt.z;
					rx = evt.rx * radToDegree;
					ry = evt.ry * radToDegree;
					rz = evt.rz * radToDegree;
				}
			}
			break;

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

	float mx;
	float my;
	float mz;

	float lx;
	float ly;
	float lz;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	SystemManager* sys = SystemManager::GetInstance();

	Config* cfg = new Config("../../data/test.cfg");

#ifdef LAPTOP
	cfg->SetDisplayConfig("--eq-config ../../data/eqc/omegalaptop.eqc");
#else
	cfg->SetDisplayConfig("--eq-config ../../data/eqc/omegadesk.eqc");
#endif

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

#ifdef LAPTOP
	sys->GetInputManager()->AddService(new TrackIRService());
	sys->GetInputManager()->AddService(new MouseService());
#else
	sys->GetInputManager()->AddService(new MoCapService());
	sys->GetInputManager()->AddService(new PQService());
#endif

	sys->Initialize();

	sys->GetDisplaySystem()->SetLayerEnabled(0, "view3D", true);
	sys->GetDisplaySystem()->SetLayerEnabled(0, "view2D", true);
	sys->GetDisplaySystem()->SetLayerEnabled(1, "view2D", true);

#ifdef LAPTOP
	Observer* obs = sys->GetDisplaySystem()->GetObserver(0);
    Matrix4f m( eq::Matrix4f::IDENTITY );
	Vector3f pos;
	pos.x() = 0;
	pos.y() = 1.7f;
	pos.z() = 0;
	m.set_translation(pos);
	obs->SetWorldToEmitter(m);
#endif LAPTOP

	sys->Run();
	sys->Cleanup();
}
