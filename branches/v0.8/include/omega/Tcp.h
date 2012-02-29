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
#ifndef __TCP_H__
#define __TCP_H__

#include "omega/osystem.h"
#include "omega/TypeInfo.h"
#include "omega/glheaders.h"

#ifdef OMEGA_OS_WIN
	#define NOMINMAX
	#define BOOST_DATE_TIME_NO_LIB
	#define BOOST_REGEX_NO_LIB
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif
#include <asio.hpp>

using asio::ip::tcp;


namespace omega {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class TcpConnection: public OmegaObject
	{
	OMEGA_DECLARE_TYPE(TcpConnection)
	public:
		TcpConnection(asio::io_service& ioService);
		tcp::socket& getSocket() { return mySocket; }

		bool poll();
		void close();

		bool isOpen() { return myOpen; }

		//! Data IO
		//@{
		void write(const String& data);
		String readLine();
		//! Synchronously read character data until the specified delimiter is found or the buffer fills up.
		size_t readString(char* buffer, size_t size, char delimiter = '\0');
		//@}

		//! Connection event management
		//@{
		virtual void handleConnected();
		virtual void handleClosed();
		virtual void handleError();
		virtual void handleData();
		virtual void handleWrite(const asio::error_code& err, size_t size);
		//@}

	protected:

	protected:
		bool myOpen;
		tcp::socket mySocket;
		asio::streambuf myInputBuffer;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class TcpServer: public OmegaObject
	{
	OMEGA_DECLARE_TYPE(TcpServer)
	public:
		TcpServer();
		~TcpServer();
		virtual void initialize(int port);
		virtual void start();
		virtual void stop();
		virtual void poll();
		int getPort() { return myPort; }

	protected:
		virtual TcpConnection* createConnection();
		virtual void accept();
		virtual void handleAccept(TcpConnection* newConnection, const asio::error_code& error);

	protected:
		int myPort;
		bool myInitialized;
		bool myRunning;
		tcp::acceptor* myAcceptor;
		asio::io_service myIOService;
		List<TcpConnection*> myClients;
	};
}; // namespace omega

#endif