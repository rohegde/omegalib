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
#include "omega/RenderTarget.h"
#include "omega/GpuManager.h"
#include "omega/StringUtils.h"

#include "eqinternal.h"

using namespace omega;
using namespace co::base;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
WindowImpl::WindowImpl(eq::Pipe* parent): 
	eq::Window(parent), myFrameBuffer(NULL), myInitialized(false)
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
WindowImpl::~WindowImpl() 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool WindowImpl::configInitGL(const uint128_t& initID)
{
	// Initialize this window frame buffer.
	bool res = Window::configInitGL(initID);
	const eq::fabric::PixelViewport& pvp = getPixelViewport();
	myFrameBuffer = new RenderTarget();
	myFrameBuffer->initialize(RenderTarget::TypeFrameBuffer, pvp.w, pvp.h);

	// Get the gpu manager from the client instance.
	PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
	ApplicationClient* client = pipe->getClient();
	myGpu = client->getGpu();

	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void WindowImpl::initialize()
{
	String name = getName();
	vector<String> args = StringUtils::split(name, "x,");
	myIndex = Vector2i(atoi(args[0].c_str()), atoi(args[1].c_str()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void WindowImpl::frameStart(const uint128_t &frameID, const uint32_t frameNumber) 
{
	if(!myInitialized)
	{
		initialize();
		myInitialized = true;
	}
	// Set the frame buffer for the client gpu to this window frame buffer.
	//myGpu->setFrameBuffer(myFrameBuffer);
}
