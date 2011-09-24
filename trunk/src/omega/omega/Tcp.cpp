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
#include "omega/Tcp.h"

#include <boost/bind.hpp>

using namespace omega;

OMEGA_DEFINE_TYPE(TcpConnection, OmegaObject)
OMEGA_DEFINE_TYPE(TcpServer, OmegaObject)

///////////////////////////////////////////////////////////////////////////////////////////////////
TcpServer::TcpServer():
	myRunning(false),
	myInitialized(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpServer::initialize(int port)
{
	if(!myInitialized)
	{
		myPort = port;
		myInitialized = true;
		myAcceptor = new tcp::acceptor(myIOService, tcp::endpoint(tcp::v4(), port));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TcpServer::~TcpServer()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpServer::start()
{
	myRunning = true;
	accept();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpServer::stop()
{
	myRunning = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpServer::poll()
{
	if(myRunning)
	{
		myIOService.poll();
		foreach(TcpConnection* c, myClients)
		{
			c->poll();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpServer::accept()
{
	TcpConnection* conn = createConnection();
	myAcceptor->async_accept(conn->getSocket(), boost::bind(&TcpServer::handleAccept, this, conn, asio::placeholders::error));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpServer::handleAccept(TcpConnection* newConnection, const asio::error_code& error)
{
	if(!error)
	{
		newConnection->handleConnected();
		accept();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TcpConnection* TcpServer::createConnection()
{
	TcpConnection* conn = new TcpConnection(myIOService);
	myClients.push_back(conn);
	return conn;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TcpConnection::TcpConnection(asio::io_service& ioService):
	mySocket(ioService)
{}
	
///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::poll()
{
	if(mySocket.available() != 0)
	{
		handleData();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::write(const String& data)
{
	// TODO: make sure data is not invalidated before transmission ends. On request, create copies
	// of data and keep a list of pending operations & buffers.
	asio::async_write(mySocket, asio::buffer(data),
		boost::bind(
			&TcpConnection::handleWrite, this, 
			asio::placeholders::error, asio::placeholders::bytes_transferred));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
String TcpConnection::readLine()
{
	asio::error_code error;
	size_t size = asio::read_until(mySocket, myInputBuffer, '\n', error);
	if(!error)
	{
		char* buf = new char[size];
		myInputBuffer.sgetn(buf, size);
		String str(buf);
		return str;
	}
	return "";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::handleConnected()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::handleClosed()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::handleError()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::handleWrite(const asio::error_code& err, size_t size)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::handleData()
{
}

