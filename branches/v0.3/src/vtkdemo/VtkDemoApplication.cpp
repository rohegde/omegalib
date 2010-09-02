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
#include "VtkDemoApplication.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VtkDemoServer::initialize()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VtkDemoServer::update(const UpdateContext& context)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VtkDemoServer::handleEvent(const InputEvent& evt)
{
/*
	if(!myUI->handleEvent(evt))
	{
		switch(evt.serviceType)
		{
		case InputService::Pointer:
			myMouseX = evt.position.x();
			myMouseY = evt.position.y();
			if(evt.type == InputEvent::Down)
			{
				myRotate = true;
			}
			else if(evt.type == InputEvent::Up)
			{
				myRotate = false;
			}
			break;
		case InputService::Touch:
			myTouchX[myNumTouches] = evt.position.x();
			myTouchY[myNumTouches] = evt.position.y();
			myNumTouches++;
		break;

		case InputService::Mocap:
		{
		}
		}
		return true;
	}
*/
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VtkDemoClient::setup()
{
	//// Declare variables
	vtkCylinderSource* source						= NULL;
	vtkPolyDataMapper* mapper						= NULL;
	vtkActor* actor									= NULL;
	vtkRenderer* ren								= NULL;
	vtkRenderWindowInteractor* iren					= NULL;
	vtkInteractorStyleTrackballCamera* trackball	= NULL;
	
	// Setup renderer and render window
	ren	= vtkRenderer::New();

	myRenderWindow = (vtkOpenGLRenderWindow*)vtkRenderWindow::New();
	myRenderWindow->AddRenderer(ren);
	myRenderWindow->DoubleBufferOff();
	myRenderWindow->SwapBuffersOff();
	
	myRenderWindow->SetPosition(50, 50);
	myRenderWindow->SetSize(300, 300);
	
	myRenderWindow->OffScreenRenderingOn();
	
	// Setup render window interactor and interactor style
	iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(myRenderWindow);
	
	// Geometry
	source = vtkCylinderSource::New();
	
	// Mapper
	mapper = vtkPolyDataMapper::New();
	mapper->ImmediateModeRenderingOn();
	mapper->SetInputConnection(source->GetOutputPort());
	
	// Actor in scene
	actor = vtkActor::New();
	actor->SetMapper(mapper);
			
	// Add Actor to renderer
	ren->AddActor(actor);
	
	// Reset camera and render scene	
	ren->ResetCamera();
	ren->SetBackground(53.0/255.0, 52.0/255.0, 102.0/255.0);
	myRenderWindow->Render();

	myPixelData = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VtkDemoClient::initialize()
{
	myGpu = new GpuManager();
	myGpu->initialize();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VtkDemoClient::update(const UpdateContext& context)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VtkDemoClient::draw(const DrawContext& context)
{
	int w = context.viewportWidth;
	int h = context.viewportHeight;

	if(myPixelData == NULL)
	{
		myPixelData = new unsigned char[w*h*4];
	}
	if(context.layer == 1)
	{
		myRenderWindow->MakeCurrent();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		myRenderWindow->SetSize(w, h);
		myRenderWindow->Render();
		myRenderWindow->GetRGBACharPixelData(0, 0, w-1, h-1, 1, myPixelData);

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
	else
	{
		// Create an image object to temporarily hold the VTK scene
		//QRect ri = r.toRect();

		glDisable(GL_TEXTURE_2D);
		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_LIGHTING);
		GfxUtils::beginOverlayMode(context);
		glRasterPos2f(0, h - 1);
		//memset(myPixelData, 250, w*h*4);
		glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, myPixelData);
		GfxUtils::endOverlayMode();


		// Render the VTK scene

		// applyBuffer

	}
}
