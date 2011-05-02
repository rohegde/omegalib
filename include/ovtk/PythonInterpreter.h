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
#ifndef __PYTHON_INTERPRETER_H__
#define __PYTHON_INTERPRETER_H__

#include "ovtkbase.h"
#include "omega/osystem.h"

#include <vtkPython.h>

namespace ovtk
{
	using namespace omega;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class PythonInterpreter
	{
	  friend struct PythonInterpreterWrapper;
	public:
	  PythonInterpreter();
	  ~PythonInterpreter();

	  void initialize(const char* programName);
	  void addModule(const char* name, PyMethodDef* methods);
 	  void eval(const String& script, const char* format = NULL, ...);
	  void runFile(const String& filename);
	  void addPythonPath(const char*);

	protected:
	  char* myExecutablePath;

	private:
	  PythonInterpreter(const PythonInterpreter&); // Not implemented.
	  void operator=(const PythonInterpreter&); // Not implemented.
	};
};
#endif

