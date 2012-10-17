/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/ApplicationBase.h"
#include "omega/SystemManager.h"
#include "omega/DisplaySystem.h"
#include "omicron/StringUtils.h"

#include <iostream>

#ifdef WIN32
#include <windows.h> // needed for Sleep 
#else
#include <unistd.h>
#include<sys/wait.h>
#define Sleep(x) usleep((x)*1000)
#endif

// TODO: move thi to osystem (ogetcwd)
#ifdef OMEGA_OS_WIN
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

namespace omega
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	libconfig::ArgumentHelper sArgs;
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
	OMEGA_API libconfig::ArgumentHelper& oargs()
	{
		return sArgs;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	extern "C" void abortHandler(int signal_number)
	{
		// Just exit.
		exit(-1);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	int omain(omega::ApplicationBase& app, int argc, char** argv)
	{
		// register the abort handler.
		signal(SIGABRT, &abortHandler);

#ifdef OMEGA_ENABLE_VLD
		// Mark everything before this point as already reported to avoid reporting static global objects as leaks.
		// This makes the report less precise but gets rid of a lot of noise.
		VLDMarkAllLeaksAsReported();
#endif
		{
			bool remote = false;
			String masterHostname;
			String configFilename = ostr("%1%.cfg", %app.getName());
#ifdef OMEGA_APPROOT_DIRECTORY
			String dataPath = OMEGA_APPROOT_DIRECTORY;
#else
			String dataPath = "";
#endif
			String logFilename = ostr("%1%.log", %app.getName());

			bool kill = false;
			bool help = false;

			bool logRemoteNodes = false;

			sArgs.newNamedString(
				'c',
				"config", 
				ostr("configuration file to use with this application (default: %1% or default.cfg if the previous is not found)", %configFilename).c_str(), "",
				configFilename);

			sArgs.newFlag(
				'K',
				"kill",
				"Don't run the application, only run the nodeKiller command on all nodes in a clustered configuration",
				kill);

			sArgs.newFlag(
				'?',
				"help",
				"Prints this application help screen",
				help);

			sArgs.newFlag(
				'r',
				"log-remote",
				"generate log for remote nodes",
				logRemoteNodes);
				
			sArgs.newNamedString(
				'D',
				"data",
				"Data path for this application", "",
				dataPath);
		
			sArgs.newNamedString(
				'L',
				"log",
				ostr("log file to use with this application (default: %1%)", %logFilename).c_str(), "",
				logFilename);

			sArgs.setName(app.getName());
			sArgs.setAuthor("Alessandro Febretti");
			sArgs.setDescription("An omegalib application");
			sArgs.setVersion(OMEGA_VERSION);
			
			// If argument processing fails, exit immediately.
			if(!sArgs.process(argc, argv))
			{
				return -1;
			}

			if(help)
			{
				sArgs.writeUsage(std::cout);
				return 0;
			}

			std::vector<std::string> args = StringUtils::split(configFilename, "@");
			configFilename = args[0];
			if(args.size() == 2)
			{
				remote = true;
				masterHostname = args[1];
			
				// If logging on remote nodes is enabled, set up an output file using the app + node name.
				// otherwise, disable logging.
				if(logRemoteNodes)
				{
					omsg("Remote node logging enabled");
					String hostLogFilename = masterHostname + "-" + logFilename;
					ologopen(hostLogFilename.c_str());
				}
				else
				{
					ologdisable();
				}
			}
			else
			{
				ologopen(logFilename.c_str());
			}
		
			SystemManager* sys = SystemManager::instance();
			DataManager* dm = sys->getDataManager();
			
			omsg("omegalib data search paths:");
			String cwd = ogetcwd();
			ofmsg("::: %1%", %cwd);

			// Add some default filesystem search paths: 
			// - an empty search path for absolute paths
			// - the current directory
			// - the omegalib applications root path (if exists)
			// - the default omegalib data path
			dm->addSource(new FilesystemDataSource("./"));
			dm->addSource(new FilesystemDataSource(""));
			dm->addSource(new FilesystemDataSource(dataPath));
			ofmsg("::: %1%", %dataPath);
			dm->addSource(new FilesystemDataSource(OMEGA_DATA_PATH));
			ofmsg("::: %1%", %OMEGA_DATA_PATH);


			omsg("omegalib application config lookup:");
			String curCfgFilename = ostr("%1%/%2%", %app.getName() %configFilename);
			ofmsg("::: trying %1%", %curCfgFilename);
			String path;
			if(!DataManager::findFile(curCfgFilename, path))
			{
				curCfgFilename = configFilename;
				ofmsg("::: not found, trying %1%", %curCfgFilename);

				if(!DataManager::findFile(curCfgFilename, path))
				{
					curCfgFilename = "default.cfg";
					ofmsg("::: not found, trying %1%", %curCfgFilename);
					if(!DataManager::findFile(curCfgFilename, path))
					{
						oerror("FATAL: Could not load default.cfg. Aplication will exit now.");
						return -1;
					}
				}
			}

			ofmsg("::: found config: %1%", %curCfgFilename);

			Config* cfg = new Config(curCfgFilename);
			
			if(kill)
			{
				sys->setApplication(&app);
				sys->setupConfig(cfg);
				sys->setupDisplaySystem();
				DisplaySystem* ds = sys->getDisplaySystem();
				ds->killCluster();
			}
			else
			{
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

				omsg("===================== ReferenceType object leaks follow:");
				ReferenceType::printObjCounts();
			}

			ologclose();
		}

#ifdef OMEGA_ENABLE_VLD
		_cexit();
		VLDReportLeaks();
#endif

		return 0;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void osleep(uint msecs)
	{
#ifdef WIN32
		Sleep(msecs);
#else
		usleep((msecs)*1000);
#endif
	}

	#ifndef OMEGA_OS_WIN
	// the signal handler for SIGCHILD
	static void sigChildHandler( int /*signal*/ )
	{
		// Get exit status to avoid zombies
		int status;
		::wait( &status );

		// DO NOT USE cout/cerr: signal handler might be called while another cout
		//            is in progress, which will cause a deadlock due to a double
		//            flockfile() 
    
		// Re-install signal handler
		signal( SIGCHLD, sigChildHandler );
	}
	#endif

	//////////////////////////////////////////////////////////////////////////////////////////////////
	bool olaunch(const String& command)
	{
		if( command.empty( )) return false;

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

		return true;
#else
		std::vector<std::string> commandLine = StringUtils::split(command, " ");

		signal( SIGCHLD, sigChildHandler );
		const int result = fork();
		switch( result )
		{
			case 0: // child
				break;

			case -1: // error
				ofwarn("Launching command %1% failed.", %command);
				return false;
			default: // parent
				return true;
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

		ofmsg("Executing: %1%", %command);
		int nTries = 10;
		while( nTries-- )
		{
			execvp( argv[0], argv );
			ofwarn("Error executing %1%", %command);
			// EQWARN << "Error executing '" << argv[0] << "': " << sysError
				   // << std::endl;
			if( errno != ETXTBSY )
				break;
		}

		// Launch failed. Crash and burn.
		exit(-1);
		return false;
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	String ogetcwd()
	{
		char cCurrentPath[FILENAME_MAX];
		GetCurrentDir(cCurrentPath, sizeof(cCurrentPath));
		return cCurrentPath;
	}

	String _dataPrefix;

	///////////////////////////////////////////////////////////////////////////////////////////////
	void osetdataprefix(const String& data)
	{
		_dataPrefix = data;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	String ogetdataprefix()
	{
		return _dataPrefix;
	}
}
