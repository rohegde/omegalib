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

	virtual unsigned int GetId() { return Id; }

public:
	static const unsigned int Id; 

private:
	SystemManager* mySys;
};

}; // namespace omega

#endif