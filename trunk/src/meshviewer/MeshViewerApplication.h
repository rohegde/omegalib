/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 *********************************************************************************************************************/
#ifndef __MESHVIEWER_APPLICATION_H__
#define __MESHVIEWER_APPLICATION_H__

#include "MeshViewerUI.h"

#include "omega/scene.h"

// Using namespace declarations in a header file make emokitteh cry
// but we'll accept them here since it's application-level code, and we can have
// declarations shorter and more readable for the sake of clarity.
using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerClient: public ApplicationClient
{
public:
	MeshViewerClient(Application* app): ApplicationClient(app), myGpu(NULL) {}

	virtual void initialize();
	virtual bool handleEvent(const InputEvent& evt);
	virtual void update(const UpdateContext& context);
	virtual void draw(const DrawContext& context);

private:
	// Managers
	GpuManager* myGpu;
	TextureManager* myTexMng;
	FontManager* myFontMng;
	SceneManager* mySceneManager;

	SimplePrimitive* myTestDrawable;

	// User interface.
	MeshViewerUI* myUI;

	int mouseX;
	int mouseY;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new MeshViewerClient(this); }
};

#endif