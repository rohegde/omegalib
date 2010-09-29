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
 * Input filter definition
 *********************************************************************************************************************/
#ifndef __INPUT_FILTER_H__
#define __INPUT_FILTER_H__

#include "osystem.h"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
struct InputEvent;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class InputFilter
{
friend class InputManager;
public:
	enum FilterPriority { PriorityLow, PriorityMedium, PriorityHigh };

public:
	// Class constructor
	InputFilter(): myManager(NULL) {}

   // Class destructor
	virtual ~InputFilter() {}

	InputManager* getManager() { return myManager; }

	virtual void setup(Setting& settings) {}
	virtual void initialize() {}
	virtual void start() {}
	virtual void poll() {}
	virtual void stop() {}
	virtual void dispose() {}

protected:

private:
	void setManager(InputManager* mng) { myManager = mng; }

private:
	InputManager* myManager;
};
}; // namespace omega

#endif