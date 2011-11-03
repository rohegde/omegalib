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
#ifndef __SOLID_EFFECT__
#define __SOLID_EFFECT__

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <oengine.h>

#include <osgFX/Export>
#include <osgFX/Effect>

#include <osg/Material>
#include <osg/LineWidth>

#include "SceneManager.h"

namespace hpl {
	using namespace osgFX;
	class SolidEffect: public Effect 
	{
	public:
		SolidEffect();
		SolidEffect(const SolidEffect& copy, const osg::CopyOp& copyop);

		// effect class informations
		META_Effect(
			osgFX, 
			SolidEffect, 
			"SolidEffect", 
			"SolidEffect",
			"Alessandro Febretti");

		void load(TiXmlElement* xdata);

		osg::Texture2D* getDiffuseTexture() { return myDiffuse; }
		osg::Texture2D* getNMapTexture() { return myNMap; }
		osg::Texture2D* getSpecularTexture() { return mySpecular; }
		osg::Texture2D* getHeightTexture() { return myHeight; }

	protected:
		virtual ~SolidEffect() {}
		SolidEffect& operator=(const SolidEffect&) { return *this; }

		bool define_techniques();

	private:
		SceneManager* mySceneManager;
		osg::ref_ptr<osg::Material> myMaterial;
		osg::Texture2D* myDiffuse;
		osg::Texture2D* myNMap;
		osg::Texture2D* mySpecular;
		osg::Texture2D* myHeight;
	};
}

#endif
