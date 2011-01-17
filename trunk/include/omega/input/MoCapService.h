/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *  [PLACE YOUR NAME AND MAIL HERE IF YOU CONTRIBUTED TO WRITE THIS SOURCE FILE]
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

namespace omega
{
	class MoCapService : public InputService
	{
	public:
		// Allocator function
		static MoCapService* New() { return new MoCapService(); }

	public:
		MoCapService();
		~MoCapService();
		void setup( Setting& settings);
		static void __cdecl frameController(sFrameOfMocapData* data, void* pUserData);
		static void __cdecl messageController(int msgType, char* msg);
		virtual void initialize();
		virtual void start();//initialize and start service here
		virtual void stop();//destroy service instance to stop
		virtual void dispose();
		void useQuaternion ();
		void useEuler ();
		//may want to support the option to choose whether to have unicast or multicast networking
		//for now it is hard coded to multicast
	private:
		static double PI;
		static MoCapService* myMoCap;
		NatNetClient* pClient;
		int castType;			//This determines wether the information is multicast or unicast across the network sockets. 0 = multicast and 1 = unicast
		char localIP[128];		//the IP address of this machine, it is found automatically if it is set to an empty string (e.g. "")
		char serverIP[128];		//Server's IP address assumed to be local if left blank
		static bool isEuler;			//tells whether Euler or Quaternion angles are used. TRUE = Euler  FALSE = Quaternion
	};//class MoCapService

};//namespace omega
#endif
