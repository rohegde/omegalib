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

using namespace omega;
using namespace outk::gfx;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OUTK_API GpuBuffer::GpuBuffer():
	myGLBuffer(0),
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
	glBindBuffer(GL_ARRAY_BUFFER, myGLBuffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

	mySize = size;
	myElementSize = elementSize;
	myLength = mySize / myElementSize;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VertexBuffer::activate()
{
	// Should I go somewhere else?
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, getGLBuffer());
	glVertexPointer(myPositionComponents, GL_FLOAT, getElementSize(), 0);
}
