/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#include "omega/GpuProgram.h"
#include "omega/GpuManager.h"
#include "omega/GpuBuffer.h"
#include "omega/glheaders.h"

using namespace omega;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgramParams::bind(GpuProgram* program, GpuProgram::Stage stage)
{
	for(int i = 0; i < MaxParams; i++)
	{
		GpuData* data = myInput[i];
		if(data != NULL)
		{
			data->bind(program, i, stage);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgramParams::unbind(GpuProgram* program, GpuProgram::Stage stage)
{
	for(int i = 0; i < MaxParams; i++)
	{
		GpuData* data = myInput[i];
		if(data != NULL)
		{
			data->unbind(program, i, stage);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgramParams::setParam(int index, GpuData* inputData)
{
	myInput[index] = inputData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgramParams::clearInput()
{
	memset(myInput, 0, MaxParams * sizeof(GpuData*));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VertexShader::VertexShader(GLuint GLShader, const omega::String& name):
	myGLShader(GLShader), 
	myName(name)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VertexShader::dispose()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FragmentShader::FragmentShader(GLuint GLShader, const omega::String& name):
	myGLShader(GLShader), 
	myName(name)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FragmentShader::dispose()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GeometryShader::GeometryShader(GLuint GLShader, const omega::String& name):
	myGLShader(GLShader), 
	myName(name)
{
}

#ifdef OMEGA_USE_OPENCL
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ComputeShader::ComputeShader(cl_program program, const omega::String& kernelEntryPoint, const omega::String& name)
{
	cl_int status;
	myCLProgram = program;
	myCLKernel = clCreateKernel(myCLProgram, kernelEntryPoint.c_str(), &status);
	if(!clSuccessOrDie(status)) return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ComputeShader::dispose()
{
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GpuProgram::GpuProgram(GpuManager* gpuMng):
#ifdef OMEGA_USE_OPENCL
	myComputeShader(NULL),
#endif
	myGeometryShader(NULL),
	myVertexShader(NULL),
	myFragmentShader(NULL),
	myGpuMng(gpuMng),
	myGLProgram(0)
{
	oassert(gpuMng != NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::initialize()
{
	myGLProgram = glCreateProgram();

	// Attach shaders.
	if(myGeometryShader != NULL)
		glAttachShader(myGLProgram, myGeometryShader->getGLShader());

	if(myVertexShader != NULL)
		glAttachShader(myGLProgram, myVertexShader->getGLShader());

	if(myFragmentShader != NULL)
		glAttachShader(myGLProgram, myFragmentShader->getGLShader());

	// Link the GL program.
	glLinkProgram(myGLProgram);
	printProgramLog(myGLProgram);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::printProgramLog(GLuint program)
{
    GLint infoLogLength = 0;
    GLsizei charsWritten  = 0;
    GLchar *infoLog;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0)
    {
        infoLog = (char *) malloc(infoLogLength);
        glGetProgramInfoLog(program, infoLogLength, &charsWritten, infoLog);
		omsg(infoLog);
        free(infoLog);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef OMEGA_USE_OPENCL
void GpuProgram::runComputeStage(int dimensions, const Vector3i& localThreads, const Vector3i globalThreads)
{
	cl_event events;
	cl_int status;

	cl_kernel kern = myComputeShader->getCLKernel();
	cl_command_queue clqueue = myGpuMng->getCLCommandQueue();

	status = clEnqueueNDRangeKernel(
					clqueue,
					kern,
					myComputeDimensions,
					NULL,
					myGlobalComputeThreads,
					myLocalComputeThreads,
					0,
					NULL,
					&events);
	if(!clSuccessOrDie(status)) return;
	status = clWaitForEvents(1, &events);
	if(!clSuccessOrDie(status)) return;
	clReleaseEvent(events);
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::runRenderStage(int items, PrimType primType, unsigned int* indices)
{
	if(primType != PrimNone)
	{
		GLenum mode = GL_POINTS;
		switch(primType)
		{
		case PrimPoints: mode = GL_POINTS;
		case PrimTriangles: mode = GL_TRIANGLES;
		}

		glUseProgram(myGLProgram);

		if(indices == NULL)
		{
			glDrawArrays(mode, 0, items);
		}
		else
		{
			glDrawElements(mode, items, GL_UNSIGNED_INT, indices);
		}

		glUseProgram(0);
	}
}

