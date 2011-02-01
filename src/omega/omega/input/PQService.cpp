/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010-2011							Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Arthur Nishimoto								anishimoto42@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/input/PQService.h"
#include "omega/SystemManager.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PQService* PQService::mysInstance = NULL;
int PQService::maxBlobSize = 1000;
int PQService::maxTouches = 1000; // Number of IDs assigned before resetting. Should match touchID array initialization
int PQService::serverX = 0; // Resolution of the machine running PQLabs
int PQService::serverY = 0; 
int PQService::screenX = 0; // If set to 1,1 PQService will send events as a normalized coordinates.
int PQService::screenY = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService::setup(Setting& settings)
{
	if(settings.exists("serverIP"))
	{
		server_ip =  (const char*)settings["serverIP"];
	}
	if(settings.exists("maxBlobSize"))
	{
		maxBlobSize =  atoi((const char*)settings["maxBlobSize"]);
	}
	if(settings.exists("ServerX"))
	{
		serverX =  atoi((const char*)settings["ServerX"]);
		printf("PQService: ServerX set to %d\n", serverX);
	}
	if(settings.exists("ServerY"))
	{
		serverY =  atoi((const char*)settings["ServerY"]);
		printf("PQService: ServerY set to %d\n", serverY);
	}
	if(settings.exists("ScreenX"))
	{
		screenX =  atoi((const char*)settings["ScreenX"]);
		printf("PQService: ScreenX set to %d\n", screenX);
	}
	if(settings.exists("ScreenY"))
	{
		screenY =  atoi((const char*)settings["ScreenY"]);
		printf("PQService: ScreenY set to %d\n", screenY);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService::initialize( ) 
{
	mysInstance = this;
	
	memset(m_pf_on_tges,0, sizeof(m_pf_on_tges));
	//server_ip = "131.193.77.102";

	nextID = 0;
	for(int i = 0; i < maxTouches; i++){
		touchID[i] = 0;
	}

	init();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService::initialize(  char* local_ip ) 
{
	mysInstance = this;
	
	memset(m_pf_on_tges,0, sizeof(m_pf_on_tges));
	server_ip = local_ip;
	init();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int PQService::init()
{
	int err_code = PQMTE_SUCESS;	
	// initialize the handle functions of gestures;
	InitFuncOnTG();
	// set the functions on server callback
	SetFuncsOnReceiveProc();
	// connect server
	printf("PQService: connect to server on %s \n", server_ip);
	if((err_code = ConnectServer(server_ip)) != PQMTE_SUCESS){
		printf("PQService: connect server fail, socket error code: %d\n", err_code);
		return err_code;
	}
	// send request to server
	printf("PQService: connect success, send request.\n");
	TouchClientRequest tcq = {0};
	tcq.app_id = GetTrialAppID();
	tcq.type = RQST_RAWDATA_ALL | RQST_GESTURE_ALL;
	if((err_code = SendRequest(tcq)) != PQMTE_SUCESS){
		printf("PQService: send request fail, error code: %d\n", err_code);
		return err_code;
	}
	//////////////you can set the move_threshold when the tcq.type is RQST_RAWDATA_INSIDE;
	//send threshold
	//int move_threshold = 1;// 1 pixel
	//if((err_code = SendThreshold(move_threshold) != PQMTE_SUCESS){
	//	printf(" send threadhold fail, error code:" << err_code << endl;
	//	return err_code;
	//}
	//
	////////////////////////
	//get server resolution
	if((err_code = GetServerResolution(OnGetServerResolution, NULL)) != PQMTE_SUCESS){
		printf("PQService: get server resolution fail,error code: %d\n", err_code);
		return err_code;
	};
	//
	// start receiving
	printf("PQService: send request success, start recv.\n");
	printf("PQService: Maximum blob size set to %i pixels \n", maxBlobSize);
	return err_code;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: InitFuncOnTG()
{
	// initialize the call back functions of toucha gestures;
	m_pf_on_tges[TG_TOUCH_START] = &PQService::OnTG_TouchStart;
	m_pf_on_tges[TG_DOWN] = &PQService::OnTG_Down;
	m_pf_on_tges[TG_MOVE] = &PQService::OnTG_Move;
	m_pf_on_tges[TG_UP] = &PQService::OnTG_Up;

	m_pf_on_tges[TG_SECOND_DOWN] = &PQService::OnTG_SecondDown;
	m_pf_on_tges[TG_SECOND_UP] = &PQService::OnTG_SecondUp;

	m_pf_on_tges[TG_SPLIT_START] = &PQService::OnTG_SplitStart;
	m_pf_on_tges[TG_SPLIT_APART] = &PQService::OnTG_SplitApart;
	m_pf_on_tges[TG_SPLIT_CLOSE] = &PQService::OnTG_SplitClose;
	m_pf_on_tges[TG_SPLIT_END] = &PQService::OnTG_SplitEnd;

	m_pf_on_tges[TG_TOUCH_END] = &PQService::OnTG_TouchEnd;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnTG_TouchStart(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_TOUCH_START);
	//printf("  here, the touch start, initialize something.\n");;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: DefaultOnTG(const TouchGesture & tg,void * call_object) // just show the gesture
{
	//cout <<"ges,name:"<< GetGestureName(tg) << " type:" << tg.type << ",param size:" << tg.param_size << " ";
	//for(int i = 0; i < tg.param_size; ++ i)
	//	cout << tg.params[i] << " ";
	//cout << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnTG_Down(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_DOWN && tg.param_size >= 2);
	//printf("  the single finger touching at :( " 
	//	<< tg.params[0] << "," << tg.params[1] << " )\n");;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnTG_Move(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_MOVE && tg.param_size >= 2);
	//printf("  the single finger moving on the screen at :( " 
	//	<< tg.params[0] << "," << tg.params[1] << " )\n");;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnTG_Up(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_UP && tg.param_size >= 2);
	//printf(" the single finger is leaving the screen at :( " 
	//	<< tg.params[0] << "," << tg.params[1] << " )\n");;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnTG_SecondDown(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SECOND_DOWN && tg.param_size >= 4);
	//printf("  the second finger touching at :( " 
	//	<< tg.params[0] << "," << tg.params[1] << " ),"
	//	<< " after the first finger touched at :( "
	//	<< tg.params[2] << "," << tg.params[3] << " )\n");;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnTG_SecondUp(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SECOND_UP && tg.param_size >= 4);
	//printf("  the second finger is leaving at :( " 
	//	<< tg.params[0] << "," << tg.params[1] << " ),"
	//	<< " while the first finger still anchored around :( "
	//	<< tg.params[2] << "," << tg.params[3] << " )\n");;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnTG_SplitStart(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SPLIT_START && tg.param_size >= 4);
	//printf("  the two fingers is splitting with one finger at: ( " 
	//	<< tg.params[0] << "," << tg.params[1] << " ),"
	//	<< " , the other at :( "
	//	<< tg.params[2] << "," << tg.params[3] << " )\n");;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnTG_SplitApart(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SPLIT_APART && tg.param_size >= 1);
	//printf("  the two fingers is splitting apart with there distance incresed by " 
	//	<< tg.params[0]
	//	<< " with a ratio :" << tg.params[1]
	//	<< endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnTG_SplitClose(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SPLIT_CLOSE && tg.param_size >= 1);
	//printf("  the two fingers is splitting close with there distance decresed by " 
	//	<< tg.params[0]
	//	<< " with a ratio :" << tg.params[1]
	//	<< endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnTG_SplitEnd(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SPLIT_END);
	//printf("  the two splitting fingers with one finger at: ( " 
	//	<< tg.params[0] << "," << tg.params[1] << " ),"
	//	<< " , the other at :( "
	//	<< tg.params[2] << "," << tg.params[3] << " )" 
	//	<< " will end\n");;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OnTG_TouchEnd: to clear what need to clear
void PQService:: OnTG_TouchEnd(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_TOUCH_END);
	//printf("  all the fingers is leaving and there is no fingers on the screen.\n");;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService::SetFuncsOnReceiveProc()
{
	PFuncOnReceivePointFrame old_rf_func = SetOnReceivePointFrame(&PQService::onReceivePointFrame,this);
	PFuncOnReceiveGesture old_rg_func = SetOnReceiveGesture(&PQService::onReceiveGesture,this);
	PFuncOnServerBreak old_svr_break = SetOnServerBreak(&PQService::onServerBreak,NULL);
	PFuncOnReceiveError old_rcv_err_func = SetOnReceiveError(&PQService::onReceiveError,NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: onReceivePointFrame(int frame_id, int time_stamp, int moving_point_count, const TouchPoint * moving_point_array, void * call_back_object)
{
	PQService * omegaDesk = static_cast<PQService*>(call_back_object);
	assert(omegaDesk != NULL);
	const char * tp_event[] = 
	{
		"down",
		"move",
		"up",
	};

	//printf(" frame_id:" << frame_id << " time:"  << time_stamp << " ms" << " moving point count:" << moving_point_count << endl;
	for(int i = 0; i < moving_point_count; ++ i){
		TouchPoint tp = moving_point_array[i];
		omegaDesk->OnTouchPoint(tp);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: onReceiveGesture(const TouchGesture & ges, void * call_back_object)
{
	PQService * omegaDesk = static_cast<PQService*>(call_back_object);
	assert(omegaDesk != NULL);
	omegaDesk->OnTouchGesture(ges);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: onServerBreak(void * param, void * call_back_object)
{
	// when the server break, disconenct server;
	printf("PQService: server break, disconnect here\n");;
	DisconnectServer();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService::onReceiveError(int err_code, void * call_back_object)
{
	switch(err_code)
	{
	case PQMTE_RCV_INVALIDATE_DATA:
		printf("PQService: error: receive invalidate data.\n");;
		break;
	case PQMTE_SERVER_VERSION_OLD:
		printf("PQService: error: the multi-touch server is old for this client, please update the multi-touch server.\n");;
		break;
	default:
		printf("PQService: socket error, socket error code:%d\n", err_code);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnGetServerResolution(int x, int y, void * call_back_object)
{
	serverX = x;
	serverY = y;
	
	printf("PQService: server resolution: %d , %d \n",serverX ,serverY );
	if( screenX == 0 && screenY == 0 ){
		screenX = x;
		screenY = y;
		printf("PQService: screen resolution not specified. using server resolution. \n");
	} else if( screenX == 1 && screenY == 1 ){
		printf("PQService: using normalized coordinates.\n");
	} else {
		printf("PQService: using screen resolution: %d , %d \n",screenX ,screenY );
		
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnTouchGesture(const TouchGesture & tg)
{
	if(TG_NO_ACTION == tg.type)
		return ;
	
	assert(tg.type <= TG_TOUCH_END);
	DefaultOnTG(tg,this);
	PFuncOnTouchGesture pf = m_pf_on_tges[tg.type];
	if(NULL != pf){
		pf(tg,this);
	}

	mysInstance->lockEvents();

	// Magic numbers for split and rotate gestures yay! Not needed for start/end events.
    float specialDividerX = 4096.0f;
	float specialDividerY = 4096.0f;
	
	bool debugText = false;

	InputEvent* evt;
	
	bool validEvent = false;
	switch(tg.type)
	{
		case TG_DOWN:
			evt = mysInstance->writeHead();
			evt->type = InputEvent::Down;

			evt->position[0] = tg.params[0] * screenX / serverX; // xPos
			evt->position[1] = tg.params[1] * screenY / serverY; // yPos
			if( debugText ){
				printf(" Touch down at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->position[0], evt->position[1] );
			}
			validEvent = true;
			break;
		case TG_MOVE:
			evt = mysInstance->writeHead();
			evt->type = InputEvent::Move;
			evt->position[0] = tg.params[0] * screenX / serverX; // xPos
			evt->position[1] = tg.params[1] * screenY / serverY; // yPos
			if( debugText ){
				printf(" Touch move at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->position[0], evt->position[1] );
			}
			validEvent = true;
			break;
		case TG_UP:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::Up;
			evt->position[0] = tg.params[0] * screenX / serverX; // xPos
			evt->position[1] = tg.params[1] * screenY / serverY; // yPos
			if( debugText ){
				printf(" Touch up at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->position[0], evt->position[1] );
			}
			validEvent = true;
			break;
		case TG_CLICK:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::Click;
			evt->position[0] = tg.params[0] * screenX / serverX; // xPos
			evt->position[1] = tg.params[1] * screenY / serverY; // yPos
			if( debugText ){
				printf(" Touch click at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->position[0], evt->position[1] );
			}
			validEvent = true;
			break;
		case TG_DB_CLICK:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::DoubleClick;
			evt->position[0] = tg.params[0] * screenX / serverX; // xPos
			evt->position[1] = tg.params[1] * screenY / serverY; // yPos
			if( debugText ){
				printf(" Touch db click at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->position[0], evt->position[1] );
			}
			validEvent = true;
			break;
		case TG_MOVE_RIGHT:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::MoveRight;
			evt->position[0] = tg.params[0] * screenX / serverX; // xPos
			evt->position[1] = tg.params[1] * screenY / serverY; // yPos
			if( debugText ){
				printf(" Touch move right at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->position[0], evt->position[1] );
			}
			validEvent = true;
			break;
		case TG_MOVE_UP:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::MoveUp;
			evt->position[0] = tg.params[0] * screenX / serverX; // xPos
			evt->position[1] = tg.params[1] * screenY / serverY; // yPos
			if( debugText ){
				printf(" Touch move up at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->position[0], evt->position[1] );
			}
			validEvent = true;
			break;
		case TG_MOVE_LEFT:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::MoveLeft;
			evt->position[0] = tg.params[0] * screenX / serverX; // xPos
			evt->position[1] = tg.params[1] * screenY / serverY; // yPos
			if( debugText ){
				printf(" Touch move left at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->position[0], evt->position[1] );
			}
			validEvent = true;
			break;
		case TG_MOVE_DOWN:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::MoveDown;
			evt->position[0] = tg.params[0] * screenX / serverX; // xPos
			evt->position[1] = tg.params[1] * screenY / serverY; // yPos
			if( debugText ){
				printf(" Touch move down at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->position[0], evt->position[1] );
			}
			validEvent = true;
			break;
		case TG_SPLIT_START:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::SplitStart;

			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[0] * screenX / serverX; // x2
			evt->pointSet[0][1] = tg.params[1] * screenY / serverY; // y2
			evt->pointSet[1][0] = tg.params[2] * screenX / serverX; // x1
			evt->pointSet[1][1] = tg.params[3] * screenY / serverY; // y1

			evt->position = (evt->pointSet[0] + evt->pointSet[1]) / 2;
			if( debugText ){
				printf(" Touch Split start at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->pointSet[0][0], evt->pointSet[0][1] );
				printf("                      %f,%f (%f, %f)\n", tg.params[2], tg.params[3], evt->pointSet[1][0], evt->pointSet[1][1] );
			}
			validEvent = true;
			break;
		case TG_SPLIT_END:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::SplitEnd;

			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[0] * screenX / serverX; // x2
			evt->pointSet[0][1] = tg.params[1] * screenY / serverY; // y2
			evt->pointSet[1][0] = tg.params[2] * screenX / serverX; // x1
			evt->pointSet[1][1] = tg.params[3] * screenY / serverY; // y1

			evt->position = (evt->pointSet[0] + evt->pointSet[1]) / 2;
			if( debugText ){
				printf(" Touch Split end at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->pointSet[0][0], evt->pointSet[1][0] );
				printf("                    %f,%f (%f, %f)\n", tg.params[2], tg.params[3], evt->pointSet[1][0], evt->pointSet[1][1] );
			}
			validEvent = true;
			break;
		case TG_SPLIT_APART:
		case TG_SPLIT_CLOSE:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::Split;
			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[2] * screenX / specialDividerX; // x2
			evt->pointSet[0][1] = tg.params[3] * screenY / specialDividerY; // y2
			evt->pointSet[1][0] = tg.params[4] * screenX / specialDividerX; // x1
			evt->pointSet[1][1] = tg.params[5] * screenY / specialDividerY; // y1

			evt->position = (evt->pointSet[0] + evt->pointSet[1]) / 2; // midpoint

			evt->value[0] = tg.params[0]; // delta distance
			evt->value[1] = tg.params[1]; // delta ratio
			if( debugText ){
				printf(" Touch Split at %f,%f (%f, %f)\n", tg.params[2], tg.params[3], evt->pointSet[0][0], evt->pointSet[1][0] );
				printf("                %f,%f (%f, %f)\n", tg.params[4], tg.params[5], evt->pointSet[1][0], evt->pointSet[1][1] );
			}
			validEvent = true;
			break;
		case TG_ROTATE_START:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::RotateStart;
			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[0] * screenX / serverX; // anchorX
			evt->pointSet[0][1] = tg.params[1] * screenY / serverY; // anchorY
			evt->pointSet[1][0] = tg.params[2] * screenX / serverX; // rotFingerX
			evt->pointSet[1][1] = tg.params[3] * screenY / serverY; // rotFingerY
			evt->position = evt->pointSet[0];	// Point 0 is the center of rotation.
			if( debugText ){
				printf(" Touch rotate start at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->pointSet[0][0], evt->pointSet[1][0] );
				printf("                       %f,%f (%f, %f)\n", tg.params[2], tg.params[3], evt->pointSet[1][0], evt->pointSet[1][1] );
			}
			validEvent = true;
			break;
		case TG_ROTATE_END:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::RotateEnd;
			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[0] * screenX / serverX; // anchorX
			evt->pointSet[0][1] = tg.params[1] * screenY / serverY; // anchorY
			evt->pointSet[1][0] = tg.params[2] * screenX / serverX; // rotFingerX
			evt->pointSet[1][1] = tg.params[3] * screenY / serverY; // rotFingerY
			evt->position = evt->pointSet[0];	// Point 0 is the center of rotation.
			if( debugText ){
				printf(" Touch rotate end at %f,%f (%f, %f)\n", tg.params[0], tg.params[1], evt->pointSet[0][0], evt->pointSet[1][0] );
				printf("                     %f,%f (%f, %f)\n", tg.params[2], tg.params[3], evt->pointSet[1][0], evt->pointSet[1][1] );
			}
			validEvent = true;
			break;
		case TG_ROTATE_ANTICLOCK:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::Rotate;

			evt->rotation[0] = -tg.params[0] * Math::RadToDeg; // angle

			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[1] * screenX / specialDividerX; // anchorX
			evt->pointSet[0][1] = tg.params[2] * screenY / specialDividerY; // anchorY
			evt->pointSet[1][0] = tg.params[3] * screenX / specialDividerX; // rotFingerX
			evt->pointSet[1][1] = tg.params[4] * screenY / specialDividerY; // rotFingerY
			evt->position = evt->pointSet[0]; // Point 0 is the center of rotation.
			if( debugText ){
				printf(" Touch Rotate anti-clock at %f,%f (%f, %f)\n", tg.params[1], tg.params[2], evt->pointSet[0][0], evt->pointSet[1][0] );
				printf("                            %f,%f (%f, %f)\n", tg.params[3], tg.params[4], evt->pointSet[1][0], evt->pointSet[1][1] );
			}
			validEvent = true;
			break;
		case TG_ROTATE_CLOCK:
			evt = mysInstance->writeHead();
			evt->type  = InputEvent::Rotate;

			evt->rotation[0] = tg.params[0] * Math::RadToDeg; // angle

			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[1] * screenX / specialDividerX; // anchorX
			evt->pointSet[0][1] = tg.params[2] * screenY / specialDividerY; // anchorY
			evt->pointSet[1][0] = tg.params[3] * screenX / specialDividerX; // rotFingerX
			evt->pointSet[1][1] = tg.params[4] * screenY / specialDividerY; // rotFingerY
			evt->position = evt->pointSet[0]; // Point 0 is the center of rotation.
			if( debugText ){
				printf(" Touch Rotate clock at %f,%f (%f, %f)\n", tg.params[1], tg.params[2], evt->pointSet[0][0], evt->pointSet[1][0] );
				printf("                       %f,%f (%f, %f)\n", tg.params[3], tg.params[4], evt->pointSet[1][0], evt->pointSet[1][1] );
			}
			validEvent = true;
			break;
		default:
			break;
	}// switch		
	if( validEvent ){
		evt->serviceType = InputService::Touch;
		evt->sourceId = -1; // Gestures have no id
	}
	mysInstance->unlockEvents();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// here, just record the position of point,
//	you can do mouse map like "OnTG_Down" etc;
void PQService:: OnTouchPoint(const TouchPoint & tp)
{
	timeb tb;
	ftime( &tb );
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	
	int tEvent = tp.point_event;
	int xWidth = tp.dx;
	int yWidth = tp.dy;

	if(mysInstance && xWidth <= maxBlobSize && yWidth <= maxBlobSize)
	{
		mysInstance->lockEvents();

		InputEvent* evt = mysInstance->writeHead();
		switch(tp.point_event)
		{
			case TP_DOWN:
				evt->type = InputEvent::Down;
				touchID[tp.id] = nextID;
				if( nextID < maxTouches - 100 ){
					nextID++;
				} else {
					nextID = 0;
				}
				break;
			case TP_MOVE:
				evt->type = InputEvent::Move;
				break;
			case TP_UP:
				evt->type  = InputEvent::Up;
				break;
		}		
		evt->serviceType = InputService::Touch;
		if( serverX != 0 && serverY != 0 ){
			evt->position[0] = tp.x * (float)screenX / (float)serverX;
			evt->position[1] = tp.y * (float)screenY / (float)serverY;

			evt->numberOfPoints = 1;
			evt->pointSet[0][0] = xWidth * (float)screenX / (float)serverX;
			evt->pointSet[0][1] = yWidth * (float)screenY / (float)serverY;
		} else {
			evt->position[0] = tp.x;
			evt->position[1] = tp.y;

			evt->numberOfPoints = 1;
			evt->pointSet[0][0] = xWidth;
			evt->pointSet[0][1] = yWidth;
		}

		//printf(" Server %d,%d Screen %d, %d\n", serverX, serverY, screenX, screenY );
		//printf("      at %d,%d (%f, %f)\n", tp.x, tp.y, evt->position[0], evt->position[1] );
		evt->sourceId = touchID[tp.id];

		mysInstance->unlockEvents();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService::dispose() 
{
	mysInstance = NULL;
	DisconnectServer();
}
