/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Victor Mateevitsi		mvictoras@gmail.com
 *	Alessandro Febretti		febret@gmail.com
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
#include "omega.h"
#include "omega/OpenNIService.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
class MultipleKinectDemoClient: public ApplicationClient
{
public:
	MultipleKinectDemoClient(ApplicationServer* server): ApplicationClient(server) {}
	virtual void initialize();
	virtual bool handleEvent(const Event& evt, UpdateContext& context);
	virtual void draw(const DrawContext& context);
	void processData();
private:

	Vector3f myCurrentMocapReadingR;
	Vector3f myCurrentMocapReadingL;

	bool myUseTrackable;
	int myTrackableId;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class MultipleKinectDemoApplication: public Application
{
public:
	virtual ApplicationClient* createClient(ApplicationServer* server) { return new MultipleKinectDemoClient(server); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void MultipleKinectDemoClient::initialize()
{
	myUseTrackable = false;

	Config* cfg = getSystemManager()->getAppConfig();

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MultipleKinectDemoClient::processData()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MultipleKinectDemoClient::handleEvent(const Event& evt, UpdateContext& context)
{
	switch(evt.serviceType)
	{
	case Service::Mocap:
		myCurrentMocapReadingL = evt.pointSet[OMEGA_SKEL_LEFT_HAND];
		myCurrentMocapReadingR = evt.pointSet[OMEGA_SKEL_RIGHT_HAND];
		
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MultipleKinectDemoClient::draw(const DrawContext& context)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPointSize(32);
		glColor3f(0.3f, 1.0f, 0.3f);
	glBegin(GL_POINTS);
	glVertex3f(
		myCurrentMocapReadingR.x(),
		myCurrentMocapReadingR.y(),
		0.0f);
	glEnd();

	glColor3f(1.0f, 0.3f, 0.3f);
	glBegin(GL_POINTS);
	glVertex3f(
		myCurrentMocapReadingL.x(),
		myCurrentMocapReadingL.y(),
		0.0f);
	glEnd();
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	MultipleKinectDemoApplication app;

	// Read config file name from command line or use default one.
	const char* cfgName = "multipleKinectDemo.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "multipleKinectDemo.log", new FilesystemDataSource("./../../data/"));
	
	return 0;
}
