#include "kinectdemo.h"

void KinectDemoClient::draw(const DrawContext& context)
{

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the OpenGL viewpoint
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	GLuint texID = 0;
	glGenTextures(1,&texID);

	glBindTexture(GL_TEXTURE_2D,texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//xn::DepthMetaData depthMD;

	SystemManager::instance()->getServiceManager()->poll();

	int numberOfEvents = SystemManager::instance()->getServiceManager()->getAvailableEvents();

	omega::Event ptrEvents[OMEGA_MAX_EVENTS];

	if( numberOfEvents > 0 ) {
		SystemManager::instance()->getServiceManager()->getEvents(ptrEvents, omega::ServiceManager::MaxEvents);

		for ( int i = 0; i < numberOfEvents; i++ ) {

				// Select only the openni events
			if( ptrEvents[i].serviceType == omega::Service::Mocap ) {
				Vector3f pos = ptrEvents[i].getPosition() * 0.001;
				
				ofmsg("Position: %1%", %pos);

				glBindTexture(GL_TEXTURE_2D, texID);
				//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, ptrEvents[i].userData);

				// Display the OpenGL texture map
				glColor4f(0.75,0.75,0.75,1);

				glEnable(GL_TEXTURE_2D);
				GLfloat verts[8] = {	640, 0,
					640, 480,
					0, 480,
					0, 0
				};
				glVertexPointer(2, GL_FLOAT, 0, verts);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);	
				glDisable(GL_TEXTURE_2D);
			}
		}
	}




	//glDisable(GL_TEXTURE_2D);
}

int main(int argc, char** argv)
{
	KinectDemoApplication app;
	const char* cfgName = "kinect.cfg";

	omain(app, cfgName, "kinect.log", new FilesystemDataSource("./../../data/"));

	return 0;
}


