/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * GLUTDisplaySystem class declaration.
 *********************************************************************************************************************/
#ifdef false
#ifndef __GLUT_DISPLAY_SYSTEM_H__
#define __GLUT_DISPLAY_SYSTEM_H__

#include "osystem.h"
#include "DisplaySystem.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Implements a display system based on GLUT, offering a single render window and mouse input support.
	//! NOTE: Usage of this class is now deprecated in favor of EqualizerDisplaySystem. GLUTDisplaySystem can be used 
	//! as not a simple reference on how to implement a custom display system but is not updated regularly or tested, 
	//! and  may  work as espected.
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
#endif