#ifndef __KINECTDEMO_H__
#define __KINECTDEMO_H__

#include <omega.h>

using namespace omega;

class KinectDemoClient : public ApplicationClient
{

public:
	KinectDemoClient(ApplicationServer* server): ApplicationClient(server) {}
	virtual void draw(const DrawContext& context);	
};

class KinectDemoApplication : public Application
{
public:
	virtual ApplicationClient* createClient(ApplicationServer* server) { return new KinectDemoClient(server); }

};

#endif
