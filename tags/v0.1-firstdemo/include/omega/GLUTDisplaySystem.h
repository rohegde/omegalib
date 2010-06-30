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
	OMEGA_API GLUTDisplaySystem();
	OMEGA_API virtual ~GLUTDisplaySystem();

	OMEGA_API virtual void Initialize(SystemManager* sys); 
	OMEGA_API virtual void Run(); 
	OMEGA_API virtual void Cleanup(); 

	// Layer and view management.
	OMEGA_API virtual void SetLayerEnabled(int layerNum, const char* viewName, bool enabled);
	OMEGA_API virtual bool IsLayerEnabled(int layerNum, const char* viewName);

	OMEGA_API virtual unsigned int GetId() { return Id; }

public:
	static const unsigned int Id; 

private:
	SystemManager* mySys;
	bool* myLayerEnabled;
};

}; // namespace omega

#endif