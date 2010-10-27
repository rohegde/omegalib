/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * DrawContext
 *********************************************************************************************************************/
#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "omega/osystem.h"
#include "omega/GpuProgram.h"
#include "omega/Color.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class Effect
	{
	public:
		Effect(): myProgram(NULL), myEmissiveColor(0, 0, 0, 1), mySpecularColor(0, 0, 0, 1), myShininess(100) {}

		void setProgram(GpuProgram* program) { myProgram = program; }
		GpuProgram* getProgram() { return myProgram; }
		
		OUTILS_API void activate();
		OUTILS_API void deactivate();

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

	private:
		GpuProgram* myProgram;
		GpuProgramParams myParams;
		Color myDiffuseColor;
		Color mySpecularColor;
		Color myAmbientColor;
		Color myEmissiveColor;
		float myShininess;
	};

}; // namespace scene
}; // namespace omega

#endif