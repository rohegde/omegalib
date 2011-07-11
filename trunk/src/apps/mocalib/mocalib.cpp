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
#include "omega.h"
#include "omega/OpenNIService.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Marker
{
	Vector3f reference;
	Vector3f readingR;
	Vector3f readingL;
};

#define MAX_MARKERS 32

///////////////////////////////////////////////////////////////////////////////////////////////////
class MocalibClient: public ApplicationClient
{
public:
	MocalibClient(Application* app): ApplicationClient(app), myCurMarker(0) {}
	virtual void initialize();
	virtual bool handleEvent(const Event& evt, UpdateContext& context);
	virtual void draw(const DrawContext& context);
	void processData();
private:
	int myNumMarkers;
	Marker myMarkers[MAX_MARKERS];
	int myCurMarker;
	AffineTransform3 myTransformR;
	AffineTransform3 myTransformL;

	Vector3f myCurrentMocapReadingR;
	Vector3f myCurrentMocapReadingL;

	bool myUseTrackable;
	int myTrackableId;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class MocalibApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new MocalibClient(this); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void MocalibClient::initialize()
{
	myUseTrackable = false;
	myTransformR = AffineTransform3::Identity();
	myTransformL = AffineTransform3::Identity();

	Config* cfg = getSystemManager()->getAppConfig();

	if(cfg->exists("config/testPoints"))
	{
		myNumMarkers = 0;
		Setting& pts = cfg->lookup("config/testPoints");
		for(int i = 0; i < pts.getLength(); i++)
		{
			Setting& smk = pts[i];
			myMarkers[myNumMarkers].reference.x() = smk[0];
			myMarkers[myNumMarkers].reference.y() = smk[1];
			myMarkers[myNumMarkers].reference.z() = smk[2];
			myMarkers[myNumMarkers].readingR = Vector3f::Zero();
			myMarkers[myNumMarkers].readingL = Vector3f::Zero();
			myNumMarkers++;
		}
	}

	if(cfg->exists("config/trackableId"))
	{
		myUseTrackable = true;
		myTrackableId = cfg->lookup("config/trackableId");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MocalibClient::processData()
{
	Vectors3f srcR;
	Vectors3f srcL;
	Vectors3f dst;

	srcR.resize(3, myNumMarkers);
	srcL.resize(3, myNumMarkers);
	dst.resize(3, myNumMarkers);

	for(int i = 0; i < myNumMarkers; i++)
	{
		dst.col(i) = myMarkers[i].reference;
		srcR.col(i) = myMarkers[i].readingR;
		srcL.col(i) = myMarkers[i].readingL;
	}
	myTransformR = Math::computeMatchingPointsTransform(srcR, dst);
	myTransformL = Math::computeMatchingPointsTransform(srcL, dst);
	omsg("srcR:");
	ofmsg("%1%", %srcR);
	omsg("srcL:");
	ofmsg("%1%", %srcL);

	omsg("dst:");
	ofmsg("%1%", %dst);

	Vector3f trR = myTransformR.translation();
	Matrix3f lnR = myTransformR.linear();

	Vector3f trL = myTransformL.translation();
	Matrix3f lnL = myTransformL.linear();

	omsg("Reference frame settings (copy the following code into mocap service configuration section)");
	omsg("-------------------------------------------------");
	omsg("referenceTransform:");
	omsg("{");
	ofmsg("referenceTranslationR = [%.6f, %.6f, %.6f];", 
		%trR(0) %trR(1) %trR(2));
	ofmsg("referenceLinearR = [%.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f];", 
		%lnR(0) %lnR(1) %lnR(2) %lnR(3) %lnR(4) %lnR(5) %lnR(6) %lnR(7) %lnR(8));
	omsg("};");
	omsg("{");
	ofmsg("referenceTranslationL = [%.6f, %.6f, %.6f];", 
		%trL(0) %trL(1) %trL(2));
	ofmsg("referenceLinearL = [%.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f];", 
		%lnL(0) %lnL(1) %lnL(2) %lnL(3) %lnL(4) %lnL(5) %lnL(6) %lnL(7) %lnL(8));
	omsg("};");
	omsg("-------------------------------------------------");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MocalibClient::handleEvent(const Event& evt, UpdateContext& context)
{
	switch(evt.serviceType)
	{
	case Service::Mocap:
		// What is this ?? - Vic
		if(myUseTrackable)
		{
			if(evt.sourceId == myTrackableId)
			{
				if( evt.deviceId == 0 ) myCurrentMocapReadingR = myTransformR * evt.position;
				else if( evt.deviceId == 1 ) myCurrentMocapReadingL = myTransformL * evt.position;
			}
		}
		else
		{
			//ofmsg("Got from device: %1% this: %2%", %(int)evt.deviceId %(float)evt.pointSet[OMEGA_SKEL_LEFT_HAND][0]);
			if( evt.deviceId == 0 ) myCurrentMocapReadingR = myTransformR * evt.pointSet[OMEGA_SKEL_LEFT_HAND];
			else if( evt.deviceId == 1 ) myCurrentMocapReadingL = myTransformL * evt.pointSet[OMEGA_SKEL_LEFT_HAND];
		}
		//ofmsg("id: %1% pos: %2%", %evt.sourceId %evt.position);
		//ofmsg("%1% %2% %3%", 
		//	%myCurrentMocapReading.x() 
		//	%myCurrentMocapReading.y()
		//	%myCurrentMocapReading.z());
		return true;
	case Service::Pointer:
		if(evt.type == Event::Down)
		{
			myMarkers[myCurMarker].readingR = myCurrentMocapReadingR; 
			myMarkers[myCurMarker].readingL = myCurrentMocapReadingL; 
			myCurMarker++;
			if(myCurMarker == myNumMarkers) processData();
		}
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MocalibClient::draw(const DrawContext& context)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	if(myCurMarker < myNumMarkers)
	{
		glPointSize(64);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex3fv(myMarkers[myCurMarker].reference.data());
		glEnd();
	}
	else
	{
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	MocalibApplication app;

	// Read config file name from command line or use default one.
	const char* cfgName = "mocalib.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "mocalib.log", new FilesystemDataSource("./../../data/"));
	
	return 0;
}
