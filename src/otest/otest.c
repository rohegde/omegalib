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
#include "othread.h"
#include "olist.h"

unsigned OM_THREADPROC test(void* d)
{
	int i = 0;
	while(i < 1000)
	{
	printf("Hello World!\n");
	i++;
	}
}

/**********************************************************************************************************************
 */
void update(float dt) 
{
	int av;
	int ls;
	float rx;
	float ry;
	OMEvent evt;
	float lpos[] = {-0.5f, 1.0f, 0.0f, 1.0f};

	av = omegaAvailableEvents();
	ls = omegaLostEvents();

	glEnable (GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPushMatrix();
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, lpos); 
	glPopMatrix();

	if(av != 0)
	{
		omegaGetEvents(&evt, 1, 0);
	
		rx = evt.x;
		ry = evt.y;

		glLoadIdentity();
		glRotated(rx, 0, 1, 0);
		glRotated(ry, 1, 0, 0);
	}

	glutSolidTeapot(0.4);
	glFlush();

	//printf("available: %d    lost: %d    dt: %f\n", av, ls, dt);
}

/**********************************************************************************************************************
*/
void main()
{
	//othreadCreate(test, NULL);
	//OMList* list = olistCreate();
	//olistInit(list, 4, "Node %d", 0);

	OMConfig cfg;

	ocfgInitDefault(&cfg);

	omegaInit(&cfg);
	omegaSetUpdateCallback(update);
	omegaRun();
}
