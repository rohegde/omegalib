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
#ifndef __SCENE_EDITOR_MODULE_H__
#define __SCENE_EDITOR_MODULE_H__

#include "oengine/EngineServer.h"
#include "oengine/BoundingSphere.h"

namespace oengine
{
	class SceneEditorModule;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class EditableObject
	{
	public:
		EditableObject(SceneNode* node, SceneEditorModule* editor);

		SceneNode* getSceneNode() { return mySceneNode; }
		BoundingSphere* getBoundingSphere() { return mySelectionSphere; }
		SceneEditorModule* getEditor() { return myEditor; }

	private:
		SceneNode* mySceneNode;
		BoundingSphere* mySelectionSphere;
		SceneEditorModule* myEditor;
		String myName;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API SceneEditorModule
	{
	public:
		enum InteractorStyle { MouseInteractorStyle, ControllerInteractorStyle };

	public:
		SceneEditorModule();
		~SceneEditorModule();

		void initialize(EngineServer* server);
		void update(const UpdateContext& context);
		void handleEvent(const Event& evt);

		void setInteractorStyle(InteractorStyle style);
		InteractorStyle getInteractorStyle() { return myInteractorStyle; }

		void addNode(SceneNode* node);
		void removeNode(SceneNode* node);

		SceneNode* getSelectedNode();

		EngineServer* getEngine() { return myEngine; }

	private:
		EditableObject* findEditableObject(SceneNode* node);
		void updateSelection(const Ray& ray);

	private:
		EngineServer* myEngine;
		
		InteractorStyle myInteractorStyle;
		Actor* myInteractor;

		EditableObject* mySelectedObject;
		List<EditableObject*> myObjects;
	};
};
#endif