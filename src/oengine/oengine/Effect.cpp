/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#include "oengine/Effect.h"
#include "oengine/EffectManager.h"
#include "oengine/Mesh.h"
#include "omega/GpuManager.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Effect::Effect(EffectManager* mng): 
	myMng(mng), 
	myEmissiveColor(0, 0, 0, 1), 
	mySpecularColor(1, 1, 1, 1), 
	myAmbientColor(1, 1, 1, 1),
	myDiffuseColor(1, 1, 1, 1),
	myForcedDiffuseColor(false),
	myShininess(32) 
{
	myProgram = myMng->getGpuManager()->getDefaultProgram();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Effect::activate()
{
	myParams.bind(myProgram, GpuProgram::RenderStage);
	glEnable(GL_COLOR_MATERIAL);
	if(myForcedDiffuseColor)
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, myAmbientColor.data());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, myDiffuseColor.data());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mySpecularColor.data());
	glMaterialfv(GL_FRONT, GL_EMISSION, myEmissiveColor.data());
	glMaterialf(GL_FRONT, GL_SHININESS, myShininess);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Effect::deactivate()
{
	myParams.unbind(myProgram, GpuProgram::RenderStage);
}

