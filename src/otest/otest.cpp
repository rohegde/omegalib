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
		//glRotated(ry + 3.14f / 2, 0, 1, 0);
		//glRotated(rz, 0, 0, 1);
		//glRotated(rx, 1, 0, 0);

		glPushMatrix();
		glTranslatef(0, -.2, -1);
		GfxUtils::DrawSolidTeapot(0.1f);
		glPopMatrix();


		glPushMatrix();
		glTranslatef(0, -.2, -1.3);
		glutSolidSphere(0.1f, 20, 20);
		glPopMatrix();

		glColor3f(0.3, 1.0, 0.3);
		glPushMatrix();
		glTranslatef(0, -.2, -1.5);
		GfxUtils::DrawWireTeapot(0.1f);
		glPopMatrix();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Draw2D(DrawContext& context)
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
	virtual void Draw(DrawContext& context)
	{
		switch(context.layer)
		{
			case 0:
				Draw3D(context);
				break;
			case 1:
				Draw2D(context);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool HandleEvent(const InputEvent& evt)
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
			Observer* obs = GetDisplaySystem()->GetObserver(0);
			obs->Update(evt.x, evt.y, evt.z, evt.ry, evt.rx, evt.rz);

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
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	// TODO: Logging does not work with visual studio 2010, apparently because of incompatibilities betwen VC9 and VC10 standard
	// libraries (Equalizer included binaries are compiled with VC9 as for now).
	OMEGA_LOG_INIT_FILE("otest.txt");

	SystemManager* sys = SystemManager::GetInstance();

	Config* cfg = new Config("../../data/test.cfg");
	cfg->SetDisplayConfig("--eq-config ../../data/eqc/hd.eqc");

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
	//sys->GetInputManager()->AddService(new MoCapService());
	sys->GetInputManager()->AddService(new MouseService());
	//sys->GetInputManager()->AddService(new TrackIRService());
	//sys->GetInputManager()->AddService(new PQService());

	sys->Initialize();

	sys->GetDisplaySystem()->SetLayerEnabled(0, "view3D", true);
	//sys->GetDisplaySystem()->SetLayerEnabled(1, "view2D", true);

	Observer* obs = sys->GetDisplaySystem()->GetObserver(0);
    Matrix4f m( eq::Matrix4f::IDENTITY );
    //m.scale( 1.f, 1.f, -1.f );
	Vector3f pos;
	pos.x() = 0;
	pos.y() = 0;
	pos.z() = -2;
	//m.set_translation(pos);

	//Vector3f rr = Vector3f(;

	obs->SetWorldToEmitter(m);

    m = eq::Matrix4f::IDENTITY;
    m.rotate_z( -M_PI_2 );
	obs->SetSensorToObject(m);

	sys->Run();

	sys->Cleanup();

	OMEGA_LOG_CLOSE();
}
