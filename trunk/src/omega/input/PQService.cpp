/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Arthur Nishimoto								anishimoto42@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * PQService method definitions. See PQService.h for more details.
 *********************************************************************************************************************/
#include "omega/input/PQService.h"
#include "omega/SystemManager.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PQService* PQService::mysInstance = NULL;
int PQService::maxBlobSize = 1000;
int PQService::maxTouches = 1000; // Number of IDs assigned before resetting. Should match touchID array initialization

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
	printf("PQService: connect success, send request.");
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
	printf("PQService: server resolution: %d , %d \n",x ,y );
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

	InputEvent* evt = mysInstance->writeHead();
	switch(tg.type)
	{
		case TG_DOWN:
			evt->type = InputEvent::Down;
			evt->position[0] = tg.params[0];
			evt->position[1] = tg.params[1];
			break;
		case TG_MOVE:
			evt->type = InputEvent::Move;
			evt->position[0] = tg.params[0];
			evt->position[1] = tg.params[1];
			break;
		case TG_UP:
			evt->type  = InputEvent::Up;
			evt->position[0] = tg.params[0];
			evt->position[1] = tg.params[1];
			break;
		case TG_CLICK:
			evt->type  = InputEvent::Click;
			evt->position[0] = tg.params[0];
			evt->position[1] = tg.params[1];
			break;
		case TG_DB_CLICK:
			evt->type  = InputEvent::DoubleClick;
			evt->position[0] = tg.params[0];
			evt->position[1] = tg.params[1];
			break;
		case TG_MOVE_RIGHT:
			evt->type  = InputEvent::MoveRight;
			evt->position[0] = tg.params[0];
			evt->position[1] = tg.params[1];
			break;
		case TG_MOVE_UP:
			evt->type  = InputEvent::MoveUp;
			evt->position[0] = tg.params[0];
			evt->position[1] = tg.params[1];
			break;
		case TG_MOVE_LEFT:
			evt->type  = InputEvent::MoveLeft;
			evt->position[0] = tg.params[0];
			evt->position[1] = tg.params[1];
			break;
		case TG_MOVE_DOWN:
			evt->type  = InputEvent::MoveDown;
			evt->position[0] = tg.params[0];
			evt->position[1] = tg.params[1];
			break;
		case TG_SPLIT_START:
			evt->type  = InputEvent::SplitStart;

			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[0];
			evt->pointSet[0][1] = tg.params[1];
			evt->pointSet[1][0] = tg.params[2];
			evt->pointSet[1][1] = tg.params[3];

			evt->position = (evt->pointSet[0] + evt->pointSet[1]) / 2;
			break;
		case TG_SPLIT_END:
			evt->type  = InputEvent::SplitEnd;

			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[0];
			evt->pointSet[0][1] = tg.params[1];
			evt->pointSet[1][0] = tg.params[2];
			evt->pointSet[1][1] = tg.params[3];

			evt->position = (evt->pointSet[0] + evt->pointSet[1]) / 2;
			break;
		case TG_SPLIT_APART:
		case TG_SPLIT_CLOSE:
			evt->type  = InputEvent::Split;
			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[2];
			evt->pointSet[0][1] = tg.params[3];
			evt->pointSet[1][0] = tg.params[4];
			evt->pointSet[1][1] = tg.params[5];

			evt->position = (evt->pointSet[0] + evt->pointSet[1]) / 2;

			evt->value[0] = tg.params[0];
			evt->value[1] = tg.params[1];
			break;
		case TG_ROTATE_START:
			evt->type  = InputEvent::RotateStart;
			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[0];
			evt->pointSet[0][1] = tg.params[1];
			evt->pointSet[1][0] = tg.params[2];
			evt->pointSet[1][1] = tg.params[3];
			evt->position = (evt->pointSet[0] + evt->pointSet[1]) / 2;
			break;
		case TG_ROTATE_END:
			evt->type  = InputEvent::RotateEnd;
			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[0];
			evt->pointSet[0][1] = tg.params[1];
			evt->pointSet[1][0] = tg.params[2];
			evt->pointSet[1][1] = tg.params[3];
			evt->position = (evt->pointSet[0] + evt->pointSet[1]) / 2;
			break;
		case TG_ROTATE_ANTICLOCK:
		case TG_ROTATE_CLOCK:
			evt->type  = InputEvent::Rotate;
			evt->rotation[0] = tg.params[0];
			evt->numberOfPoints = 2;
			evt->pointSet[0][0] = tg.params[1];
			evt->pointSet[0][1] = tg.params[2];
			evt->pointSet[1][0] = tg.params[3];
			evt->pointSet[1][1] = tg.params[4];
			evt->position = (evt->pointSet[0] + evt->pointSet[1]) / 2;
			break;
	}// switch		
	evt->serviceType = InputService::Touch;
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
		evt->position[0] = tp.x;
		evt->position[1] = tp.y;
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
