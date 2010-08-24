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

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TestApplication: public Application
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void draw(DrawContext& context)
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool handleEvent(const InputEvent& evt)
	{
		return true;
	}

private:
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	SystemManager* sys = SystemManager::instance();

	Config* cfg = new Config("../../data/test.cfg");
	//cfg->setDisplayConfig("--eq-config ../../data/eqc/test.eqc");
	cfg->load();

	sys->setup(cfg);

	TestApplication app;
	sys->setApplication(&app);

	sys->initialize();

	sys->run();
	sys->cleanup();
}
