#include "inc/stdafx.h"
#include <conio.h>    //== For sample simplicity: kbhit()

#include "inc/socket_utils.h"

//== NECESSARY OPTITRACK INCLUDES AND DEFINITIONS ========================-----

#include <objbase.h>
#include <atlbase.h>
#include "inc/optitrack.h"
#import  "lib/optitrack.tlb"

int SLOW_POL_RATE = 15;
int FAST_POL_RATE = 1;

bool show_out = true;
bool calibrate = false;
bool slow_frame = false;
float x_offset, y_offset, z_offset;
float rx_offset, ry_offset, rz_offset;



void processkey()
{
	int character = getch();
	switch ( character )
	{
		case 27 : exit(0);	break;

		case 99 :	//c
			calibrate = true;
			break;

			case 100 :	//d
			slow_frame = !slow_frame;
			break;

		case 115 :	//s
			show_out = !show_out;
			if(show_out) printf( "Showing output\n");
			else printf( "Not Showing output\n");
			break;

		default : 
			printf( "key %d \n", character );
			break;
	}

}

int _tmain(int argc, TCHAR* argv[])
{
	printf("OptiTrack WIN32 Sample Application ============================================\n");
	printf("Copyright 2007 NaturalPoint =============================================------\n\n");

	//== Initialize Microsoft COM Interop ================----
	CoInitialize(NULL);

	//== Initialize OptiTrack COM Component ==============----
	CComPtr<INPCameraCollection> cameraCollection;
    CComPtr<INPCamera>           camera;
	CComPtr<INPCameraFrame>		 frame;
	CComPtr<INPVector3>          vector;

    cameraCollection.CoCreateInstance(CLSID_NPCameraCollection);
	vector.CoCreateInstance(CLSID_NPVector);

	//== Enumerate (Identify) Available Cameras ==========----
	cameraCollection->Enum();

	long cameraCount  = 0;
	int  frameCounter = 0;

	//== Determine Available Cameras =====================----
    cameraCollection->get_Count(&cameraCount);

	printf("%d Camera(s) Detected:\n\n", cameraCount);
	
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

		printf("  Camera %d\n",serial);
		printf("  =========================\n");
		printf("  Resolution: %dx%d\n",width,height);
		printf("  Revision  : 0x%8x\n",revision);
		printf("  Model     : 0x%8x\n",model);
		printf("  Frame rate: %d\n\n" ,rate);

		//== Set Some Camera Options ====================----

		//== Set to discard every other frame ===========----
		//== 60 Frames/Second on the C120 ===============----
        camera->SetOption(NP_OPTION_FRAME_DECIMATION  , (CComVariant) 1 );

		//== Always Clean-up COM Objects ================----
		camera.Release();
	}

	//== Open the first camera ==========================----
	
	if(cameraCount>0)
	{
		SOCKET sd;							/* Socket descriptor of server */
		struct sockaddr_in server;			/* Information about the client */
		tracker_frame frameData;
		char filename[100]="server.config";
		CComVariant x, y, z, yaw, pitch, roll;
		VariantInit(&x);
		VariantInit(&y);
		VariantInit(&z);
		VariantInit(&yaw);
		VariantInit(&pitch);
		VariantInit(&roll);
		HRESULT hr;
		
		printf("\nPress:\n");
		printf("  Esc: \t Exit\n");
		printf("  s:   \t Show output\n");
		printf("  c:   \t Calibrate\n");

		cameraCollection->Item(0, &camera);
		{
			sd = setupSocketClient(filename,&server);
			camera->Open();
			camera->Start();
            {
			    while( 1 )
			    {
					if ( kbhit() ) processkey();
					
				    Sleep(5);
					
					camera->GetFrame(0, &frame);
					if(frame != 0) {
						frameCounter++;
						
						int polRate;
						if ( slow_frame ) polRate = SLOW_POL_RATE;
						else polRate = FAST_POL_RATE;

						if(frameCounter % polRate == 0) {
							//Grab Data 
							hr = vector->Update(camera, frame);
							hr = vector->get_X(&x);
							hr = vector->get_Y(&y);
							hr = vector->get_Z(&z);
							hr = vector->get_Pitch(&pitch);
							hr = vector->get_Yaw(&yaw);
							hr = vector->get_Roll(&roll);

							if( calibrate )
							{
								calibrate = false;
								x_offset = -(float)x.dblVal;
								y_offset = -(float)y.dblVal;
								z_offset = -(float)z.dblVal;
								rx_offset = -(float)pitch.dblVal;
								ry_offset = -(float)yaw.dblVal;
								rz_offset = -(float)roll.dblVal;
							}

							//store data 
							frameData.x = (float)x.dblVal + x_offset;
							frameData.y = (float)y.dblVal + y_offset;
							frameData.z = (float)z.dblVal + z_offset;
							frameData.pitch = (float)pitch.dblVal + rx_offset;
							frameData.yaw = (float)yaw.dblVal + ry_offset;
							frameData.roll = (float)roll.dblVal + rz_offset;

							if(show_out)
							{
								printf("Pos: {%.2f, %.2f, %.2f}      Rot: { %.2f, %.2f, %.2f } \n", 
									frameData.x, frameData.y, frameData.z,
									frameData.pitch, frameData.yaw, frameData.roll);
							}

							if(sendData(sd, &frameData, sizeof(frameData), (struct sockaddr *)&server) != sizeof(frameData)) {
								camera->Stop();
								camera->Close();
								camera.Release();
								cameraCollection.Release();
								vector.Release();
								CoUninitialize();
								exit(1);
							}
						}
						frame->Free();
						frame.Release();
					}
					
			    }	
            }
			camera->Stop();
			camera->Close();
		}
		camera.Release();
	}


	//== Always Clean-up COM Objects ================----
	cameraCollection.Release();
	vector.Release();

	//== Uninitialize Microsoft COM Interop =============----
	CoUninitialize();

	printf("Application Exit.\n");
	getchar();
	return 0;
}

