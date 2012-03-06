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
 *---------------------------------------------------------------------------------------------------------------------
 *	ohello2
 *		A slightly more complex version of ohello, implementing event handling and data sharing. ohello2 defines a 
 *		new class, HelloServer, that reads mouse events and uses it to rotate the rendered object. 
 *		HelloServer also implements SharedObject to synchronize rotation data across all nodes.
 *********************************************************************************************************************/
#include <omega.h>
#include <omegaGl.h>

using namespace omega;

// Defined in teapot.cpp
void teapot(GLint grid, GLdouble scale, GLenum type);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HelloServer: public ServerBase, SharedObject
{
public:
	HelloServer(ApplicationBase* app, bool master): 
	  ServerBase(app, master),
	  myYaw(0), myPitch(0) 
	  {
		  // Register this as a shared object.
		  SharedDataServices::registerObject(this, "HelloServer");
	  }

	float getYaw() { return myYaw; }
	float getPitch() { return myPitch; }

	virtual void handleEvent(const Event& evt);
	virtual void commitSharedData(SharedOStream& out);
	virtual void updateSharedData(SharedIStream& in);

private:
	float myYaw;
	float myPitch;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HelloRenderer: public RendererBase
{
public:
	HelloRenderer(ServerBase* server): 
	  RendererBase(server), 
	  myServer((HelloServer*)server) 
	  { }

	virtual void draw(const DrawContext& context);

private:
	HelloServer* myServer;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HelloApplication: public ApplicationBase
{
public:
	virtual const char* getName() { return "ohello2"; }
	virtual RendererBase* createClient(ServerBase* server) { return new HelloRenderer(server); }
	virtual ServerBase* createMaster() { return new HelloServer(this, true); };
	virtual ServerBase* createServer() { return new HelloServer(this, false); };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HelloServer::handleEvent(const Event& evt)
{
	if(evt.getServiceType() == Service::Pointer)
	{
		// Normalize the mouse position using the total display resolution, 
		// then multiply to get 180 degree rotations
		Vector2i resolution = getDisplaySystem()->getCanvasSize();
		myYaw = (evt.getPosition(0) / resolution[0]) * 180;
		myPitch = (evt.getPosition(1) / resolution[1]) * 180;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HelloServer::commitSharedData(SharedOStream& out)
{
	out << myYaw << myPitch;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HelloServer::updateSharedData(SharedIStream& in)
{
 	in >> myYaw >> myPitch;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HelloRenderer::draw(const DrawContext& context)
{
	if(context.task == DrawContext::SceneDrawTask)
	{
		// Enable depth testing and lighting.
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	
		// Setup light.
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_COLOR, Color(1.0, 1.0, 1.0).data());
		glLightfv(GL_LIGHT0, GL_POSITION, Vector3f(-1.0, 1.0, 0.0).data());

		// Color using the client id.
		glColor3fv(Color::getColorByIndex(getGpuContext()->getId()).data());

		// Draw a rotating teapot.
		glRotatef(10, 1, 0, 0);
		glRotatef(myServer->getYaw(), 0, 1, 0);
		glRotatef(myServer->getPitch(), 1, 0, 0);
		teapot(14, 0.3f, GL_FILL);
		glColor3f(0, 0, 0);
		teapot(14, 0.3f, GL_LINE);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ApplicationBase entry point
int main(int argc, char** argv)
{
	HelloApplication app;
	
	// Read config file name from command line or use default one.
	// NOTE: being a simple application, ohello does not have any application-specific configuration option. 
	// So, we are going to load directly a system configuration file.
	const char* cfgName = "system/desktop.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "ohello.log", new FilesystemDataSource(OMEGA_DATA_PATH));
	
	return 0;
}
