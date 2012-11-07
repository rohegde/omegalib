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
#include "omega/SageManager.h"
#include "eqinternal/eqinternal.h"
#include "libsage.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
SageManager::SageManager(): EngineModule("SageManager"),
	mySageTile(NULL),
	mySail(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SageManager::~SageManager()
{
	// Make sure al resources have been deallocated.
	dispose();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SageManager::initialize()
{
	if(mySageTile == NULL)
	{
		owarn("SageManager::initialize: cannot initialize, tile not set");
	}
	else
	{
		sagePixFmt pixFmt;
		if(mySageTile->stereoMode == DisplayTileConfig::Mono || mySageTile->stereoMode == DisplayTileConfig::Default)
		{
			pixFmt = PIXFMT_8888;
		}
		else if(mySageTile->stereoMode == DisplayTileConfig::PixelInterleaved)
		{
			pixFmt = PIXFMT_888;
		}
		else
		{
			owarn("SageManager::initialize: unsupported tile stereo format, defaulting to mono");
			pixFmt = PIXFMT_888;
		}

		// Connect to free space manager and create a sail frame buffer
		mySail = createSAIL(
			SystemManager::instance()->getApplication()->getName(),
			mySageTile->pixelSize[0], mySageTile->pixelSize[1],
			pixFmt,
			myFsManagerAddress.c_str(),
			1, 60, 
			NULL);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SageManager::dispose()
{
	if(mySail != NULL)
	{
		omsg("SageManager::dispose: deleting SAIL buffer");
		deleteSAIL(mySail);
		mySail = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SageManager::setup(const Setting& s, DisplayConfig& dc)
{
	omsg("SageManager::setup");

	String mode = Config::getStringValue("mode", s, "disabled");
	StringUtils::toLowerCase(mode);
	if(mode == "disabled") myMode = Disabled;
	else if(mode == "singlenode") myMode = SingleNode;

	myFsManagerAddress = Config::getStringValue("fsManagerAddress", s, "localhost");
	ofmsg("\tfsManagerAddress = %1%", %myFsManagerAddress);

	// Setup single node mode
	if(myMode == SingleNode)
	{
		omsg("\tmode = SingleNode");
		String tileName = Config::getStringValue("tile", s);
		if(dc.tiles.find(tileName) == dc.tiles.end())
		{
			oferror("\tCould not find tile %1%", %tileName);
		}
		else
		{
			ofmsg("\ttile: %1%", %tileName);
			mySageTile = dc.tiles[tileName];
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SageManager::finishFrame(const DrawContext& context)
{
	if(myMode == SingleNode)
	{
		if(context.tile == mySageTile)
		{
			// We should be getting in here only through a single thread, but let's be safe.
			myLock.lock();

			// Grab data from the frame buffer and copy it to the SAGE frame buffer
			GLubyte *rgbBuffer = nextBuffer(mySail);
			glReadPixels(0, 0, mySageTile->pixelSize[0], mySageTile->pixelSize[1], GL_RGBA, GL_UNSIGNED_BYTE, rgbBuffer);	
			swapBuffer(mySail);

			myLock.unlock();
		}
	}
}


