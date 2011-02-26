/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Brad McGinnis
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
#include "omega/NaturalPointService.h"
#include "omega/SystemManager.h"

using namespace omega;

NaturalPointService* NaturalPointService :: myMoCap = NULL;
bool NaturalPointService :: isEuler = false;

///////////////////////////////////////////////////////////////////////////////////////////////////
NaturalPointService::NaturalPointService()
{
	pClient = NULL;
	strcpy ( localIP, "" );
	strcpy ( serverIP, "" );
	castType = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
NaturalPointService::~NaturalPointService()
{
	pClient->Uninitialize();
	delete pClient;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NaturalPointService::initialize()
{
	myMoCap = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NaturalPointService::setup(Setting& settings)
{
	if( settings.exists( "serverIP" ) )
	{
		strcpy( serverIP, settings[ "serverIP" ] );
	}

	if( settings.exists( "localIP" ) )
	{
		strcpy( serverIP, settings[ "localIP" ] );
	}

	if( settings.exists( "castingType" ) )
	{
		castType = atoi( (const char*) settings[ "castingType" ] );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NaturalPointService::start()
{
	if( pClient == NULL)
	{
		pClient = new NatNetClient( castType ); //0 indicates that it is operating in multicast, if it is changed to 1 it will operate in unicast
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

///////////////////////////////////////////////////////////////////////////////////////////////////
void NaturalPointService::stop()
{
	pClient->Uninitialize();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NaturalPointService::dispose()
{
	delete myMoCap;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl NaturalPointService::messageController( int msgType, char* msg)
{
	//this is where you write messages, from the NatNet server, to the log
	//Log::Message("MOCAP: %s", msg);

	//this is commented out b/c it is printing messages at every frame and I am unsure if they are worth while
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void __cdecl NaturalPointService::frameController( sFrameOfMocapData* data, void *pUserData)
{
	double verticalTest; //used to handle special case of body pointing straight along vertical axis
	double unit;

	if( myMoCap )
	{
		myMoCap->lockEvents();
		//process the frame data and store it in events, one event per rigid body
		for( int i = 0; i < data->nRigidBodies; i++)
		{
			//checks to see if the rigid body is being tracked, if all positional data is set to zero then it is more than likely not being tracked
			if ( ( data->RigidBodies[i].x == 0 ) && ( data->RigidBodies[i].y == 0 ) && ( data->RigidBodies[i].z == 0 ) )
			{
				//theEvent->type = Event::Untrace;
				//theEvent->position[0] = 0.0;
				//theEvent->position[1] = 0.0;
				//theEvent->position[2] = 0.0;

				//theEvent->orientation[0] = 0.0;
				//theEvent->orientation[1] = 0.0;
				//theEvent->orientation[2] = 0.0;
				//theEvent->orientation[3] = 0.0;
				//for( int k = 0; k < data->RigidBodies[i].nMarkers; k++)
				//{
				//	Vector3f aPoint;
				//	aPoint[0] = 0.0; //x
				//	aPoint[1] = 0.0; //y
				//	aPoint[2] = 0.0; //z
				//	theEvent->pointSet[k] = aPoint;
				//}
				continue;
			}

			//actions to process each rigid body into an event
			Event* theEvent = myMoCap->writeHead();
			theEvent->numberOfPoints = data->RigidBodies[i].nMarkers;

			theEvent->sourceId = data->RigidBodies[i].ID;
			theEvent->serviceType = Service::Mocap;

			//Set event type
			//for now only trace and untraced are going to be supported with more complex types being implemented soon
			theEvent->type = Event::Trace;

			//get x,y,z coordinates
			theEvent->position[0] = data->RigidBodies[i].x;
			theEvent->position[1] = data->RigidBodies[i].y;
			theEvent->position[2] = data->RigidBodies[i].z;

			//get makerset data (the points that define the rigid body)
			int numberOfMarkers = data->RigidBodies[i].nMarkers;
			for( int j = 0; j < numberOfMarkers; j++)
			{
				Vector3f aPoint;
				aPoint[0] = data->RigidBodies[i].Markers[j][0];//x
				aPoint[1] = data->RigidBodies[i].Markers[j][1];//y
				aPoint[2] = data->RigidBodies[i].Markers[j][2];//z
				theEvent->pointSet[j] = aPoint;
			}

			//get the quaternion orientation ( qw, qx, qy, qz) and convert it to euler angles ( roll(rx), yaw(ry), pitch(rz))
			//if ( isEuler )
			//{
			//	double sqw = data->RigidBodies[i].qw * data->RigidBodies[i].qw;
			//	double sqx = data->RigidBodies[i].qx * data->RigidBodies[i].qx;
			//	double sqy = data->RigidBodies[i].qy * data->RigidBodies[i].qy;
			//	double sqz = data->RigidBodies[i].qz * data->RigidBodies[i].qz;

			//	theEvent->rotation[3] = 0;
			//	//check for a position that is at either of the poles (pointing straight up or straight down)
			//	verticalTest = ( data->RigidBodies[i].qx * data->RigidBodies[i].qy ) + ( data->RigidBodies[i].qz * data->RigidBodies[i].qw );

			//	unit = sqx + sqy + sqz + sqw;

			//	//check for pointing North
			//	if( verticalTest > ( 0.499 * unit ) )
			//	{
			//		//owarn("MOCAP: Pointing North");
			//		theEvent->orientation[0] = 0;
			//		theEvent->orientation[1] = 2 * atan2 ( data->RigidBodies[i].qx, data->RigidBodies[i].qw );
			//		theEvent->orientation[2] = Math::Pi/2;
			//	}
			//	//check for pointing South
			//	else if( verticalTest < ( -0.499 * unit ) )
			//	{
			//		//owarn("MOCAP: Pointing South");
			//		theEvent->orientation[0] = 0;
			//		theEvent->orientation[1] = -2 * atan2 ( data->RigidBodies[i].qx, data->RigidBodies[i].qw );
			//		theEvent->orientation[2] = -Math::Pi/2;
			//	}
			//	else
			//	{
			//		//pitch
			//		theEvent->rotation[0] = atan2( (double) ( ( 2 * data->RigidBodies[i].qx * data->RigidBodies[i].qw ) - ( 2 * data->RigidBodies[i].qy * data->RigidBodies[i].qz ) ),
			//								-sqx + sqy - sqz + sqw );
			//		//roll
			//		theEvent->rotation[2] = asin( 2 * verticalTest / unit );

			//		//yaw
			//		theEvent->rotation[1] = atan2( (double) ( ( 2 * data->RigidBodies[i].qy * data->RigidBodies[i].qw ) - ( 2 * data->RigidBodies[i].qx * data->RigidBodies[i].qz ) ),
			//								sqx - sqy - sqz + sqw );

			//		// Convert to degrees.
			//		theEvent->rotation[0] *= Math::RadToDeg;
			//		theEvent->rotation[1] *= Math::RadToDeg;
			//		theEvent->rotation[2] *= Math::RadToDeg;
			//	}
			//}
			//else //if the user wants quaternion data
			{
				theEvent->orientation[3] = data->RigidBodies[i].qw;
				theEvent->orientation[0] = data->RigidBodies[i].qx;
				theEvent->orientation[1] = data->RigidBodies[i].qy;
				theEvent->orientation[2] = data->RigidBodies[i].qz;
			}
		}
		myMoCap->unlockEvents();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NaturalPointService::useEuler()
{
	isEuler = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NaturalPointService::useQuaternion()
{
	isEuler = false;
}