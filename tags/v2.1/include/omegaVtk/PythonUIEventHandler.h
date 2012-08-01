/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __PYTHON_UI_EVENT_HANDLER_H__
#define __PYTHON_UI_EVENT_HANDLER_H__

#include "omegaVtk/ovtkbase.h"

#include "omega/osystem.h"

//namespace omegaVtk
//{
//	using namespace omega;
//	using namespace omega::ui;
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////
//	class PythonUIEventHandler: public IEventListener
//	{
//	public:
//		PythonUIEventHandler(UIManager* ui, PythonInterpreter* interp);
//
//		virtual void handleEvent(const Event& evt);
//
//		void setClickCommand(const String& value);
//		const String& getClickCommand();
//
//		void setChangeValueCommand(const String& value);
//		const String& getChangeValueCommand();
//
//		void setSliderMaxValue(float value);
//		float getSliderMaxValue();
//
//		void setSliderMinValue(float value);
//		float getSliderMinValue();
//
//		void setSliderStep(float value);
//		float getSliderStep();
//
//	private:
//		String myClickCommand;
//		String myChangeValueCommand;
//		UIManager* myUI;
//		PythonInterpreter* myInterpreter;
//		float mySliderMaxValue;
//		float mySliderMinValue;
//		float mySliderStep;
//	};
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////
//	inline void PythonUIEventHandler::setClickCommand(const String& value)
//	{ myClickCommand = value; }
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////
//	inline const String& PythonUIEventHandler::getClickCommand()
//	{ return myClickCommand; }
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////
//	inline void PythonUIEventHandler::setChangeValueCommand(const String& value)
//	{ myChangeValueCommand = value; }
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////
//	inline const String& PythonUIEventHandler::getChangeValueCommand()
//	{ return myChangeValueCommand; }
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////
//	inline void PythonUIEventHandler::setSliderMaxValue(float value)
//	{ mySliderMaxValue = value; }
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////
//	inline float PythonUIEventHandler::getSliderMaxValue()
//	{ return mySliderMaxValue; }
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////
//	inline void PythonUIEventHandler::setSliderMinValue(float value)
//	{ mySliderMinValue = value; }
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////
//	inline float PythonUIEventHandler::getSliderMinValue()
//	{ return mySliderMinValue; }
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////
//	inline void PythonUIEventHandler::setSliderStep(float value)
//	{ mySliderStep = value; }
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////
//	inline float PythonUIEventHandler::getSliderStep()
//	{ return mySliderStep; }
//};
#endif