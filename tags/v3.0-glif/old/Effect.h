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
#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "omegaToolkitConfig.h"
#include "omega/GpuProgram.h"
#include "omega/RenderPass.h"

namespace omegaToolkit {

	class SceneRenderable;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API BaseEffect
	{
	public:
		virtual void load(const Setting& setting) {}
		virtual void save(Setting& setting) {}
		virtual void activate() {}
		virtual void deactivate() {}
		virtual void draw(SceneRenderable* sr, RenderState* state) = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class OTK_API Effect: public BaseEffect
	{
	public:
		enum DrawMode { DrawPoints, DrawWireframe, DrawFlat, DrawSmooth };
		enum BlendMode { BlendDisabled, BlendNormal, BlendAdditive };
		enum DepthTestMode { DepthTestDefault, DepthTestEnabled, DepthTestDisabled };

	public:
		Effect();

		void setProgram(GpuProgram* program) { myProgram = program; }
		GpuProgram* getProgram() { return myProgram; }
		
		virtual void activate();
		virtual void deactivate();

		GpuProgramParams& getParams() { return myParams; }

		//! Utility method to set ambient, diffuse and specular colors together
		void setColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) { myDiffuseColor = Color(r, g, b, a); myAmbientColor = Color(r, g, b, a); }
		void setColor(const Color& value) { myDiffuseColor = value; mySpecularColor = value; myAmbientColor = value;}

		void setDiffuseColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) { myDiffuseColor = Color(r, g, b, a); }
		void setDiffuseColor(const Color& value) { myDiffuseColor = value; }
		Color getDiffuseColor() { return myDiffuseColor; }

		void setSpecularColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) { mySpecularColor = Color(r, g, b, a); }
		void setSpecularColor(const Color& value) { mySpecularColor = value; }
		Color getSpecularColor() { return mySpecularColor; }

		void setAmbientColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) { myAmbientColor = Color(r, g, b, a); }
		void setAmbientColor(const Color& value) { myAmbientColor = value; }
		Color getAmbientColor() { return myAmbientColor; }

		void setEmissiveColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) { myEmissiveColor = Color(r, g, b, a); }
		void setEmissiveColor(const Color& value) { myEmissiveColor = value; }
		Color getEmissiveColor() { return myEmissiveColor; }

		void setShininess(float value) { myShininess = value; }
		float getShininess() { return myShininess; }

		void setForcedDiffuseColor(bool value) { myForcedDiffuseColor = value; }
		bool getForcedDiffuseColor() { return myForcedDiffuseColor; }

		void draw(SceneRenderable* sr, RenderState* state);

		//! Point size
		//@{
		bool hasPointSize() { return myHasPointSize; }
		float getPointSize() { return myPointSize; }
		void setPointSize(float value) { myPointSize = value; myHasPointSize = true; }
		void unsetPointSize() { myHasPointSize = false; }
		//@}

		//! Draw mode
		//@{
		bool hasDrawMode() { return myHasDrawMode; }
		DrawMode getDrawMode() { return myDrawMode; }
		void setDrawMode(DrawMode value) { myDrawMode = value; myHasDrawMode = true; }
		void unsetDrawMode() { myHasDrawMode = false; }
		//@}

		//! Bend mode
		//@{
		void setBlendMode(BlendMode value) { myBlendMode = value; }
		BlendMode getBlendMode() { return myBlendMode; }
		//@}

		bool isLightingEnabled() { return myLightingEnabled; }
		void setLightingEnabled(bool value) { myLightingEnabled = value; }

		DepthTestMode getDepthTestMode() { return myDepthTestMode; }
		void setDepthTestMode(DepthTestMode value) { myDepthTestMode = value; }

	private:
		GpuProgram* myProgram;
		GpuProgramParams myParams;
		bool myForcedDiffuseColor;
		Color myDiffuseColor;
		Color mySpecularColor;
		Color myAmbientColor;
		Color myEmissiveColor;
		float myShininess;
		bool myHasPointSize;
		float myPointSize;

		bool myHasDrawMode;
		DrawMode myDrawMode; 

		BlendMode myBlendMode;

		bool myLightingEnabled;
		DepthTestMode myDepthTestMode;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API MultipassEffect: public BaseEffect
	{
	public:
		void draw(SceneRenderable* sr, RenderState* state);
		void addEffect(Effect* e) 
		{
			myEffects.push_back(e);
		}
		void removeEffect(Effect* e) 
		{
			myEffects.remove(e);
		}
	private:
		List<Effect*> myEffects;
	};
}; // namespace omegaToolkit

#endif