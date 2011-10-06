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
#include "oengine/UiRenderPass.h"
#include "oengine/EngineClient.h"
#include "oengine/EngineServer.h"
#include "oengine/Renderer.h"
#include "oengine/SceneNode.h"
#include "oengine/ui/Container.h"
#include "omega/StringUtils.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace oengine;

OMEGA_DEFINE_TYPE(UiRenderPass, RenderPass)

///////////////////////////////////////////////////////////////////////////////////////////////////
void UiRenderPass::render(EngineClient* client, const DrawContext& context)
{
	RenderState state;
	state.pass = this;
	state.flags = RenderPass::RenderOverlay;
	state.client = client;
	state.context = &context;

	client->getRenderer()->beginDraw2D(context);

	ui::Container* ui = client->getServer()->getUi(0);
	const Rect& vp = Rect(0, 0, context.channel->canvasSize->x(), context.channel->canvasSize->y());

	// Update the root container size if necessary.
	if((ui->getPosition().cwiseNotEqual(vp.min.cast<float>())).all() ||
		ui->getSize().cwiseNotEqual(vp.max.cast<float>()).all())
	{
		ui->setPosition(vp.min.cast<float>());
		ui->setSize(Vector2f(vp.width(), vp.height()));
		ofmsg("ui viewport update: position = %1% size = %2% %3%",
			%vp.min %vp.width() %vp.height());
	}

	// Make sure all widget sizes are up to date (and perform autosize where necessary).
	ui->updateSize();

	// Layout ui.
	ui->layout();

	Renderable* uiRenderable = ui->getRenderable(client);
	if(uiRenderable != NULL)
	{
		uiRenderable->draw(&state);
	}

	client->getRenderer()->endDraw();
}
