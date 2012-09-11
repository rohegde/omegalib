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

#include "omega/Camera.h"
#include "omega/Application.h"
#include "omega/PythonInterpreter.h"
#include "omicron/xml/tinyxml.h"

using namespace omicron;
using namespace std;

namespace omega {

	namespace HTML {

		static const int eventsNumber = 19;

		// All HTML compatible events that could be found parsing the application xml
		static const string events[eventsNumber] = {
			"onload",  /* Script to be run when a document load */
			"onunload",  /*  Script to be run when a document unload */
			"onblur",  /* Script to be run when an element loses focus */
			"onchange",  /* Script to be run when an element changes */
			"onfocus",  /* Script to be run when an element gets focus */
			"onreset",  /*  Script to be run when a form is reset */
			"onselect",  /* Script to be run when a document load */
			"onsubmit",  /* Script to be run when a form is submitted */
			"onabort",  /* Script to be run when loading of an image is interrupted */
			"onkeydown",  /* Script to be run when a key is pressed */
			"onkeypress",  /* Script to be run when a key is pressed and released */
			"onkeyup",  /* Script to be run when a key is released */
			"onclick",  /* Script to be run on a mouse click */
			"ondblclick",  /* Script to be run on a mouse double-click */
			"onmousedown",  /* Script to be run when mouse button is pressed */
			"onmousemove",  /* Script to be run when mouse pointer moves */
			"onmouseout",  /* Script to be run when mouse pointer moves out of an element */
			"onmouseover",  /* Script to be run when mouse pointer moves over an element */
			"onmouseup",  /* Script to be run when mouse button is released */
		};

		static bool isEvent(string stringToSearch){
			for(int i=0; i < eventsNumber; i++)
				if (stringToSearch.compare(events[i]) == 0)
					return true; // found
			return false; // not found
		}

	};

	// Id to be assigned to new cameras
	static int camerasIncrementalId = 0;

	// Porthole functions binder
	struct PortholeFunctionsBinder{

		typedef void(*memberFunction)();

		void addFunction(std::string funcName, memberFunction func)
		{
			cppFuncMap[funcName] = func;
		}

		void addPythonScript(std::string script, string key){
			pythonFunMap[key] = script;
			scriptNumber++;
		}

		void callFunction(std::string funcName)
		{
			std::map<std::string, memberFunction>::const_iterator cpp_it;
			cpp_it = cppFuncMap.find(funcName);
			if (cpp_it != cppFuncMap.end())	return (*cpp_it->second)();

			std::map<std::string, string>::const_iterator py_it;
			py_it = pythonFunMap.find(funcName);
			if (py_it != pythonFunMap.end()){
				PythonInterpreter* pi = SystemManager::instance()->getScriptInterpreter();
				pi->queueCommand(py_it->second); 
			}
			return;
		}

		bool isCppDefined(string funcName)
		{
			std::map<std::string, memberFunction>::const_iterator it;
			it = cppFuncMap.find(funcName);
			if (it != cppFuncMap.end()) return true;
			return false;
		}

		std::map<std::string, memberFunction> cppFuncMap;
		std::map<std::string, string> pythonFunMap;
		int scriptNumber;
	};

	// This will old a possible interface
	typedef struct PortholeInterfaceType{
		int minWidth;
		int minHeight;
		string id;
		string orientation;
	} PortholeInterfaceType;

	// A device specifications object
	typedef struct PortholeDevice{
		int deviceWidth;
		int deviceHeight;
		string deviceOrientation; // Portrait or Landscape
		PortholeInterfaceType interfaceType;
	} PortholeDevice;

	// An element object
	typedef struct PortholeElement{
		string id;
		string type;
		string cameraType; // Defined if type is camera stream
		string htmlValue;
	} PortholeElement;

	typedef struct PortholeCamera{
		int id;
		Camera* camera;
		PixelData* canvas;
		unsigned int oldusStreamSent; // Timestamp of last stream sent via socket
	} PortholeCamera;

	// Xml Document
	static TiXmlDocument* xmlDoc;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Implements the HTML GUI Manager for Porthole Service
	class OMEGA_API PortholeGUI{

	public:

		// Constructor
		PortholeGUI();

		// TODO deallocate new objects
		~PortholeGUI();

		// Create the device spicifc html interface
		string create(bool firstTime);

		// Set device specifications
		void setDeviceSpecifications(int width, int height, string orientation);

		// Return an object that contains the device specifications
		PortholeDevice* getDevice() { return device; }

		// Number of cameras to stream over the network
		int numberOfStreamsToSend() { return sessionCameras.size(); } 

		// Get session stream data
		std::map<int,PortholeCamera> getSessionCameras() { return sessionCameras; } 

		static vector<string> findHtmlScripts();

		static void parseXmlFile(char* xmlPath);

		// Functions binder getter and setter
		static PortholeFunctionsBinder* getPortholeFunctionsBinder() { return functionsBinder; }
		static void setPortholeFunctionsBinder(PortholeFunctionsBinder* binder) { functionsBinder = binder;  functionsBinder->scriptNumber=0;}

	private:

		// XML
		TiXmlPrinter* xmlPrinter;

		// The device for which an interface will be created
		PortholeDevice* device;

		// All cameras
		std::map<int,PortholeCamera> sessionCameras;

		// Create a Porthole custom camera and a PixelData associated
		void createCustomCamera(); 

		void modCustomCamera(int cameraIterator);

		vector<int> camerasId; 

		static void searchNode(TiXmlElement* node);

		// Functions binder object
		static PortholeFunctionsBinder* functionsBinder;

		// All the possible interfaces
		static vector<PortholeInterfaceType> interfaces; 

		// A map between a device type and its GUI elements
		static std::map<string, TiXmlElement* > interfacesMap;

		// A map between an element id and the element data
		static std::map<string, PortholeElement> elementsMap;
	};

};

#endif