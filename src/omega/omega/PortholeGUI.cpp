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
#include "omega/PortholeGUI.h"
#include <iostream>

using namespace omega;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////
PortholeGUI::PortholeGUI(const char* documentName){

	xmlDoc = new TiXmlDocument( documentName );
	xmlPrinter = new TiXmlPrinter();
	bool loadOkay = xmlDoc->LoadFile();
	if (loadOkay)
	{
		// XML Node printer
		xmlPrinter->SetStreamPrinting();
	}
	else
	{
		printf("Failed to load file");
	}

	// Create Devices
	setPossibleInterfaces();

}

///////////////////////////////////////////////////////////////////////////////////////////////
void PortholeGUI::setPossibleInterfaces(){

	PortholeInterfaceType smallPort = {426,320,"small","portrait"},
						  smallLand = {320,426,"small","landscape"},
						  mediumPort = {470,320,"medium","portrait"},
						  mediumLand = {320,470,"medium","landscape"},
						  largePort = {640,480,"large","portrait"},
						  largeLand = {480,640,"large","landscape"},
						  xlargePort = {960,720,"xlarge","portrait"},
						  xlargeLand = {720,960,"xlarge","landscape"};

	interfaces.push_back(smallPort);
	interfaces.push_back(smallLand);
	interfaces.push_back(mediumPort);
	interfaces.push_back(mediumLand);
	interfaces.push_back(largePort);
	interfaces.push_back(largeLand);
	interfaces.push_back(xlargePort);
	interfaces.push_back(xlargeLand);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void PortholeGUI::setDeviceSpecifications(int width, int height, string orientation){

	// Set the device
	this->device = new PortholeDevice();
	device->deviceWidth = width;
	device->deviceHeight = height;
	device->deviceOrientation = orientation;

}

///////////////////////////////////////////////////////////////////////////////////////////////
string PortholeGUI::create(){

	string result = "";

	TiXmlNode* guiElements = xmlDoc->FirstChildElement()->FirstChildElement();
	TiXmlNode* guiDisposition = guiElements->NextSiblingElement();

	// Parse the GUI elements
	for (TiXmlNode* pChild = guiElements->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement()){

		string id = "";
		string type = "";

		string htmlValue = "";

		// Parse attributes
		TiXmlAttribute* pAttrib = pChild->ToElement()->FirstAttribute();
		while(pAttrib){

			string attribute = pAttrib->Name();
			StringUtils::toLowerCase(attribute);

			// Save id attribute
			if (strcmp(attribute.c_str(),"id")==0){
					id = pAttrib->Value();
			}

			// Save type attribute
			else if (strcmp(attribute.c_str(),"type")==0){
					type = pAttrib->Value();
			}

			// Next attribute
			pAttrib = pAttrib->Next();
		}

		StringUtils::toLowerCase(type);

		// For HTML elements, just add all the content to the element
		if  (strcmp(type.c_str(),"html")==0){
				
			// TODO concat children 
			pChild->FirstChild()->Accept( xmlPrinter );
			//cout << "Added: " << id << " -> " << xmlPrinter->CStr() << endl;
			htmlValue = xmlPrinter->CStr();

		}

		// For googlemaps element, create a div element that will contain a googlemaps view
		else if (strcmp(type.c_str(),"googlemap")==0){
			htmlValue = "<div id=\"map-canvas\" class=\"map_container\" style=\"width:";
			htmlValue.append(boost::lexical_cast<string>(device->deviceWidth)+"px; height:"+
							  boost::lexical_cast<string>(device->deviceHeight)+"px \" ");
			htmlValue.append("></div>");
		}

		// For a camera stream
		else if (strcmp(type.c_str(),"camera_stream")==0){
			htmlValue = "<canvas id=\"camera-canvas\" class=\"camera_container\" style=\"width:";
			htmlValue.append(boost::lexical_cast<string>(device->deviceWidth)+"px; height:"+
							  boost::lexical_cast<string>(device->deviceHeight)+"px \" ");
			htmlValue.append("></canvas>");
		}

		if(id.length() > 0 && type.length() > 0){
			elementsMap[id] = htmlValue;
		}

	}

	// Parse the GUI elemets disposition
	for (TiXmlNode* pChild = guiDisposition->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement()){
		
		// Get element name: should correspond to id TODO check
		const char* id = pChild->ToElement()->Value();

		string htmlElement = elementsMap[id];

		result.append(htmlElement);

	}

	return result;

}
