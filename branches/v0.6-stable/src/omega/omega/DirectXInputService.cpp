/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010-2011							Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Arthur Nishimoto							anishimoto42@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#include "omega/SystemManager.h"
#include "omega/DirectXInputService.h"

using namespace omega;

// Based on the Microsoft DirectX SDK (June 2010) joysyick_2008 sample code.
// http://www.microsoft.com/downloads/en/details.aspx?displaylang=en&FamilyID=3021d52b-514e-41d3-ad02-438a3ba730ba
// C:\Program Files\Microsoft DirectX SDK (June 2010)\Samples\C++\DirectInput\Joystick\

// Add to omega->properties->C++->General->Additional Include Directories
// S:\Visual Studio 2008\Projects\omegalib\include\directx\dxut\Core
// S:\Visual Studio 2008\Projects\omegalib\include\directx\dxut\Optional

// Add to omega->properties->Linker->Additional Dependencies
// ..\..\..\lib\win-x86-vs9-debug\dinput8.lib
// ..\..\..\lib\win-x86-vs9-debug\dxguid.lib

LPDIRECTINPUTDEVICE8 g_pJoystick[10];
LPDIRECTINPUT8 g_pDI;
int nControllers = 0;

//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{
	HRESULT hr; // Result flag
	printf("DirectXInputService: EnumJoysticksCallback called.\n");

	// Obtain an interface to the enumerated joystick.
	if( FAILED( hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick[nControllers], NULL ) ) ){
		printf("DirectXInputService: Failed to enumerate joystick.\n");
		return DIENUM_CONTINUE;
	} else {
		printf("DirectXInputService: Connecting to controller ID %d.\n",nControllers);
		nControllers++;
		return DIENUM_CONTINUE;
	}

	// Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    //return DIENUM_STOP;
	return DIENUM_CONTINUE;
}

//-----------------------------------------------------------------------------
// Name: EnumObjectsCallback()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
//       joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                   VOID* pContext )
{
    HWND hDlg = ( HWND )pContext;

    static int nSliderCount = 0;  // Number of returned slider controls
    static int nPOVCount = 0;     // Number of returned POV controls

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg;
        diprg.diph.dwSize = sizeof( DIPROPRANGE );
        diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
        diprg.diph.dwHow = DIPH_BYID;
        diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin = -1000;
        diprg.lMax = +1000;

        // Set the range for the axis
		for( int i = 0; i < nControllers; i++ )
        if( FAILED( g_pJoystick[i]->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
            return DIENUM_STOP;

    }


    // Set the UI to reflect what objects the joystick supports
    if( pdidoi->guidType == GUID_XAxis )
    {
        EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_YAxis )
    {
        EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_ZAxis )
    {
        EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_RxAxis )
    {
        EnableWindow( GetDlgItem( hDlg, IDC_X_ROT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_X_ROT_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_RyAxis )
    {
        EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_RzAxis )
    {
        EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_Slider )
    {
        switch( nSliderCount++ )
        {
            case 0 :
                EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0_TEXT ), TRUE );
                break;

            case 1 :
                EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1_TEXT ), TRUE );
                break;
        }
    }
    if( pdidoi->guidType == GUID_POV )
    {
        switch( nPOVCount++ )
        {
            case 0 :
                EnableWindow( GetDlgItem( hDlg, IDC_POV0 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_POV0_TEXT ), TRUE );
                break;

            case 1 :
                EnableWindow( GetDlgItem( hDlg, IDC_POV1 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_POV1_TEXT ), TRUE );
                break;

            case 2 :
                EnableWindow( GetDlgItem( hDlg, IDC_POV2 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_POV2_TEXT ), TRUE );
                break;

            case 3 :
                EnableWindow( GetDlgItem( hDlg, IDC_POV3 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_POV3_TEXT ), TRUE );
                break;
        }
    }

    return DIENUM_CONTINUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DirectXInputService* DirectXInputService::mysInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DirectXInputService::initialize() 
{
	printf("DirectXInputService: Initialize\n");
	mysInstance = this;

	g_pJoystick[0] = NULL; // DirectInput Object
	g_pDI = NULL; // Pointer to DirectInput8 interface

	HRESULT hr; // Result flag

	// Register with the DirectInput subsystem and get a pointer
	if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION, IID_IDirectInput8, ( VOID** )&g_pDI, NULL ) ) ){
		printf("DirectXInputService: Failed to register with DirectInput.\n");
		return;
	}
	
	checkForNewControllers();

	// Wiimote
	printf("DirectXInputService: Waiting for Wiimote\n");
	remote.Connect();
	//while(!remote.Connect()) {
	//}

	if( nControllers == 0 ){
		printf("DirectXInputService: No joysticks detected.\n");
        return;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DirectXInputService::checkForNewControllers() 
{
	HRESULT hr; // Result flag

	//DI_ENUM_CONTEXT enumContext;
	// Enumerate through devices
	if( FAILED( hr = g_pDI->EnumDevices(
         DI8DEVCLASS_GAMECTRL,
		 EnumJoysticksCallback,
         NULL,
         DIEDFL_ATTACHEDONLY
		 ) ) ){
		printf("DirectXInputService: Failed to enumerate input devices.\n");
	}

	 // Make sure we got a joystick
	for(int i = 0; i < nControllers; i++ )
    if( NULL == g_pJoystick[i] )
    {
        printf("DirectXInputService: Joystick not found.\n");
		return;
    }

	// Set the data format to "simple joystick" - a predefined data format 
    //
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
	for(int i = 0; i < nControllers; i++)
	if( FAILED( hr = g_pJoystick[i]->SetDataFormat( &c_dfDIJoystick2 ) ) ){
		printf("DirectXInputService: Failed set joystick data format.\n");
        return;
	}

	// Enumerate the joystick objects. The callback function enabled user
    // interface elements for objects that are found, and sets the min/max
    // values property for discovered axes.
    for(int i = 0; i < nControllers; i++)
	if( FAILED( hr = g_pJoystick[i]->EnumObjects( EnumObjectsCallback,
                                               NULL, DIDFT_ALL ) ) ){
		printf("DirectXInputService: Failed set joystick callback.\n");
        return;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DirectXInputService::poll() 
{
	//printf("DirectXInputService: Poll.\n");
	HRESULT hr;
	DIJOYSTATE2 js;           // DInput joystick state 
	for(int j = 0; j < nControllers; j++){
			
		if( NULL == g_pJoystick[j] )
			return;
		
		// Poll the device to read the current state
		hr = g_pJoystick[j]->Poll();
		if( FAILED( hr ) )
		{
			// DInput is telling us that the input stream has been
			// interrupted. We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done. We
			// just re-acquire and try again.
			hr = g_pJoystick[j]->Acquire();
			while( hr == DIERR_INPUTLOST )
				hr = g_pJoystick[j]->Acquire();

			// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
			// may occur when the app is minimized or in the process of 
			// switching, so just try again later 
			continue; // We continue so other connected controllers can still be checked
		}

		// Get the input's device state
		if( FAILED( hr = g_pJoystick[j]->GetDeviceState( sizeof( DIJOYSTATE2 ), &js ) ) ){
			printf("DirectXInputService: Failed to get input's device state for controller %d.\n", j);
			break; // The device should have been acquired during the Poll()
		}

		// Generate Events
		if(mysInstance)
		{
			mysInstance->lockEvents();

			Event* evt = mysInstance->writeHead();
			evt->reset(Event::Update, Service::Controller, j);

			evt->setExtraDataType(Event::ExtraDataFloatArray);

			// PS3
			evt->setExtraDataFloat(0, js.lX); 
			evt->setExtraDataFloat(1, js.lY);  // Left analog (-up, +down)

			evt->setExtraDataFloat(2, js.lRx); // Right analog (-left, +right)
			evt->setExtraDataFloat(3, js.lRy); // Right analog (-up, +down)

			evt->setExtraDataFloat(4, js.lZ); // Trigger 2 (+left, -right)
			
			// Buttons
			for( int i = 0; i < 15; i++ )
			{
				if( js.rgbButtons[i] & 0x80 )
				{
					evt->setExtraDataFloat(i + 5, 1);
				}
				else
				{
					evt->setExtraDataFloat(i + 5, 0);
				}
			 }
			evt->setExtraDataFloat(18, js.rgdwPOV[0]); // DPad

			evt->setExtraDataFloat(19, js.lRz); // Tilt (+left, -right)
			evt->setExtraDataFloat(20, js.rglSlider[1]); // Tilt (+back, -forward)
			
			mysInstance->unlockEvents();
		}
	
	}
	
	/*
	// Debug text
	// Axis state
	printf("X %d ", js.lX );
	printf("Y %d ", js.lY );
	printf("Z %d ", js.lZ );
	
	// Rotation state
	printf("rotX %d ", js.lRx );
	printf("rotY %d ", js.lRy );
	printf("rotZ %d ", js.lRz );

	// Slider states
	printf("slider0 %d ", js.rglSlider[0] );
	printf("slider1 %d ", js.rglSlider[1] );
	
	// POV state
	printf("POV0 %d ", js.rgdwPOV[0] );
	printf("POV1 %d ", js.rgdwPOV[1] );
	printf("POV2 %d ", js.rgdwPOV[2] );
	printf("POV3 %d ", js.rgdwPOV[3] );

	// Button state
	printf("Buttons: ");
	for( int i = 0; i < 128; i++ )
    {
        if( js.rgbButtons[i] & 0x80 )
        {
			printf("%d ", i);
        }
    }
	printf("\n");
	*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DirectXInputService::dispose() 
{
	mysInstance = NULL;
}