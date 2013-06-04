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
#ifndef vtkGenericOpenGLRenderWindow_hpp
#define vtkGenericOpenGLRenderWindow_hpp

#include "vtkOpenGLRenderWindow.h"

class vtkGenericOpenGLRenderWindow : public vtkOpenGLRenderWindow
{
public:
  static vtkGenericOpenGLRenderWindow* New();
  vtkTypeMacro(vtkGenericOpenGLRenderWindow, vtkOpenGLRenderWindow);
  void PrintSelf(ostream& os, vtkIndent indent);
protected:
  vtkGenericOpenGLRenderWindow();
  ~vtkGenericOpenGLRenderWindow();

public:

  //! Cleans up graphics resources allocated in the context for this VTK scene.
  void Finalize();

  //! flush the pending drawing operations
  //! Class user may to watch for WindowFrameEvent and act on it
  void Frame();

  //! Makes the context current.  It is the class user's
  //! responsibility to watch for WindowMakeCurrentEvent and set it current.
  void MakeCurrent();

  //! Returns if the context is current.  It is the class user's
  //! responsibility to watch for WindowIsCurrentEvent and set the bool* flag
  //! passed through the call data parameter.
  bool IsCurrent();

  // {@
  //! set the drawing buffers to use
  void SetFrontBuffer(unsigned int);
  void SetFrontLeftBuffer(unsigned int);
  void SetFrontRightBuffer(unsigned int);
  void SetBackBuffer(unsigned int);
  void SetBackLeftBuffer(unsigned int);
  void SetBackRightBuffer(unsigned int);
  // }@

  //! convenience function to push the state and push/init the tranform matrices
  void PushState();
  //! convenience function to pop the state and pop the tranform matrices
  void PopState();

  // {@
  //! does nothing
  void SetWindowId(void*);
  void* GetGenericWindowId();
  void SetDisplayId(void*);
  void SetParentId(void*);
  void* GetGenericDisplayId();
  void* GetGenericParentId();
  void* GetGenericContext();
  void* GetGenericDrawable();
  void SetWindowInfo(char*);
  void SetParentInfo(char*);
  int* GetScreenSize();
  void Start();
  void HideCursor();
  void ShowCursor();
  void SetFullScreen(int);
  void WindowRemap();
  int  GetEventPending();
  void SetNextWindowId(void*);
  void SetNextWindowInfo(char*);
  void CreateAWindow();
  void DestroyWindow();
  // }@

  //! 1 = Window is using hardware acceleration
  virtual int IsDirect() { return 1;}

protected:

private:
  vtkGenericOpenGLRenderWindow(const vtkGenericOpenGLRenderWindow&);  // Not implemented.
  void operator=(const vtkGenericOpenGLRenderWindow&);  // Not implemented.
};

#endif
