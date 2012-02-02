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
#ifndef __SCENENODE_H__
#define __SCENENODE_H__

#include "omega/osystem.h"
#include "omega/scene/Drawable.h"
#include "omega/scene/Node.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OUTILS_API SceneNode: public Node
	{
		friend class SceneManager; // Needs to call the node draw method.
	public:
		SceneNode(SceneManager* scene):
			myScene(scene),
			myBoundingBoxColor(1, 1, 1, 1),
			myBoundingBoxVisible(false),
			mySelectable(false),
			myChanged(false),
			myVisible(true)
			{}

		SceneManager* getScene();

		// Drawables
		//@{
		void addDrawable(Drawable* child);
		int getNumDrawables();
		void clearDrawables();
		//@}

		// Options
		//@{
		bool isSelectable();
		void setSelectable(bool value);
		bool isVisible();
		void setVisible(bool value);
		//@}

		// Bounding box handling
		//@{
		const AxisAlignedBox& getBoundingBox();
		const Sphere& getBoundingSphere();
		bool isBoundingBoxVisible();
		void setBoundingBoxVisible(bool value);
		const Color& getBoundingBoxColor();
		void setBoundingBoxColor(const Color& color);
		//@}

		virtual void update(bool updateChildren, bool parentHasChanged);

	private:
		void draw();
		void drawBoundingBox();

	private:
		SceneManager* myScene;

		Vector<Drawable*> myDrawables;

		bool mySelectable;
		bool myVisible;

		bool myChanged;
		AxisAlignedBox myBBox;
		Sphere myBSphere;

		// Bounding box stuff.
		bool myBoundingBoxVisible;
		Color myBoundingBoxColor;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const AxisAlignedBox& SceneNode::getBoundingBox() 
	{ return myBBox; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const Sphere& SceneNode::getBoundingSphere() 
	{ return myBSphere; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool SceneNode::isBoundingBoxVisible() 
	{ return myBoundingBoxVisible; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneNode::setBoundingBoxVisible(bool value) 
	{ myBoundingBoxVisible = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const Color& SceneNode::getBoundingBoxColor() 
	{ return myBoundingBoxColor; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneNode::setBoundingBoxColor(const Color& color) 
	{ myBoundingBoxColor = color; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline SceneManager* SceneNode::getScene()
	{ return myScene; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool SceneNode::isSelectable() 
	{ return mySelectable; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneNode::setSelectable(bool value) 
	{ mySelectable = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool SceneNode::isVisible()
	{ return myVisible; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneNode::setVisible(bool value)
	{ myVisible = value; }
}; // namespace scene
}; // namespace omega

#endif