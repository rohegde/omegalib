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

#include "osystem.h"
#include "omega/GpuProgram.h"
#include "boost/unordered_map.hpp"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! A dictionary containing <String, VertexShader*> pairs.
	typedef boost::unordered_map<omega::String, VertexShader*> VertexShaderDictionary;
	//! A dictionary containing <String, FragmentShader*> pairs.
	typedef boost::unordered_map<omega::String, FragmentShader*> FragmentShaderDictionary;
	//! A dictionary containing <String, GeometryShader*> pairs.
	typedef boost::unordered_map<omega::String, GeometryShader*> GeometryShaderDictionary;
	//! A dictionary containing <String, ComputeShader*> pairs.
	typedef boost::unordered_map<omega::String, ComputeShader*> ComputeShaderDictionary;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Loads images and manages OpenGL textures.
	class GpuManager
	{
	public:
		enum InitFlags { InitCL = 1 << 1, InitGL = 1 << 2};

	public:
		OMEGA_API GpuManager();
		OMEGA_API virtual ~GpuManager();

		bool isInitialized() { return myInitialized; }
		bool isCLEnabled() { return ((myInitFlags & InitCL) == InitCL); }
		bool isGLEnabled() { return ((myInitFlags & InitGL) == InitGL); }

		OMEGA_API void initialize(unsigned int initFlags = InitCL | InitGL);

		OMEGA_API void loadVertexShader(const omega::String& name, const omega::String& filename);
		OMEGA_API void loadFragmentShader(const omega::String& name, const omega::String& filename);
		OMEGA_API void loadGeometryShader(const omega::String& name, const omega::String& filename);
		OMEGA_API void loadComputeShaders(const omega::String& filename, const std::vector<omega::String>& shaderNames);

		OMEGA_API VertexShader* getVertexShader(const omega::String& name);
		OMEGA_API FragmentShader* getFragmentShader(const omega::String& name);
		OMEGA_API GeometryShader* getGeometryShader(const omega::String& name);
		OMEGA_API ComputeShader* getComputeShader(const omega::String& name);

		cl_context getCLContext() { return myCLContext; }
		cl_command_queue getCLCommandQueue() { return myCLCommandQueue; }

	private:
		void  initCL();

		GLuint loadGlShader(const omega::String& filename, GLenum type);
		void printShaderLog(GLuint shader);

	private:
		bool myInitialized;
		unsigned int myInitFlags;

		VertexShaderDictionary myVertexShaders;
		FragmentShaderDictionary myFragmentShaders;
		GeometryShaderDictionary myGeometryShaders;
		ComputeShaderDictionary myComputeShaders;

		// OpenCL stuff.
		cl_context myCLContext;
		cl_device_id* myCLDevices;
		cl_command_queue myCLCommandQueue;
	};
}; // namespace omega

#endif