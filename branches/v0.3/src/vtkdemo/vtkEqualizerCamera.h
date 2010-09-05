/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Portions of code part of the Visualization Toolkit
 *  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 *   All rights reserved.
 *   See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * VtkDemo
 *********************************************************************************************************************/
#ifndef __vtkEqualizerCamera_h
#define __vtkEqualizerCamera_h

#include "vtkCamera.h"

class vtkOpenGLRenderer;

class vtkEqualizerCamera : public vtkCamera
{
public:
  static vtkEqualizerCamera *New();
  vtkTypeRevisionMacro(vtkEqualizerCamera,vtkCamera);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Implement base class method.
  void Render(vtkRenderer *ren);

  void UpdateViewport(vtkRenderer *ren);
  
protected:  
  vtkEqualizerCamera() {};
  ~vtkEqualizerCamera() {};

private:
  vtkEqualizerCamera(const vtkEqualizerCamera&);  // Not implemented.
  void operator=(const vtkEqualizerCamera&);  // Not implemented.
};

#endif
