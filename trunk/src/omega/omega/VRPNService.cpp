/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Arthur Nishimoto							anishimoto42@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * VRPN Client service.
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

#include "omega/VRPNService.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// User routine to handle a tracker position update.  This is called when
// the tracker callback is called (when a message from its counterpart
// across the connection arrives).
/* // For additional information http://www.cs.unc.edu/Research/vrpn/vrpn_Tracker_Remote.html
typedef	struct {
	struct timeval	msg_time;	// Time of the report
	long		sensor;		// Which sensor is reporting
	double		pos[3];		// Position of the sensor
	double		quat[4];	// Orientation of the sensor
} vrpn_TRACKERCB; */
void VRPN_CALLBACK handle_tracker(void *userdata, const vrpn_TRACKERCB t)
{
  //this function gets called when the tracker's POSITION xform is updated

  //you can change what this callback function is called for by changing
  //the type of t in the function prototype above.
  //Options are:
  //   vrpn_TRACKERCB              position
  //   vrpn_TRACKERVELCB           velocity
  //   vrpn_TRACKERACCCB           acceleration
  //   vrpn_TRACKERTRACKER2ROOMCB  tracker2room transform 
  //                                 (comes from local or remote
  //                                  vrpn_Tracker.cfg file)
  //   vrpn_TRACKERUNIT2SENSORCB   unit2sensor transform (see above comment)
  //   vrpn_TRACKERWORKSPACECB     workspace bounding box (extent of tracker)

  // userdata is whatever you passed into the register_change_handler function.
  // vrpn sucks it up and spits it back out at you. It's not used by vrpn internally
/*
	printf("handle_tracker\tObject %d POS: (%g,%g,%g) QUAT: (%g,%g,%g)\n", 
	 userdata,
	 t.pos[0], t.pos[1], t.pos[2],
	 t.quat[0], t.quat[1], t.quat[2]
		);
*/
	((VRPNService*)userdata)->generateEvent(t);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VRPNService::setup(Setting& settings)
{
	if(settings.exists("serverIP"))
	{
		server_ip =  (const char*)settings["serverIP"];
	}

	if(settings.exists("rigidBodies"))
	{
		Setting& strs = settings["rigidBodies"];
        for(int i = 0; i < strs.getLength(); i++)
        {
			Setting& str = strs[i];
			TrackerInfo trackerInfo;
			trackerInfo.object_name = (const char*)str["name"];
			if( str.exists("serverIP") ){
				trackerInfo.server_ip = (const char*)str["serverIP"]; // Use object specified IP
			} else {
				trackerInfo.server_ip = server_ip; // Use global IP
			}
			trackerInfo.trackableId = str["trackableID"];
			trackerNames.push_back(trackerInfo);

		}

	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VRPNService* VRPNService::mysInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VRPNService::initialize() 
{
	printf("VRPNService: Initialize\n");
	mysInstance = this;

	for(int i = 0; i < trackerNames.size(); i++)
	{
		TrackerInfo& t = trackerNames[i];
		char trackerName[256];
		strcpy(trackerName,t.object_name);
		strcat(trackerName,"@");
		strcat(trackerName,t.server_ip);
		printf("Added %s \n" , trackerName);

		// Open the tracker: '[object name]@[tracker IP]'
		vrpn_Tracker_Remote *tkr = new vrpn_Tracker_Remote(trackerName);

		// Set up the tracker callback handler
		tkr->register_change_handler((void*)this, handle_tracker);

		// Add to tracker remote list
		trackerRemotes.push_back(tkr);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VRPNService::poll() 
{
	int done = 0;
	while ( ! done ) {
		for(int i = 0; i < trackerRemotes.size(); i++)
		{
			vrpn_Tracker_Remote *tkr = trackerRemotes[i];
			// Let the tracker do it's thing
                // It will call the callback funtions you registered above
				// as needed
			tkr->mainloop();
		}
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VRPNService::dispose() 
{
	mysInstance = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VRPNService::generateEvent(vrpn_TRACKERCB t) 
{
	Event* evt = mysInstance->writeHead();
	//	evt->id = OM_ID_MOUSE;
	//	evt->source = OM_DC_POINTER;
	//	evt->type = OM_EVENT_MOVE;
	evt->serviceType = Service::Mocap;

	evt->sourceId = 1;
	evt->position[0] = t.pos[0];
	evt->position[1] = t.pos[1];
	evt->position[2] = t.pos[2];

	Quaternion qyaw, qpitch, qroll;

	qpitch = AngleAxis(t.quat[0], Vector3f::UnitX());
	qyaw = AngleAxis(t.quat[1], Vector3f::UnitY());
	qroll = AngleAxis(t.quat[2], Vector3f::UnitZ());

	evt->orientation = qpitch * qyaw * qroll;

	printf("handle_tracker\tObject %d POS: (%g,%g,%g) QUAT: (%g,%g,%g)\n", 
	 0,
	 t.pos[0], t.pos[1], t.pos[2],
	 t.quat[0], t.quat[1], t.quat[2]
		);
	mysInstance->unlockEvents();
}