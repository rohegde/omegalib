/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * omega functions implementation. See omega.h for more details.
 *********************************************************************************************************************/
#include "Application.h"
#include "InputManager.h"
#include "SystemManager.h"
#include "Config.h"
#include "GLUTDisplaySystem.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned int GLUTDisplaySystem::Id = OID("GLUT");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void displayCallback(void) 
{
	static DrawContext context;
	static float lt = 0.0f;
	Application* app = SystemManager::GetInstance()->GetApplication();
	if(app)
	{
		// Compute dt.
		float t = (float)((double)clock() / CLOCKS_PER_SEC);
		float dt = t - lt;
		lt = t;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Setup stuff to maintain consistency across different display systems (i.e. Equalizer).
		// These settings make sure that even when switching display systems, the scene will be
		// rendered similarly (even if not exactly the same)
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective (50, 1, 0.1, 10);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 2.0f, 0, 0, 0, 0, 1.0f, 0);

		// Process events.
		InputManager* im = SystemManager::GetInstance()->GetInputManager();
		im->ProcessEvents(app);	
		app->Update(dt);

		// Setup the context viewport.
		context.viewportX = 0;
		context.viewportY = 0;
		context.viewportWidth = glutGet(GLUT_WINDOW_WIDTH);
		context.viewportHeight = glutGet(GLUT_WINDOW_HEIGHT);

		for(int layer = 0; layer < Application::MaxLayers; layer++)
		{
			if(SystemManager::GetInstance()->GetDisplaySystem()->IsLayerEnabled(layer, 0))
			{
				context.layer = layer;
				app->Draw(context);
			}
		}

		glFlush();
		glutPostRedisplay();

		// Poll the input manager for new events.
		SystemManager::GetInstance()->GetInputManager()->Poll();

		if(SystemManager::GetInstance()->IsExitRequested())
		{
			exit(0);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLUTDisplaySystem::GLUTDisplaySystem():
	mySys(NULL)
{
	myLayerEnabled = new bool[Application::MaxLayers];
	memset(myLayerEnabled, 0, Application::MaxLayers * sizeof(bool));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLUTDisplaySystem::~GLUTDisplaySystem()
{
	delete myLayerEnabled;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GLUTDisplaySystem::Initialize(SystemManager* sys)
{
	mySys = sys;

	char* argv = "";
	int argcp = 1;

	glutInit(&argcp, &argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(sys->GetApplication()->GetName()); 

	glutDisplayFunc(displayCallback); 

	// Enable lighting by default to keep consistent with equalizer rendering)
	//glEnable(GL_LIGHTING);
	glEnable (GL_DEPTH_TEST);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GLUTDisplaySystem::Run()
{
	glutMainLoop();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GLUTDisplaySystem::Cleanup()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GLUTDisplaySystem::SetLayerEnabled(int layerNum, const char* viewName, bool enabled)
{
	myLayerEnabled[layerNum] = enabled;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GLUTDisplaySystem::IsLayerEnabled(int layerNum, const char* viewName)
{
	return myLayerEnabled[layerNum];
}

