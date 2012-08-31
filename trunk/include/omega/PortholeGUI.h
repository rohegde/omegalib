/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Donghi Daniele			d.donghi@gmail.com
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

#ifndef __PORTHOLE_GUI_H__
#define __PORTHOLE_GUI_H__

#include "omicron/xml/tinyxml.h"

using namespace omicron;
using namespace std;

namespace omega {

	// Porthole functions binder
	struct PortholeFunctionsBinder{

		typedef void(*memberFunction)();

		void addFunction(std::string funcName, memberFunction func)
		{
			funcMap[funcName] = func;
		}

		void callFunction(std::string funcName)
		{
			std::map<std::string, memberFunction>::const_iterator it;

			it = funcMap.find(funcName);

			if (it == funcMap.end()) return;

			return (*it->second)();
		}

		std::map<std::string, memberFunction> funcMap;
	};

	// Functions binder object
	static PortholeFunctionsBinder* functionsBinder;

	// A device specifications object
	typedef struct PortholeDevice{
		int deviceWidth;
		int deviceHeight;
		string deviceOrientation; // Portrait or Landscape
	} PortholeDevice;

	// This will old a possible interface
	typedef struct PortholeInterfaceType{
		int minWidth;
		int minHeight;
		string type;
		string orientation;
	}PortholeInterfaceType;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Implements the HTML GUI Manager for Porthole Service
	class OMEGA_API PortholeGUI{

	public:

		// Constructor
		PortholeGUI( const char * documentName );

		~PortholeGUI();

		// Create the device spicifc html interface
		string create();

		// Set device specifications
		void setDeviceSpecifications(int width, int height, string orientation);

	private:

		// XML
		TiXmlDocument* xmlDoc;
		TiXmlPrinter* xmlPrinter;

		// The device for which an interface will be created
		PortholeDevice* device;

		// A map between an element id and the element data as html string
		std::map<string, string> elementsMap;

		// A map between a device type and its GUI elements (as a list of node id string)
		std::map<PortholeInterfaceType, vector<string>> devicesMap;

		// All the possible interfaces
		vector<PortholeInterfaceType> interfaces; 

		// Create the devices
		void setPossibleInterfaces();

	};

};

#endif