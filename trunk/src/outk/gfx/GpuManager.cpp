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
#include "outk/gfx/GpuManager.h"

using namespace omega;
using namespace outk::gfx;

#define clCheck(s) clSafe(__FILE__,__LINE__,s);

#define HANDLE_STATUS(id) case id: { oerror(#id"\n"); break; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clSafe(const char* file, int line, int status)
{
	if(status == CL_SUCCESS) return;
	
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
	
	exit(1);
}

#undef HANDLE_STATUS

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VertexShader::VertexShader(GLuint GLShader, const omega::String& name):
	myGLShader(GLShader), 
	myName(name)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VertexShader::dispose()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FragmentShader::FragmentShader(GLuint GLShader, const omega::String& name):
	myGLShader(GLShader), 
	myName(name)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FragmentShader::dispose()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GeometryShader::GeometryShader(GLuint GLShader, const omega::String& name):
	myGLShader(GLShader), 
	myName(name)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::initialize()
{
	myGLProgram = glCreateProgram();

	// Attach shaders.
	if(myGeometryShader != NULL)
		glAttachShader(myGLProgram, myGeometryShader->getGLShader());

	if(myVertexShader != NULL)
		glAttachShader(myGLProgram, myVertexShader->getGLShader());

	if(myFragmentShader != NULL)
		glAttachShader(myGLProgram, myFragmentShader->getGLShader());

	// Link the GL program.
	glLinkProgram(myGLProgram);
	printProgramLog(myGLProgram);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::setParameter(const omega::String& name, int value)
{
	GLint uid = glGetUniformLocation(myGLProgram, name.c_str());
    glUniform1i(uid, value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::setParameter(const omega::String& name, omega::Vector2i value)
{
	GLint uid = glGetUniformLocation(myGLProgram, name.c_str());
	glUniform2iv(uid, 1, value.begin());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::setParameter(const omega::String& name, Vector3i value)
{
	GLint uid = glGetUniformLocation(myGLProgram, name.c_str());
	glUniform3iv(uid, 1, value.begin());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::setParameter(const omega::String& name, Vector4i value)
{
	GLint uid = glGetUniformLocation(myGLProgram, name.c_str());
	glUniform4iv(uid, 1, value.begin());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::setParameter(const omega::String& name, float value)
{
	GLint uid = glGetUniformLocation(myGLProgram, name.c_str());
	glUniform1f(uid, value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::setParameter(const omega::String& name, Vector2f value)
{
	GLint uid = glGetUniformLocation(myGLProgram, name.c_str());
	glUniform2fv(uid, 1, value.begin());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::setParameter(const omega::String& name, Vector3f value)
{
	GLint uid = glGetUniformLocation(myGLProgram, name.c_str());
	glUniform3fv(uid, 1, value.begin());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::setParameter(const omega::String& name, Vector4f value)
{
	GLint uid = glGetUniformLocation(myGLProgram, name.c_str());
	glUniform4fv(uid, 1, value.begin());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::activate()
{
	glUseProgram(myGLProgram);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::printProgramLog(GLuint program)
{
    GLint infoLogLength = 0;
    GLsizei charsWritten  = 0;
    GLchar *infoLog;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0)
    {
        infoLog = (char *) malloc(infoLogLength);
        glGetProgramInfoLog(program, infoLogLength, &charsWritten, infoLog);
		omsg(infoLog);
        free(infoLog);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GpuManager::GpuManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GpuManager::~GpuManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::initialize()
{
	cl_int status = 0;
    size_t deviceListSize;

    /*
     * Have a look at the available platforms and pick either
     * the AMD one if available or a reasonable default.
     */

    cl_uint numPlatforms;
    cl_platform_id platform = NULL;
    
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    clCheck(status);
    
    if(numPlatforms > 0)
    {
        cl_platform_id* platforms = (cl_platform_id *)malloc(numPlatforms*sizeof(cl_platform_id));
        status = clGetPlatformIDs(numPlatforms, platforms, NULL);
        clCheck(status);
        for(unsigned int i=0; i < numPlatforms; ++i)
        {
            char pbuff[100];
            status = clGetPlatformInfo(
                        platforms[i],
                        CL_PLATFORM_VENDOR,
                        sizeof(pbuff),
                        pbuff,
                        NULL);
			clCheck(status);
            platform = platforms[i];
            if(!strcmp(pbuff, "Advanced Micro Devices, Inc."))
            {
                break;
            }
        }
        delete platforms;
    }
    /* 
     * If we could find our platform, use it. Otherwise pass a NULL and get whatever the
     * implementation thinks we should be using.
     */

    cl_context_properties cps[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0 };
    cl_context_properties* cprops = (NULL == platform) ? NULL : cps;


	/////////////////////////////////////////////////////////////////
	// Create an OpenCL context
	/////////////////////////////////////////////////////////////////
    myCLContext = clCreateContextFromType(cprops, CL_DEVICE_TYPE_GPU, NULL, NULL, &status);
    clCheck(status);

    /* First, get the size of device list data */
    status = clGetContextInfo(myCLContext, CL_CONTEXT_DEVICES, 0, NULL, &deviceListSize);
    clCheck(status);

	/////////////////////////////////////////////////////////////////
	// Detect OpenCL devices
	/////////////////////////////////////////////////////////////////
    myCLDevices = (cl_device_id *)malloc(deviceListSize);
	if(myCLDevices == 0)
	{
		printf("Error: No devices found.\n");
		return;
	}

    /* Now, get the device list data */
    status = clGetContextInfo(myCLContext, CL_CONTEXT_DEVICES, deviceListSize, myCLDevices, NULL);
    clCheck(status);

	/////////////////////////////////////////////////////////////////
	// Create an OpenCL command queue
	/////////////////////////////////////////////////////////////////
    myCLCommandQueue = clCreateCommandQueue(myCLContext, myCLDevices[0], 0, &status);
    clCheck(status);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::loadVertexShader(const omega::String& name, const omega::String& filename)
{
	GLuint s = loadGlShader(filename, GL_VERTEX_SHADER);
	VertexShader* sh = new VertexShader(s, name);
	myVertexShaders[name] = sh;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::loadFragmentShader(const omega::String& name, const omega::String& filename)
{
	GLuint s = loadGlShader(filename, GL_FRAGMENT_SHADER);
	FragmentShader* sh = new FragmentShader(s, name);
	myFragmentShaders[name] = sh;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuManager::loadGeometryShader(const omega::String& name, const omega::String& filename)
{
	GLuint s = loadGlShader(filename, GL_GEOMETRY_SHADER);
	GeometryShader* sh = new GeometryShader(s, name);
	myGeometryShaders[name] = sh;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VertexShader* GpuManager::getVertexShader(const omega::String& name)
{
	return myVertexShaders[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FragmentShader* GpuManager::getFragmentShader(const omega::String& name)
{
	return myFragmentShaders[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GeometryShader* GpuManager::getGeometryShader(const omega::String& name)
{
	return myGeometryShaders[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLuint GpuManager::loadGlShader(const omega::String& filename, GLenum type)
{
	GLuint s = glCreateShader(type);
	String ss = Utils::readTextFile(filename);
	const char* cstr = ss.c_str();
	glShaderSource(s, 1, &cstr, NULL);
	glCompileShader(s);
	printShaderLog(s);

	return s;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

