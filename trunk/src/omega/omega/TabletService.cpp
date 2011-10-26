/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#include "omega/SystemManager.h"
#include "omega/TabletService.h"
#include "omega/StringUtils.h"
#include "omega/Tcp.h"

#ifdef OMEGA_USE_DISPLAY
#include "omega/DisplaySystem.h"
#endif

using namespace omega;

namespace omega {
///////////////////////////////////////////////////////////////////////////////////////////////////
class TabletConnection: public TcpConnection
{
public:
	TabletConnection(asio::io_service& ioService, int id, TabletService* service): 
		TcpConnection(ioService),
			myId(id), myService(service)
	{
        ltClick = false;
        rtClick = false;
        anchor[0] = -1;
        anchor[1] = -1;
	}

	virtual void handleData()
	{
		// Read length.
//		readString(myBuffer, BufferSize, ':');
//		int length = atoi(myBuffer);

		// Read service id.
		readString(myBuffer, BufferSize, ':');
		int serviceId = atoi(myBuffer);

		// Read event type.
		readString(myBuffer, BufferSize, ':');
		int eventType = atoi(myBuffer);

        float pt1x , pt1y , pt2x , pt2y;

        if ( eventType == Event::RotateEnd )
        {
            rtClick = false;
            genSimpleEvent( Event::Up , Service::Pointer , 0.0 , 0.0 );
//            omsg("-----ROTATION DONE-----");
        }

		else if(eventType == Event::Move || eventType == Event::Up || eventType == Event::Down )
		{
			// Read x.
			readString(myBuffer, BufferSize, ':');
			pt1x = atof(myBuffer);

			// Read y.
			readString(myBuffer, BufferSize, ':');
			pt1y = atof(myBuffer);
            
            switch (eventType) 
            {

                case Event::Move:
                    if ( ltClick == true )  //This is not the first touch down
                    {
                        float tolerance = .1;
                        if( withinAnchor( pt1x , pt1y , tolerance) ) //if ess. the same pt.
                        {
                            genSimpleEvent( Event::Move , Service::Pointer , pt1x , pt1y );
//                            ofmsg("move @ %1% , %2% " , %pt1x %pt1y);
                            anchor[0] = pt1x;
                            anchor[1] = pt1y;
                        }
                        else // if not the same point
                        {
                            return; //do nothing cause this is a second touch
                            //may turn into a gesture 
                        }
                    }
                    else                    //This is the first touch down
                    {
                        
                    }
                    break;
                case Event::Up:
                    ltClick = false;
                    rtClick = false;
                    genSimpleEvent( Event::Up , Service::Pointer , pt1x , pt1y );
//                    ofmsg("-----UP @ %1% , %2% -----" , %pt1x %pt1y);
                    break;
            
                case Event::Down:
                    if ( ltClick == true )  //This is not the first touch down
                    {
                        return;//Do nothing.
                    }
                    else                    //This is the first touch down
                    {
                        ltClick = true;                        
                        genSimpleEvent( Event::Down , Service::Pointer , pt1x , pt1y );
//                        ofmsg("******DOWN @ %1% , %2% ******" , %pt1x %pt1y);
                        anchor[0] = pt1x;
                        anchor[1] = pt1y;                        
                        
                    }
                    break;

                default:
                    break;
            }
        }
		else if(eventType == Event::Zoom || eventType == Event::Rotate )
		{
            // Read point 1 x.
			readString(myBuffer, BufferSize, ':');
            pt1x = atof(myBuffer);
            
			// Read point 1 y.
			readString(myBuffer, BufferSize, ':');
			pt1y = atof(myBuffer);

            // Read point 2 x.
			readString(myBuffer, BufferSize, ':');
			pt2x = atof(myBuffer);
            
			// Read point 2 y.
			readString(myBuffer, BufferSize, ':');
			pt2y = atof(myBuffer);

            readString(myBuffer, BufferSize, ':');
            float param = atof(myBuffer);

            switch (eventType)
            {
                case Event::Rotate: // param = angle
                    if( rtClick == false )  //This is the first rotate call so send a down
                    {
//                        omsg("***** ROTATION START *****");
                        rtClick = true;
                        anchor[0] = pt1x;
                        anchor[1] = pt1y;                        
                        genSimpleEvent( Event::Down , Service::Pointer , (pt1x + pt2x) * 0.5 , (pt1y + pt2y) * 0.5 );
                    }
                    //The rest can be simple moves with average out the rotation pts
                    genSimpleEvent( Event::Move , Service::Pointer , (anchor[0] + pt2x) * 0.5 , (anchor[1] + pt2y) * 0.5 );                    
//                    omsg("ROTATE:");
//                    ofmsg("\tPoint 1 @      %1% , %2% " , %pt1x %pt1y);
//                    ofmsg("\tPoint 2 @      %1% , %2% " , %pt2x %pt2y);
//                    ofmsg("\tAngle in rad : %1%" , %param );            
                    break;
                    
                case Event::Zoom:   // param = scale
                    if( rtClick == false )  //This is the first rotate call so send a down
                    {
                        //                        omsg("***** ROTATION START *****");
                        rtClick = true;
                        anchor[0] = pt1x;
                        anchor[1] = pt1y;                        
                        genSimpleEvent( Event::Down , Service::Pointer , (pt1x + pt2x) * 0.5 , (pt1y + pt2y) * 0.5 );
                    }
                    //The rest can be simple moves with average out the rotation pts
                    genSimpleEvent( Event::Move , Service::Pointer , (anchor[0] + pt2x) * 0.5 , (anchor[1] + pt2y) * 0.5 );                    
//                    omsg("ZOOM:");
//                    ofmsg("\tPoint 1 @ %1% , %2% " , %pt1x %pt1y);
//                    ofmsg("\tPoint 2 @ %1% , %2% " , %pt2x %pt2y);
//                    ofmsg("\tScale   : %1%" , %param );            
                    break;
                    
                default:
                    break;
            }             
            
           

        }
		else
		{
			// Read until the end of the message
			readString(myBuffer, BufferSize, '|');
		}
	}

    bool withinAnchor( float x , float y , float tolerance )
    {
        if( x + tolerance > anchor[0] && x - tolerance < anchor[0] &&  
           y + tolerance > anchor[1] && y - tolerance < anchor[1]) return true;
        else return false;
    }

    void genSimpleEvent( Event::Type evtType ,Service::ServiceType servType , float x , float y)
    {
        
        Event::Flags myFlag;
        
        if( ltClick ) myFlag = Event::Left;
        if( rtClick ) myFlag = Event::Right;
        
#ifdef OMEGA_USE_DISPLAY
         DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
         Vector2i canvasSize = ds->getCanvasSize();
         
         myTouchPosition[0] = x * canvasSize[0];
         myTouchPosition[1] = y * canvasSize[1];
         
         myService->lockEvents();
         Event* evt = myService->writeHead();
         evt->reset(evtType, servType, myId);
         evt->setPosition(myTouchPosition[0], myTouchPosition[1]);
         evt->setFlags(myFlag);
         
         Ray ray = ds->getViewRay(myTouchPosition);
         evt->setExtraDataType(Event::ExtraDataVector3Array);
         evt->setExtraDataVector3(0, ray.getOrigin());
         evt->setExtraDataVector3(1, ray.getDirection());
         
         myService->unlockEvents();

#else
        myService->lockEvents();
        Event* evt = myService->writeHead();
        evt->reset(evtType, servType, myId);
        evt->setPosition( x , y );
        evt->setFlags(myFlag);
        
        myService->unlockEvents();
#endif
    }

    
	virtual void handleClosed()
	{
		ofmsg("Connection closed (id=%1%)", %myId);
	}

private:
	static const int BufferSize = 1024;
	char myBuffer[BufferSize];

	TabletService* myService;
	int myId;
	Vector2i myTouchPosition;
    Vector2f anchor;
    bool ltClick;
    bool rtClick;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class TabletServer: public TcpServer
{
public:
	TabletServer(TabletService* service):
		myPointerCounter(0),
		myService(service)
		{}

	virtual TcpConnection* createConnection()
	{
		ofmsg("New tablet connection (id=%1%)", %myPointerCounter);
		TabletConnection* conn = new TabletConnection(myIOService, ++myPointerCounter, myService);
	    myClients.push_back(conn);
	    return conn;
	}

private:
	int myPointerCounter;
	TabletService* myService;
};
};

///////////////////////////////////////////////////////////////////////////////////////////////////
TabletService::TabletService()
{
	myServer = new TabletServer(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TabletService::~TabletService()
{
	delete myServer;
	myServer = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletService::setup(Setting& settings)
{
	int port = 3490;
	if(settings.exists("port"))
	{
		port = settings["port"];
	}
	myServer->initialize(port);
	myServer->start();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TabletService::poll() 
{
	myServer->poll();
}
