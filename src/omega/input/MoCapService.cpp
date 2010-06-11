#include "input/MoCapService.h"
#include "SystemManager.h"
#include <math.h>
using namespace omega;

MoCapService* MoCapService :: myMoCap = NULL;
//int MoCapService :: PI =  = 3.14159265;

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

void MoCapService::Start()
{
	if( pClient == NULL)
	{
		pClient = new NatNetClient( 0 ); //0 indicates that it is operating in multicast, if it is changed to 1 it will operate in unicast
	}
	
	//set call back functions
	pClient->SetMessageCallback(MessageController);
	pClient->SetVerbosityLevel(Verbosity_Debug);
	pClient->SetDataCallback( FrameController, pClient);

	//initialize pClient and ensure that it completed successfully
	int retCode = pClient->Initialize( localIP, serverIP);
	
	if ( retCode != ErrorCode_OK)
	{
		//printf("Unable to connect to server. Error code: %d. Exiting", retCode);
		//put error printing to log here
		exit(1);
	}
	//printf("Initialization succeeded\n");
	//put success message to log here

}

void MoCapService::Stop()
{
	pClient->Uninitialize();
}

void __cdecl MoCapService::MessageController( int msgType, char* msg)
{
	//this is where you would write messages, from the NatNet server, to the log
}

void __cdecl MoCapService::FrameController( sFrameOfMocapData* data, void *pUserData)
{
	if( myMoCap )
	{
		myMoCap->LockEvents();
		//process the frame data and store it in events, one event per rigid body
		for( int i = 0; i < data->nRigidBodies; i++)
		{
			//actions to process each rigid body into an event
			InputEvent* theEvent = myMoCap->WriteHead();

			theEvent->id = data->RigidBodies[i].ID;

			//get x,y,z coordinates
			theEvent->x = data->RigidBodies[i].x;
			theEvent->y = data->RigidBodies[i].y;
			theEvent->z = data->RigidBodies[i].z;

			//get the quaternion orientation ( qw, qx, qy, qz) and convert it to euler angles ( roll(rx), yaw(ry), pitch(rz))
			
			//check for a position that is at either of the poles (pointing straight up or straight down)
			//check for pointing North
			/*
			if( ( data->RigidBodies[i].qx * data->RigidBodies[i].qy ) + ( data->RigidBodies[i].qz * data->RigidBodies[i].qw ) = 0.5 )
			{
				theEvent->rx = 0;
				theEvent->ry = 2 * atan2 ( data->RigidBodies[i].qx, data->RigidBodies[i].qw );
				theEvent->rz = PI/2;
			}
			else if( ( data->RigidBodies[i].qx * data->RigidBodies[i].qy ) + ( data->RigidBodies[i].qz * data->RigidBodies[i].qw ) = -0.5 )
			{
				theEvent->rx = 0;
				theEvent->ry = -2 * atan2 ( data->RigidBodies[i].qx, data->RigidBodies[i].qw );
				theEvent->rz = -PI/2;
			}*/
		}
		myMoCap->UnlockEvents();
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