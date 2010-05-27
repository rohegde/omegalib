//+---------------------------------------------------------------------------
//
//  PQLabs.
//
//  Copyright (c) PQLabs.  All rights reserved.
//
//  File:       SDK_OmegaTouchAPICode.cpp
//
//  Contents:   Implementation of SDK_OmegaTouchAPICode.h
//
//	Date:		2008-12-19
//
//----------------------------------------------------------------------------

#include "stdafx.h"
#include "OmegaTouchAPI.h"

#include <iostream>
#include <set>
#include <map>
#include <cassert>
#include <functional>
using namespace std;

namespace OmegaAPI
{

OmegaTouchAPI::OmegaTouchAPI()
{
	memset(m_pf_on_tges,0, sizeof(m_pf_on_tges));
	server_ip = "131.193.77.102";
	mostRecentDataString[0] = NULL; // Cleans up string
	newDataFlag = false;
	Init();
}

OmegaTouchAPI::OmegaTouchAPI( char* local_ip )
{
	memset(m_pf_on_tges,0, sizeof(m_pf_on_tges));
	server_ip = local_ip;
	mostRecentDataString[0] = NULL; // Cleans up string
	newDataFlag = false;
	Init();
}

OmegaTouchAPI::~OmegaTouchAPI()
{
	CloseHandle(listSem);
	DisconnectServer();
}
/////////////////////////// functions ///////////////////////////////////
int OmegaTouchAPI::Init( char* local_ip )
{
	server_ip = local_ip;
	return Init();
}

int OmegaTouchAPI::Init()
{
	int err_code = PQMTE_SUCESS;
	mostRecentTouch = new Touches();
	listSem = CreateSemaphore( NULL, 1, 1, NULL ); // security, initial count, max count, name
	if( listSem == NULL ){
		printf("Semaphore creation error: %d\n", GetLastError() );
	}

	// initialize the handle functions of gestures;
	InitFuncOnTG();
	// set the functions on server callback
	SetFuncsOnReceiveProc();
	// connect server
	cout << " connect to server..." << endl;
	if((err_code = ConnectServer(server_ip)) != PQMTE_SUCESS){
		cout << " connect server fail, socket error code:" << err_code << endl;
		return err_code;
	}
	// send request to server
	cout << " connect success, send request." << endl;
	TouchClientRequest tcq = {0};
	tcq.app_id = GetTrialAppID();
	tcq.type = RQST_RAWDATA_ALL | RQST_GESTURE_ALL;
	if((err_code = SendRequest(tcq)) != PQMTE_SUCESS){
		cout << " send request fail, error code:" << err_code << endl;
		return err_code;
	}
	//////////////you can set the move_threshold when the tcq.type is RQST_RAWDATA_INSIDE;
	//send threshold
	//int move_threshold = 1;// 1 pixel
	//if((err_code = SendThreshold(move_threshold) != PQMTE_SUCESS){
	//	cout << " send threadhold fail, error code:" << err_code << endl;
	//	return err_code;
	//}
	//
	////////////////////////
	//get server resolution
	if((err_code = GetServerResolution(OnGetServerResolution, NULL)) != PQMTE_SUCESS){
		cout << " get server resolution fail,error code:" << err_code << endl;
		return err_code;
	};
	//
	// start receiving
	cout << " send request success, start recv." << endl;
	return err_code;
}

void OmegaTouchAPI:: InitFuncOnTG()
{
	// initialize the call back functions of toucha gestures;
	m_pf_on_tges[TG_TOUCH_START] = &OmegaTouchAPI::OnTG_TouchStart;
	m_pf_on_tges[TG_DOWN] = &OmegaTouchAPI::OnTG_Down;
	m_pf_on_tges[TG_MOVE] = &OmegaTouchAPI::OnTG_Move;
	m_pf_on_tges[TG_UP] = &OmegaTouchAPI::OnTG_Up;

	m_pf_on_tges[TG_SECOND_DOWN] = &OmegaTouchAPI::OnTG_SecondDown;
	m_pf_on_tges[TG_SECOND_UP] = &OmegaTouchAPI::OnTG_SecondUp;

	m_pf_on_tges[TG_SPLIT_START] = &OmegaTouchAPI::OnTG_SplitStart;
	m_pf_on_tges[TG_SPLIT_APART] = &OmegaTouchAPI::OnTG_SplitApart;
	m_pf_on_tges[TG_SPLIT_CLOSE] = &OmegaTouchAPI::OnTG_SplitClose;
	m_pf_on_tges[TG_SPLIT_END] = &OmegaTouchAPI::OnTG_SplitEnd;

	m_pf_on_tges[TG_TOUCH_END] = &OmegaTouchAPI::OnTG_TouchEnd;
}
void OmegaTouchAPI::SetFuncsOnReceiveProc()
{
	PFuncOnReceivePointFrame old_rf_func = SetOnReceivePointFrame(&OmegaTouchAPI::OnReceivePointFrame,this);
	PFuncOnReceiveGesture old_rg_func = SetOnReceiveGesture(&OmegaTouchAPI::OnReceiveGesture,this);
	PFuncOnServerBreak old_svr_break = SetOnServerBreak(&OmegaTouchAPI::OnServerBreak,NULL);
	PFuncOnReceiveError old_rcv_err_func = SetOnReceiveError(&OmegaTouchAPI::OnReceiveError,NULL);
}

void OmegaTouchAPI:: OnReceivePointFrame(int frame_id, int time_stamp, int moving_point_count, const TouchPoint * moving_point_array, void * call_back_object)
{
	OmegaTouchAPI * omegaDesk = static_cast<OmegaTouchAPI*>(call_back_object);
	assert(omegaDesk != NULL);
	const char * tp_event[] = 
	{
		"down",
		"move",
		"up",
	};

	//cout << " frame_id:" << frame_id << " time:"  << time_stamp << " ms" << " moving point count:" << moving_point_count << endl;
	for(int i = 0; i < moving_point_count; ++ i){
		TouchPoint tp = moving_point_array[i];
		omegaDesk->OnTouchPoint(tp);
	}
}
void OmegaTouchAPI:: OnReceiveGesture(const TouchGesture & ges, void * call_back_object)
{
	OmegaTouchAPI * omegaDesk = static_cast<OmegaTouchAPI*>(call_back_object);
	assert(omegaDesk != NULL);
	omegaDesk->OnTouchGesture(ges);
}
void OmegaTouchAPI:: OnServerBreak(void * param, void * call_back_object)
{
	// when the server break, disconenct server;
	cout << "server break, disconnect here" << endl;
	DisconnectServer();
}
void OmegaTouchAPI::OnReceiveError(int err_code, void * call_back_object)
{
	switch(err_code)
	{
	case PQMTE_RCV_INVALIDATE_DATA:
		cout << " error: receive invalidate data." << endl;
		break;
	case PQMTE_SERVER_VERSION_OLD:
		cout << " error: the multi-touch server is old for this client, please update the multi-touch server." << endl;
		break;
	default:
		cout << " socket error, socket error code:" << err_code << endl;
	}
}
void OmegaTouchAPI:: OnGetServerResolution(int x, int y, void * call_back_object)
{
	cout << " server resolution:" << x << "," << y << endl;
}
// here, just record the position of point,
//	you can do mouse map like "OnTG_Down" etc;
void OmegaTouchAPI:: OnTouchPoint(const TouchPoint & tp)
{
	timeb tb;
	ftime( &tb );
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;

	Touches newTouch = Touches( tp.point_event, nCount, tp.x, tp.y, tp.dx, tp.dy, tp.id );
	touchList.push_back( newTouch );
	ReleaseSemaphore( listSem, 1, NULL ); // semaphore handle, increase count, not interested in prev count
	mostRecentTouch->updateTouch(newTouch);

	char buffer[5];

	ClearDataString();
	
	switch(tp.point_event)
	{
	case TP_DOWN:
		strcat(mostRecentDataString,"TP_DOWN ");
		break;
	case TP_MOVE:
		strcat(mostRecentDataString,"TP_MOVE ");
		break;
	case TP_UP:
		strcat(mostRecentDataString,"TP_UP ");
		break;
	}

	// Add ID to string
	itoa(tp.id,buffer,10); // int to char
	strcat(mostRecentDataString,buffer);

	strcat(mostRecentDataString," ");
	
	// Add xPos
	itoa(tp.x,buffer,10);
	strcat(mostRecentDataString,buffer);

	strcat(mostRecentDataString," ");
	
	// Add yPos
	itoa(tp.y,buffer,10);
	strcat(mostRecentDataString,buffer);

	strcat(mostRecentDataString," ");
	
	// Add xWidth
	itoa(tp.dx,buffer,10);
	strcat(mostRecentDataString,buffer);

	strcat(mostRecentDataString," ");
	
	// Add yWidth
	itoa(tp.dy,buffer,10);
	strcat(mostRecentDataString,buffer);
	newDataFlag = true;
	//printf("%s \n",mostRecentDataString);
}
void OmegaTouchAPI:: OnTouchGesture(const TouchGesture & tg)
{
	if(TG_NO_ACTION == tg.type)
		return ;
	
	assert(tg.type <= TG_TOUCH_END);
	DefaultOnTG(tg,this);
	PFuncOnTouchGesture pf = m_pf_on_tges[tg.type];
	if(NULL != pf){
		pf(tg,this);
	}
}
void OmegaTouchAPI:: OnTG_TouchStart(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_TOUCH_START);
	//cout << "  here, the touch start, initialize something." << endl;
}
void OmegaTouchAPI:: DefaultOnTG(const TouchGesture & tg,void * call_object) // just show the gesture
{
	//cout <<"ges,name:"<< GetGestureName(tg) << " type:" << tg.type << ",param size:" << tg.param_size << " ";
	//for(int i = 0; i < tg.param_size; ++ i)
	//	cout << tg.params[i] << " ";
	//cout << endl;
}
void OmegaTouchAPI:: OnTG_Down(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_DOWN && tg.param_size >= 2);
	//cout << "  the single finger touching at :( " 
	//	<< tg.params[0] << "," << tg.params[1] << " )" << endl;
}
void OmegaTouchAPI:: OnTG_Move(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_MOVE && tg.param_size >= 2);
	//cout << "  the single finger moving on the screen at :( " 
	//	<< tg.params[0] << "," << tg.params[1] << " )" << endl;
}
void OmegaTouchAPI:: OnTG_Up(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_UP && tg.param_size >= 2);
	//cout << " the single finger is leaving the screen at :( " 
	//	<< tg.params[0] << "," << tg.params[1] << " )" << endl;
}
//
void OmegaTouchAPI:: OnTG_SecondDown(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SECOND_DOWN && tg.param_size >= 4);
	//cout << "  the second finger touching at :( " 
	//	<< tg.params[0] << "," << tg.params[1] << " ),"
	//	<< " after the first finger touched at :( "
	//	<< tg.params[2] << "," << tg.params[3] << " )" << endl;
}
void OmegaTouchAPI:: OnTG_SecondUp(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SECOND_UP && tg.param_size >= 4);
	//cout << "  the second finger is leaving at :( " 
	//	<< tg.params[0] << "," << tg.params[1] << " ),"
	//	<< " while the first finger still anchored around :( "
	//	<< tg.params[2] << "," << tg.params[3] << " )" << endl;
}
//
void OmegaTouchAPI:: OnTG_SplitStart(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SPLIT_START && tg.param_size >= 4);
	//cout << "  the two fingers is splitting with one finger at: ( " 
	//	<< tg.params[0] << "," << tg.params[1] << " ),"
	//	<< " , the other at :( "
	//	<< tg.params[2] << "," << tg.params[3] << " )" << endl;
}

void OmegaTouchAPI:: OnTG_SplitApart(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SPLIT_APART && tg.param_size >= 1);
	//cout << "  the two fingers is splitting apart with there distance incresed by " 
	//	<< tg.params[0]
	//	<< " with a ratio :" << tg.params[1]
	//	<< endl;
}
void OmegaTouchAPI:: OnTG_SplitClose(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SPLIT_CLOSE && tg.param_size >= 1);
	//cout << "  the two fingers is splitting close with there distance decresed by " 
	//	<< tg.params[0]
	//	<< " with a ratio :" << tg.params[1]
	//	<< endl;
}
void OmegaTouchAPI:: OnTG_SplitEnd(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SPLIT_END);
	//cout << "  the two splitting fingers with one finger at: ( " 
	//	<< tg.params[0] << "," << tg.params[1] << " ),"
	//	<< " , the other at :( "
	//	<< tg.params[2] << "," << tg.params[3] << " )" 
	//	<< " will end" << endl;
}
// OnTG_TouchEnd: to clear what need to clear
void OmegaTouchAPI:: OnTG_TouchEnd(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_TOUCH_END);
	//cout << "  all the fingers is leaving and there is no fingers on the screen." << endl;
}
/////////////////////////// functions ///////////////////////////////////

void OmegaTouchAPI::ClearDataString(){
	mostRecentDataString[0] = NULL; // Cleans up string
}

char* OmegaTouchAPI::GetMostRecentDataString()
{
	newDataFlag = false;
	return mostRecentDataString;
}

Touches* OmegaTouchAPI::GetMostRecentTouch(){
	newDataFlag = false;
	return mostRecentTouch;
}

vector<Touches> OmegaTouchAPI::getTouchList(){
	vector<Touches> emptyList;
	dwWaitResult = WaitForSingleObject( listSem, 0L );
	switch( dwWaitResult ){
		case WAIT_OBJECT_0:
			//printf("Got list \n");
			return touchList;
		case WAIT_TIMEOUT:
			//printf("Refused list \n");
			return emptyList;
	}
}

bool OmegaTouchAPI::hasNewData()
{
	return newDataFlag;
}
}