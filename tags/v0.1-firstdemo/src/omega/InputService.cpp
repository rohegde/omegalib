/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * InputService members implementation. See InputManager.h for more details.
 *********************************************************************************************************************/
#include "omega/InputService.h"
#include "omega/InputManager.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputService::LockEvents() 
{ 
	myManager->LockEvents(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputService::UnlockEvents() 
{ 
	myManager->UnlockEvents(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputEvent* InputService::WriteHead()
{ 
	return myManager->WriteHead(); 

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputEvent* InputService::ReadHead()
{ 
	return myManager->ReadHead();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputEvent* InputService::ReadTail()
{ 
	return myManager->ReadTail(); 
}