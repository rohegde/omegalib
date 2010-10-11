/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * VtkDemo
 *********************************************************************************************************************/
#ifndef __VTKDEMO_APPLICATION_H__
#define __VTKDEMO_APPLICATION_H__

//#include "PlaygroundUI.h"

#include "omega.h"

// Vtk includes
#include <vtkCylinderSource.h>
#include <vtkContourFilter.h>
#include <vtkQuadric.h>
#include <vtkSampleFunction.h>
#include <vtkPolyDataMapper.h>
#include <vtkPerspectiveTransform.h>
#include <vtkActor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include "vtkEqualizerCamera.h"

#include "VtkDemoUI.h"

// Using namespace declarations in a header file make emokitteh cry
// but we'll accept them here since it's application-level code, and we can have
// declarations shorter and more readable for the sake of clarity.
using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VtkDemoServer: public ApplicationServer
{
public:
	VtkDemoServer(Application* app): ApplicationServer(app) {}

	virtual void initialize();
	virtual void update(const UpdateContext& context);
	virtual bool handleEvent(const InputEvent& evt);

private:
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VtkDemoClient: public ApplicationClient
{
public:
	VtkDemoClient(Application* app): ApplicationClient(app), myGpu(NULL) {}

	virtual void setup();
	virtual void initialize();
	virtual void update(const UpdateContext& context);
	virtual void draw(const DrawContext& context);

private:
	GpuManager* myGpu;
	vtkOpenGLRenderWindow* myRenderWindow;
	vtkRenderer* myRenderer;
	vtkCamera* myCamera;
	vtkUnsignedCharArray* myPixelData;

	// UI stuff.
	TextureManager* myTexMng;
	FontManager* myFontMng;
	VtkDemoUI* myUI;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VtkDemoApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new VtkDemoClient(this); }
	virtual ApplicationServer* createServer() { return new VtkDemoServer(this); }
};

#endif