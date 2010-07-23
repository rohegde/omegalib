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

#include "omega.h"

#include "outk/gfx/GpuBuffer.h"

#include "CL/cl.h"

namespace outk
{
namespace gfx
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @interal Checks safety of OpenCL calls.
	void __clCheck(const char* file, int line, int status);
	#define clCheck(s) __clCheck(__FILE__,__LINE__,s);

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

		OUTK_API void dispose();

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

		OUTK_API void dispose();

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

		OUTK_API void dispose();

	private:
		GLuint myGLShader;
		omega::String myName;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class ComputeShader
	{
	public:
		OUTK_API ComputeShader(cl_program program, const omega::String& kernelEntryPoint, const omega::String& name);

		cl_program getCLProgram() { return myCLProgram; }
		cl_kernel getCLKernel() { return myCLKernel; }
		omega::String getName() { return myName; }

		OUTK_API void dispose();

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
		enum PrimType { PrimPoints, PrimLines, PrimTriangles, PrimTriangleStrip };

	public:
		OUTK_API GpuProgram();

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

		void setParameter(const omega::String& name, int value);
		void setParameter(const omega::String& name, omega::Vector2i value);
		void setParameter(const omega::String& name, omega::Vector3i value);
		void setParameter(const omega::String& name, omega::Vector4i value);

		void setParameter(const omega::String& name, float value);
		void setParameter(const omega::String& name, omega::Vector2f value);
		void setParameter(const omega::String& name, omega::Vector3f value);
		void setParameter(const omega::String& name, omega::Vector4f value);

		OUTK_API void initialize();
		OUTK_API void activate();

		OUTK_API void run(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer = NULL, PrimType primType = PrimPoints);

	private:
		void printProgramLog(GLuint program);

	private:
		GeometryShader* myGeometryShader;
		VertexShader* myVertexShader;
		FragmentShader* myFragmentShader;
		ComputeShader* myComputeShader;

		GLuint myGLProgram;

		omega::String myName;
	};
};
}; // namespace omega

#endif