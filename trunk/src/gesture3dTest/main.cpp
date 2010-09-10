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
#include <vector>

using namespace omega;
using namespace outk::gfx;
using namespace outk::ui;

//#define LAPTOP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TestApplication: public Application, IUIEventHandler
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TestApplication()
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void initialize()
	{
		myFontMng = new FontManager();
		myFontMng->createFont("arial30", "../../data/fonts/Arial.ttf", 22);

		myTexMng = new TextureManager();
		myTexMng->loadTexture("mario", "../../data/images/mario.png");

		Texture* tx = myTexMng->getTexture("mario");

		myFont = myFontMng->getFont("arial30");

		myUI = new UIManager();
		myUI->setEventHandler(this);
		myUI->setDefaultFont(myFont);

		WidgetFactory* wf = myUI->getWidgetFactory();

		Image* i1 = wf->createImage("i1", myUI->getRootWidget());
		i1->setTexture(tx);
		i1->setRotation(-20);
		i1->setUserMoveEnabled(true);
		i1->setPosition(Vector2f(400, 400));
		i1->setSize(Vector2f(300, 300));


		Box* b1 = wf->createBox("box1", myUI->getRootWidget(), Box::LayoutHorizontal);
		b1->setUserMoveEnabled(true);
		b1->setDebugModeEnabled(true);
		b1->setPosition(Vector2f(100, 100));
		b1->setSize(Vector2f(400, 100));
		b1->setRotation(20);

		Box* b2 = wf->createBox("box2", b1, Box::LayoutVertical);

		Box* b3 = wf->createBox("box3", b1, Box::LayoutVertical);
		b3->setUserMoveEnabled(true);
		b3->setDebugModeEnabled(true);
		b3->setPadding(2);

		Box* b4 = wf->createBox("box4", b3, Box::LayoutHorizontal);
		Box* b5 = wf->createBox("box5", b3, Box::LayoutHorizontal);
		Box* b6 = wf->createBox("box6", b3, Box::LayoutHorizontal);

		b1->layoutChildren();
		b2->layoutChildren();
		b3->layoutChildren();

		Button* l1 = wf->createButton("L1", b4);
		l1->setText("Hello Button!");
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void draw3D(const DrawContext& context)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		//const float lightPos[] = { 0.0f, 1.6f, 0.0f, 0.0f };
#ifdef LAPTOP
		const float lightPos[] = { 0, 1.8, 0, 1.0f };
		const float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		const float lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
		glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0 );
#else
		const float lightPos[] = { lx, ly, lz, 1.0f };
		const float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		const float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.6 );
#endif
		glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

		glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse );


		glEnable(GL_FOG);

		const float fogCol[] = { 0.0f, 0.0f, 0.0f };
		glFogfv( GL_FOG_COLOR, fogCol );
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, 1);
		glFogf(GL_FOG_END, 7);

		glColor3f( 1.0, 1.0, 0.0);
		glPushMatrix();
		glTranslatef(0.0, 1.7, -0.1);
		glutSolidCube( 0.1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(mx, my, mz);
		glRotatef(ry , 0, 1, 0);
		glRotatef(rz, 0, 0, 1);
		glRotatef(rx, 1, 0, 0);
		glTranslatef(0, 0.1f, 0);
		glutWireCone( 0.05, 0.1, 4, 5);
		glPopMatrix();

		glColor3f(1.0, 1.0, 1.0);
		//DrawRoom(0.0f, 1.9f, -0.407f, 0.61f, 0, -3);
		drawRoom(0.3f, 2.0f, -0.59f, 0.59f, 0, -3);
		drawCubes();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	void draw2D(const DrawContext& context)
	{
		GfxUtils::beginOverlayMode(context);

		glColor4f (1.0f, 0.2f, 0.2f, 1.0f);
		glPointSize (8.0);
		glBegin(GL_POINTS);
			glVertex2f(x, y);    // lower left vertex
		glEnd();

		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		myUI->draw(context);

		GfxUtils::endOverlayMode();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	 *	This function will go through the myCubes list and draw all corresponding cubes
	 */
	void drawCubes()
	{
		for ( int i = 0; i < myCubes.size(); i++)
		{
			glColor3f( 1.0, 1.0, 0.0);
			glPushMatrix();
			glTranslatef( myCubes[i][0], myCubes[i][1], myCubes[i][2]);
			glutSolidCube( 0.05);
			glPopMatrix();
		}
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
		myUI->update(context);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool handleEvent(const InputEvent& evt)
	{
		switch(evt.serviceType)
		{
		case InputService::Touch:
		case InputService::Pointer:
			x = evt.position[0];
			y = evt.position[1];	
			myUI->processInputEvent(evt);
		break;

		case InputService::Mocap:
		{
#ifdef LAPTOP
			Observer* obs = getDisplaySystem()->getObserver(0);
			obs->update(evt.position, evt.rotation);
#else
			if(evt.sourceId == 1)
			{
				if(evt.position[0] != 0 || evt.position[1] != 0)
				{
					//I need to offset the observer position by the original camera position set in the config file for the z-axis
					//for now I'll cheat instead of actually reading the config
					Vector3f peiceOfCrap;
					for( int i = 0; i < 3; i++)
					{
						peiceOfCrap[i] = evt.position[i];
					}
					peiceOfCrap[2] += 2.0;
					Observer* obs = getDisplaySystem()->getObserver(0);
					obs->update(peiceOfCrap, evt.rotation);
				}
			}
			else if(evt.sourceId == 3) //glove
			{
				if(evt.position[0] != 0 || evt.position[1] != 0)
				{
					lx = evt.position[0];
					ly = evt.position[1];
					lz = evt.position[2];
				}
			}
			else if(evt.sourceId ==2) //handheld
			{
				if(evt.position[0] != 0 || evt.position[1] != 0)
				{
					mx = evt.position[0];
					my = evt.position[1];
					mz = evt.position[2];
					rx = evt.rotation[0];
					ry = evt.rotation[1];
					rz = evt.rotation[2];

					//myCubes
				}
			}
#endif
			break;

			break;
		}
		default: break;
		}
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void handleUIEvent(const UIEvent& evt)
	{
		printf("click yay!\n");
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

	std::vector<Vector3f> myCubes;
	//int myIndex = 0;



	UIManager* myUI;
	FontManager* myFontMng;
	TextureManager* myTexMng;
	Font* myFont;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	Config* cfg = new Config("../../data/gesture3d.cfg");

	SystemManager* sys = SystemManager::instance();
	sys->setup(cfg);

	TestApplication app;
	sys->setApplication(&app);
	sys->run();

	sys->cleanup();
}
