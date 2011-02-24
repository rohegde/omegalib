#include "omega/input/OpenNIService.h"
#include "omega/SystemManager.h"

using namespace omega;

OpenNIService* OpenNIService::myOpenNI = NULL;
XnChar OpenNIService::omg_strPose[20] = ""; // XXX - What is this ???
XnBool OpenNIService::omg_bNeedPose = FALSE;
xn::Context OpenNIService::omg_Context = NULL;
xn::DepthGenerator OpenNIService::omg_DepthGenerator = NULL;
xn::UserGenerator OpenNIService::omg_UserGenerator = NULL;
//xn::SceneMetaData OpenNIService::omg_sceneMD = NULL;


OpenNIService::OpenNIService()
{
	//omg_strPose[0] = "";
	strcpy ( localIP, "" );
	strcpy ( serverIP, "" );
	castType = 0;
}

OpenNIService::~OpenNIService()
{
}

void OpenNIService::initialize()
{
	myOpenNI = this;

	// Check the return value of nRetVal
	//if( omg_Context.Init() != XN_STATUS_OK ) 
	//{
		// Display an error
	//}
	
	
	if( omg_Context.InitFromXmlFile(KINECT_CONFIG) != XN_STATUS_OK )
	{
		// Display an error
		omsg("ERROR initializing");
	}

	if( omg_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, omg_DepthGenerator) != XN_STATUS_OK ) 
	{
		omsg("Error initializing Depth Generator.");
	}
	if( omg_Context.FindExistingNode(XN_NODE_TYPE_USER,  omg_UserGenerator)  != XN_STATUS_OK ) 
	{
		omsg("Error initializing User Generator.");
		omg_UserGenerator.Create(omg_Context);
	}

	// Callbacks
	XnCallbackHandle hUserCallbacks, hCalibrationCallbacks, hPoseCallbacks;

	if (!omg_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
	{
		printf("Supplied user generator doesn't support skeleton\n");
		
	}
	omg_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
	omg_UserGenerator.GetSkeletonCap().RegisterCalibrationCallbacks(UserCalibration_CalibrationStart, UserCalibration_CalibrationEnd, NULL, hCalibrationCallbacks);

	if (omg_UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
	{
		OpenNIService::omg_bNeedPose = TRUE;

		if (!omg_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
		{
			printf("Pose required, but not supported\n");
			//return 1;
		}
		omg_UserGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks(UserPose_PoseDetected, NULL, NULL, hPoseCallbacks);
		omg_UserGenerator.GetSkeletonCap().GetCalibrationPose(omg_strPose);
	}

	omg_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
	omg_Context.StartGeneratingAll();	

}


void OpenNIService::setup(Setting& settings)
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

void OpenNIService::start()
{
}

void OpenNIService::stop()
{
}

void OpenNIService::dispose()
{
	delete myOpenNI;
}


/**********************************************************************************************
 *	For a better idea of what is going on in the quaternion to euler conversion check out:
 *		http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
 *		http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
 **********************************************************************************************/
void OpenNIService::poll(void)
{
	double verticalTest; //used to handle special case of body pointing straight along vertical axis
	double unit;

	if( myOpenNI )
	{

		omg_Context.WaitAndUpdateAll();

		xn::SceneMetaData sceneMD;
		xn::DepthMetaData depthMD;
		omg_DepthGenerator.GetMetaData(depthMD);
		omg_UserGenerator.GetUserPixels(0, sceneMD);

		// Read next available data
		

		//process the frame data and store it in events, one event per rigid body
		XnUserID aUsers[OMEGA_OPENNI_MAX_USERS];
		XnUInt16 nUsers = OMEGA_OPENNI_MAX_USERS;
		omg_UserGenerator.GetUsers(aUsers, nUsers);

		for (int i = 0; i < nUsers; ++i)
		{
			
			

			XnPoint3D com;
			omg_UserGenerator.GetCoM(aUsers[i], com);
			omg_DepthGenerator.ConvertRealWorldToProjective(1, &com, &com);

			// Save the state in a variable somehow

			if ( omg_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i]) )
			{
				myOpenNI->lockEvents();

				InputEvent* theEvent = myOpenNI->writeHead();
				theEvent->sourceId = aUsers[i];
				theEvent->serviceType = InputService::Mocap;

				joint2eventPointSet(aUsers[i], OMEGA_SKEL_HEAD, theEvent);
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_NECK, theEvent);
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_TORSO, theEvent);

				// Left side
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_LEFT_SHOULDER, theEvent);
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_LEFT_ELBOW, theEvent);
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_LEFT_HAND, theEvent);
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_LEFT_HIP, theEvent);
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_LEFT_KNEE, theEvent);
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_LEFT_FOOT, theEvent);

				// Right side
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_RIGHT_SHOULDER, theEvent);
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_RIGHT_ELBOW, theEvent);
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_RIGHT_HAND, theEvent);
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_RIGHT_HIP, theEvent);
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_RIGHT_KNEE, theEvent);
				joint2eventPointSet(aUsers[i], OMEGA_SKEL_RIGHT_FOOT, theEvent);

				myOpenNI->unlockEvents();

			}
		}

		
	}
}

void OpenNIService::joint2eventPointSet(XnUserID player, XnSkeletonJoint joint, InputEvent* theEvent) {
	Vector3f pos;
	if( getJointPosition(player, joint, pos) ) {
		theEvent->pointSet[joint][0] = pos[0];
		theEvent->pointSet[joint][1] = pos[1];
		theEvent->pointSet[joint][2] = pos[2];

		// Save the center of gravity
		if( joint == OMEGA_SKEL_TORSO ) {
			theEvent->position[0] = pos[0]; theEvent->position[1] = pos[1]; theEvent->position[2] = pos[2];
		}
	}
	else {
		theEvent->pointSet[joint][0] = FLT_MIN;
		theEvent->pointSet[joint][1] = FLT_MIN;
		theEvent->pointSet[joint][2] = FLT_MIN;
	}
}

/**
  * getJointPosition: 
  * returns false if there is a problem
  * else position of joint is stored in pos
  */
bool OpenNIService::getJointPosition(XnUserID player, XnSkeletonJoint joint, Vector3f &pos) {
	if( !omg_UserGenerator.GetSkeletonCap().IsTracking(player) )
		return false;

	XnSkeletonJointPosition jointPos;
	omg_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, joint, jointPos);

	if( jointPos.fConfidence < 0.5 )
		return false;

	// Do we need this ???
	//omg_DepthGenerator.ConvertRealWorldToProjective(2, pt, pt);
	pos[0] = jointPos.position.X;
	pos[1] = jointPos.position.Y;
	pos[2] = jointPos.position.Z;

	return true;
}

// Callback: New user was detected
void XN_CALLBACK_TYPE OpenNIService::User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	omsg("New User %d\n", nId);
	// New user found
	if ( OpenNIService::omg_bNeedPose)
	{
		omg_UserGenerator.GetPoseDetectionCap().StartPoseDetection(omg_strPose, nId);
	}
	else
	{
		omg_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
	}
}

// Callback: An existing user was lost
void XN_CALLBACK_TYPE OpenNIService::User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	omsg("Lost user %d\n", nId);
}

// Callback: Detected a pose
void XN_CALLBACK_TYPE OpenNIService::UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	omsg("Pose %s detected for user %d\n", strPose, nId);
	omg_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
	omg_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

// Callback: Started calibration
void XN_CALLBACK_TYPE OpenNIService::UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
	omsg("Calibration started for user %d\n", nId);
}

// Callback: Finished calibration
void XN_CALLBACK_TYPE OpenNIService::UserCalibration_CalibrationEnd(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie)
{
	if (bSuccess)
	{
		// Calibration succeeded
		omsg("Calibration complete, start tracking user %d\n", nId);
		omg_UserGenerator.GetSkeletonCap().StartTracking(nId);
	}
	else
	{
		// Calibration failed
		omsg("Calibration failed for user %d\n", nId);
		if ( OpenNIService::omg_bNeedPose )
		{
			omg_UserGenerator.GetPoseDetectionCap().StartPoseDetection(omg_strPose, nId);
		}
		else
		{
			omg_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
		}
	}
}