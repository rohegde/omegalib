/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __RENDER_PASS_H__
#define __RENDER_PASS_H__

#include "osystem.h"
#include "omega/ApplicationBase.h"
#include "DrawInterface.h"

namespace omega {
	class Renderer;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API RenderPass: public ReferenceType
	{
	public:
		enum RenderFlags { 
			RenderOpaque = 1 << 1, 
			RenderTransparent = 1 << 2,
			RenderOverlay = 1 << 3,
			RenderCustom = 1 << 8 };

	public:
		RenderPass(Renderer* client, const String& name): 
		  myInitialized(false), myDisposeRequested(false), myClient(client), myName(name), myCameraMask(0)
		  {}
		virtual ~RenderPass()
		{ ofmsg("~RenderPass %1%", %myName); }

		virtual void initialize() { myInitialized = true; }
		virtual void render(Renderer* client, const DrawContext& context) = 0;
		virtual void dispose() {}
		void requestDispose() { myDisposeRequested = true; }
		bool needsDispose() { return myDisposeRequested; }

		const String& getName() { return myName; }

		void setUserData(void* value) { myUserData = value; }
		void* getUserData() { return myUserData; }

		bool isInitialized() { return myInitialized; }

		Renderer* getClient() { return myClient; }

		void setCameraMask(uint mask) { myCameraMask = mask; }
		uint getCameraMask() { return myCameraMask; }

	private: 
		bool myInitialized;
		bool myDisposeRequested;
		void* myUserData;
		String myName;
		Renderer* myClient;
		unsigned int myCameraMask;
	};

}; // namespace omega

#endif