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
#include "outk/gfx/GpuBuffer.h"
#include "outk/gfx/GpuManager.h"

using namespace omega;
using namespace outk::gfx;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuConstant::bind(GpuProgram* prog, int index, GpuData::BindType bindType)
{
	if(bindType == GpuData::BindToComputeStage)
	{
		cl_int status;
		cl_event events;
		cl_kernel kernel = prog->getComputeShader()->getCLKernel();

		switch(myType)
		{
		case TypeFloat:
			status = clSetKernelArg(kernel, index, getSize(), getAddress());
			break;
		case TypeInt:
			status = clSetKernelArg(kernel, index, getSize(), getAddress());
			break;
		}
		if(!clSuccessOrDie(status)) return;
	}
	else if(bindType == GpuData::BindToRenderStage)
	{
		GLint uid = glGetUniformLocation(prog->getGLProgram(), myName.c_str());
		switch(myType)
		{
		case TypeFloat:
			if(myComponents == 1) glUniform1f(uid, myFloatVector[0]);
			else if(myComponents == 2) glUniform2f(uid, myFloatVector[0], myFloatVector[1]);
			else if(myComponents == 3) glUniform3f(uid, myFloatVector[0], myFloatVector[1], myFloatVector[2]);
			else if(myComponents == 4) glUniform4f(uid, myFloatVector[0], myFloatVector[1], myFloatVector[2], myFloatVector[3]);
			break;
		case TypeInt:
			if(myComponents == 1) glUniform1i(uid, myIntVector[0]);
			else if(myComponents == 2) glUniform2i(uid, myIntVector[0], myIntVector[1]);
			else if(myComponents == 3) glUniform3i(uid, myIntVector[0], myIntVector[1], myIntVector[2]);
			else if(myComponents == 4) glUniform4i(uid, myIntVector[0], myIntVector[1], myIntVector[2], myIntVector[3]);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuConstant::unbind(GpuProgram* prog, int index, GpuData::BindType bindType)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OUTK_API GpuBuffer::GpuBuffer(GpuManager* mng):
	myGpu(mng),
	myGLBuffer(0),
	myCLBuffer(NULL),
	mySize(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GpuBuffer::~GpuBuffer()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuBuffer::initialize(int size, int elementSize, void* data)
{
	glGenBuffers(1, &myGLBuffer);

	mySize = size;
	myElementSize = elementSize;
	myLength = mySize / myElementSize;

	setData(data);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuBuffer::setData(void* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, myGLBuffer);
	glBufferData(GL_ARRAY_BUFFER, mySize, data, GL_DYNAMIC_DRAW);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuBuffer::bind(GpuProgram* prog, int index, GpuData::BindType bindType)
{
	if(bindType == GpuData::BindToComputeStage)
	{
		cl_int status;
		cl_event events;
		cl_kernel kernel = prog->getComputeShader()->getCLKernel();
		cl_command_queue clqueue = prog->getManager()->getCLCommandQueue();
		cl_mem clbuf = getCLBuffer();

		status = clSetKernelArg(kernel, index, sizeof(cl_mem), (const void*)&clbuf);
		if(!clSuccessOrDie(status)) return;

		status = clEnqueueAcquireGLObjects(clqueue, 1, &clbuf, 0, NULL, &events);
		if(!clSuccessOrDie(status)) return;
		status = clWaitForEvents(1, &events);
		if(!clSuccessOrDie(status)) return;
		clReleaseEvent(events);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuBuffer::unbind(GpuProgram* prog, int index, GpuData::BindType bindType)
{
	if(bindType == GpuData::BindToComputeStage)
	{
		cl_int status;
		cl_event events;
		cl_kernel kernel = prog->getComputeShader()->getCLKernel();
		cl_command_queue clqueue = prog->getManager()->getCLCommandQueue();
		cl_mem clbuf = getCLBuffer();

		status = clEnqueueReleaseGLObjects(clqueue, 1, &clbuf, 0, NULL, &events);
		if(!clSuccessOrDie(status)) return;
		status = clWaitForEvents(1, &events);
		if(!clSuccessOrDie(status)) return;
		clReleaseEvent(events);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VertexBuffer::clearAttributes()
{
	myAttributes.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int VertexBuffer::getNumAttributes()
{
	return myAttributes.size();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int VertexBuffer::addAttribute(const VertexAttribute& attrib)
{
	myAttributes.push_back(attrib);
	return myAttributes.size();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VertexBuffer::bind(GpuProgram* prog, int index, GpuData::BindType bindType)
{
	if(bindType == GpuData::BindToRenderStage)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, getGLBuffer());

		for(int i = 0; i < getNumAttributes(); i++)
		{
			VertexAttribute* attrib = getAttribute(i);
			GLenum glType;
			switch(attrib->type)
			{
			case VertexAttribute::TypeInt:
				glType = GL_INT;
				break;
			case VertexAttribute::TypeFloat:
				glType = GL_FLOAT;
				break;
			}

			// Handle position attribute.
			if(attrib->target == VertexAttribute::TargetPosition)
			{
				glVertexPointer(attrib->components, glType, getElementSize(), (GLvoid*)attrib->offset);
			}
			else if(attrib->target == VertexAttribute::TargetPrimaryColor)
			{
				glColorPointer(attrib->components, glType, getElementSize(), (GLvoid*)attrib->offset);
			}
			else if(attrib->target == VertexAttribute::TargetSecondaryColor)
			{
				glSecondaryColorPointer(attrib->components, glType, getElementSize(), (GLvoid*)attrib->offset);
			}
		}
	}
	else
	{
		// For compute stage, just perform standard buffer binding.
		GpuBuffer::bind(prog, index, bindType);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VertexBuffer::unbind(GpuProgram* prog, int index, GpuData::BindType bindType)
{
	// Nothing to do for now.
	GpuBuffer::unbind(prog, index, bindType);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cl_mem GpuBuffer::getCLBuffer()
{ 
	if(myCLBuffer == NULL)
	{
		cl_int status;
		myCLBuffer = clCreateFromGLBuffer(myGpu->getCLContext(), CL_MEM_READ_WRITE, myGLBuffer, &status);
		if(!clSuccessOrDie(status)) return NULL;
	}
	return myCLBuffer; 
}
