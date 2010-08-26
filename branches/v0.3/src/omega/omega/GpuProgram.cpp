/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "omega/GpuProgram.h"
#include "omega/GpuManager.h"

using namespace omega;

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GpuProgram::GpuProgram(GpuManager* gpuMng):
	myComputeShader(NULL),
	myGeometryShader(NULL),
	myVertexShader(NULL),
	myFragmentShader(NULL),
	myGpuMng(gpuMng)
{
	oassert(gpuMng != NULL);
	clearInput();
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
void GpuProgram::setInput(int index, GpuData* inputData)
{
	myInput[index] = inputData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::clearInput()
{
	memset(myInput, 0, MaxInputs * sizeof(GpuData*));
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
void GpuProgram::runComputeShader()
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::run(PrimType primType)
{
	// If a compute shader is present, run it.
	if(myComputeShader != NULL)
	{
		for(int i = 0; i < MaxInputs; i++)
		{
			GpuData* data = myInput[i];
			if(data != NULL)
			{
				data->bind(this, i, GpuData::BindToComputeStage);
			}
		}
		runComputeShader();
		for(int i = 0; i < MaxInputs; i++)
		{
			GpuData* data = myInput[i];
			if(data != NULL)
			{
				data->unbind(this, i, GpuData::BindToComputeStage);
			}
		}
	}

	if(primType != PrimNone)
	{
		GLenum mode = GL_POINTS;
		switch(primType)
		{
		case PrimPoints: mode = GL_POINTS;
		}

		glUseProgram(myGLProgram);

		for(int i = 0; i < MaxInputs; i++)
		{
			GpuData* data = myInput[i];
			if(data != NULL)
			{
				data->bind(this, i, GpuData::BindToRenderStage);
			}
		}
		glDrawArrays(mode, 0, myNumRenderItems);
		for(int i = 0; i < MaxInputs; i++)
		{
			GpuData* data = myInput[i];
			if(data != NULL)
			{
				data->unbind(this, i, GpuData::BindToRenderStage);
			}
		}

		glUseProgram(0);
	}
}
