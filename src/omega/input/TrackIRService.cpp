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
#include "input/TrackIRService.h"
#include "SystemManager.h"
#include "Log.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void MouseService::mouseMotionCallback(int x, int y)
//{
//	if(myInstance)
//	{
//		myInstance->LockEvents();
//
//		InputEvent* evt = myInstance->WriteHead();
//		//	evt->id = OM_ID_MOUSE;
//		//	evt->source = OM_DC_POINTER;
//		//	evt->type = OM_EVENT_MOVE;
//		evt->x = x;
//		evt->y = y;
//
//		myInstance->UnlockEvents();
//	}
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TrackIRService::Initialize() 
{
	//== Initialize Microsoft COM Interop ================----
	CoInitialize(NULL);

	myVector.CoCreateInstance(CLSID_NPVector);

	//== Initialize OptiTrack COM Component ==============----
	CComPtr<INPCameraCollection> cameraCollection;
    CComPtr<INPCamera>           camera;

    cameraCollection.CoCreateInstance(CLSID_NPCameraCollection);

	//== Enumerate (Identify) Available Cameras ==========----
	cameraCollection->Enum();

	long cameraCount  = 0;
	int  frameCounter = 0;

	//== Determine Available Cameras =====================----
    cameraCollection->get_Count(&cameraCount);

	Log::Message("Optitrack: %d Camera(s) Detected:\n\n", cameraCount);
	
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

		Log::Message("  Camera %d",serial);
		Log::Message("  =========================");
		Log::Message("  Resolution: %dx%d",width,height);
		Log::Message("  Revision  : 0x%8x",revision);
		Log::Message("  Model     : 0x%8x",model);
		Log::Message("  Frame rate: %d" ,rate);

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
void TrackIRService::Start()
{
	if(myCamera != NULL)
	{
		myCamera->Open();
		myCamera->Start();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TrackIRService::Poll() 
{
	double radToDegree = 180.0 / 3.14159265;
	double degreeToRad =  3.14159265 / 180.0;

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
			LockEvents();

			InputEvent* evt = WriteHead();
			//	evt->id = OM_ID_MOUSE;
			//	evt->source = OM_DC_POINTER;
			//	evt->type = OM_EVENT_MOVE;
			evt->source = InputEvent::Mocap;

			evt->x = x.dblVal / 100.0f;
			evt->y = y.dblVal  / 100.0f;
			evt->z = z.dblVal  / 100.0f;
			evt->rx = pitch.dblVal * degreeToRad;
			evt->ry = yaw.dblVal  * degreeToRad;
			evt->rz = roll.dblVal  * degreeToRad;

			UnlockEvents();


			frame->Free();
			frame.Release();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TrackIRService::Stop()
{
	myCamera->Stop();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TrackIRService::Dispose() 
{
}
