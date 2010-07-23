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

#include "outk/gfx/GpuProgram.h"

#include "boost/unordered_map.hpp"

namespace outk
{
namespace gfx
{
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

		OUTK_API void initialize();

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

		// Active gpu program
		GpuProgram* myActiveProgram;

		// OpenCL stuff.
		cl_context myCLContext;
		cl_device_id* myCLDevices;
		cl_command_queue myCLCommandQueue;
	};
};
}; // namespace omega

#endif