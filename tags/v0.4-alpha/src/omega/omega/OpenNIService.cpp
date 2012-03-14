#include "omega/OpenNIService.h"
#include "omega/SystemManager.h"
#include "omega/StringUtils.h"

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

	Colors[0][0] = 0; Colors[0][1] = 1; Colors[0][2] = 1;
	Colors[1][0] = 0; Colors[1][1] = 0; Colors[1][2] = 1;
	Colors[2][0] = 0; Colors[2][1] = 1; Colors[2][2] = 0;
	Colors[3][0] = 1; Colors[3][1] = 1; Colors[3][2] = 0;
	Colors[4][0] = 1; Colors[4][1] = 0; Colors[4][2] = 0;
	Colors[5][0] = 1; Colors[5][1] = .5; Colors[5][2] = 0;
	Colors[6][0] = .5; Colors[6][1] = 1; Colors[6][2] = 0;
	Colors[7][0] = 0; Colors[7][1] = .5; Colors[7][2] = 1;
	Colors[8][0] = .5; Colors[8][1] = 0; Colors[8][2] = 1;
	Colors[9][0] = 1; Colors[9][1] = 1; Colors[9][2] = .5;
	Colors[10][0] = 1; Colors[10][1] = 1; Colors[10][2] = 1;

	pDepthTexBuf = new unsigned char[640 * 480 * 4];
}

OpenNIService::~OpenNIService()
{
	delete pDepthTexBuf;
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


void OpenNIService::poll(void)
{
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

		// Store the texture
		getTexture(depthMD, sceneMD);
		for (int i = 0; i < nUsers; ++i)
		{
			
			

			XnPoint3D com;
			omg_UserGenerator.GetCoM(aUsers[i], com);
			omg_DepthGenerator.ConvertRealWorldToProjective(1, &com, &com);

			// Save the state in a variable somehow

			if ( omg_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i]) )
			{
				myOpenNI->lockEvents();

				Event* theEvent = myOpenNI->writeHead();
				theEvent->sourceId = aUsers[i];
				theEvent->serviceType = Service::Mocap;
				theEvent->userData = pDepthTexBuf;
				theEvent->userDataSize = 640 * 480 * 4;

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

void OpenNIService::joint2eventPointSet(XnUserID player, XnSkeletonJoint joint, Event* theEvent) {
	Vector3f pos;
	if( getJointPosition(player, joint, pos) ) {
		theEvent->pointSet[joint][0] = pos[0];
		theEvent->pointSet[joint][1] = pos[1];
		theEvent->pointSet[joint][2] = pos[2];

		// Save the center of gravity
		if( joint == OMEGA_SKEL_HEAD ) {
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
	ofmsg("New User %1%", %(int)nId);
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
	ofmsg("Lost user %1%\n", %(int)nId);
}

// Callback: Detected a pose
void XN_CALLBACK_TYPE OpenNIService::UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	ofmsg("Pose %1% detected for user %2%\n", %strPose %(int)nId);
	omg_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
	omg_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

// Callback: Started calibration
void XN_CALLBACK_TYPE OpenNIService::UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
	ofmsg("Calibration started for user %1%\n", %(int)nId);
}

// Callback: Finished calibration
void XN_CALLBACK_TYPE OpenNIService::UserCalibration_CalibrationEnd(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie)
{
	if (bSuccess)
	{
		// Calibration succeeded
		ofmsg("Calibration complete, start tracking user %1%\n", %(int)nId);
		omg_UserGenerator.GetSkeletonCap().StartTracking(nId);
	}
	else
	{
		// Calibration failed
		ofmsg("Calibration failed for user %1%\n", %(int)nId);
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

void OpenNIService::getTexture(xn::DepthMetaData& dmd, xn::SceneMetaData& smd) 
{
	static bool bInitialized = false;	
	static GLuint depthTexID;
	static int texWidth, texHeight;
	float topLeftX = dmd.XRes();
	float topLeftY = 0;
	float bottomRightY = dmd.YRes();
	float bottomRightX = 0;
	float texXpos = (float)dmd.XRes()/texWidth;
	float texYpos = (float)dmd.YRes()/texHeight; 

	//memset(texcoords, 0, 8*sizeof(float));
	//texcoords[0] = texXpos, texcoords[1] = texYpos, texcoords[2] = texXpos, texcoords[7] = texYpos;

	unsigned int nValue = 0;
	unsigned int nHistValue = 0;
	unsigned int nIndex = 0;
	unsigned int nX = 0;
	unsigned int nY = 0;
	unsigned int nNumberOfPoints = 0;
	XnUInt16 g_nXRes = dmd.XRes();
	XnUInt16 g_nYRes = dmd.YRes();

	//pDepthTexBuf = new unsigned char[g_nXRes * g_nYRes * 4];

	unsigned char* pDestImage = pDepthTexBuf;

	const XnDepthPixel* pDepth = dmd.Data();
	const XnLabel* pLabels = smd.Data();

	// Calculate the accumulative histogram
	memset(g_pDepthHist, 0, OMEGA_OPENNI_MAX_DEPTH*sizeof(float));
	for (nY=0; nY<g_nYRes; nY++)
	{
		for (nX=0; nX<g_nXRes; nX++)
		{
			nValue = *pDepth;

			if (nValue != 0)
			{
				g_pDepthHist[nValue]++;
				nNumberOfPoints++;
			}

			pDepth++;
		}
	}

	for (nIndex=1; nIndex<OMEGA_OPENNI_MAX_DEPTH; nIndex++)
	{
		g_pDepthHist[nIndex] += g_pDepthHist[nIndex-1];
	}
	if (nNumberOfPoints)
	{
		for (nIndex=1; nIndex<OMEGA_OPENNI_MAX_DEPTH; nIndex++)
		{
			g_pDepthHist[nIndex] = (unsigned int)(256 * (1.0f - (g_pDepthHist[nIndex] / nNumberOfPoints)));
		}
	}

	pDepth = dmd.Data();
	nIndex = 0;
	// Prepare the texture map
	for (nY=0; nY<g_nYRes; nY++)
	{
		for (nX=0; nX < g_nXRes; nX++, nIndex++)
		{

			pDestImage[0] = 0;
			pDestImage[1] = 0;
			pDestImage[2] = 0;
			if ( *pLabels != 0 )
			{
				nValue = *pDepth;
				XnLabel label = *pLabels;
				XnUInt32 nColorID = label % nColors;
				if (label == 0)
				{
					nColorID = nColors;
				}

				if (nValue != 0)
				{
					nHistValue = g_pDepthHist[nValue];

					pDestImage[0] = nHistValue * Colors[nColorID][0]; 
					pDestImage[1] = nHistValue * Colors[nColorID][1];
					pDestImage[2] = nHistValue * Colors[nColorID][2];
				}
			}

			pDepth++;
			pLabels++;
			pDestImage+=3;
		}

		pDestImage += (texWidth - g_nXRes) *3;
	}
}