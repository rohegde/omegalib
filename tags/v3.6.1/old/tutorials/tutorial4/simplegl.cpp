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
#include <omega.h>
#include <omegaGl.h>

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SimpleGLClient: public ApplicationClient
{
public:
	SimpleGLClient(ApplicationServer* server): ApplicationClient(server) {}
	virtual void initialize();
	virtual void draw(const DrawContext& context);

private:
	Vector3f myNormals[6];
	Vector4i myFaces[6]; 
	Vector3f myVertices[8];
	Color myFaceColors[6];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SimpleGLApplication: public Application
{
public:
	virtual ApplicationClient* createClient(ApplicationServer* server) { return new SimpleGLClient(server); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleGLClient::initialize()
{
	// Initialize cube normals.
	myNormals[0] = Vector3f(-1, 0, 0);
	myNormals[1] = Vector3f(0, 1, 0);
	myNormals[2] = Vector3f(1, 0, 0);
	myNormals[3] = Vector3f(0, -1, 0);
	myNormals[4] = Vector3f(0, 0, 1);
	myNormals[5] = Vector3f(0, 0, -1);

	// Initialize cube face indices.
	myFaces[0] = Vector4i(0, 1, 2, 3);
	myFaces[1] = Vector4i(3, 2, 6, 7);
	myFaces[2] = Vector4i(7, 6, 5, 4);
	myFaces[3] = Vector4i(4, 5, 1, 0);
	myFaces[4] = Vector4i(5, 6, 2, 1);
	myFaces[5] = Vector4i(7, 4, 0, 3);

	// Initialize cube face colors.
	myFaceColors[0] = Color::Aqua;
	myFaceColors[1] = Color::Orange;
	myFaceColors[2] = Color::Olive;
	myFaceColors[3] = Color::Navy;
	myFaceColors[4] = Color::Red;
	myFaceColors[5] = Color::Yellow;

	// Setup cube vertex data
	float size = 0.2f;
	myVertices[0][0] = myVertices[1][0] = myVertices[2][0] = myVertices[3][0] = -size;
	myVertices[4][0] = myVertices[5][0] = myVertices[6][0] = myVertices[7][0] = size;
	myVertices[0][1] = myVertices[1][1] = myVertices[4][1] = myVertices[5][1] = -size;
	myVertices[2][1] = myVertices[3][1] = myVertices[6][1] = myVertices[7][1] = size;
	myVertices[0][2] = myVertices[3][2] = myVertices[4][2] = myVertices[7][2] = size;
	myVertices[1][2] = myVertices[2][2] = myVertices[5][2] = myVertices[6][2] = -size;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleGLClient::draw(const DrawContext& context)
{
	if(context.task == DrawContext::SceneDrawTask)
	{
		// Enable depth testing and lighting.
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	
		// Setup light.
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		glLightfv(GL_LIGHT0, GL_COLOR, Color(1.0, 1.0, 1.0).data());
		glLightfv(GL_LIGHT0, GL_POSITION, Vector3f(0.0f, 0.0f, 1.0f).data());

		// Draw a rotating teapot.
		glRotatef(10, 1, 0, 0);
		glRotatef((float)context.frameNum * 0.1f, 0, 1, 0);
		glRotatef((float)context.frameNum * 0.2f, 1, 0, 0);

		// Draw a box
		for (int i = 0; i < 6; i++) 
		{
			glBegin(GL_QUADS);
			glColor3fv(myFaceColors[i].data());
			glNormal3fv(myNormals[i].data());
			glVertex3fv(myVertices[myFaces[i][0]].data());
			glVertex3fv(myVertices[myFaces[i][1]].data());
			glVertex3fv(myVertices[myFaces[i][2]].data());
			glVertex3fv(myVertices[myFaces[i][3]].data());
			glEnd();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	SimpleGLApplication app;

	// Read config file name from command line or use default one.
	// NOTE: being a simple application, ohello does not have any application-specific configuration option. 
	// So, we are going to load directly a system configuration file.
	const char* cfgName = "system/desktop.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "simplegl.log", new FilesystemDataSource(OMEGA_DATA_PATH));
	
	return 0;
}
