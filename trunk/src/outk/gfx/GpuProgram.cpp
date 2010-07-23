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
#include "outk/gfx/GpuProgram.h"

using namespace omega;
using namespace outk::gfx;

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
ComputeShader::ComputeShader(cl_program program, const omega::String& kernelEntryPoint, const omega::String& name)
{
	cl_int status;
	myCLProgram = program;
	myCLKernel = clCreateKernel(myCLProgram, kernelEntryPoint.c_str(), &status);
	clCheck(status);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ComputeShader::dispose()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GpuProgram::GpuProgram():
	myComputeShader(NULL),
	myGeometryShader(NULL),
	myVertexShader(NULL),
	myFragmentShader(NULL)
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
void GpuProgram::run(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimType primType)
{
	oassert(vertexBuffer);	
	
	vertexBuffer->activate();

	GLenum mode = GL_POINTS;
	switch(primType)
	{
	case PrimPoints: mode = GL_POINTS;
	}

	glDrawArrays(mode, 0, vertexBuffer->getLength());
}
