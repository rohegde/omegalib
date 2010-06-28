/********************************************************************************************************************** 
* THE OMEGA LIB PROJECT
*---------------------------------------------------------------------------------------------------------------------
* Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
* Authors:										
*  [Author]									[Mail]
*---------------------------------------------------------------------------------------------------------------------
* [LICENSE NOTE]
*---------------------------------------------------------------------------------------------------------------------
* [SUMMARY OF FILE CONTENTS]
*********************************************************************************************************************/
#include "omega/osystem.h"
#include "omega/InputManager.h"
#include "pqlabs/PQMTClient.h"

//#include "input/Touches.h"
#include <vector>
#include <sys/timeb.h>

#ifndef __PQ_LAB_TOUCH__SERVICE_H__
#define __PQ_LAB_TOUCH__SERVICE_H__

using namespace PQ_SDK_MultiTouch;

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class PQService: public InputService
	{
	public:
		OMEGA_API virtual void Initialize();
		OMEGA_API virtual void Dispose();

		OMEGA_API void Initialize(  char* local_ip );
		OMEGA_API int Init();
	
	private:
		static PQService* myInstance;	
		static int maxBlobSize;
		char* server_ip;
		char mostRecentDataString[100];
		bool newDataFlag;
		int touchID[1000]; // Max IDs assigned before resetting
		static int maxTouches; // Should be same number as touchID array init
		static int nextID;

		void ClearDataString();

		//////////////////////call back functions///////////////////////
		// OnReceivePointFrame: function to handle when recieve touch point frame
		//	the unmoving touch point won't be sent from server. The new touch point with its pointevent is TP_DOWN
		//	and the leaving touch point with its pointevent will be always sent from server;
		static void OnReceivePointFrame(int frame_id,int time_stamp,int moving_point_count,const TouchPoint * moving_point_array, void * call_back_object);
		// OnReceivePointFrame: function to handle when recieve touch gesture
		static void OnReceiveGesture(const TouchGesture & ges, void * call_back_object);
		// OnServerBreak: function to handle when server break(disconnect or network error)
		static void OnServerBreak(void * param, void * call_back_object);
		// OnReceiveError: function to handle when some errors occur on the process of receiving touch datas.
		static void OnReceiveError(int err_code,void * call_back_object);
		//
		static void OnGetServerResolution(int x, int y, void * call_back_object);
		//////////////////////call back functions end ///////////////////////

		// functions to handle TouchGestures, attention the means of the params
		void InitFuncOnTG();
		
		// set the call back functions while reciving touch data;
		void SetFuncsOnReceiveProc();

		// OnTouchPoint: function to handle TouchPoint
		void OnTouchPoint(const TouchPoint & tp);
		// OnTouchGesture: function to handle TouchGesture
		void OnTouchGesture(const TouchGesture & tg);
		//

		//here use function pointer table to handle the different gesture type;
		typedef void (*PFuncOnTouchGesture)(const TouchGesture & tg,void * call_object);
		PFuncOnTouchGesture m_pf_on_tges[TG_TOUCH_END + 1];
		static void DefaultOnTG(const TouchGesture & tg,void * call_object); // just show the gesture
		static void OnTG_TouchStart(const TouchGesture & tg,void * call_object);
		static void OnTG_Down(const TouchGesture & tg,void * call_object);
		static void OnTG_Move(const TouchGesture & tg,void * call_object);
		static void OnTG_Up(const TouchGesture & tg,void * call_object);
		static void OnTG_SecondDown(const TouchGesture & tg,void * call_object);
		static void OnTG_SecondUp(const TouchGesture & tg,void * call_object);
		static void OnTG_SplitStart(const TouchGesture & tg,void * call_object);
		static void OnTG_SplitApart(const TouchGesture & tg,void * call_object);
		static void OnTG_SplitClose(const TouchGesture & tg,void * call_object);
		static void OnTG_SplitEnd(const TouchGesture & tg,void * call_object);
		static void OnTG_TouchEnd(const TouchGesture & tg,void * call_object);

	};

}; // namespace omega

#endif