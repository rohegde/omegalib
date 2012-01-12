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
#include "oengine/EngineClient.h"
#include "oengine/EngineServer.h"
#include "oengine/LightingPass.h"
#include "oengine/Light.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
void LightingPass::render(EngineClient* client, const DrawContext& context)
{
	glPushMatrix();
	//glLoadMatrixf(mng->getViewTransform().data());
	glLoadIdentity();
    glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);

	EngineServer* es = client->getServer();

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, es->getAmbientLightColor().data());

	GLenum lightId = GL_LIGHT0;
	for(int i = 0; i < EngineServer::MaxLights; i++)
	{
		Light* light = es->getLight(i);
		if(light->isEnabled())
		{
			glEnable(lightId);

			glLightfv(lightId, GL_DIFFUSE, light->getColor().data());
			glLightfv(lightId, GL_SPECULAR, light->getColor().data());

			// Set position
			const Vector3f& pos = light->getPosition();
			float fv[] = {pos[0], pos[1], pos[2], 0.0f };
			glLightfv(lightId, GL_POSITION, fv);

			lightId++;
		}
		else
		{
			glDisable(lightId);
		}
	}

    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable( GL_COLOR_MATERIAL );

	glPopMatrix();
}
