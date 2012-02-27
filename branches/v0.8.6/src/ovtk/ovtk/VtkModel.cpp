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
#include "omega/DataManager.h"

#include "ovtk/PyVtk.h"
#include "ovtk/PythonInterpreter.h"
#include "ovtk/VtkModule.h"
#include "ovtk/VtkRenderable.h"
#include "ovtk/VtkViewClient.h"
#include "ovtk/VtkModel.h"

using namespace omega::mvc;
using namespace ovtk;

OMEGA_DEFINE_TYPE(VtkModel, Model);

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* ovtk_addActor(PyObject* self, PyObject* args)
{
	PyObject* pyactor = NULL;
	PyArg_ParseTuple(args, "O", &pyactor);

	PyVTKObject* pyvtkactor = (PyVTKObject *)pyactor;
	vtkProp3D* vtkactor = (vtkActor*)pyvtkactor->vtk_ptr;

	VtkModule::instance()->getActiveClient()->addActor(vtkactor);

	return Py_BuildValue("s", "ok");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* ovtk_findFile(PyObject* self, PyObject* args)
{
	const char* name;
	if(!PyArg_ParseTuple(args, "s", &name)) return NULL;

	DataManager* dm = SystemManager::instance()->getDataManager();
	DataInfo info = dm->getInfo(name);

	if(info.isNull())
	{
		return Py_BuildValue("s", "");
	}

	return Py_BuildValue("s", info.path.c_str());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* ovtk_addButton(PyObject* self, PyObject* args)
{
	const char* buttonName;
	const char* command;
	if(!PyArg_ParseTuple(args, "s|s", &buttonName, &command)) return NULL;

	//VtkModule::instance()->getActiveEntity()->addButton(buttonName, command);

	return Py_BuildValue("s", "ok");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* ovtk_addCheckButton(PyObject* self, PyObject* args)
{
	const char* buttonName;
	const char* getValueCommand;
	const char* changeValueCommand;
	if(!PyArg_ParseTuple(args, "s|s|s", &buttonName, &getValueCommand, &changeValueCommand)) return NULL;

	//VtkModule::instance()->getActiveEntity()->addCheckButton(buttonName, getValueCommand, changeValueCommand);

	return Py_BuildValue("s", "ok");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* ovtk_addSlider(PyObject* self, PyObject* args)
{
	const char* sliderName;
	const char* getValueCommand;
	const char* changeValueCommand;
	float minValue;
	float maxValue;
	float step;
	if(!PyArg_ParseTuple(args, "s|f|f|f|s|s", &sliderName, &minValue, &maxValue, &step, &getValueCommand, &changeValueCommand)) return NULL;

	//VtkModule::instance()->getActiveEntity()->addSlider(sliderName, minValue, maxValue, step, getValueCommand, changeValueCommand);

	return Py_BuildValue("s", "ok");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyMethodDef ovtkMethods[] = 
{
    {"addActor", ovtk_addActor, METH_VARARGS, "Adds an actor."},
    {"findFile", ovtk_findFile, METH_VARARGS, "Finds a file given a partial path using the omegalib data manager."},
    {"addButton", ovtk_addButton, METH_VARARGS, "Add button."},
    {"addCheckButton", ovtk_addCheckButton, METH_VARARGS, "Add check button."},
    {"addSlider", ovtk_addSlider, METH_VARARGS, "Add slider."},
    {NULL, NULL, 0, NULL}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkModel::VtkModel()
{
	myInterpreter = onew(PythonInterpreter)();
	myInterpreter->initialize("program");
	myInterpreter->addModule("ovtk", ovtkMethods);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModel::loadScript(const String& filename)
{
	setState(StateLoading);
	myInterpreter->runFile(filename.c_str());
	setState(StateLoaded);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModel::evalCommand(const String cmd)
{
	myInterpreter->eval(cmd);
}


