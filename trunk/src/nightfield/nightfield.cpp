/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Test executable
 *********************************************************************************************************************/
#include "NightfieldApplication.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	ologopen("nightfield-log.txt");

	Config* cfg = new Config(argv[1]);

	SystemManager* sys = SystemManager::instance();
	sys->setup(cfg);

	NightfieldApplication app;
	sys->setApplication(&app);

	sys->initialize();

	// Set the initial observer position (do we still need this - can't we specify it in the settings file?)
	Observer* obs = sys->getDisplaySystem()->getObserver(0);
	obs->update(Vector3f(0, 1.6f, 0), Vector3f(0, 0, 0));

	sys->run();

	sys->cleanup();

	ologclose();
}
