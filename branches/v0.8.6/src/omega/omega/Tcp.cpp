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
#include "omega/StringUtils.h"

#include <boost/bind.hpp>

using namespace omega;

OMEGA_DEFINE_TYPE(TcpConnection, OmegaObject)
OMEGA_DEFINE_TYPE(TcpServer, OmegaObject)

///////////////////////////////////////////////////////////////////////////////////////////////////
TcpServer::TcpServer():
	myRunning(false),
	myInitialized(false),
	myConnectionCounter(0)
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
		Queue<TcpConnection*> closedConnections;

		myIOService.poll();
		foreach(TcpConnection* c, myClients)
		{
			if(!c->poll()) closedConnections.push(c);
		}

		while(closedConnections.size() != 0)
		{
			myClients.remove(closedConnections.front());
			closedConnections.pop();
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
	TcpConnection* conn = new TcpConnection(ConnectionInfo(myIOService, myConnectionCounter++));
	myClients.push_back(conn);
	return conn;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TcpConnection* TcpServer::getConnection(int id)
{
	foreach(TcpConnection* c, myClients)
	{
		if(c->getConnectionInfo().id == id) return c;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TcpConnection::TcpConnection(ConnectionInfo ci):
	myConnectionInfo(ci),
	mySocket(ci.ioService),
	myOpen(true)
{}
	
///////////////////////////////////////////////////////////////////////////////////////////////////
bool TcpConnection::poll()
{
	if(!myOpen)
	{
		handleClosed();
		return false;
	} 

	while(mySocket.is_open() && mySocket.available() != 0)
	{
		handleData();
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::close()
{
	if(myOpen)
	{
		mySocket.close();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::write(const String& data)
{
	asio::error_code error;
	asio::write(mySocket, asio::buffer(data), error);
	if(error)
	{
		handleError(error);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::write(void* data, size_t size)
{
	asio::error_code error;
	asio::write(mySocket, asio::buffer(data, size), error);
	if(error)
	{
		handleError(error);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
size_t TcpConnection::readUntil(char* buffer, size_t size, char delimiter)
{
		asio::error_code error;
		size_t bufsize = asio::read_until(mySocket, myInputBuffer, delimiter, error);
		if(!error)
		{
			if(bufsize > size - 1) bufsize = size;
			myInputBuffer.sgetn((char*)buffer, bufsize);
			buffer[bufsize] = '\0';
			return bufsize;
		}
		else
		{
			handleError(error);
		}
		return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
size_t TcpConnection::read(byte* buffer, size_t size)
{
		asio::error_code error;
		size_t bufsize = asio::read(mySocket, myInputBuffer, asio::transfer_exactly(size), error);
		if(!error)
		{
			return size;
		}
		else
		{
			handleError(error);
		}
		return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//String TcpConnection::readLine()
//{
//	asio::error_code error;
//	size_t size = asio::read_until(mySocket, myInputBuffer, '\n', error);
//	if(!error)
//	{
//		char* buf = new char[size];
//		myInputBuffer.sgetn(buf, size);
//		buf[size] = '\0';
//		String str(buf);
//		delete buf;
//		return str;
//	}
//	return "";
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::handleConnected()
{
	myOpen = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::handleClosed()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::handleError(const ConnectionError& err)
{
	ofwarn("TcpConnection: %1%", %err.message());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TcpConnection::handleData()
{
}

