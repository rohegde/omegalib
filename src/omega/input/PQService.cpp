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
#include "input/PQService.h"
#include "SystemManager.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PQService* PQService::myInstance = NULL;
int PQService::maxBlobSize = 15;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService::Initialize( ) 
{
	myInstance = this;
	
	memset(m_pf_on_tges,0, sizeof(m_pf_on_tges));
	server_ip = "131.193.77.116";
	mostRecentDataString[0] = NULL; // Cleans up string
	newDataFlag = false;
	Init();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService::Initialize(  char* local_ip ) 
{
	myInstance = this;
	
	memset(m_pf_on_tges,0, sizeof(m_pf_on_tges));
	server_ip = local_ip;
	mostRecentDataString[0] = NULL; // Cleans up string
	newDataFlag = false;
	Init();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int PQService::Init()
{
	int err_code = PQMTE_SUCESS;	
	// initialize the handle functions of gestures;
	InitFuncOnTG();
	// set the functions on server callback
	SetFuncsOnReceiveProc();
	// connect server
	printf(" connect to server...\n");
	if((err_code = ConnectServer(server_ip)) != PQMTE_SUCESS){
		printf("connect server fail, socket error code: %d\n", err_code);
		return err_code;
	}
	// send request to server
	printf("connect success, send request.");
	TouchClientRequest tcq = {0};
	tcq.app_id = GetTrialAppID();
	tcq.type = RQST_RAWDATA_ALL | RQST_GESTURE_ALL;
	if((err_code = SendRequest(tcq)) != PQMTE_SUCESS){
		printf(" send request fail, error code: %d\n", err_code);
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
		printf(" get server resolution fail,error code: %d\n", err_code);
		return err_code;
	};
	//
	// start receiving
	printf(" send request success, start recv.\n");
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
	PFuncOnReceivePointFrame old_rf_func = SetOnReceivePointFrame(&PQService::OnReceivePointFrame,this);
	PFuncOnReceiveGesture old_rg_func = SetOnReceiveGesture(&PQService::OnReceiveGesture,this);
	PFuncOnServerBreak old_svr_break = SetOnServerBreak(&PQService::OnServerBreak,NULL);
	PFuncOnReceiveError old_rcv_err_func = SetOnReceiveError(&PQService::OnReceiveError,NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnReceivePointFrame(int frame_id, int time_stamp, int moving_point_count, const TouchPoint * moving_point_array, void * call_back_object)
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
void PQService:: OnReceiveGesture(const TouchGesture & ges, void * call_back_object)
{
	PQService * omegaDesk = static_cast<PQService*>(call_back_object);
	assert(omegaDesk != NULL);
	omegaDesk->OnTouchGesture(ges);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnServerBreak(void * param, void * call_back_object)
{
	// when the server break, disconenct server;
	printf("server break, disconnect here\n");;
	DisconnectServer();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService::OnReceiveError(int err_code, void * call_back_object)
{
	switch(err_code)
	{
	case PQMTE_RCV_INVALIDATE_DATA:
		printf(" error: receive invalidate data.\n");;
		break;
	case PQMTE_SERVER_VERSION_OLD:
		printf(" error: the multi-touch server is old for this client, please update the multi-touch server.\n");;
		break;
	default:
		printf(" socket error, socket error code:%d\n", err_code);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService:: OnGetServerResolution(int x, int y, void * call_back_object)
{
	printf(" server resolution: %d , %d \n",x ,y );
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

	if(myInstance && xWidth <= maxBlobSize && yWidth <= maxBlobSize)
	{
		myInstance->LockEvents();

		InputEvent* evt = myInstance->WriteHead();
		switch(tp.point_event)
		{
			case TP_DOWN:
				evt->type = InputEvent::Down;
				break;
			case TP_MOVE:
				evt->type = InputEvent::Move;
				break;
			case TP_UP:
				evt->type  = InputEvent::Up;
				break;
		}		
		evt->serviceId = InputService::Touch;
		evt->x = tp.x;
		evt->y = tp.y;
		evt->id = tp.id;

		myInstance->UnlockEvents();
		
		newDataFlag = true;
	}

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
	_itoa(tp.id,buffer,10); // int to char
	strcat(mostRecentDataString,buffer);

	strcat(mostRecentDataString," ");
	
	// Add xPos
	_itoa(tp.x,buffer,10);
	strcat(mostRecentDataString,buffer);

	strcat(mostRecentDataString," ");
	
	// Add yPos
	_itoa(tp.y,buffer,10);
	strcat(mostRecentDataString,buffer);

	strcat(mostRecentDataString," ");

	// Add xWidth
	_itoa(xWidth,buffer,10);
	strcat(mostRecentDataString,buffer);

	strcat(mostRecentDataString," ");
	
	// Add yWidth
	_itoa(yWidth,buffer,10);
	strcat(mostRecentDataString,buffer);

	if( newDataFlag )
		printf("%s \n",mostRecentDataString);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService::ClearDataString(){
	mostRecentDataString[0] = NULL; // Cleans up string
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PQService::Dispose() 
{
	myInstance = NULL;
	DisconnectServer();
}
