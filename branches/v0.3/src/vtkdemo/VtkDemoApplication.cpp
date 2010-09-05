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
	source->SetResolution(20);
	
	// Mapper
	mapper = vtkPolyDataMapper::New();
	mapper->ImmediateModeRenderingOn();
	mapper->SetInputConnection(source->GetOutputPort());
	
	// Actor in scene
	actor = vtkActor::New();
	actor->SetMapper(mapper);
			
	// Add Actor to renderer
	ren->AddActor(actor);
	
	myCamera = vtkEqualizerCamera::New();
	ren->SetActiveCamera(myCamera);

	// Reset camera and render scene	
	ren->ResetCamera();
	ren->SetBackground(53.0/255.0, 52.0/255.0, 102.0/255.0);
	myRenderWindow->Render();

	//myCamera->SetPosition(0, 0, 1);

	myPixelData = vtkUnsignedCharArray::New();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VtkDemoClient::initialize()
{
	myGpu = new GpuManager();
	myGpu->initialize();

	myFontMng = new FontManager();
	myTexMng = new TextureManager();

	myUI = new VtkDemoUI(myTexMng, myFontMng);
	myUI->initialize();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VtkDemoClient::update(const UpdateContext& context)
{
	myUI->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VtkDemoClient::draw(const DrawContext& context)
{
	int w = context.viewportWidth;
	int h = context.viewportHeight;

	if(context.layer == 0)
	{
		// read the current projection and modelview matrices.
		double modelview[16];
		double proj[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, proj);

		// Switch the openGL context to the one manager by Vtk.
		myRenderWindow->MakeCurrent();

		// Set the vtk initial matrices to the omegalib ones.
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadMatrixd(proj);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadMatrixd(modelview);

		glRotatef(context.frameNum, 0, 0, 1);

		myRenderWindow->SetSize(w, h);
		myRenderWindow->Render();

		myRenderWindow->GetRGBACharPixelData(0, 0, w-1, h-1, 1, myPixelData);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		// Reset the openGL context to the original one.
		context.glContext->makeCurrent();

		// draw the vtk rendered image to the omegalib framebuffer.
		glDisable(GL_TEXTURE_2D);
		GfxUtils::beginOverlayMode(context);
		glRasterPos2f(0, h - 1);
		glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, myPixelData->GetPointer(0));
		GfxUtils::endOverlayMode();
	}
	else if(context.layer == 1)
	{
		myUI->draw(context);
	}
}
