/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#ifndef __MOTION_CAPTURE_SERVICE_H__
#define __MOTION_CAPTURE_SERVICE_H__

#include "omega/osystem.h"
#include "omega/InputManager.h"
#include "natnet/NatNetTypes.h"
#include "natnet/NatNetClient.h"
#include "winsock2.h"

//#define PI 3.14159265

namespace omega
{
	class MoCapService : public InputService
	{
	public:
		OMEGA_API MoCapService();
		OMEGA_API ~MoCapService();
		OMEGA_API static void __cdecl frameController(sFrameOfMocapData* data, void* pUserData);
		OMEGA_API static void __cdecl messageController(int msgType, char* msg);
		OMEGA_API virtual void initialize();
		OMEGA_API virtual void start();//initialize and start service here
		OMEGA_API virtual void stop();//destroy service instance to stop
		OMEGA_API virtual void dispose();
		OMEGA_API void setLocalIP (char* theAdress);
		OMEGA_API void setServerIP (char* theAdress);
		OMEGA_API void useQuaternion ();
		OMEGA_API void useEuler ();
		//may want to support the option to choose whether to have unicast or multicast networking
		//for now it is hard coded to multicast
	private:
		static double PI;
		static MoCapService* myMoCap;
		NatNetClient* pClient;
		char localIP[128];			//the IP address of this machine, it is found automatically if it is set to an empty string (e.g. "")
		char serverIP[128];		//Server's IP address assumed to be local if left blank
		static bool isEuler;			//tells whether Euler or Quaternion angles are used. TRUE = Euler  FALSE = Quaternion
	};//class MoCapService

};//namespace omega
#endif
