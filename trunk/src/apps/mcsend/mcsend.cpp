/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010-2013							Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010- 2012, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------------------------------------------------
 *	mcsend
 *		Sends a single command to a running omegalib application, using the Mission Control protocol
 *********************************************************************************************************************/
#include <omega.h>
#include "omicron/Tcp.h"

using namespace omega;
using namespace omicron;

class MCSender: public TcpClientConnection
{
public:
	MCSender(const ConnectionInfo& info): TcpClientConnection(info), done(false)
	{}

	virtual void handleConnected()
	{
		const char* header = "scmd";
		int size = command.size();
		const char* data = command.c_str();

		printf("Connected: sending command %s\n", data);

		write((void*)header, 4);
		write(&size, sizeof(int32_t));
		write((void*)data, command.size());
		done = true;
	}

	virtual void handleError(const ConnectionError& err)
	{
		TcpClientConnection::handleError(err);
		done = true;
	}

	String command;
	bool done;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	String host = "127.0.0.1";
	int port = omega::MissionControlServer::DefaultPort;
	String command;

	for(int i = 1; i < argc; i++)
	{
		if(!strcmp(argv[i], "-h"))
		{
			i++;
			host = argv[i];
		}
		else if(!strcmp(argv[i], "-p"))
		{
			i++;
			port = boost::lexical_cast<int>(argv[i]);
		}
		else
		{
			//command += " ";
			command = argv[i];
		}
	}

	asio::io_service ioService;
	Ref<MCSender> ssender = new MCSender(ConnectionInfo(ioService));
	ssender->open(host, port);
	ssender->command = command;
	while(!ssender->done)
	{
		ioService.run_one();
		omega::osleep(500);
	}
}
