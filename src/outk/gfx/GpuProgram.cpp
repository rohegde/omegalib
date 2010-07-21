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
	glUniform2iv(uid, value.begin());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::setParameter(const omega::String& name, Vector3i value)
{
	GLint uid = glGetUniformLocation(myGLProgram, name.c_str());
	glUniform3iv(uid, value.begin());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::setParameter(const omega::String& name, Vector4i value)
{
	GLint uid = glGetUniformLocation(myGLProgram, name.c_str());
	glUniform4iv(uid, value.begin());
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
	glUniform2fv(uid, value.begin());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GpuProgram::setParameter(const omega::String& name, Vector3f value)
{
	GLint uid = glGetUniformLocation(myGLProgram, name.c_str());
	glUniform3fv(uid, value.begin());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setParameter(const omega::String& name, Vector4f value)
{
	GLint uid = glGetUniformLocation(myGLProgram, name.c_str());
	glUniform4fv(uid, value.begin());
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
VertexShader* getVertexShader(const omega::String& name)
{
	return myVertexShaders[fontName];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FragmentShader* getFragmentShader(const omega::String& name)
{
	return myFragmentShaders[fontName];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GeometryShader* getGeometryShader(const omega::String& name)
{
	return myGeometryShaders[fontName];
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

