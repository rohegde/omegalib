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
#ifndef __GPU_MANAGER_H__
#define __GPU_MANAGER_H__

#include "omega.h"

#include "boost/unordered_map.hpp"

namespace outk
{
namespace gfx
{
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
	/*class ComputeShader
	{
	public:
		ComputeShader(GLuint glShader, const omega::String& name);

		//GLUint getGLShader() { return myGLShader; }
		String getName() { return myName; }

		OUTK_API dispose();

	private:
		//GLuint myGLShader;
		omega::String myName;
	}*/

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Represents a program runnable on a Gpu unit.
	//! A program is composed by a geometry, vertex and fragment shader. Not all programs need to be present.
	class GpuProgram
	{
	public:
		GpuProgram();

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

	private:
		void printProgramLog(GLuint program);

	private:
		GeometryShader* myGeometryShader;
		VertexShader* myVertexShader;
		FragmentShader* myFragmentShader;

		GLuint myGLProgram;

		omega::String myName;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! A dictionary containing <String, VertexShader*> pairs.
	typedef boost::unordered_map<omega::String, VertexShader*> VertexShaderDictionary;
	//! A dictionary containing <String, FragmentShader*> pairs.
	typedef boost::unordered_map<omega::String, FragmentShader*> FragmentShaderDictionary;
	//! A dictionary containing <String, GeometryShader*> pairs.
	typedef boost::unordered_map<omega::String, GeometryShader*> GeometryShaderDictionary;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Loads images and manages OpenGL textures.
	class GpuManager
	{
	public:
		OUTK_API GpuManager();
		OUTK_API virtual ~GpuManager();

		OUTK_API void loadVertexShader(const omega::String& name, const omega::String& filename);
		OUTK_API void loadFragmentShader(const omega::String& name, const omega::String& filename);
		OUTK_API void loadGeometryShader(const omega::String& name, const omega::String& filename);

		OUTK_API VertexShader* getVertexShader(const omega::String& name);
		OUTK_API FragmentShader* getFragmentShader(const omega::String& name);
		OUTK_API GeometryShader* getGeometryShader(const omega::String& name);

	private:
		GLuint loadGlShader(const omega::String& filename, GLenum type);
		void printShaderLog(GLuint shader);

	private:
		VertexShaderDictionary myVertexShaders;
		FragmentShaderDictionary myFragmentShaders;
		GeometryShaderDictionary myGeometryShaders;
	};
};
}; // namespace omega

#endif