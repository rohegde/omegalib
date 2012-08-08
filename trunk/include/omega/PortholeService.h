/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Donghi Daniele		d.donghi@gmail.com
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

#ifndef __PORTHOLE_SERVICE_H__
#define __PORTHOLE_SERVICE_H__

#define PORT 4080

#include "osystem.h"
#include <string.h>
#include "websockets/libwebsockets.h"
#include "omega/Engine.h"

using namespace std;
using namespace omicron;

namespace omega {

	// Path to resources, such as html files
	static string DATA_PATH; 

	class OMEGA_API ServerThread: public Thread{

	public:

		// Constructor
		ServerThread();

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

		// Base64 encode/decode functions
		static string base64_encode(unsigned char const* , unsigned int len);
		static string base64_decode(string const& s);

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
	class OMEGA_API PortholeService: public Service{

	public:

		//! Allocator function (will be used to register the service inside SystemManager)
		static PortholeService* New() { return new PortholeService(); }

		// Default constructor and destructor
		PortholeService();
		~PortholeService();

		// Setup and poll
		virtual void setup(omicron::Setting& settings);
		void start(); // Start the server and listen to port
		virtual void poll();

		// Server instance. It will manage the incoming connections
		//thread server_thread;
		ServerThread* portholeServer;

	};

};

#endif
