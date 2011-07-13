/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Arthur Nishimoto							anishimoto42@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * 
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

// Based on WiiYourself! v1.15 by gl.tter (http://gl.tter.org)

#include <windows.h>
#include "omega/Wiimote.h"

#include <ddk/setupapi.h>

extern "C" {
#include "ddk/hidsdi.h"
}

#include <stdio.h>

using namespace omega;

Wiimote::Wiimote(){
}

Wiimote::~Wiimote(){
}

bool Wiimote::Connect(){
	//if(wiimote_index == FIRST_AVAILABLE)
	//	TRACE(_T("Connecting first available Wiimote:"));
	//else
	//	TRACE(_T("Connecting Wiimote %u:"), wiimote_index);

	// auto-disconnect if user is being naughty
	//if(IsConnected())
	//	Disconnect();

	// get the GUID of the HID class
	GUID guid;
	HidD_GetHidGuid(&guid);

	// get a handle to all devices that are part of the HID class
	// Brian: Fun fact:  DIGCF_PRESENT worked on my machine just fine.  I reinstalled
	//   Vista, and now it no longer finds the Wiimote with that parameter enabled...
	HDEVINFO dev_info = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_DEVICEINTERFACE);// | DIGCF_PRESENT);
	if(!dev_info) {
		printf("couldn't get device info\n");
		return false;
		}

	// enumerate the devices
	SP_DEVICE_INTERFACE_DATA didata;
	didata.cbSize = sizeof(didata);
	
	unsigned index			= 0;
	unsigned wiimotes_found = 0;
	while(SetupDiEnumDeviceInterfaces(dev_info, NULL, &guid, index, &didata))
		{
		// get the buffer size for this device detail instance
		DWORD req_size = 0;
		SetupDiGetDeviceInterfaceDetail(dev_info, &didata, NULL, 0, &req_size, NULL);

		// (bizarre way of doing it) create a buffer large enough to hold the
		//  fixed-size detail struct components, and the variable string size
		SP_DEVICE_INTERFACE_DETAIL_DATA *didetail =
								(SP_DEVICE_INTERFACE_DETAIL_DATA*) new BYTE[req_size];
		//printf(didetail);
		didetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		// now actually get the detail struct
		if(!SetupDiGetDeviceInterfaceDetail(dev_info, &didata, didetail,
											req_size, &req_size, NULL)) {
			printf("couldn't get devinterface info for %u\n", index);
			break;
			}

		// open a shared handle to the device to query it (this will succeed even
		//  if the wiimote is already Connect()'ed)
		//printf(".. querying device %s\n", didetail->DevicePath);
		Handle = CreateFile(didetail->DevicePath, 0, FILE_SHARE_READ|FILE_SHARE_WRITE,
												  NULL, OPEN_EXISTING, 0, NULL);
		if(Handle == INVALID_HANDLE_VALUE) {
			//printf(".... failed with err %x (probably harmless).\n", GetLastError());
			goto skip;
			}
	
		// get the device attributes
		HIDD_ATTRIBUTES attrib;
		attrib.Size = sizeof(attrib);
		if(HidD_GetAttributes(Handle, &attrib))	{
			// is this a wiimote?
				if((attrib.VendorID != VID) || (attrib.ProductID != PID)){
					//printf("Non-wiimote found VID %d PID %d \n", attrib.VendorID, attrib.ProductID );
					goto skip;
				}
			// yes, but is it the one we're interested in?
			printf("Wiimote found VID %d PID %d \n", attrib.VendorID, attrib.ProductID );
			++wiimotes_found;
			//if((wiimote_index != FIRST_AVAILABLE) &&
			//   (wiimote_index != wiimotes_found))
			//	goto skip;

			// the wiimote is installed, but it may not be currently paired:
			//if(wiimote_index == FIRST_AVAILABLE)
			//	TRACE(_T(".. opening Wiimote %u:"), wiimotes_found);
			//else
			//	TRACE(_T(".. opening:"));


			// re-open the handle, but this time we don't allow write sharing
			//  (that way subsequent calls can still _discover_ wiimotes above, but
			//   will correctly fail here if they're already connected)
			CloseHandle(Handle);
			
			// note this also means that if another application has already opened
			//  the device, the library can no longer connect it (this may happen
			//  with software that enumerates all joysticks in the system, because
			//  even though the wiimote is not a standard joystick (and can't
			//  be read as such), it unfortunately announces itself to the OS
			//  as one.  The SDL library was known to do grab wiimotes like this.
			//  If you cannot stop the application from doing it, you may change the
			//  call below to open the device in full shared mode - but then the
			//  library can no longer detect if you've already connected a device
			//  and will allow you to connect it twice!  So be careful ...
			Handle = CreateFile(didetail->DevicePath, GENERIC_READ|GENERIC_WRITE,
													FILE_SHARE_READ,
													NULL, OPEN_EXISTING,
													FILE_FLAG_OVERLAPPED, NULL);
			if(Handle == INVALID_HANDLE_VALUE) {
				printf(".... failed with err %x\n", GetLastError());
				goto skip;
				}
			/*
			// clear the wiimote state & buffers
			Clear		  (false);		// preserves existing deadzones
			Internal.Clear(false);		// "
			InternalChanged = NO_CHANGE;
			memset(ReadBuff , 0, sizeof(ReadBuff));
			bConnectionLost	   = false;
			bConnectInProgress = true; // don't parse extensions or request regular
									   //  updates until complete
			// enable async reading
			BeginAsyncRead();

			// autodetect which write method the Bluetooth stack supports,
			//  by requesting the wiimote status report:
			if(force_hidwrites && !_HidD_SetOutputReport) {
				TRACE(_T(".. can't force HID writes (not supported)"));
				force_hidwrites = false;
				}

			if(force_hidwrites)
				TRACE(_T(".. (HID writes forced)"));
			else{
				//  - try WriteFile() first as it's the most efficient (it uses
				//     harware interrupts where possible and is async-capable):
				bUseHIDwrite = false;
				RequestStatusReport();
				//  and wait for the report to arrive:
				DWORD last_time = timeGetTime();
				while(!bStatusReceived && ((timeGetTime()-last_time) < 500))
					Sleep(10);
				TRACE(_T(".. WriteFile() %s."), bStatusReceived? _T("succeeded") :
																 _T("failed"));
				}

			// try HID write method (if supported)
			if(!bStatusReceived && _HidD_SetOutputReport)
				{
				bUseHIDwrite = true;
				RequestStatusReport();
				// wait for the report to arrive:
				DWORD last_time = timeGetTime();
				while(!bStatusReceived && ((timeGetTime()-last_time) < 500))
					Sleep(10);
				// did we get it?
				TRACE(_T(".. HID write %s."), bStatusReceived? _T("succeeded") :
															   _T("failed"));
				}

			// still failed?
			if(!bStatusReceived) {
				WARN(_T("output failed - wiimote is not connected (or confused)."));
				Disconnect();
				goto skip;
				}

//Sleep(500);
			// reset it
			Reset();

			// read the wiimote calibration info
			ReadCalibration();

			// allow the result(s) to come in (so that the caller can immediately test
			//  MotionPlusConnected()
			Sleep(300); // note, don't need it on my system, better to be safe though

			// connected succesfully:
			_TotalConnected++;

			// use the first incomding analogue sensor values as the 'at rest'
			//  offsets (only supports the Balance Board currently)
			bCalibrateAtRest = true;

			// refresh the public state from the internal one (so that everything
			//  is available straight away
			RefreshState();

			// attempt to construct a unique hardware ID from the calibration
			//  data bytes (this is obviously not guaranteed to be unique across
			//  all devices, but may work fairly well in practice... ?)
			memcpy(&UniqueID, &CalibrationInfo, sizeof(CalibrationInfo));

			_ASSERT(UniqueID != 0); // if this fires, the calibration data didn't
									//  arrive - this shouldn't happen

#ifdef ID2_FROM_DEVICEPATH		// (see comments in header)
			// create a 2nd alternative id by simply adding all the characters
			//  in the device path to create a single number
			UniqueID2 = 0;
			for(unsigned index=0; index<_tcslen(didetail->DevicePath); index++)
				UniqueID2 += didetail->DevicePath[index];
#endif
			// and show when we want to trigger the next periodic status request
			//  (for battery level and connection loss detection)
			NextStatusTime		= timeGetTime() + REQUEST_STATUS_EVERY_MS;
			NextMPlusDetectTime = timeGetTime() + DETECT_MPLUS_EVERY_MS;
			MPlusDetectCount	= DETECT_MPLUS_COUNT;

			// tidy up
			delete[] (BYTE*)didetail;
			break;*/
			}
skip:
		// tidy up
		delete[] (BYTE*)didetail;

		if(Handle != INVALID_HANDLE_VALUE) {
			CloseHandle(Handle);
			Handle = INVALID_HANDLE_VALUE;
			}
		// if this was the specified wiimote index, abort
		//if((wiimote_index != FIRST_AVAILABLE) &&
		 //  (wiimote_index == (wiimotes_found-1)))
		//   break;

		index++;
		}

	// clean up our list
	SetupDiDestroyDeviceInfoList(dev_info);
	/*
	//bConnectInProgress = false;
	if(IsConnected()) {
		TRACE(_T(".. connected!"));
		// notify the callbacks (if requested to do so)
		if(CallbackTriggerFlags & CONNECTED)
			{
			ChangedNotifier(CONNECTED, Internal);
			if(ChangedCallback)
				ChangedCallback(*this, CONNECTED, Internal);
			}
		return true;
		}
	TRACE(_T(".. connection failed.")); */
	return false;
}

void Wiimote::Disconnect(){
	if(Handle == INVALID_HANDLE_VALUE)
		return;

	printf("Wiimote: Disconnect().");
	/*
	if(IsConnected()){
		_ASSERT(_TotalConnected > 0); // sanity
		_TotalConnected--;
		
		if(!bConnectionLost)
			Reset();
	}
	*/
	CloseHandle(Handle);
	Handle = INVALID_HANDLE_VALUE;
}