#include "kinectdemo.h"

void KinectDemoClient::draw(const DrawContext& context)
{

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the OpenGL viewpoint
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//xn::DepthMetaData depthMD;

	Matrix4f xf = Matrix4f::IDENTITY;
	xf.rotate_x(80);
	xf.set_translation(-2.1f, 0.0f, -0.2f);

	SystemManager::instance()->getServiceManager()->poll();

	int numberOfEvents = SystemManager::instance()->getServiceManager()->getAvailableEvents();

	omega::Event ptrEvents[OMEGA_MAX_EVENTS];

	if( numberOfEvents > 0 ) {
		SystemManager::instance()->getServiceManager()->getEvents(ptrEvents, omega::ServiceManager::MaxEvents);

		for ( int i = 0; i < numberOfEvents; i++ ) {

				// Select only the openni events
			if( ptrEvents[i].serviceType == omega::Service::Mocap ) {
				ptrEvents[i].position *= 0.001;
				
				Vector4f hp = xf * Vector4f(ptrEvents[i].position, 1.0f);
				Vector3f p = hp / hp[3];
				ofmsg("Position: %1%", %p);

				//for( int j = 1; j < 25; j++ ) {
				//	if( ptrEvents[i].pointSet[j][0] != FLT_MIN ) {
				//		char debug_message[256];
				//		sprintf(debug_message, "Joint found. ID:%d, X:%.2f, Y:%.2f, Z:%.2f\n", i, ptrEvents[i].pointSet[j][0], ptrEvents[i].pointSet[j][1], ptrEvents[i].pointSet[j][2] );
				//		omsg(debug_message);
				//	}

				//}
			}
		}
	}

	glDisable(GL_TEXTURE_2D);
}

int main(int argc, char** argv)
{
	KinectDemoApplication app;
	const char* cfgName = "kinect.cfg";

	omain(app, cfgName, "kinect.log", new FilesystemDataSource("./../../data/"));

	return 0;
}


