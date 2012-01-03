/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __GPU_MANAGER_H__
#define __GPU_MANAGER_H__

#include "osystem.h"
#include "omega/GpuProgram.h"

// HACK: To be removed (see GpuManager::TextureUnit)
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8

namespace omega
{
	class CLManager;
	class Texture;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! A dictionary containing <String, VertexShader*> pairs.
	typedef Dictionary<String, VertexShader*> VertexShaderDictionary;
	//! A dictionary containing <String, FragmentShader*> pairs.
	typedef Dictionary<String, FragmentShader*> FragmentShaderDictionary;
	//! A dictionary containing <String, GeometryShader*> pairs.
	typedef Dictionary<String, GeometryShader*> GeometryShaderDictionary;
	//! A dictionary containing <String, ComputeShader*> pairs.
	typedef Dictionary<String, ComputeShader*> ComputeShaderDictionary;
	//! A dictionary containing <String, Texture*> pairs.
	typedef Dictionary<String, Texture*> TextureDictionary;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Loads images and manages OpenGL textures.
	class OMEGA_API GpuManager
	{
	public:
		enum InitFlags { InitCL = 1 << 1, InitGL = 1 << 2};
		enum TextureUnit {
			TextureUnitInvalid = 0,
			TextureUnit0 = GL_TEXTURE0, 
			TextureUnit1 = GL_TEXTURE1,
			TextureUnit2 = GL_TEXTURE2,
			TextureUnit3 = GL_TEXTURE3 };
	public:
		GpuManager();
		virtual ~GpuManager();

		//! Initialization
		//@{
		void setInitFlags(unsigned int initFlags) { myInitFlags = initFlags; }
		bool isInitialized() { return myInitialized; }
		void initialize();
		//@}

		//! Gpu configuration
		//@{
		bool isCLEnabled() { return ((myInitFlags & InitCL) == InitCL); }
		bool isGLEnabled() { return ((myInitFlags & InitGL) == InitGL); }
		//@}

		//! Returns a numeric identifier for this gpu.
		uint getId();

		void loadVertexShader(const String& name, const String& filename);
		void loadFragmentShader(const String& name, const String& filename);
		void loadGeometryShader(const String& name, const String& filename);
		void loadComputeShaders(const String& filename, const Vector<omega::String>& shaderNames);

		VertexShader* getVertexShader(const String& name);
		FragmentShader* getFragmentShader(const String& name);
		GeometryShader* getGeometryShader(const String& name);
		ComputeShader* getComputeShader(const String& name);
		//void beginDraw();
		//void endDraw();

		//! Data
		//@{
		//GpuProgram* getDefaultProgram();
		//@}

		CLManager* getCLManager() { return myCLManager; }

	private:
		GLuint loadGlShader(const String& filename, GLenum type);
		void printShaderLog(GLuint shader);

	private:
		bool myInitialized;
		unsigned int myInitFlags;

		uint myId;

		VertexShaderDictionary myVertexShaders;
		FragmentShaderDictionary myFragmentShaders;
		GeometryShaderDictionary myGeometryShaders;
		ComputeShaderDictionary myComputeShaders;
		TextureDictionary myTextures;

		// Default gpu program
		//GpuProgram* myDefaultProgram;

		CLManager* myCLManager;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API GpuContext
	{
	public:
		static const unsigned int MaxContexts = 64;

		GpuContext(GpuManager* gpu);

		uint getId() { return myId; }
		GpuManager* getGpu() { return myGpu; }

	private:
		static uint mysNumContexts;
		static Lock mysContextLock;

		uint myId;
		GpuManager* myGpu;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API GpuResource: public DynamicObject
	{
	public:
		GpuResource(GpuContext* ctx): myContext(ctx) {}
		GpuContext* getContext() { return myContext; }
	private:
		GpuContext* myContext;
	};
}; // namespace omega

#endif