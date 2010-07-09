/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *  [PLACE YOUR NAME AND MAIL HERE IF YOU CONTRIBUTED TO WRITE THIS SOURCE FILE]
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

	OMEGA_API virtual void initialize(SystemManager* sys); 
	OMEGA_API virtual void run(); 
	OMEGA_API virtual void cleanup(); 

	// Layer and view management.
	OMEGA_API virtual void setLayerEnabled(int layerNum, const char* viewName, bool enabled);
	OMEGA_API virtual bool isLayerEnabled(int layerNum, const char* viewName);

	OMEGA_API virtual unsigned int getId() { return Id; }

public:
	static const unsigned int Id; 

private:
	SystemManager* mySys;
	bool* myLayerEnabled;
};

}; // namespace omega

#endif