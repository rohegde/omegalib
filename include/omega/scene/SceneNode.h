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
#ifndef __SCENENODE_H__
#define __SCENENODE_H__

#include "omega/osystem.h"
#include "omega/Sphere.h"
#include "omega/scene/Drawable.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class OUTILS_API SceneNode
	{
		friend class SceneManager; // Needs to call the node draw method.
	public:
		SceneNode():
			myPosition(0, 0, 0),
			myRotation(0, 0, 0),
			myScale(1, 1, 1),
			myPivot(0, 0, 0),
			myBoundingBoxColor(1, 1, 1, 1),
			myBoundingBoxVisible(false)
			{}

		void addChild(SceneNode* child) { myChildren.push_back(child); }
		int getNumChildren() { return myChildren.size(); }
		void clearChildren() { myChildren.empty(); }

		void addDrawable(Drawable* child) { myDrawables.push_back(child); myChanged = true;}
		int getNumDrawables() { return myDrawables.size(); }
		void clearDrawables() { myDrawables.empty(); myChanged = true;}

		bool hasChanged() { return myChanged; }

		// Transformation
		//@{
		void setPosition(const Vector3f& value) { myPosition = value; }
		void setRotation(const Vector3f& value) { myRotation = value; myChanged = true;}
		void setRotation(float x, float y, float z) { myRotation[0] = x; myRotation[1] = y; myRotation[2] = z; myChanged = true;}
		void setScale(const Vector3f& value) { myScale = value; myChanged = true;}
		void setScale(float value) { myScale[0] = value; myScale[1] = value; myScale[2] = value; myChanged = true;}
		void setPivot(const Vector3f& value) { myPivot = value; myChanged = true;}

		const Vector3f& getPosition() { return myPosition; }
		const Vector3f& getRotation() { return myRotation; }
		const Vector3f& getScale() { return myScale; }
		const Vector3f& getPivot() { return myPivot; }
		//@}

		// Bounding box handling
		//@{
		const BoundingBox& getBoundingBox() { if(myChanged) updateBounds(); return myBBox; }
		const Sphere& getBoundingSphere() { if(myChanged) updateBounds(); return myBSphere; }
		bool isBoundingBoxVisible() { return myBoundingBoxVisible; }
		void setBoundingBoxVisible(bool value) { myBoundingBoxVisible = value; }
		const Color& getBoundingBoxColor() { return myBoundingBoxColor; }
		void setBoundingBoxColor(const Color& color) { myBoundingBoxColor = color; }
		//@}

	private:
		void pushTransform();
		void popTransform();
		void draw();

		void updateBounds();
		void drawBoundingBox();

	private:
		std::vector<SceneNode*> myChildren;
		std::vector<Drawable*> myDrawables;

		Vector3f myRotation;
		Vector3f myPivot;
		Vector3f myScale;
		Vector3f myPosition;

		bool myChanged;
		BoundingBox myBBox;
		Sphere myBSphere;

		// Bounding box stuff.
		bool myBoundingBoxVisible;
		Color myBoundingBoxColor;
	};

}; // namespace scene
}; // namespace omega

#endif