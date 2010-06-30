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
#include "omega/input/PQService.h"

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
		glRotated(ry + 3.14f / 2, 0, 1, 0);
		glRotated(rz, 0, 0, 1);
		glRotated(rx, 1, 0, 0);

		glPushMatrix();
		glTranslatef(-2.0,5.0,0.0);

		switch( CURRENT_OBJECT ){
			case(TEAPOT):
				GfxUtils::DrawSolidTeapot(0.3f);
				break;
			default:
				DrawBox();
				break;
		}// switch

		glPopMatrix();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Draw2D(DrawContext& context)
	{
		glDisable(GL_LIGHTING);
		//glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GfxUtils::BeginOverlayMode(context);

		glColor4f(1.0, 1.0, 1.0, 1.0);
		GfxUtils::DrawText(10, 20, "Hello World!\nFrom OmegaLib!!!", GfxUtils::Helvetica18);

		// Control Panel Boundary
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_LINES);
			glVertex2f(200, 0);
			glVertex2f(200, 1080);
		glEnd();
		glBegin(GL_LINES);
			glVertex2f(0, 540);
			glVertex2f(200, 540);
		glEnd();
		
		// Create texture from viewport
		int viewport[4];
		//glGetIntegerv(GL_VIEWPORT,(int*)viewport);

		// Region that should be painted on
		// Canvas upper left is at (800,400), canvas width/height is 400
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, texture );

		glBegin(GL_LINE_LOOP);
			glTexCoord2d(0.0,1.0); glVertex2f(800, 400);
			glTexCoord2d(0.0,0.0); glVertex2f(800, 800);
			glTexCoord2d(1.0,0.0); glVertex2f(1200, 800);
			glTexCoord2d(1.0,1.0); glVertex2f(1200, 400);
		glEnd();
		glDisable( GL_TEXTURE_2D );

		glBindTexture( GL_TEXTURE_2D, texture );
		glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 0, 0, xSize, ySize, 0 );

		//glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

		// Last Touch/Mouse point
		glColor4f (1.0f, 0.2f, 0.2f, 1.0f);
		glPointSize (5.0);
		glBegin(GL_POINTS);
			glVertex2f(x, y);    // lower left vertex
		glEnd();
		

		GfxUtils::EndOverlayMode();

		glEnable(GL_LIGHTING);
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
			y = evt.y;
			ControlPanel_OnInput( x, y );
			printf("received touchevent x:%f, y:%f\n", x ,y);
			return true;

		case InputService::Mocap:
			//if ( ( evt.id == 2 ) && ( evt.type == InputEvent::Trace ) )
			//{
			//	rx = evt.rx * radToDegree;
			//	ry = evt.ry * radToDegree;
			//	rz = evt.rz * radToDegree;
			//printf("head rx:%f, ry:%f, rz:%f \n", evt.rx, evt.ry, evt.rz);
			//printf("     x:%f, y:%f, z:%f \n", evt.x, evt.y, evt.z);
			//}
			return true;
		}
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ControlPanel_OnInput( float x, float y ){
		if( x <= 200 ){ // Panel width
			if( y < 540 ){ // Top button
				CURRENT_OBJECT = TEAPOT;
			} else if( y > 540 ){ //Bottom button
				CURRENT_OBJECT = -1;
			}// if-else
		}// if
	}// ControlPanel_OnInput

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void DrawBox(){
		bool isWire = false;
		float baseWidth = 0.5;
		float height = 0.5;

		GLfloat border_color[] = { 0.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR, border_color );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, texture );

			// base
		if(isWire) glBegin(GL_LINE_LOOP);
		else glBegin(GL_POLYGON);
		glNormal3d( 0.0, -1.0, 0.0 );
		glTexCoord2d(0.0,0.0); glVertex3d(  baseWidth/2.0,   0.0,  baseWidth/2.0 ); // 1
		glTexCoord2d(1.0,0.0); glVertex3d( -baseWidth/2.0,   0.0,  baseWidth/2.0 ); // 0
		glTexCoord2d(1.0,1.0); glVertex3d( -baseWidth/2.0,   0.0, -baseWidth/2.0 ); // 3
		glTexCoord2d(0.0,1.0); glVertex3d(  baseWidth/2.0,   0.0, -baseWidth/2.0 ); // 2
		glEnd( );

		// top
		if(isWire) glBegin(GL_LINE_LOOP);
		else glBegin(GL_POLYGON);
		glNormal3d( 0.0, 1.0, 0.0 );
		glTexCoord2d(0.0,0.0); glVertex3d( -baseWidth/2.0, height,  baseWidth/2.0 ); // 4
		glTexCoord2d(1.0,0.0); glVertex3d(  baseWidth/2.0, height,  baseWidth/2.0 ); // 5
		glTexCoord2d(1.0,1.0); glVertex3d(  baseWidth/2.0, height, -baseWidth/2.0 ); // 6
		glTexCoord2d(0.0,1.0); glVertex3d( -baseWidth/2.0, height, -baseWidth/2.0 ); // 7
		glEnd( );

		// back
		if(isWire) glBegin(GL_LINE_LOOP);
		else glBegin(GL_POLYGON);
		glNormal3d( 0.0, 0.0, -1.0 );
		glTexCoord2d(0.0,0.0); glVertex3d(  baseWidth/2.0,   0.0, -baseWidth/2.0 ); // 2
		glTexCoord2d(1.0,0.0); glVertex3d( -baseWidth/2.0,   0.0, -baseWidth/2.0 ); // 3
		glTexCoord2d(1.0,1.0); glVertex3d( -baseWidth/2.0, height, -baseWidth/2.0 ); // 7
		glTexCoord2d(0.0,1.0); glVertex3d(  baseWidth/2.0, height, -baseWidth/2.0 ); // 6
		glEnd( );

		// left
		if(isWire) glBegin(GL_LINE_LOOP);
		else glBegin(GL_POLYGON);
		glNormal3d( -1.0, 0.0, 0.0 );
		glTexCoord2d(0.0,0.0); glVertex3d(  baseWidth/2.0,   0.0,  baseWidth/2.0 ); // 1
		glTexCoord2d(1.0,0.0); glVertex3d(  baseWidth/2.0,   0.0, -baseWidth/2.0 ); // 2
		glTexCoord2d(1.0,1.0); glVertex3d(  baseWidth/2.0, height, -baseWidth/2.0 ); // 6
		glTexCoord2d(0.0,1.0); glVertex3d(  baseWidth/2.0, height,  baseWidth/2.0 ); // 5
		glEnd( );

		// front
		if(isWire) glBegin(GL_LINE_LOOP);
		else glBegin(GL_POLYGON);
		glNormal3d( 0.0, 0.0, 1.0 );
		glTexCoord2d(0.0,0.0); glVertex3d( -baseWidth/2.0,   0.0,  baseWidth/2.0 ); // 0
		glTexCoord2d(1.0,0.0); glVertex3d(  baseWidth/2.0,   0.0,  baseWidth/2.0 ); // 1
		glTexCoord2d(1.0,1.0); glVertex3d(  baseWidth/2.0, height,  baseWidth/2.0 ); // 5
		glTexCoord2d(0.0,1.0); glVertex3d( -baseWidth/2.0, height,  baseWidth/2.0 ); // 4
		glEnd( );

		// right
		if(isWire) glBegin(GL_LINE_LOOP);
		else glBegin(GL_POLYGON);
		glNormal3d( 1.0, 0.0, 0.0 );
		glTexCoord2d(0.0,0.0); glVertex3d( -baseWidth/2.0,   0.0, -baseWidth/2.0 ); // 3
		glTexCoord2d(1.0,0.0); glVertex3d( -baseWidth/2.0,   0.0,  baseWidth/2.0 ); // 0
		glTexCoord2d(1.0,1.0); glVertex3d( -baseWidth/2.0, height,  baseWidth/2.0 ); // 4
		glTexCoord2d(0.0,1.0); glVertex3d( -baseWidth/2.0, height, -baseWidth/2.0 ); // 7
		glEnd( );

		glDisable( GL_TEXTURE_2D );
	}// DrawBox

	// Creates a texture that will be mapped to object
	void CreateTexture(){
		xSize = ySize = 256;
		char* colorBits = new char[ xSize * ySize * 3];

		// Generate texture
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );

		glTexImage2D( GL_TEXTURE_2D, 0, 3, xSize, ySize, 0, GL_RGB, GL_UNSIGNED_BYTE, colorBits );

		delete[] colorBits;

	}// Create Texture

private:
	float rx;
	float ry;
	float rz;

	float x;
	float y;

	static const int TEAPOT = 0;
	static const int BOX = 1;
	int CURRENT_OBJECT;

	// Texture Drawing
	int xSize, ySize; // Size of texture
	unsigned int texture;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	SystemManager* sys = SystemManager::GetInstance();

	Config* cfg = new Config("../../data/test.cfg");
	cfg->SetDisplayConfig("--eq-config ../../data/eqc/omegalaptop.eqc");

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
	app.CreateTexture();
	sys->SetApplication(&app);

	sys->SetDisplaySystem(new EqualizerDisplaySystem());
	//sys->SetDisplaySystem(new GLUTDisplaySystem());
	//sys->GetInputManager()->AddService(new MoCapService());
	sys->GetInputManager()->AddService(new MouseService());
	//sys->GetInputManager()->AddService(new TrackIRService());
	//sys->GetInputManager()->AddService(new PQService());

	sys->Initialize();

	sys->GetDisplaySystem()->SetLayerEnabled(0, "view3D", true);
	sys->GetDisplaySystem()->SetLayerEnabled(1, "view2D", true);

	sys->Run();

	sys->Cleanup();
}
