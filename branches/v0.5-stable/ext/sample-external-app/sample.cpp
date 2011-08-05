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

const char* configString = 
"@config: { "
" display: { "
"  type = \"Glut\";"
"  config = \"854 480 --fov 10\";"
"  views: { view3D: { layers = [0]; }; };"
"  observers: { observer0: { referencePosition = [ 0.0, 0.0, 2.0 ]; }; };"
"}; };";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SampleClient: public ApplicationClient
{
public:
	SampleClient(Application* app): ApplicationClient(app) {}
	virtual void draw(const DrawContext& context);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SampleApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new SampleClient(this); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SampleClient::draw(const DrawContext& context)
{
	// Enable depth testing and lighting.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	// As simple as it gets: draw a dot.
	glPointSize(320);
	glColor3f(0.8f, 1.0f, 0.8f);
	glBegin(GL_POINTS);
	glVertex3f(0, 0, 0);
	glEnd();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	SampleApplication app;
	omain(app, configString, "ohello.log", new FilesystemDataSource("./../../data/"));
	return 0;
}
