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
#ifndef __SCENENODE_H__
#define __SCENENODE_H__

#include "omega/osystem.h"
#include "omega/scene/Drawable.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class SceneNode
	{
		friend class SceneManager; // Needs to call the node draw method.
	public:
		SceneNode():
			myPosition(0, 0, 0),
			myRotation(0, 0, 0),
			myScale(1, 1, 1),
			myPivot(0, 0, 0) {}

		void addChild(SceneNode* child) { myChildren.push_back(child); }
		int getNumChildren() { return myChildren.size(); }
		void clearChildren() { myChildren.empty(); }

		void addDrawable(Drawable* child) { myDrawables.push_back(child); }
		int getNumDrawables() { return myDrawables.size(); }
		void clearDrawables() { myDrawables.empty(); }

		void setPosition(const Vector3f& value) { myPosition = value; }
		void setRotation(const Vector3f& value) { myRotation = value; }
		void setScale(const Vector3f& value) { myScale = value; }
		void setPivot(const Vector3f& value) { myPivot = value; }

		Vector3f& getPosition() { return myPosition; }
		Vector3f& getRotation() { return myRotation; }
		Vector3f& getScale() { return myScale; }
		Vector3f& getPivot() { return myPivot; }

	private:
		void pushTransform();
		void popTransform();
		void draw();

	private:
		std::vector<SceneNode*> myChildren;
		std::vector<Drawable*> myDrawables;

		Vector3f myRotation;
		Vector3f myPivot;
		Vector3f myScale;
		Vector3f myPosition;
	};

}; // namespace scene
}; // namespace omega

#endif