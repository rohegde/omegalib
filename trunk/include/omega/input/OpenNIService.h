/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Victor Mateevitsi							vmatee2@uic.edu
 *  [PLACE YOUR NAME AND MAIL HERE IF YOU CONTRIBUTED TO WRITE THIS SOURCE FILE]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#ifndef __OPENNI_SERVICE_H__
#define __OPENNI_SERVICE_H__

#include "omega/osystem.h"
#include "omega/InputManager.h"
#include "natnet/NatNetTypes.h"
#include "openni/XnCppWrapper.h"

#include "winsock2.h"

//#define KINECT_CONFIG "../../data/openni/conf/KinectConfig.xml"
#define KINECT_CONFIG "./../../data/openni/conf/KinectConfig.xml"
#define OMEGA_OPENNI_MAX_USERS 15

#define OMEGA_SKEL_HEAD				XN_SKEL_HEAD
#define OMEGA_SKEL_NECK				XN_SKEL_NECK
#define	OMEGA_SKEL_TORSO			XN_SKEL_TORSO

#define OMEGA_SKEL_LEFT_SHOULDER	XN_SKEL_LEFT_SHOULDER
#define OMEGA_SKEL_LEFT_ELBOW		XN_SKEL_LEFT_ELBOW
#define OMEGA_SKEL_LEFT_HAND		XN_SKEL_LEFT_HAND
#define OMEGA_SKEL_LEFT_HIP			XN_SKEL_LEFT_HIP
#define OMEGA_SKEL_LEFT_KNEE		XN_SKEL_LEFT_KNEE
#define OMEGA_SKEL_LEFT_FOOT		XN_SKEL_LEFT_FOOT

#define OMEGA_SKEL_RIGHT_SHOULDER	XN_SKEL_RIGHT_SHOULDER
#define OMEGA_SKEL_RIGHT_ELBOW		XN_SKEL_RIGHT_ELBOW
#define OMEGA_SKEL_RIGHT_HAND		XN_SKEL_RIGHT_HAND
#define OMEGA_SKEL_RIGHT_HIP		XN_SKEL_RIGHT_HIP
#define OMEGA_SKEL_RIGHT_KNEE		XN_SKEL_RIGHT_KNEE
#define OMEGA_SKEL_RIGHT_FOOT		XN_SKEL_RIGHT_FOOT

namespace omega
{
	// Typedefs for the OpenNIService - omega integration
	typedef xn::DepthMetaData DepthMetaData;
	typedef xn::SceneMetaData RGBMetaData;

	class OpenNIService : public InputService
	{
	public:
		// Allocator function
		static OpenNIService* New() { return new OpenNIService(); }

	public:
		OMEGA_API OpenNIService();
		OMEGA_API ~OpenNIService();
		OMEGA_API void setup( Setting& settings);
		OMEGA_API virtual void initialize();
		OMEGA_API virtual void start();//initialize and start service here
		OMEGA_API virtual void stop();//destroy service instance to stop
		OMEGA_API virtual void dispose();
		OMEGA_API virtual void poll();
		//may want to support the option to choose whether to have unicast or multicast networking
		//for now it is hard coded to multicast
	private:
		static OpenNIService* myOpenNI;
		int castType;			//This determines wether the information is multicast or unicast across the network sockets. 0 = multicast and 1 = unicast
		char localIP[128];		//the IP address of this machine, it is found automatically if it is set to an empty string (e.g. "")
		char serverIP[128];		//Server's IP address assumed to be local if left blank

	public:
		// For the openni interaction
		static XnChar omg_strPose[20]; // XXX - What is this ???
		static XnBool omg_bNeedPose;
		static xn::Context omg_Context;
		static xn::DepthGenerator omg_DepthGenerator;
		static xn::UserGenerator omg_UserGenerator;
		static xn::SceneMetaData omg_sceneMD;
		

	private:

		static void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
		static void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie);

		static void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
		static void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie);

		static void XN_CALLBACK_TYPE UserCalibration_CalibrationEnd(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie);

		bool getJointPosition(XnUserID player, XnSkeletonJoint joint, Vector3f &pos);
		void joint2eventPointSet(XnUserID player, XnSkeletonJoint joint, InputEvent* theEvent);

	};//class KinectService
};//namespace omega

// XXX - Callbacks have to br globals!! Fix this ASAPs


//xn::DepthMetaData depthMD;

#endif
