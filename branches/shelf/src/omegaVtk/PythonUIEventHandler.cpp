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
#include "omegaVtk/PythonUIEventHandler.h"
#include "omegaToolkit/ui/AbstractButton.h"
#include "omegaToolkit/ui/Slider.h"

//using namespace omegaVtk;

///////////////////////////////////////////////////////////////////////////////////////////////////
//PythonUIEventHandler::PythonUIEventHandler(UIManager* ui, PythonInterpreter* interp):
//	myInterpreter(interp),
//	myUI(ui)
//{
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
//void PythonUIEventHandler::handleEvent(const Event& evt)
//{
//	if(evt.getType() == Event::Click)
//	{
//		myInterpreter->eval(myClickCommand);
//	}
//	else if(evt.getType() == Event::Toggle)
//	{
//		AbstractButton* btn = myUI->getWidgetById<AbstractButton>(evt.getSourceId());
//		String expr = StringUtils::replaceAll(myChangeValueCommand, "${value}", ostr("%1%", %btn->isChecked()));
//		myInterpreter->eval(expr);
//	}
//	else if(evt.getType() == Event::ChangeValue)
//	{
//		Slider* sld = myUI->getWidgetById<Slider>(evt.getSourceId());
//		float interval = mySliderMaxValue - mySliderMinValue;
//		float value = ((float)sld->getValue() / sld->getTicks() * interval) + mySliderMinValue;
//		String expr = StringUtils::replaceAll(myChangeValueCommand, "${value}", ostr("%1%", %value));
//		myInterpreter->eval(expr);
//	}
//}
//
