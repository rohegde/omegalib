#ifndef __KINECTDEMO_H__
#define __KINECTDEMO_H__

#include "omega.h"

using namespace omega;

class KinectDemoClient : public ApplicationClient
{

public:
	KinectDemoClient(Application* app): ApplicationClient(app) {}
	virtual void draw(const DrawContext& context);	
};

class KinectDemoApplication : public Application
{
public:
	virtual ApplicationClient* createClient() { return new KinectDemoClient(this); }
};

#endif
