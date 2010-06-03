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
#include "SystemManager.h"
#include "Config.h"
#include "GLUTDisplaySystem.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned int GLUTDisplaySystem::Id = OID("GLUT");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void displayCallback(void) 
{
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

		app->Update(dt);
		app->Draw(dt);

		glFlush();
		glutPostRedisplay();

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
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLUTDisplaySystem::~GLUTDisplaySystem()
{
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
float GLUTDisplaySystem::GetValue(DisplayParam param)
{
	switch(param)
	{
	case DisplaySystem::GlobalHeight:
	case DisplaySystem::LocalHeight:
		return glutGet(GLUT_WINDOW_HEIGHT);
		break;
	case DisplaySystem::GlobalWidth:
	case DisplaySystem::LocalWidth:
		return glutGet(GLUT_WINDOW_WIDTH);
		break;
	default:
		Log::Warning("GLUTDisplaySystem::GetValue: unsupported display param, %d", param);
		return 0;
	}
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
