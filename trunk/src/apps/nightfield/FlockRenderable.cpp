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
#include "Flock.h"

OMEGA_DEFINE_TYPE(FlockRenderable, SceneRenderable)

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockRenderable::initialize()
{
	//myCurrentPreset = mySettings.presets[0];

	//myFontMng = new FontManager();

	GpuManager* gpu = getClient()->getGpu();

	ImageData img;
	if(ImageUtils::loadImage("images/glow2.png", &img))
	{
		ImageData& img = myOwner->getAgentImage();
		myAgentTexture = gpu->createTexture(
			img.filename,
			img.width,
			img.height,
			img.data);
	}

	gpu->loadFragmentShader("smoke", "shaders/smoke.frag");

	// Create the gpu buffers and constants.
	int bufSize = myOwner->getSettings().numAgents * sizeof(Agent);

	myAgentBuffer = new VertexBuffer(gpu);
	myAgentBuffer->addAttribute(VertexAttribute(VertexAttribute::TargetPosition, VertexAttribute::TypeFloat, 0, 3));
	myAgentBuffer->initialize(bufSize, sizeof(Agent), myOwner->getAgents());

	myLightPos = new GpuConstant();
	myLightPos->setName("lightpos");
	myLightPos->setFloatValue(1.0f, 1.0f);

	// Setup data and parameters for the agent render program
	myAgentRenderer = new GpuProgram(gpu);
	//myAgentRenderer->setFragmentShader(myGpu->getFragmentShader("smoke"));
	myAgentRenderParams.setParam(0, myAgentBuffer);
	//myAgentRenderer->setInput(1, myLightPos);
	//myAgentRenderer->setNumRenderItems(mySettings.numAgents);
	myAgentRenderer->initialize();
	myAgentRenderOptions.items = myOwner->getSettings().numAgents;
	myAgentRenderOptions.primType = RenderStageOptions::PrimPoints;

	//myNumTouches = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockRenderable::dispose()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockRenderable::refresh()
{
	int dataSize = myOwner->getSettings().numAgents * sizeof(Agent);
	myAgentBuffer->write(myOwner->getAgents(), 0, dataSize);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockRenderable::draw(RenderState* state)
{
	if(state->isFlagSet(RenderPass::RenderOpaque))
	{
		// We don't use lighting for this application.
		glDisable(GL_LIGHTING);
		glEnable(GL_FOG);

		//if(myCurrentPreset.useFog)
		{
			//glDisable(GL_FOG);
			const float fogCol[] = { 0.6f, 0.6f, 0.8f, 0.0f };
			glFogfv( GL_FOG_COLOR, fogCol );
			glFogi(GL_FOG_MODE, GL_LINEAR);
			glFogf(GL_FOG_START, 0);
			glFogf(GL_FOG_END, 2);
		}
		//else
		{
			glEnable(GL_FOG);
		}

		glColor3f(1.0, 1.0, 1.0);

		//glTranslatef(mySettings.center[0], mySettings.center[1], mySettings.center[2]);
		//glRotatef(myRotateX / 3, 0, 1, 0);
		//glRotatef(myRotateY / 3, 1, 0, 0);
		//glTranslatef(-mySettings.center[0], -mySettings.center[1], -mySettings.center[2]);

		//GfxUtils::beginOverlayMode(context);
		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		//glDisable(GL_TEXTURE_2D);

		//GfxUtils::drawVGradient(Vector2i(0, context.viewportHeight - 100), Vector2i(context.viewportWidth, 100), Color(0, 0, 0), Color(50, 50, 60), 0.3);
		glDisable(GL_DEPTH_TEST);
		//glDisable(GL_LIGHTING);
		//GfxUtils::endOverlayMode();

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_POINT_SPRITE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myAgentTexture->getGLTexture());
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		//int ps = context.viewportHeight < context.viewportWidth ? context.viewportHeight / 8 : context.viewportWidth / 8;
		int ps = 32;
		glPointSize(ps);

		myAgentRenderer->runRenderStage(myAgentRenderOptions, &myAgentRenderParams);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}
