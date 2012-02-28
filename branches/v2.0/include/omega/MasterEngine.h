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
#ifndef __MASTER_ENGINE_H__
#define __MASTER_ENGINE_H__

#include "ServerEngine.h"

namespace omega {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API MasterEngine: public ServerEngine
	{
	public:
		typedef List<Camera*> CameraCollection;

	public:
		static const int MaxActivePointers = 128;
		static MasterEngine* instance() { return mysInstance; }

	public:
		MasterEngine(ApplicationBase* app);

		void addInteractive(InteractiveBase* ib);
		void removeInteractive(InteractiveBase* ib);

		Camera* getDefaultCamera();
		Camera* createCamera(uint flags = Camera::DefaultFlags);
		void destroyCamera(Camera* cam);
		CameraCollection::Range getCameras();
		CameraCollection::ConstRange getCameras() const;

		virtual void initialize();
		//! Internal method.
		void clientInitialize(Renderer* client);

		virtual void finalize();
		virtual void handleEvent(const Event& evt);
		virtual void update(const UpdateContext& context);

	private:
		static MasterEngine* mysInstance;

		// Actors
		List<InteractiveBase*> myInteractives;

		// Cameras.
		Camera* myDefaultCamera;
		CameraCollection myCameras;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Camera* MasterEngine::getDefaultCamera()
	{ return myDefaultCamera; }
}; // namespace omega

#endif
