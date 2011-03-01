/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * MouseService method definitions. See MouseService.h for more details.
 *********************************************************************************************************************/
#include "omega/OptiTrackService.h"
#include "omega/SystemManager.h"
#include "omega/StringUtils.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void MouseService::mouseMotionCallback(int x, int y)
//{
//	if(mysInstance)
//	{
//		mysInstance->lockEvents();
//
//		Event* evt = mysInstance->writeHead();
//		//	evt->id = OM_ID_MOUSE;
//		//	evt->source = OM_DC_POINTER;
//		//	evt->type = OM_EVENT_MOVE;
//		evt->x = x;
//		evt->y = y;
//
//		mysInstance->unlockEvents();
//	}
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OptiTrackService::initialize() 
{
	//== initialize Microsoft COM Interop ================----
	CoInitialize(NULL);

	myVector.CoCreateInstance(CLSID_NPVector);

	//== initialize OptiTrack COM Component ==============----
	CComPtr<INPCameraCollection> cameraCollection;
    CComPtr<INPCamera>           camera;

    cameraCollection.CoCreateInstance(CLSID_NPCameraCollection);

	//== Enumerate (Identify) Available Cameras ==========----
	cameraCollection->Enum();

	long cameraCount  = 0;
	int  frameCounter = 0;

	//== Determine Available Cameras =====================----
    cameraCollection->get_Count(&cameraCount);

	ofmsg("Optitrack: %1% Camera(s) Detected:\n\n", %cameraCount);
	
	//== Display Camera Information for All Cameras ======----
	for(int index=0; index<cameraCount; index++)
	{
		cameraCollection->Item(index, &camera);

		long serial,width,height,model,revision,rate;

		camera->get_SerialNumber(&serial);
		camera->get_Width       (&width);
		camera->get_Height      (&height);
		camera->get_Model       (&model);
		camera->get_Revision    (&revision);
		camera->get_FrameRate   (&rate);

		//ofmsg("  Camera %d",serial);
		//ofmsg("  =========================");
		//ofmsg("  Resolution: %dx%d",width,height);
		//ofmsg("  Revision  : 0x%8x",revision);
		//ofmsg("  Model     : 0x%8x",model);
		//ofmsg("  Frame rate: %d" ,rate);

		//== Set Some Camera Options ====================----

		//== Set to discard every other frame ===========----
		//== 60 Frames/Second on the C120 ===============----
        camera->SetOption(NP_OPTION_FRAME_DECIMATION  , (CComVariant) 1 );

		//== Always Clean-up COM Objects ================----
		//camera.Release();
	}

	myCamera = camera;


}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OptiTrackService::start()
{
	if(myCamera != NULL)
	{
		myCamera->Open();
		myCamera->Start();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OptiTrackService::poll() 
{
	// If an optitrack camera has not been set, just return.
	if(!myCamera) return;

	HRESULT hr;
	CComPtr<INPCameraFrame> frame;

	CComVariant x, y, z, yaw, pitch, roll;
	VariantInit(&x);
	VariantInit(&y);
	VariantInit(&z);
	VariantInit(&yaw);
	VariantInit(&pitch);
	VariantInit(&roll);
	
	VARIANT_BOOL empty = VARIANT_FALSE;
	
	myCamera->GetFrame(0, &frame);
	
	if(frame != NULL)
	{
		frame->get_IsEmpty(&empty);

		if(empty == VARIANT_FALSE )
		{
			hr = myVector->Update(myCamera, frame);
			hr = myVector->get_X(&x);
			hr = myVector->get_Y(&y);
			hr = myVector->get_Z(&z);
			hr = myVector->get_Yaw(&yaw);
			hr = myVector->get_Pitch(&pitch);
			hr = myVector->get_Roll(&roll);

			//printf("x=%.3f  y=%.3f  z=%.3f   yaw=%.3f  pitch=%.3f  roll=%.3f \n", x.dblVal, y.dblVal, z.dblVal, yaw.dblVal, pitch.dblVal, roll.dblVal);
			lockEvents();

			Event* evt = writeHead();
			//	evt->id = OM_ID_MOUSE;
			//	evt->source = OM_DC_POINTER;
			//	evt->type = OM_EVENT_MOVE;
			evt->serviceType = Service::Mocap;

			evt->sourceId = 1;
			evt->position[0] = -x.dblVal / 1000.0f;
			evt->position[1] = y.dblVal  / 1000.0f;
			evt->position[2] = z.dblVal  / 1000.0f;
			
			Quaternion qyaw, qpitch, qroll;

			qpitch.fromAngleAxis(pitch.dblVal, Vector3f::UNIT_X);
			qyaw.fromAngleAxis(yaw.dblVal, Vector3f::UNIT_Y);
			qroll.fromAngleAxis(roll.dblVal, Vector3f::UNIT_Z);

			evt->orientation = qpitch * qyaw * qroll;

			unlockEvents();


			frame->Free();
			frame.Release();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OptiTrackService::stop()
{
	myCamera->Stop();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OptiTrackService::dispose() 
{
}
