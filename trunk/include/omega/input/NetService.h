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
#ifndef __NET_SERVICE_H__
#define __NET_SERVICE_H__

#include "omega/osystem.h"
#include "omega/InputManager.h"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NetService: public InputService
{
public:
	// Allocator function
	static NetService* New() { return new NetService(); }

public:
	OMEGA_API virtual void initialize();
	OMEGA_API virtual void dispose();

private:
};

}; // namespace omega

#endif