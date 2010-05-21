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
	static float f = 0.0f;

	glRotated(dt * 16, 0, 1, 0);
	glRotated(dt * 32, 1, 0, 0);
	glRotated(dt * 8, 0, 0, 1);
	glutSolidCube(0.3);
	glFlush();
}

/**********************************************************************************************************************
*/
void main()
{
	//othreadCreate(test, NULL);
	//OMList* list = olistCreate();
	//olistInit(list, 4, "Node %d", 0);

	OMConfig cfg;

	omegaInit(&cfg);
	omegaSetUpdateCallback(update);
	omegaRun();
}
