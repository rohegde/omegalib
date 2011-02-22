#include "kinectdemo.h"

void KinectDemoClient::draw(const DrawContext& context)
{

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the OpenGL viewpoint
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//xn::DepthMetaData depthMD;

	SystemManager::instance()->getInputManager()->poll();

	int numberOfEvents = SystemManager::instance()->getInputManager()->getAvailableEvents();

	omega::InputEvent ptrEvents[omega::InputManager::MaxEvents];

	if( numberOfEvents > 0 ) {
		SystemManager::instance()->getInputManager()->getEvents(ptrEvents, omega::InputManager::MaxEvents);

		for ( int i = 0; i < numberOfEvents; i++ ) {

				// Select only the openni events
			if( ptrEvents[i].serviceType == omega::InputService::OpenNI ) {

				for( int j = 1; j < 25; j++ ) {
					if( ptrEvents[i].pointSet[j][0] != FLT_MIN ) {
						char debug_message[256];
						sprintf(debug_message, "Joint found. ID:%d, X:%f, Y:%f, Z:%f\n", i, ptrEvents[i].pointSet[j][0], ptrEvents[i].pointSet[j][1], ptrEvents[i].pointSet[j][2] );
						omsg(debug_message);
					}

				}
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


