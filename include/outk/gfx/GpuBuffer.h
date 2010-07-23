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
#ifndef __GPU_BUFFER_H__
#define __GPU_BUFFER_H__

#include "omega.h"

namespace outk
{
namespace gfx
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class GpuBuffer
	{
	public:
		OUTK_API GpuBuffer();
		OUTK_API ~GpuBuffer();

		OUTK_API void initialize(int size, int elementSize, void* data = NULL);

		GLuint getGLBuffer() { return myGLBuffer; }

		int getSize() { return mySize; }
		int getElementSize() { return myElementSize; }
		int getLength() { return myLength; }

	private:
		GLuint myGLBuffer;
		int mySize;
		int myElementSize;
		int myLength;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class VertexBuffer: public GpuBuffer
	{
	public:
		VertexBuffer():
		  myPositionComponents(3),
		  myPositionOffset(0)
		  {}

		int getPositionOffset() { return myPositionOffset; }
		void setPositionOffset(int value) { myPositionOffset = value; }

		int getPositionComponents() { return myPositionComponents; }
		void setPositionComponents(int value) { myPositionComponents = value; }

		//! @internal Applies the vertex buffer attributes, binding vertex data to their specified semantics.
		//! This method is always called before rendering data from the vertex buffer.
		OUTK_API void activate();

	private:
		int myPositionOffset;
		int myPositionComponents;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class IndexBuffer: public GpuBuffer
	{
	public:

	private:
	};

}; // namespace gfx
}; // namespace outk

#endif