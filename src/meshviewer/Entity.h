/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *--------------------------------------------------------------------------------------------------------------------- 
 *********************************************************************************************************************/
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "omega.h"
#include "omega/scene.h"

// Using namespace declarations in a header file make emokitteh cry
// but we'll accept them here since it's application-level code, and we can have
// declarations shorter and more readable for the sake of clarity.
using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Entity
{
public:
private:
	Mesh* myMesh;
};

#endif