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
#include <vtkPython.h>

#include "omegaVtk/PyVtk.h"
#include "omega/PythonInterpreter.h"
#include "omegaVtk/VtkModule.h"
#include "omega/PythonInterpreterWrapper.h"

using namespace omegaVtk;
using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
void attachProp(boost::python::object pyactor, SceneNode* node)
{
	if(pyactor != NULL && node != NULL)
	{
		PyVTKObject* vtkactor = (PyVTKObject*)pyactor.ptr();
		VtkModule::instance()->attachProp((vtkProp3D*)vtkactor->vtk_ptr, node);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void detachProp(boost::python::object pyactor, SceneNode* node)
{
	if(pyactor != NULL && node != NULL)
	{
		PyVTKObject* vtkactor = (PyVTKObject*)pyactor.ptr();
		VtkModule::instance()->detachProp((vtkProp3D*)vtkactor->vtk_ptr, node);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addLight(boost::python::object pylight)
{
	if(pylight != NULL)
	{
		PyVTKObject* vtklight = (PyVTKObject*)pylight.ptr();
		VtkModule::instance()->addLight((vtkLight*)vtklight->vtk_ptr);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void removeLight(boost::python::object pylight)
{
	if(pylight != NULL)
	{
		PyVTKObject* vtklight = (PyVTKObject*)pylight.ptr();
		VtkModule::instance()->removeLight((vtkLight*)vtklight->vtk_ptr);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void removeAllLights()
{
	VtkModule::instance()->removeAllLights();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(omegaVtk)
{
	// SceneLoader
	PYAPI_REF_BASE_CLASS(VtkModule)
		PYAPI_STATIC_REF_GETTER(VtkModule, createAndInitialize)
		//PYAPI_METHOD(VtkModule, attachProp)
		//PYAPI_METHOD(VtkModule, detachProp)
		;

	def("vtkAttachProp", attachProp);
	def("vtkDetachProp", detachProp);
	def("vtkAddLight", addLight);
	def("vtkRemoveLight", removeLight);
	def("vtkRemoveAllLights", removeAllLights);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OVTK_API omegaVtkPythonApiInit()
{
	static bool sApiInitialized = false;

	if(!sApiInitialized)
	{
		omsg("omegaVtkPythonApiInit()");
		//omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		//interp->addModule("omegaVtk", ovtkMethods);
		initomegaVtk();
	}
}

