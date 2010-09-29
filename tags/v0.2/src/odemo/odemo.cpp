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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	class TestApplication: public Application
 *		
 *		This class will contain the draw and event handler functions that OmegaLib's System Manager will use.
 *      Additional functions can be added to handle any data that will be required by the applicaiton.
 */
class TestApplication: public Application
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	 *		
	 */
	void draw3D(const DrawContext& context)
	{
		

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		const float lightPos[] = { 0, 1.8, 0, 1.0f };
		const float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		const float lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
		glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0 );

		glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
		glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse );

		//Generate Fog effect
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
		GfxUtils::drawSolidTeapot(0.1f);
		glPopMatrix();

		glColor3f(0.7, 0.4, 0.4);
		glPushMatrix();
		glTranslatef(0.0, 0.5, -0.7);
		GfxUtils::drawSolidTeapot(0.1f);
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
		GfxUtils::drawSolidTeapot(0.05f);
		glPopMatrix();

		glColor3f(1.0, 1.0, 1.0);
		//DrawRoom(0.0f, 1.9f, -0.407f, 0.61f, 0, -3);
		drawRoom(0.3f, 2.0f, -0.59f, 0.59f, 0, -3);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	 *		
	 */
	void drawRoom(float bottom, float top, float left, float right, float nearEnd, float farEnd)
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
	/*
	 *		
	 */
	void draw2D(DrawContext& context)
	{
		GfxUtils::beginOverlayMode(context);

		glColor4f(1.0, 1.0, 1.0, 1.0);
		GfxUtils::drawText(10, 20, "Hello World!\nFrom OmegaLib!!!", GfxUtils::Helvetica18);

		glColor4f (1.0f, 0.2f, 0.2f, 1.0f);
		glPointSize (8.0);
		glBegin(GL_POINTS);
			glVertex2f(x, y);    // lower left vertex
		glEnd();
		

		GfxUtils::endOverlayMode();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	 *	virtual void draw(DrawContext& context)
	 *		
	 *		This is a virtual function that can be implemented to handle drawing.
	 *      This exmaple breaks it up into 3d and 2d drawingA.
	 */
	virtual void draw(DrawContext& context)
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
	/*
	 *	virtual bool handleEvent(const InputEvent& evt)
	 *		
	 *		This is a virtual function that can be implemented to handle input events
	 */
	virtual bool handleEvent(const InputEvent& evt)
	{
		switch(evt.serviceType)
		{
		case InputService::Touch:
		case InputService::Pointer:
			x = evt.position[0];
			y = evt.position[1];					
		break;

		case InputService::Mocap:
		{
			/* better calculations for tracker done here before passing */			
			Observer* obs = getDisplaySystem()->getObserver(0);
			obs->update(evt.position, evt.rotation);
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
/*
 *	void main(int argc, char** argv)
 *
 *    This is the main for oDemo.  Here is where to setup :
 *       - the configuration file and application to pass to OmegaLib
 *       - data structures for data that will be manipulated before relinquishing control to OmegaLib
 *		 - relingquish control to Omegalib for rendering.
 *
 *    The paradigm that drives OmegaLib is much like OpenGL.  Data structures and setup functionality is setup in the
 *      Main Program.  Control is then relinquished to OmegaLib where rendering and data manipulation will take place.
 *		
 */
void main(int argc, char** argv)
{
	SystemManager* sys = SystemManager::instance();		//Setup the system manager for OmegaLib
	
	Config* cfg = new Config("../../data/test.cfg");	//define the config file and its path
	sys->setup(cfg);									//notify OmegaLib's sys mngr of the configuration file

	TestApplication app;								//inialize the app that will will be passed to OmegaLib
	sys->setApplication(&app);							//notify OmegaLib's sys mngr of the app
	
	/* Setup data structures for data that will be manipulated before relinquishing control to OmegaLib*/

	sys->run();											//Pass control to OmegaLib's sys mngr

	sys->cleanup();										//cleanup
}
