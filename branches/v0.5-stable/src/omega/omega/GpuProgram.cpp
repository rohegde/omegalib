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

#ifdef OMEGA_USE_OPENCL
#include "omega/CLManager.h"
#endif

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
void GpuProgram::runComputeStage(const ComputeStageOptions& options, GpuProgramParams* params)
{
#ifdef OMEGA_USE_OPENCL
	if(params != NULL) params->bind(this, GpuProgram::ComputeStage);

	cl_event events;
	cl_int status;

	cl_kernel kern = myComputeShader->getCLKernel();
	cl_command_queue clqueue = myGpuMng->getCLManager()->getCommandQueue();

	status = clEnqueueNDRangeKernel(
					clqueue,
					kern,
					options.dimensions,
					NULL,
					(size_t*)options.globalThreads.data(),
					(size_t*)options.localThreads.data(),
					0,
					NULL,
					&events);
	if(!clSuccessOrDie(status)) return;
	status = clWaitForEvents(1, &events);
	if(!clSuccessOrDie(status)) return;
	clReleaseEvent(events);

	if(params != NULL) params->unbind(this, GpuProgram::ComputeStage);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::runRenderStage(const RenderStageOptions& options, GpuProgramParams* params)
{
	if(params != NULL) params->bind(this, GpuProgram::RenderStage);

	if(options.primType != RenderStageOptions::PrimNone)
	{
		GLenum mode = GL_POINTS;
		switch(options.primType)
		{
		case RenderStageOptions::PrimPoints: mode = GL_POINTS; break;
		case RenderStageOptions::PrimTriangles: mode = GL_TRIANGLES; break;
		}

		glUseProgram(myGLProgram);

		if(options.indices == NULL)
		{
			glDrawArrays(mode, 0, options.items);
		}
		else
		{
			glDrawElements(mode, options.items, GL_UNSIGNED_INT, options.indices);
		}

		glUseProgram(0);
	}

	if(params != NULL) params->unbind(this, GpuProgram::RenderStage);
}

