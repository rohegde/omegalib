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
#include "eqinternal.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
ViewProxy::ViewProxy(ViewImpl* view): 
	myView(view) 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ViewProxy::serialize( co::DataOStream& os, const uint64_t dirtyBits )
{
	if( dirtyBits & DIRTY_LAYER )
	{
		for(int i = 0; i < Application::MaxLayers; i++)
		{
			os << myView->myLayer;
		}
	}
	if( dirtyBits & DIRTY_DRAW_STATS )
	{
		os << myView->myDrawStatistics;
	}
	if( dirtyBits & DIRTY_DRAW_FPS )
	{
		os << myView->myDrawFps;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ViewProxy::deserialize( co::DataIStream& is, const uint64_t dirtyBits )
{
	if( dirtyBits & DIRTY_LAYER )
	{
		for(int i = 0; i < Application::MaxLayers; i++)
		{
			is >> myView->myLayer;
		}
	}
	if( dirtyBits & DIRTY_DRAW_STATS )
	{
		is >> myView->myDrawStatistics;
	}
	if( dirtyBits & DIRTY_DRAW_FPS )
	{
		is >> myView->myDrawFps;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ViewProxy::notifyNewVersion() 
{ 
	sync(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ViewImpl::ViewImpl(eq::Layout* parent): 
	eq::View(parent)
#pragma warning( push )
#pragma warning( disable : 4355 )
	, myProxy( this )
#pragma warning( pop )
{
	myDrawStatistics = false;
	myDrawFps = false;
	myLayer = Layer::Null;
	setUserData(&myProxy);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ViewImpl::~ViewImpl()
{
	this->setUserData(NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Layer::Enum ViewImpl::getLayer() 
{ 
	return myLayer; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ViewImpl::setLayer(Layer::Enum layer) 
{ 
	myLayer = layer; 
	myProxy.setDirty( ViewProxy::DIRTY_LAYER );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ViewImpl::drawStatistics(bool enable)
{
	myDrawStatistics = enable;
	myProxy.setDirty( ViewProxy::DIRTY_DRAW_STATS );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ViewImpl::isDrawStatisticsEnabled()
{
	return myDrawStatistics;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ViewImpl::drawFps(bool enable)
{
	myDrawFps = enable;
	myProxy.setDirty( ViewProxy::DIRTY_DRAW_FPS );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ViewImpl::isDrawFpsEnabled()
{
	return myDrawFps;
}
