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
#ifndef __VIEW_H__
#define __VIEW_H__

#include "omega/Application.h"
#include "omega/mvc/Model.h"
#include "omega/mvc/Controller.h"

namespace omega { namespace mvc {
	class ViewClient;
	class Controller;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OUTILS_API View: public IModelListener
	{
	OMEGA_DECLARE_TYPE(View);
	public:
		View();

		void update(const UpdateContext& context) {}

		void setModel(Model* value);
		Model* getModel();

		void attachController(Controller* controller);
		void detachController(Controller* controller);

		void attachClient(ViewClient* client);
		void detachClient(ViewClient* client);

		virtual void onModelStateChanged(Model* model);
		virtual void onModelOperationProgress(Model* model, int progress);

	protected:
		//virtual void create();
		//virtual void destroy();
		
	private:
		Model* myModel;
		List<Controller*> myControllers;
		List<ViewClient*> myClients;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Model* View::getModel()
	{ return myModel; }
}; }; // namespace omega

#endif