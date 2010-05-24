/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "ogl.h"

/**********************************************************************************************************************
 */
extern void (*gUpdateCallback)(float dt);

/**********************************************************************************************************************
 * GLUT display function callback.
 */
void displayCallback(void) 
{
	static float lt = 0.0f;
	if(gUpdateCallback)
	{
		// Compute dt.
		float t = (float)((double)clock() / CLOCKS_PER_SEC);
		float dt = t - lt;
		lt = t;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gUpdateCallback(dt);
		glutPostRedisplay();
	}
}

/**********************************************************************************************************************
*/
void oglInit(const OMConfig* cfg)
{
	char* argv = "undefined";
	int argcp = 1;

	glutInit(&argcp, &argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(cfg->width,cfg->height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(cfg->appName); 

	glutDisplayFunc(displayCallback); 
}

/**********************************************************************************************************************
*/
void oglCleanup()
{
}

/**********************************************************************************************************************
*/
void oglStart()
{
	glutMainLoop();
}

/**********************************************************************************************************************
*/
void oglStop()
{
}

