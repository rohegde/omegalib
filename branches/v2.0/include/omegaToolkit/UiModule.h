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
#ifndef __UI_MODULE_H__
#define __UI_MODULE_H__

#include "omega/ServerEngine.h"
#include "omega/Application.h"
#include "omegaToolkit/BoundingSphere.h"
#include "ui/Container.h"
#include "ui/WidgetFactory.h"

namespace omegaToolkit
{
	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API UiModule: public ServerModule
	{
	public:
		static const int MaxUis = 3;
		static UiModule* instance() { return mysInstance; }

	public:
		UiModule();
		~UiModule();

		void initialize(ServerEngine* server);
		void update(const UpdateContext& context);
		void handleEvent(const Event& evt);

		ServerEngine* getEngine() { return myEngine; }

		ui::Container* getUi(int id);
		ui::WidgetFactory* getWidgetFactory();

	private:
		static UiModule* mysInstance;

		ServerEngine* myEngine;
		ui::Container* myUi[MaxUis];
		ui::WidgetFactory* myWidgetFactory;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline ui::WidgetFactory* UiModule::getWidgetFactory()
	{ return myWidgetFactory; }
		
};
#endif