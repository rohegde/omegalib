#include "omega/input/MoCapService.h"
#include "omega/SystemManager.h"
#include <math.h>

#include "omega/Utils.h"

using namespace omega;

MoCapService* MoCapService :: myMoCap = NULL;
bool MoCapService :: isEuler = true;


MoCapService::MoCapService()
{
	pClient = NULL;
	strcpy ( localIP, "" );
	strcpy ( serverIP, "" );
}

MoCapService::~MoCapService()
{
	pClient->Uninitialize();
	delete pClient;
}

void MoCapService::initialize()
{
	myMoCap = this;
}


void MoCapService::start()
{
	if( pClient == NULL)
	{
		pClient = new NatNetClient( 0 ); //0 indicates that it is operating in multicast, if it is changed to 1 it will operate in unicast
	}
	
	//set call back functions
	pClient->SetMessageCallback(messageController);
	pClient->SetVerbosityLevel(Verbosity_Debug);
	pClient->SetDataCallback( frameController, pClient);

	//initialize pClient and ensure that it completed successfully
	int retCode = pClient->Initialize( localIP, serverIP);
	
	if ( retCode != ErrorCode_OK)
	{
		oerror("MOCAP: Unable to connect to server. Error code: %d. Exiting", retCode);
		exit(1);
	}
	omsg("MOCAP: Initialization succeeded\n");

	// send/receive test request
	omsg("MOCAP: Sending Test Request\n");
	void* response;
	int nBytes;
	int iResult = pClient->SendMessageAndWait("TestRequest", &response, &nBytes);
	if (iResult == ErrorCode_OK)
	{
		printf("MOCAP: Received: %s", (char*)response);
	}

}

void MoCapService::stop()
{
	pClient->Uninitialize();
}

void MoCapService::dispose()
{
	delete myMoCap;
}

void __cdecl MoCapService::messageController( int msgType, char* msg)
{
	//this is where you write messages, from the NatNet server, to the log
	//Log::Message("MOCAP: %s", msg);

	//this is commented out b/c it is printing messages at every frame and I am unsure if they are worth while
}

/**********************************************************************************************
 *	For a better idea of what is going on in the quaternion to euler conversion check out:
 *		http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
 *		http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
 **********************************************************************************************/
void __cdecl MoCapService::frameController( sFrameOfMocapData* data, void *pUserData)
{
	double verticalTest; //used to handle special case of body pointing straight along vertical axis
	double unit;

	if( myMoCap )
	{
		myMoCap->lockEvents();
		//process the frame data and store it in events, one event per rigid body
		for( int i = 0; i < data->nRigidBodies; i++)
		{
			//actions to process each rigid body into an event
			InputEvent* theEvent = myMoCap->writeHead();

			theEvent->sourceId = data->RigidBodies[i].ID;
			theEvent->serviceType = InputService::Mocap;

			//checks to see if the rigid body is being tracked, if all positional data is set to zero then it is more than likely not being tracked
			if ( ( data->RigidBodies[i].x == 0 ) && ( data->RigidBodies[i].y == 0 ) && ( data->RigidBodies[i].z == 0 ) )
			{
				theEvent->type = InputEvent::Untrace;
				theEvent->x = 0.0;
				theEvent->y = 0.0;
				theEvent->z = 0.0;
				theEvent->rw = 0.0;
				theEvent->rx = 0.0;
				theEvent->ry = 0.0;
				theEvent->rz = 0.0;
				//for( int k = 0; k < data->RigidBodies[i].nMarkers; k++)
				//{
				//	Point aPoint;
				//	aPoint.x = 0.0;
				//	aPoint.y = 0.0;
				//	aPoint.z = 0.0;
				//	theEvent->markerSet.push_back ( aPoint);
				//}
				continue;
			}

			//Set event type
			//for now only trace and untraced are going to be supported with more complex types being implemented soon
			theEvent->type = InputEvent::Trace;

			//get x,y,z coordinates
			theEvent->x = data->RigidBodies[i].x;
			theEvent->y = data->RigidBodies[i].y;
			theEvent->z = data->RigidBodies[i].z;

			//get makerset data (the points that define the rigid body)
			int numberOfMarkers = data->RigidBodies[i].nMarkers;
			//for( int j = 0; j < numberOfMarkers; j++)
			//{
			//	Point aPoint;
			//	aPoint.x = data->RigidBodies[i].Markers[j][0];
			//	aPoint.y = data->RigidBodies[i].Markers[j][1];
			//	aPoint.z = data->RigidBodies[i].Markers[j][2];
			//	theEvent->markerSet.push_back ( aPoint);
			//}

			//get the quaternion orientation ( qw, qx, qy, qz) and convert it to euler angles ( roll(rx), yaw(ry), pitch(rz))
			if ( isEuler )
			{
				double sqw = data->RigidBodies[i].qw * data->RigidBodies[i].qw;
				double sqx = data->RigidBodies[i].qx * data->RigidBodies[i].qx;
				double sqy = data->RigidBodies[i].qy * data->RigidBodies[i].qy;
				double sqz = data->RigidBodies[i].qz * data->RigidBodies[i].qz;

				theEvent->rw = 0;
				//check for a position that is at either of the poles (pointing straight up or straight down)
				verticalTest = ( data->RigidBodies[i].qx * data->RigidBodies[i].qy ) + ( data->RigidBodies[i].qz * data->RigidBodies[i].qw );

				unit = sqx + sqy + sqz + sqw;

				//check for pointing North
				if( verticalTest > ( 0.499 * unit ) )
				{
					//owarn("MOCAP: Pointing North");
					theEvent->rx = 0;
					theEvent->ry = 2 * atan2 ( data->RigidBodies[i].qx, data->RigidBodies[i].qw );
					theEvent->rz = Math::Pi/2;
				}
				//check for pointing South
				else if( verticalTest < ( -0.499 * unit ) )
				{
					//owarn("MOCAP: Pointing South");
					theEvent->rx = 0;
					theEvent->ry = -2 * atan2 ( data->RigidBodies[i].qx, data->RigidBodies[i].qw );
					theEvent->rz = -Math::Pi/2;
				}
				else
				{
					//pitch
					theEvent->rx = atan2( (double) ( ( 2 * data->RigidBodies[i].qx * data->RigidBodies[i].qw ) - ( 2 * data->RigidBodies[i].qy * data->RigidBodies[i].qz ) ),
											-sqx + sqy - sqz + sqw );
					//roll
					theEvent->rz = asin( 2 * verticalTest / unit );

					//yaw
					theEvent->ry = atan2( (double) ( ( 2 * data->RigidBodies[i].qy * data->RigidBodies[i].qw ) - ( 2 * data->RigidBodies[i].qx * data->RigidBodies[i].qz ) ),
											sqx - sqy - sqz + sqw );
				}
			}
			else //if the user wants quaternion data
			{
				theEvent->rw = data->RigidBodies[i].qw;
				theEvent->rx = data->RigidBodies[i].qx;
				theEvent->ry = data->RigidBodies[i].qy;
				theEvent->rz = data->RigidBodies[i].qz;
			}
		}
		myMoCap->unlockEvents();
	}
}

void MoCapService::setLocalIP(	char* theAdress)
{
	strcpy( localIP, theAdress);
}

void MoCapService::setServerIP( char* theAdress)
{
	strcpy( serverIP, theAdress);
}

void MoCapService::useEuler()
{
	isEuler = true;
}

void MoCapService::useQuaternion()
{
	isEuler = false;
}