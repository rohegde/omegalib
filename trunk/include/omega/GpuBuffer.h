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

#include "osystem.h"
#include "omega/GpuProgram.h"

#include "CL/cl.h"
#include "CL/cl_gl.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class GpuManager;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class GpuData
	{
	public:
		virtual void bind(GpuProgram* prog, int index, GpuProgram::Stage stage) = 0;
		virtual void unbind(GpuProgram* prog, int index, GpuProgram::Stage stage) = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class GpuConstant: public GpuData
	{
	public:
		//! The supported uniform types.
		enum Type {TypeInt, TypeFloat };

	public:
		OMEGA_API virtual void bind(GpuProgram* prog, int index, GpuProgram::Stage stage);
		OMEGA_API virtual void unbind(GpuProgram* prog, int index, GpuProgram::Stage stage);

		//! Sets the uniform name.
		void setName(omega::String value) { myName = value; }

		//! Gets the uniform name.
		omega::String getName() { return myName; }

		Type getType() { return myType; }

		GLint getGLUniformId() { return myGLUniformId; }
		void setGLUniformId(GLint value) { myGLUniformId = value; }

		//! @internal Returns the memory address of the constant. User for OpenCL
		//! parameter initialization.
		void* getAddress()
		{
			if(myType == TypeInt) return (void*)myIntVector.begin();
			return (void*)myFloatVector.begin();
		}

		//! return the size in bytes of this constant.
		int getSize()
		{
			return myComponents * sizeof(float);
		}

		void setIntValue(int x)
		{
			myType = TypeInt;
			myComponents = 1;
			myIntVector.x() = x;
		}

		void setIntValue(int x, int y)
		{
			myType = TypeInt;
			myComponents = 2;
			myIntVector.x() = x;
			myIntVector.y() = y;
		}

		void setIntValue(int x, int y, int z)
		{
			myType = TypeInt;
			myComponents = 3;
			myIntVector.x() = x;
			myIntVector.y() = y;
			myIntVector.z() = z;
		}

		void setIntValue(int x, int y, int z, int w)
		{
			myType = TypeInt;
			myComponents = 4;
			myIntVector.x() = x;
			myIntVector.y() = y;
			myIntVector.z() = z;
			myIntVector.w() = w;
		}

		omega::Vector4i& getIntValue()
		{
			oassert(myType == TypeInt);
			return myIntVector;
		}

		void setFloatValue(float x)
		{
			myType = TypeFloat;
			myComponents = 1;
			myFloatVector.x() = x;
		}

		void setFloatValue(float x, float y)
		{
			myType = TypeFloat;
			myComponents = 2;
			myFloatVector.x() = x;
			myFloatVector.y() = y;
		}

		void setFloatValue(float x, float y, float z)
		{
			myType = TypeFloat;
			myComponents = 3;
			myFloatVector.x() = x;
			myFloatVector.y() = y;
			myFloatVector.z() = z;
		}

		void setFloatValue(float x, float y, float z, float w)
		{
			myType = TypeFloat;
			myComponents = 4;
			myFloatVector.x() = x;
			myFloatVector.y() = y;
			myFloatVector.z() = z;
			myFloatVector.w() = w;
		}

		omega::Vector4f& getFloatValue()
		{
			oassert(myType == TypeFloat);
			return myFloatVector;
		}

		int getComponents()
		{
			return myComponents;
		}

	private:
		Type myType;
		omega::String myName;

		GLint myGLUniformId;

		omega::Vector4f myFloatVector;
		omega::Vector4i myIntVector;
		int myComponents;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class GpuBuffer: public GpuData
	{
	public:
		enum BufferFlags { BufferFlagsCLNative = 1 << 1};

	public:
		OMEGA_API GpuBuffer(GpuManager* mng);
		OMEGA_API ~GpuBuffer();

		OMEGA_API void initialize(int size, int elementSize, void* data = NULL, unsigned int bufferFlags = 0);
		OMEGA_API virtual void bind(GpuProgram* prog, int index, GpuProgram::Stage stage);
		OMEGA_API virtual void unbind(GpuProgram* prog, int index, GpuProgram::Stage stage);
		
		OMEGA_API void copyTo(GpuBuffer* destination, int srcOffset,int dstOffset,  int length);

		//! Gets wether this buffer is a CL native buffer. 
		bool isCLNative() { return myBufferFlags & BufferFlagsCLNative; } 

		//! Sets the gpu buffer data
		OMEGA_API void setData(void* data);

		GLuint getGLBuffer() { return myGLBuffer; }
		OMEGA_API cl_mem getCLBuffer();

		int getSize() { return mySize; }
		int getElementSize() { return myElementSize; }
		int getLength() { return myLength; }

	protected:
		GpuManager* myGpu;

	private:
		void acquireGLBuffer();
		void unacquireGLBuffer();

	private:
		GLuint myGLBuffer;
		cl_mem myCLBuffer;

		unsigned int myBufferFlags;

		int mySize;
		int myElementSize;
		int myLength;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct VertexAttribute
	{
		//! The attirbute components type.
		enum Type 
		{ 
			TypeInt, 
			TypeFloat 
		};

		//! Specified the supported attribute destination semantics.
		enum Target 
		{ 
			TargetPosition, 
			TargetPrimaryColor, 
			TargetSecondaryColor, 
			TargetTexture0, 
			TargetTexture1, 
			TargetTexture2, 
			TargetTexture3 
		};

		//! Constructor
		VertexAttribute() {}

		//! Constructor
		VertexAttribute(Target _target, Type _type, int _offset, int _components):
		  target(_target), type(_type), offset(_offset), components(_components) {}

		//! The number of components for this attribute.
		int components;
		//! The offset in the buffer for this attribute's data.
		int offset;
		//! The component time for this attribute.
		Type type;
		//! The destination semantics for the attribute.
		Target target;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class VertexBuffer: public GpuBuffer
	{
	public:
		VertexBuffer(GpuManager* mng):
		  GpuBuffer(mng)
		  {}

		//! @name Attribute Management
		VertexAttribute* getAttribute(int index) { return &myAttributes[index]; }
		OMEGA_API void clearAttributes();
		OMEGA_API int getNumAttributes();
		OMEGA_API int addAttribute(const VertexAttribute& attrib);

		//! @name Internal Functions
		//! @internal Applies the vertex buffer attributes, binding vertex data to their specified semantics.
		//! This method is always called before rendering data from the vertex buffer.
		OMEGA_API virtual void bind(GpuProgram* prog, int index, GpuProgram::Stage stage);
		OMEGA_API virtual void unbind(GpuProgram* prog, int index, GpuProgram::Stage stage);

	private:
		std::vector<VertexAttribute> myAttributes;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class IndexBuffer: public GpuBuffer
	{
	public:
		IndexBuffer(GpuManager* mng):
		  GpuBuffer(mng)
		  {}

	private:
	};

}; // namespace omega

#endif