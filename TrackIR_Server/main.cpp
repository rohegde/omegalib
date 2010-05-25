#include "inc/stdafx.h"
#include <conio.h>    //== For sample simplicity: kbhit()

#include "inc/socket_utils.h"

//== NECESSARY OPTITRACK INCLUDES AND DEFINITIONS ========================-----

#include <objbase.h>
#include <atlbase.h>
#include "inc/optitrack.h"
#import  "lib/optitrack.tlb"

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
		CComVariant x, y, z, yaw, pitch, roll;
		VariantInit(&x);
		VariantInit(&y);
		VariantInit(&z);
		VariantInit(&yaw);
		VariantInit(&pitch);
		VariantInit(&roll);
		
		HRESULT hr;
		
		printf("\nPress Return To Exit\n\n");
		
		cameraCollection->Item(0, &camera);
		{
			camera->Open();
			camera->Start();
            {
			    while(!_kbhit())
			    {
				    Sleep(5);
					
					camera->GetFrame(0, &frame);
					if(frame != 0) {
						frameCounter++;
						if(frameCounter % 30 == 0) {
							printf("Frame data ok.\n");
							hr = vector->Update(camera, frame);
							hr = vector->get_X(&x);
							hr = vector->get_Y(&y);
							hr = vector->get_Z(&z);
							hr = vector->get_Yaw(&yaw);
							hr = vector->get_Pitch(&pitch);
							hr = vector->get_Roll(&roll);
							printf("x=%.3f  y=%.3f  z=%.3f   yaw=%.3f  pitch=%.3f  roll=%.3f \n", x.dblVal, y.dblVal, z.dblVal, yaw.dblVal, pitch.dblVal, roll.dblVal);
						}
						frame->Free();
						frame.Release();
					}
					
					/* Is this necessary?
	                MSG msg;
                    if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
                    {
                        if(GetMessage( &msg, NULL, 0, 0 ) )
                        {
                            TranslateMessage(&msg);
                            DispatchMessage(&msg);
                        }
                    }
					*/
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

