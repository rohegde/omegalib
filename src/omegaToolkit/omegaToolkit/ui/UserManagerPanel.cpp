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
#include "omega/ServerEngine.h"
#include "omegaToolkit/UiModule.h"
#include "omegaToolkit/ui/UserManagerPanel.h"
#include "omegaToolkit/ui/WidgetFactory.h"

#ifdef OMEGA_USE_OPENNI
#include "omega/OpenNIService.h"
#endif

using namespace omega;
using namespace omegaToolkit;
using namespace omegaToolkit::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
void UserManagerPanel::initialize(Container* owner, const String& openNIServiceName, const String& observerUpdateServiceName)
{
	ServiceManager* sm = SystemManager::instance()->getServiceManager();
	myObserverUpdateService =  sm->findService<ObserverUpdateService>(observerUpdateServiceName);

	owner->addChild(this);
	WidgetFactory* wf = UiModule::instance()->getWidgetFactory();

#ifdef OMEGA_USE_OPENNI
	myOpenNIService = sm->findService<OpenNIService>(openNIServiceName);
	this->setLayout(Container::LayoutHorizontal);
	setVerticalAlign(Container::AlignBottom);

	myDepthImage = wf->createImage("kinectDepthBuffer", this);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UserManagerPanel::update(const UpdateContext& context)
{
#ifdef OMEGA_USE_OPENNI
	OpenNIService* svc = (OpenNIService*)myOpenNIService;

	if( myDepthImageData == NULL ) 
	{
		myDepthImageData = new PixelData(PixelData::FormatRgb, svc->getImageDataWidth(), svc->getImageDataHeight(), (byte*)svc->getDepthImageData());
		myDepthImage->setData(myDepthImageData);
	}
	myDepthImage->refresh();
#endif
}
