/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "eqinternal.h"

using namespace omega;
using namespace co::base;
using namespace std;

// Uncomment to print debug messages about client flow.
//#define OMEGA_DEBUG_FLOW

///////////////////////////////////////////////////////////////////////////////////////////////////
PipeImpl::PipeImpl(eq::Node* parent): 
	eq::Pipe(parent), myGpu(NULL), myClient(NULL), myNode((NodeImpl*)parent)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
RendererBase* PipeImpl::getClient() 
{ return myClient.get(); }

///////////////////////////////////////////////////////////////////////////////////////////////////
PipeImpl::~PipeImpl() 
{}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool PipeImpl::configInit(const uint128_t& initID)
{
	ApplicationBase* app = SystemManager::instance()->getApplication();
	if(app)
	{
		myClient = new Renderer(Engine::instance());
		myGpu = new GpuManager();
		myGpu->initialize();
		myGpuContext = new GpuContext(myGpu.get());
		myClient->setGpuContext(myGpuContext.get());
		myClient->initialize();

		return Pipe::configInit(initID);
	}
	// no application: fail initialization.
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PipeImpl::frameStart( const uint128_t& frameID, const uint32_t frameNumber )
{
	eq::Pipe::frameStart(frameID, frameNumber);

#ifdef OMEGA_DEBUG_FLOW
	ofmsg("PipeImpl::frameStart %1%", %frameNumber);
#endif

	// Activate the glew context for this pipe, so initialize and update client
	// methods can handle openGL buffers associated with this Pipe.
	// NOTE: getting the glew context from the first window is correct since all
	// windows attached to the same pape share the same Glew (and OpenGL) contexts.
	const GLEWContext* glewc = getWindows()[0]->glewGetContext();
	glewSetContext(glewc);

	myClient->startFrame(FrameInfo(frameID.low(), getGpuContext()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PipeImpl::frameFinish( const uint128_t& frameID, const uint32_t frameNumber )
{
	eq::Pipe::frameFinish(frameID, frameNumber);

#ifdef OMEGA_DEBUG_FLOW
	ofmsg("PipeImpl::frameFinish %1%", %frameNumber);
#endif

	myClient->finishFrame(FrameInfo(frameID.low(), getGpuContext()));
}