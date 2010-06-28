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
#ifndef __GLUT_DISPLAY_SYSTEM_H__
#define __GLUT_DISPLAY_SYSTEM_H__

#include "osystem.h"
#include "DisplaySystem.h"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GLUTDisplaySystem: public DisplaySystem
{
public:
	GLUTDisplaySystem();
	virtual ~GLUTDisplaySystem();

	virtual void Initialize(SystemManager* sys); 
	virtual void Run(); 
	virtual void Cleanup(); 

	// Layer and view management.
	virtual void SetLayerEnabled(int layerNum, const char* viewName, bool enabled);
	virtual bool IsLayerEnabled(int layerNum, const char* viewName);

	virtual unsigned int GetId() { return Id; }

public:
	static const unsigned int Id; 

private:
	SystemManager* mySys;
	bool* myLayerEnabled;
};

}; // namespace omega

#endif