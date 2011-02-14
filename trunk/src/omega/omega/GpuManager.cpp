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
#include "omega/GpuManager.h"
#include "omega/SystemManager.h"
#include "omega/RenderTarget.h"

#include "boost/foreach.hpp"
#define boost_foreach BOOST_FOREACH

using namespace omega;

#define HANDLE_STATUS(id) case id: { oerror(#id); break; }

///////////////////////////////////////////////////////////////////////////////////////////////////
bool omega::__clSuccessOrDie(const char* file, int line, int status)
{
	if(status == CL_SUCCESS) return true;
	
	oerror("At %s:%d ", file, line);
	
	switch(status)
	{
		HANDLE_STATUS(CL_SUCCESS                                  )
		HANDLE_STATUS(CL_DEVICE_NOT_FOUND                         )
		HANDLE_STATUS(CL_DEVICE_NOT_AVAILABLE                     )
		HANDLE_STATUS(CL_COMPILER_NOT_AVAILABLE                   )
		HANDLE_STATUS(CL_MEM_OBJECT_ALLOCATION_FAILURE            )
		HANDLE_STATUS(CL_OUT_OF_RESOURCES                         )
		HANDLE_STATUS(CL_OUT_OF_HOST_MEMORY                       )
		HANDLE_STATUS(CL_PROFILING_INFO_NOT_AVAILABLE             )
		HANDLE_STATUS(CL_MEM_COPY_OVERLAP                         )
		HANDLE_STATUS(CL_IMAGE_FORMAT_MISMATCH                    )
		HANDLE_STATUS(CL_IMAGE_FORMAT_NOT_SUPPORTED               )
		HANDLE_STATUS(CL_BUILD_PROGRAM_FAILURE                    )
		HANDLE_STATUS(CL_MAP_FAILURE                              )
		HANDLE_STATUS(CL_INVALID_VALUE                            )
		HANDLE_STATUS(CL_INVALID_DEVICE_TYPE                      )
		HANDLE_STATUS(CL_INVALID_PLATFORM                         )
		HANDLE_STATUS(CL_INVALID_DEVICE                           )
		HANDLE_STATUS(CL_INVALID_CONTEXT                          )
		HANDLE_STATUS(CL_INVALID_QUEUE_PROPERTIES                 )
		HANDLE_STATUS(CL_INVALID_COMMAND_QUEUE                    )
		HANDLE_STATUS(CL_INVALID_HOST_PTR                         )
		HANDLE_STATUS(CL_INVALID_MEM_OBJECT                       )
		HANDLE_STATUS(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR          )
		HANDLE_STATUS(CL_INVALID_IMAGE_SIZE                       )
		HANDLE_STATUS(CL_INVALID_SAMPLER                          )
		HANDLE_STATUS(CL_INVALID_BINARY                           )
		HANDLE_STATUS(CL_INVALID_BUILD_OPTIONS                    )
		HANDLE_STATUS(CL_INVALID_PROGRAM                          )
		HANDLE_STATUS(CL_INVALID_PROGRAM_EXECUTABLE               )
		HANDLE_STATUS(CL_INVALID_KERNEL_NAME                      )
		HANDLE_STATUS(CL_INVALID_KERNEL_DEFINITION                )
		HANDLE_STATUS(CL_INVALID_KERNEL                           )
		HANDLE_STATUS(CL_INVALID_ARG_INDEX                        )
		HANDLE_STATUS(CL_INVALID_ARG_VALUE                        )
		HANDLE_STATUS(CL_INVALID_ARG_SIZE                         )
		HANDLE_STATUS(CL_INVALID_KERNEL_ARGS                      )
		HANDLE_STATUS(CL_INVALID_WORK_DIMENSION                   )
		HANDLE_STATUS(CL_INVALID_WORK_GROUP_SIZE                  )
		HANDLE_STATUS(CL_INVALID_WORK_ITEM_SIZE                   )
		HANDLE_STATUS(CL_INVALID_GLOBAL_OFFSET                    )
		HANDLE_STATUS(CL_INVALID_EVENT_WAIT_LIST                  )
		HANDLE_STATUS(CL_INVALID_EVENT                            )
		HANDLE_STATUS(CL_INVALID_OPERATION                        )
		HANDLE_STATUS(CL_INVALID_GL_OBJECT                        )
		HANDLE_STATUS(CL_INVALID_BUFFER_SIZE                      )
		HANDLE_STATUS(CL_INVALID_MIP_LEVEL                        )
		HANDLE_STATUS(CL_INVALID_GLOBAL_WORK_SIZE                 )
	}

	SystemManager::instance()->postExitRequest("Fatal OpenCL failure");
	return false;
}

#undef HANDLE_STATUS

///////////////////////////////////////////////////////////////////////////////////////////////////
// Move this into a stream reading class in the future.
String readTextFile(const String& filename)
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	fp = fopen(filename.c_str(), "rt");

	if (fp != NULL) 
	{
		fseek(fp, 0, SEEK_END);
		count = ftell(fp);
		rewind(fp);
		if (count > 0) 
		{
			content = (char *)malloc(sizeof(char) * (count+1));
			count = fread(content,sizeof(char),count,fp);
			content[count] = '\0';
		}
		fclose(fp);
	}
	
	if (content == NULL)
	{
		fprintf(stderr, "ERROR: could not load in file %s\n", filename.c_str());
		exit(1);
	}
	String result = String(content);
	free(content);
	return result;
}                    

///////////////////////////////////////////////////////////////////////////////////////////////////
GpuManager::GpuManager():
	myInitialized(false),
	myFrameBuffer(NULL),
	myClient(NULL)
{
	myDefaultProgram = new GpuProgram(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
GpuManager::~GpuManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::initialize(ApplicationClient* client, unsigned int initFlags)
{
	myInitFlags = initFlags;
	myClient = client;

	if(isCLEnabled())
	{
		initCL();
	}

	Vector2i res = myClient->getResolution();

	myFrameBuffer = new RenderTarget();
	myFrameBuffer->initialize(RenderTarget::TypeFrameBuffer, res[0], res[1]);

	myInitialized = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::initCL()
{
#ifdef OMEGA_USE_OPENCL
	cl_int status = 0;
    size_t deviceListSize;

	// Have a look at the available platforms and pick either
	// the AMD one if available or a reasonable default.
    cl_uint numPlatforms;
    cl_platform_id platform = NULL;
    
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if(!clSuccessOrDie(status)) return;
    
    if(numPlatforms > 0)
    {
        cl_platform_id* platforms = (cl_platform_id *)malloc(numPlatforms*sizeof(cl_platform_id));
        status = clGetPlatformIDs(numPlatforms, platforms, NULL);
        if(!clSuccessOrDie(status)) return;
        for(unsigned int i=0; i < numPlatforms; ++i)
        {
            char pbuff[100];
            status = clGetPlatformInfo(
                        platforms[i],
                        CL_PLATFORM_VENDOR,
                        sizeof(pbuff),
                        pbuff,
                        NULL);
			if(!clSuccessOrDie(status)) return;
            platform = platforms[i];
            if(!strcmp(pbuff, "Advanced Micro Devices, Inc."))
            {
                break;
            }
        }
        delete platforms;
    }

	// If we could find our platform, use it. Otherwise pass a NULL and get whatever the
	// implementation thinks we should be using.
	cl_context_properties* cps;

	if(isGLEnabled())
	{
		// The properties specify an OpenGL / OpenCL shared context
		cl_context_properties glcps[] = 
		{ 
			CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 
			CL_GL_CONTEXT_KHR,
			(cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR,
			(cl_context_properties)wglGetCurrentDC(),
			0
		};
		cps = glcps;
	}
	else
	{
		// The properties specify an OpenGL / OpenCL shared context
		cl_context_properties noglcps[] = 
		{ 
			CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 
			0
		};
		cps = noglcps;
	}

    cl_context_properties* cprops = (NULL == platform) ? NULL : cps;


	// Create an OpenCL context
    myCLContext = clCreateContextFromType(cprops, CL_DEVICE_TYPE_GPU, NULL, NULL, &status);
    if(!clSuccessOrDie(status)) return;

    // First, get the size of device list data
    status = clGetContextInfo(myCLContext, CL_CONTEXT_DEVICES, 0, NULL, &deviceListSize);
    if(!clSuccessOrDie(status)) return;

	// Detect OpenCL devices
    myCLDevices = (cl_device_id *)malloc(deviceListSize);
	if(myCLDevices == 0)
	{
		oerror("Error: No devices found.\n");
		return;
	}

    /* Now, get the device list data */
    status = clGetContextInfo(myCLContext, CL_CONTEXT_DEVICES, deviceListSize, myCLDevices, NULL);
    if(!clSuccessOrDie(status)) return;

	// Create an OpenCL command queue
    myCLCommandQueue = clCreateCommandQueue(myCLContext, myCLDevices[0], 0, &status);
    if(!clSuccessOrDie(status)) return;

	omsg("OpenCL: initialization successful!");
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::loadVertexShader(const String& name, const String& filename)
{
	GLuint s = loadGlShader(filename, GL_VERTEX_SHADER);
	VertexShader* sh = new VertexShader(s, name);
	myVertexShaders[name] = sh;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::loadFragmentShader(const String& name, const String& filename)
{
	GLuint s = loadGlShader(filename, GL_FRAGMENT_SHADER);
	FragmentShader* sh = new FragmentShader(s, name);
	myFragmentShaders[name] = sh;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::loadGeometryShader(const String& name, const String& filename)
{
	GLuint s = loadGlShader(filename, GL_GEOMETRY_SHADER);
	GeometryShader* sh = new GeometryShader(s, name);
	myGeometryShaders[name] = sh;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::loadComputeShaders(const String& filename, const Vector<String>& shaderNames)
{
#ifdef OMEGA_USE_OPENCL
	cl_int status = 0;
    
	String ss = readTextFile(filename);
    const char * source    = ss.c_str();
    size_t sourceSize[]    = { strlen(source) };

	omsg("Loading OpenCL program: %s", filename.c_str());

	cl_program program = clCreateProgramWithSource(myCLContext, 1, &source, sourceSize, &status);
    if(!clSuccessOrDie(status)) return;

    // create a cl program executable for all the devices specified 
    status = clBuildProgram(program, 1, myCLDevices, NULL, NULL, NULL);

	// Get build log
	char *build_log;
	size_t ret_val_size;
	clGetProgramBuildInfo(program, myCLDevices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &ret_val_size);
	build_log = (char*)malloc(ret_val_size+1);
	clGetProgramBuildInfo(program, myCLDevices[0], CL_PROGRAM_BUILD_LOG, ret_val_size, build_log, NULL);

	// to be carefully, terminate with \0
	// there's no information in the reference whether the string is 0 terminated or not
	build_log[ret_val_size] = '\0';

	// Print build log.
	omsg(build_log);
	free(build_log);

    if(!clSuccessOrDie(status)) return;

	boost_foreach(String shaderName, shaderNames)
	{
		ComputeShader* sh = new ComputeShader(program, shaderName, shaderName);
		myComputeShaders[shaderName] = sh;
		omsg("Compute shader created: %s", shaderName.c_str());
	}
#endif OMEGA_USE_OPENCL
}

///////////////////////////////////////////////////////////////////////////////////////////////////
VertexShader* GpuManager::getVertexShader(const String& name)
{
	return myVertexShaders[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
FragmentShader* GpuManager::getFragmentShader(const String& name)
{
	return myFragmentShaders[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
GeometryShader* GpuManager::getGeometryShader(const String& name)
{
	return myGeometryShaders[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ComputeShader* GpuManager::getComputeShader(const String& name)
{
	return myComputeShaders[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
GLuint GpuManager::loadGlShader(const String& filename, GLenum type)
{
	GLuint s = glCreateShader(type);
	String ss = readTextFile(filename);
	const char* cstr = ss.c_str();
	glShaderSource(s, 1, &cstr, NULL);
	glCompileShader(s);
	printShaderLog(s);

	return s;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::printShaderLog(GLuint shader)
{
    GLint infoLogLength = 0;
    GLsizei charsWritten  = 0;
    GLchar *infoLog;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0)
    {
        infoLog = (char *) malloc(infoLogLength);
        glGetShaderInfoLog(shader, infoLogLength, &charsWritten, infoLog);
		omsg(infoLog);
        free(infoLog);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::beginDraw()
{
	myFrameBuffer->beginDraw();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::endDraw()
{
	myFrameBuffer->endDraw();
}
