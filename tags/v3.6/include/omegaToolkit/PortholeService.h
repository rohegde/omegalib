/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Donghi Daniele			d.donghi@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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

#ifndef __PORTHOLE_SERVICE_H__
#define __PORTHOLE_SERVICE_H__

///////////////////////////////////////////////////////////////////////////////////////////////////
// Testing variables
//#define PORTHOLE_TEST_RTT
//#define PORTHOLE_TEST_DIM
//#define PORTHOLE_TEST_TIME_COMPRESSION
//#define PORTHOLE_TEST_TIME_BASE64
//#define PORTHOLE_TEST_TIME_WEBSOCKET 
//#define PORTHOLE_TEST_TIME_GLOBAL

#include "omegaToolkitConfig.h"
#include "websockets/libwebsockets.h"
#include "omega/Engine.h"
#include "omicron/xml/tinyxml.h"
#include "PortholeGUI.h"

using namespace std;
using namespace omicron;

#define ZOOM_STEP 0.025

namespace omega {

	//! Path to resources, such as html files
	static String DATA_PATH; 

	//! CSS path
	static String css_path;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Implements, in a separate thread, the HTTP server for Porthole Service
	class OTK_API ServerThread: public Thread{

	public:

		// Constructor
		ServerThread();

		// Destructor
		~ServerThread();

		// Set port
		void setPort(int portNumber); 

		// Set funtions binder
		void setFunctionsBinder(PortholeFunctionsBinder* binder); 

		void setXMLfile(char* xmlPath);

		// Set funtions binder
		void setCSSPath(char* cssPath); 

		// Thread process
		virtual void threadProc();

		// Handshake manager
		static void dump_handshake_info(struct lws_tokens *lwst);

		// HTTP callback
		static int callback_http(struct libwebsocket_context *context,
			struct libwebsocket *wsi,
			enum libwebsocket_callback_reasons reason, void *user,
							   void *in, size_t len);

		// websocket callback
		static int callback_websocket(struct libwebsocket_context *context,
			struct libwebsocket *wsi,
			enum libwebsocket_callback_reasons reason,
					       void *user, void *in, size_t len);

	private:

		// Server params
		int port;
		struct libwebsocket_context *context; 
		int opts;
		int n;
		unsigned int oldus;
		char* minterface;

		// SSL vars - NOT TESTED
		int use_ssl;
		const char* cert_path;
		const char* key_path;

	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Implements an interface to handled device applications
	class OTK_API PortholeService: public Service
	{
	public:
		// Wrapper for creation and start of the service
		static PortholeService* createAndInitialize(int port, const String& xmlPath, const String& cssPath);

		//! Allocator function (will be used to register the service inside SystemManager)
		//static PortholeService* New() { return new PortholeService(); }

		~PortholeService();

		// Setup and poll
		virtual void setup(omicron::Setting& settings);
		void start(int port, char* xmlPath, char* cssPath); // Start the server and listen to port
		virtual void poll();
		PortholeFunctionsBinder* getFunctionsBinder() { return myBinder; }

		// Server instance. It will manage the incoming connections
		//thread server_thread;
		ServerThread* portholeServer;

	private:
		// Default constructor and destructor
		PortholeService();
		PortholeFunctionsBinder* myBinder;
	};

};

#endif
