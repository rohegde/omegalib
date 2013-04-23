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
#ifndef __CY_MATERIAL__
#define __CY_MATERIAL__

#include "cyclopsConfig.h"
#include "Uniforms.h"

#include <osg/Material>
#include <osg/StateSet>

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <omegaOsg.h>
#include <omegaToolkit.h>

namespace cyclops {
	using namespace omega;
	using namespace omegaOsg;
	class SceneManager;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Encapsulates an OpenSceneGraph stateset and offers a quick interface to a few commonly
	//! used settings.
	class CY_API Material: public Uniforms
	{
	public:
		Material(osg::StateSet* ss, SceneManager* sm);

		void setColor(const Color& diffuse, const Color& emissive);
		void setShininess(float value);
		void setGloss(float value);

		//! Transparency control
		//@{
		void setTransparent(bool value);
		bool isTransparent() { return myTransparent; }
		//! Sets the material alpha value.
		//! @remarks setAlpha only sets the value of the unif_Alpha uniform. By default, cyclops shaders use this 
		//! uniform to modulate fragment alpha values after lighting (see postLightingSection/default.frag).
		//! This behavior can be modified by the user, redefining the @postLightingSection shader macro.
		void setAlpha(float value);
		//! Gets the material alpha value.
		float getAlpha();
		//@}

		//void setProgram(const String& program) { myProgramName = program; }
		//String getProgram() { return myProgramName; }

		osg::StateSet* getStateSet() { return myStateSet; }

		//! Resets all material properties to their default values.
		void reset();

	private:
		Ref<SceneManager> mySceneManager;
		Ref<osg::StateSet> myStateSet;
		Ref<osg::Material> myMaterial;
		Ref<osg::Uniform> myShininess;
		Ref<osg::Uniform> myGloss;
		Ref<cyclops::Uniform> myAlpha;
		bool myTransparent;

		//String myProgramName;
		//Ref<ProgramAsset> myProgram;
	};
};

#endif
