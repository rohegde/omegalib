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
#include "omega/osystem.h"
#include "omega/SystemManager.h"
#include "omicron/StringUtils.h"

namespace omega
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	GLEWContext* sGlewContext;

	////////////////////////////////////////////////////////////////////////////////////////////////
	GLEWContext* glewGetContext()
	{
		return sGlewContext;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void glewSetContext(const GLEWContext* context)
	{
		sGlewContext = (GLEWContext*)context;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void omain(ApplicationBase& app, const char* configFile, const char* logFile, DataSource* dataSource)
	{
		bool remote = false;
		String masterHostname;
		String configFilename;
		
		std::vector<std::string> args = StringUtils::split(configFile, "@");
		configFilename = args[0];
		if(args.size() == 2)
		{
			remote = true;
			masterHostname = args[1];
			
			String hostLogFilename = masterHostname + "-" + logFile;
			ologopen(hostLogFilename.c_str());
		}
		else
		{
			ologopen(logFile);
		}
		
		Config* cfg = new Config(configFilename);
		
		SystemManager* sys = SystemManager::instance();
		DataManager* dm = sys->getDataManager();
		// Add a default filesystem data source using current work dir.
		dm->addSource(new FilesystemDataSource("./"));

		// Add optional data source.
		if(dataSource != NULL)
		{
			dm->addSource(dataSource);
		}

		sys->setApplication(&app);
		if(remote)
		{
			sys->setupRemote(cfg, masterHostname);
		}
		else
		{
			sys->setup(cfg);
		}
		sys->initialize();
		sys->run();

		sys->cleanup();

		ologclose();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void olaunch(const String& command)
	{
		if( command.empty( )) return;

#ifdef OMEGA_OS_WIN
		STARTUPINFO         startupInfo;
		ZeroMemory(&startupInfo, sizeof(STARTUPINFO));

		PROCESS_INFORMATION procInfo;
		ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));

		const char*         cmdLine     = command.c_str();

		startupInfo.cb = sizeof( STARTUPINFO );
		const bool success = 
			CreateProcess( 0, LPSTR( cmdLine ), // program, command line
						   0, 0,                // process, thread attributes
						   FALSE,               // inherit handles
						   0,                   // creation flags
						   0,                   // environment
						   0,                   // current directory
						   &startupInfo,
						   &procInfo );

		//WaitForInputIdle( procInfo.hProcess, 1000 );
		CloseHandle( procInfo.hProcess );
		CloseHandle( procInfo.hThread );
#else
		std::vector<std::string> commandLine = StringUtils::split(command, " ");

		//signal( SIGCHLD, sigChildHandler );
		const int result = fork();
		switch( result )
		{
			case 0: // child
				break;

			case -1: // error
			default: // parent
				return;
		}

		// child
		const size_t  argc         = commandLine.size();
		char*         argv[argc+1];
		std::ostringstream stringStream;

		for( size_t i=0; i<argc; i++ )
		{
			argv[i] = (char*)commandLine[i].c_str();
			stringStream << commandLine[i] << " ";
		}

		argv[argc] = 0;

		int nTries = 10;
		while( nTries-- )
		{
			execvp( argv[0], argv );
			// EQWARN << "Error executing '" << argv[0] << "': " << sysError
				   // << std::endl;
			if( errno != ETXTBSY )
				break;
		}
#endif
	}
}
