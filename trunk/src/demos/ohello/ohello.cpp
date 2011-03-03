/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#include "omega.h"

using namespace omega;

// Defined in teapot.cpp
void teapot(GLint grid, GLdouble scale, GLenum type);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HelloClient: public ApplicationClient
{
public:
	HelloClient(Application* app): ApplicationClient(app) {}
	virtual void draw(const DrawContext& context);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HelloApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new HelloClient(this); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HelloClient::draw(const DrawContext& context)
{
	// Enable depth testing and lighting.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	
	// Setup light.
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_COLOR, Color(1.0, 1.0, 1.0).begin());
	glLightfv(GL_LIGHT0, GL_POSITION, Vector3f(-1.0, 1.0, 0.0).begin());

	// Draw a rotating teapot.
	glRotatef(10, 1, 0, 0);
	glRotatef((float)context.frameNum * 0.1f, 0, 1, 0);
	teapot(14, 0.1f, GL_FILL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	HelloApplication app;

	// Read config file name from command line or use default one.
	const char* cfgName = "ohello.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "ohello.log", new FilesystemDataSource("./../../data/"));
	
	return 0;
}
