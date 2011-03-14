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
#ifndef __GPU_PROGRAM_H__
#define __GPU_PROGRAM_H__

#include "osystem.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class GpuBuffer;
	class GpuData;
	class GpuManager;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @interal Checks safety of OpenCL calls.
	bool __clSuccessOrDie(const char* file, int line, int status);
	#define clSuccessOrDie(s) __clSuccessOrDie(__FILE__,__LINE__,s)

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class VertexShader
	{
	public:
		//! @internal Class contructor.
		//! @param glShader - the OpenGl vertex shader id.
		//! @param name - the unique shader name.
		VertexShader(GLuint glShader, const omega::String& name);

		//! @internal Gets the OpenGl vertex shader id.
		GLuint getGLShader() { return myGLShader; }

		omega::String getName() { return myName; }

		OMEGA_API void dispose();

	private:
		GLuint myGLShader;
		omega::String myName;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// @todo implement
	class FragmentShader
	{
	public:
		//! @internal Class contructor.
		//! @param glShader - the OpenGl fragment shader id.
		//! @param name - the unique shader name.
		FragmentShader(GLuint glShader, const omega::String& name);

		//! @internal Gets the OpenGl fragment shader id.
		GLuint getGLShader() { return myGLShader; }

		omega::String getName() { return myName; }

		OMEGA_API void dispose();

	private:
		GLuint myGLShader;
		omega::String myName;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class GeometryShader
	{
	public:
		//! @internal Class contructor.
		//! @param glShader - the OpenGl geometry shader id.
		//! @param name - the unique shader name.
		GeometryShader(GLuint glShader, const omega::String& name);

		//! @internal Gets the OpenGl geometry shader id.
		GLuint getGLShader() { return myGLShader; }

		omega::String getName() { return myName; }

		OMEGA_API void dispose();

	private:
		GLuint myGLShader;
		omega::String myName;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class ComputeShader
	{
	public:
		OMEGA_API ComputeShader(cl_program program, const omega::String& kernelEntryPoint, const omega::String& name);

		cl_program getCLProgram() { return myCLProgram; }
		cl_kernel getCLKernel() { return myCLKernel; }
		omega::String getName() { return myName; }

		OMEGA_API void dispose();

	private:
		cl_program myCLProgram;
		cl_kernel myCLKernel;

		omega::String myName;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Represents a program runnable on a Gpu unit.
	//! A program is composed by a geometry, vertex and fragment shader. Not all programs need to be present.
	class OMEGA_API GpuProgram
	{
	public:
		enum PrimType { PrimNone, PrimPoints, PrimLines, PrimTriangles, PrimTriangleStrip };
		enum Stage { ComputeStage, RenderStage };

	public:
		GpuProgram(GpuManager* gpuMng);

		//! Returns the Gpu manager owning this program.
		GpuManager* getManager() { return myGpuMng; }

		//! Sets the compute shader for this program.
		//! Must be called before initialize()
		void setComputeShader(ComputeShader* shader) { myComputeShader = shader; }
		ComputeShader* getComputeShader() { return myComputeShader; }

		//! Sets the geometry shader for this program.
		//! Must be called before initialize()
		void setGeometryShader(GeometryShader* shader) { myGeometryShader = shader; }
		GeometryShader* getGeometryShader() { return myGeometryShader; }

		//! Sets the vertex shader for this program.
		//! Must be called before initialize()
		void setVertexShader(VertexShader* shader) { myVertexShader = shader; }
		VertexShader* getVertexShader() { return myVertexShader; }

		//! Sets the fragment shader for this program.
		//! Must be called before initialize()
		void setFragmentShader(FragmentShader* shader) { myFragmentShader = shader; }
		FragmentShader* getFragmentShader() { return myFragmentShader; }

		GLuint getGLProgram() { return myGLProgram; }


		void initialize();

		int getComputeDimensions() { return myComputeDimensions; }
		void setComputeDimensions(int value) { myComputeDimensions = value; }
		void runComputeStage(int dimensions, const Vector3i& localThreads, const Vector3i globalThreads);
		void runRenderStage(int items, PrimType primType = PrimNone, unsigned int* indices = NULL);

	private:
		void printProgramLog(GLuint program);

	private:
		omega::String myName;

		GpuManager* myGpuMng;

		// OpenGL program stuff
		GLuint myGLProgram;
		GeometryShader* myGeometryShader;
		VertexShader* myVertexShader;
		FragmentShader* myFragmentShader;
		ComputeShader* myComputeShader;

		// OpenCL program stuff.
		int myComputeDimensions;
	    size_t myGlobalComputeThreads[3];
	    size_t myLocalComputeThreads[3];
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class GpuProgramParams
	{
	public:
		static const int MaxParams = 64;

	public:
		GpuProgramParams() { clearInput(); }

		//! Add a Gpu constant to the program.
		OMEGA_API void setParam(int index, GpuData* input);

		//! Clear the program constant table.
		OMEGA_API void clearInput();

		OMEGA_API void bind(GpuProgram* program, GpuProgram::Stage stage);
		OMEGA_API void unbind(GpuProgram* program, GpuProgram::Stage stage);

	private:
		// Program inputs.
		GpuData* myInput[MaxParams];
	};
}; // namespace omega

#endif