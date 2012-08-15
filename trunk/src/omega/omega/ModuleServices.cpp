/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/ModuleServices.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
EngineModule::~EngineModule()
{
	ofmsg("~EngineModule %1%", %myName);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineModule::doInitialize(Engine* server) 
{ 
	myEngine = server; 
	if(!myInitialized) 
	{
		initialize(); 
		foreach(Renderer* r, server->getClients())
		{
			initializeRenderer(r);
		}

		SharedDataServices::registerObject(this, myName);
		myInitialized = true; 
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ModuleServices::addModule(EngineModule* module)
{ 
	mysModules.push_back(module); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ModuleServices::update(Engine* srv, const UpdateContext& context)
{
	foreach(EngineModule* module, mysModules)
	{
		module->doInitialize(srv);
		module->update(context);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ModuleServices::handleEvent(const Event& evt)
{
	for(int i = EngineModule::PriorityHigh; i >= EngineModule::PriorityLow; i--)
	{
		foreach(EngineModule* module, mysModules)
		{
			// Only send events to initialized modules.
			if(module->isInitialized())
			{
				if(module->getPriority() == i)
				{
					module->handleEvent(evt);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ModuleServices::handleCommand(const String& cmd)
{
	for(int i = EngineModule::PriorityHigh; i >= EngineModule::PriorityLow; i--)
	{
		foreach(EngineModule* module, mysModules)
		{
			// Only send commands to initialized modules.
			if(module->isInitialized())
			{
				if(module->getPriority() == i)
				{
					module->handleCommand(cmd);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//void ModuleServices::initializeRenderer(Engine* srv, Renderer* r)
//{
//	foreach(EngineModule* module, mysModules)
//	{
//		//module->doInitialize(srv);
//		//module->initializeRenderer(r);
//	}
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ModuleServices::disposeAll()
{
	foreach(EngineModule* module, mysModules)
	{
		module->dispose();
	}
	mysModules.clear();
}

//static void preDraw(Engine* srv, Renderer* r, const DrawContext& context)
//{
//	foreach(EngineModule* module, mysModules)
//	{
//		module->doInitialize(srv);
//		module->preDraw(r, context);
//	}
//}

//static void postDraw(Engine* srv, Renderer* r, const DrawContext& context)
//{
//	foreach(EngineModule* module, mysModules)
//	{
//		module->doInitialize(srv);
//		module->postDraw(r, context);
//	}
//}
