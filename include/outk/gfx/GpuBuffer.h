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

		OUTK_API void initialize(int size, void* data = NULL);

		GLuint getGLBuffer() { return myGLBuffer; }

		int getSize() { return mySize; }

	private:
		GLuint myGLBuffer;
		int mySize;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class VertexBuffer: public GpuBuffer
	{
	public:

	private:
	};

}; // namespace gfx
}; // namespace outk

#endif