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

		app->Update(dt);
		app->Draw(dt);

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
