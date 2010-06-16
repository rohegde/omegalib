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

#include "osystem.h"
#include "InputManager.h"
#include "NatNetTypes.h"
#include "NatNetClient.h"
#include "winsock2.h"

//#define PI 3.14159265

namespace omega
{

	class MoCapService : public InputService
	{
	public:
		MoCapService();
		~MoCapService();
		static void __cdecl FrameController(sFrameOfMocapData* data, void* pUserData);
		static void __cdecl MessageController(int msgType, char* msg);
		virtual void Initialize();
		virtual void Start();//initialize and start service here
		virtual void Stop();//destroy service instance to stop
		virtual void Dispose();
		void setLocalIP (char* theAdress);
		void setServerIP (char* theAdress);
		//may want to support the option to choose whether to have unicast or multicast networking
		//for now it is hard coded to multicast
	private:
		static double PI;
		static MoCapService* myMoCap;
		NatNetClient* pClient;
		char localIP[128];			//the IP address of this machine, it is found automatically if it is set to an empty string (e.g. "")
		char serverIP[128];		//Server's IP address assumed to be local if left blank
	};//class MoCapService

};//namespace omega
#endif
