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
#include "ovtk/VtkClient.h"
#include "ovtk/PyVtk.h"
#include "ovtk/VtkRenderable.h"
#include "ovtk/PythonInterpreter.h"
#include "omega/scene.h"
#include "omega/SystemManager.h"
#include "omega/DataManager.h"

using namespace ovtk;

VtkClient* VtkClient::myInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* ovtk_addActor(PyObject* self, PyObject* args)
{
	PyObject* pyactor = NULL;
	PyArg_ParseTuple(args, "O", &pyactor);

	PyVTKObject* pyvtkactor = (PyVTKObject *)pyactor;
	vtkActor* vtkactor = (vtkActor*)pyvtkactor->vtk_ptr;

	VtkClient::instance()->addActor(vtkactor);

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
static PyMethodDef ovtkMethods[] = 
{
    {"addActor", ovtk_addActor, METH_VARARGS, "Adds an actor."},
    {"findFile", ovtk_findFile, METH_VARARGS, "Finds a file given a partial path using the omegalib data manager."},
    {NULL, NULL, 0, NULL}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkClient::VtkClient(EngineClient* engine): 
	myEngine(engine),
	myRenderPass(NULL),
	myInterpreter(NULL) 
{
	myInstance = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkClient::~VtkClient()
{
	odelete(myRenderPass);
	odelete(myInterpreter);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkClient::initialize()
{
	oassert(myEngine != NULL);

	myInterpreter = onew(PythonInterpreter)();
	myRenderPass = onew(VtkRenderPass)();
	myRenderPass->initialize();

	myInterpreter->initialize("program");
	myInterpreter->addModule("ovtk", ovtkMethods);

	myEngine->getSceneManager()->addRenderPass(myRenderPass);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkClient::addActor(vtkActor* actor)
{
	SceneManager* sm = myEngine->getSceneManager();
	SceneNode* sn = onew(SceneNode)(sm);
	sm->getRootNode()->addChild(sn);

	BoundingSphere* ss = onew(BoundingSphere)();
	ss->setVisible(false);
	ss->setDrawOnSelected(true);

	VtkRenderable* vdw = onew(VtkRenderable)();
	vdw->setActor(actor);

	sn->addRenderable(vdw);
	sn->addRenderable(ss);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkClient::runScript(const String& name)
{
	myInterpreter->runSimpleFile(name.c_str());
}

