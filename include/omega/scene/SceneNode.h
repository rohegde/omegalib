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
#include "omega/scene/Drawable.h"
#include "omega/scene/Node.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class OUTILS_API SceneNode: public Node
	{
		friend class SceneManager; // Needs to call the node draw method.
	public:
		SceneNode(SceneManager* scene):
			myScene(scene),
			myBoundingBoxColor(1, 1, 1, 1),
			myBoundingBoxVisible(false),
			mySelectable(false),
			myChanged(false)
			{}

		SceneManager* getScene() { return myScene; }

		void addDrawable(Drawable* child);
		int getNumDrawables();
		void clearDrawables();

		bool isSelectable() { return mySelectable; }
		void setSelectable(bool value) { mySelectable = value; }

		// Bounding box handling
		//@{
		const AxisAlignedBox& getBoundingBox() { return myBBox; }
		const Sphere& getBoundingSphere() { return myBSphere; }
		bool isBoundingBoxVisible() { return myBoundingBoxVisible; }
		void setBoundingBoxVisible(bool value) { myBoundingBoxVisible = value; }
		const Color& getBoundingBoxColor() { return myBoundingBoxColor; }
		void setBoundingBoxColor(const Color& color) { myBoundingBoxColor = color; }
		//@}

		virtual void update(bool updateChildren, bool parentHasChanged);
	private:
		void draw();
		void drawBoundingBox();

	private:
		SceneManager* myScene;

		std::vector<Drawable*> myDrawables;

		bool mySelectable;

		bool myChanged;
		AxisAlignedBox myBBox;
		Sphere myBSphere;

		// Bounding box stuff.
		bool myBoundingBoxVisible;
		Color myBoundingBoxColor;
	};

}; // namespace scene
}; // namespace omega

#endif