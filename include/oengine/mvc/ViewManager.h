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
#ifndef __VIEW_MANAGER_H__
#define __VIEW_MANAGER_H__

#include "oengine/mvc/View.h"
#include "oengine/EngineClient.h"

namespace oengine { namespace mvc {

	typedef ViewClient* (*ViewClientNew)(View* owner, EngineClient* client);

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API ViewManager
	{
	OMEGA_DECLARE_TYPE(ViewManager);
	public:
		ViewManager();

		template<typename T> void registerViewClientClass();
		void addClient(EngineClient*);

		void update(const UpdateContext& context) {}

	private:
		List<EngineClient*> myClients;
		List<View*> myViews;
		Dictionary<String, ViewClientNew> myViewClientClasses;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T> 
	inline void ViewManager::registerViewClientClass()
	{
		myViewClientClasses[T::Type->getName()] = T::New;
	}
}; }; // namespace oengine

#endif