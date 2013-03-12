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
 *-------------------------------------------------------------------------------------------------
 * Original code Copyright (c) Kitware, Inc.
 * All rights reserved.
 * See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.
 *************************************************************************************************/
#include "omegaVtk/vtkGenericOpenGLRenderWindow.h"
#include "vtkObjectFactory.h"
#include "vtkRendererCollection.h"
#include "vtkOpenGLRenderer.h"
#include "vtkCommand.h"

vtkStandardNewMacro(vtkGenericOpenGLRenderWindow);

///////////////////////////////////////////////////////////////////////////////////////////////////
vtkGenericOpenGLRenderWindow::vtkGenericOpenGLRenderWindow()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
vtkGenericOpenGLRenderWindow::~vtkGenericOpenGLRenderWindow()
{
  this->Finalize();

  vtkRenderer* ren;
  this->Renderers->InitTraversal();
  for ( ren = vtkOpenGLRenderer::SafeDownCast(this->Renderers->GetNextItemAsObject());
    ren != NULL;
    ren = vtkOpenGLRenderer::SafeDownCast(this->Renderers->GetNextItemAsObject()))
    {
    ren->SetRenderWindow(NULL);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetFrontBuffer(unsigned int b)
{
  this->FrontBuffer = b;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetFrontLeftBuffer(unsigned int b)
{
  this->FrontLeftBuffer = b;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetFrontRightBuffer(unsigned int b)
{
  this->FrontRightBuffer = b;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetBackBuffer(unsigned int b)
{
  this->BackBuffer = b;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetBackLeftBuffer(unsigned int b)
{
  this->BackLeftBuffer = b;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetBackRightBuffer(unsigned int b)
{
  this->BackRightBuffer = b;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::Finalize()
{
  // tell each of the renderers that this render window/graphics context
  // is being removed (the RendererCollection is removed by vtkRenderWindow's
  // destructor)
  vtkRenderer* ren;
  this->Renderers->InitTraversal();
  for ( ren = vtkOpenGLRenderer::SafeDownCast(this->Renderers->GetNextItemAsObject());
    ren != NULL;
    ren = vtkOpenGLRenderer::SafeDownCast(this->Renderers->GetNextItemAsObject())  )
    {
    ren->SetRenderWindow(NULL);
    ren->SetRenderWindow(this);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::Frame()
{
  //this->InvokeEvent(vtkCommand::WindowFrameEvent, NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::MakeCurrent()
{
  //this->InvokeEvent(vtkCommand::WindowMakeCurrentEvent, NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool vtkGenericOpenGLRenderWindow::IsCurrent()
{
  //this->InvokeEvent(vtkCommand::WindowIsCurrentEvent, &current);
  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::PushState()
{
  glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::PopState()
{
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glPopClientAttrib();
  glPopAttrib();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetWindowId(void*)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void* vtkGenericOpenGLRenderWindow::GetGenericWindowId()
{
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetDisplayId(void*)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetParentId(void*)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void* vtkGenericOpenGLRenderWindow::GetGenericDisplayId()
{
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void* vtkGenericOpenGLRenderWindow::GetGenericParentId()
{
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void* vtkGenericOpenGLRenderWindow::GetGenericContext()
{
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void* vtkGenericOpenGLRenderWindow::GetGenericDrawable()
{
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetWindowInfo(char*)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetParentInfo(char*)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int* vtkGenericOpenGLRenderWindow::GetScreenSize()
{
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::Start()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::HideCursor()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::ShowCursor()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetFullScreen(int)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::WindowRemap()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int vtkGenericOpenGLRenderWindow::GetEventPending()
{
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetNextWindowId(void*)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::SetNextWindowInfo(char*)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::CreateAWindow()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void vtkGenericOpenGLRenderWindow::DestroyWindow()
{
}
