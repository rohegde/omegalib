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
#ifndef __GPU_PROGRAM_H__
#define __GPU_PROGRAM_H__

#include "osystem.h"
#include "omega/GpuBuffer.h"

namespace omega
{
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
	class GpuProgram
	{
	public:
		enum PrimType { PrimNone, PrimPoints, PrimLines, PrimTriangles, PrimTriangleStrip };
		static const int MaxInputs = 64;

	public:
		OMEGA_API GpuProgram(GpuManager* gpuMng);

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

		//! Add a Gpu constant to the program.
		OMEGA_API void setInput(int index, GpuData* input);

		//! Clear the program constant table.
		OMEGA_API void clearInput();

		int getComputeDimensions() { return myComputeDimensions; }
		void setComputeDimensions(int value) { myComputeDimensions = value; }

		size_t getGlobalComputeThreads(int dim) { return myGlobalComputeThreads[dim]; }
		size_t getLocalComputeThreads(int dim) { return myLocalComputeThreads[dim]; }
		void setGlobalComputeThreads(int dim, size_t value) { myGlobalComputeThreads[dim] = value; }
		void setLocalComputeThreads(int dim, size_t value) { myLocalComputeThreads[dim] = value; }
		
		void setNumRenderItems(int size) { myNumRenderItems = size; }
		int getNumrenderItems() { return myNumRenderItems; }

		OMEGA_API void initialize();

		OMEGA_API void run(PrimType primType = PrimNone);

	private:
		void printProgramLog(GLuint program);
		void runComputeShader();

	private:
		omega::String myName;

		GpuManager* myGpuMng;

		// Program inputs.
		GpuData* myInput[MaxInputs];

		// OpenGL program stuff
		GLuint myGLProgram;
		GeometryShader* myGeometryShader;
		VertexShader* myVertexShader;
		FragmentShader* myFragmentShader;
		int myNumRenderItems;

		// OpenCL program stuff.
		ComputeShader* myComputeShader;
		int myComputeDimensions;
	    size_t myGlobalComputeThreads[3];
	    size_t myLocalComputeThreads[3];
	};
}; // namespace omega

#endif