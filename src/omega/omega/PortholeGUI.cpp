/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Donghi Daniele			d.donghi@gmail.com
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
#include "omega/PortholeGUI.h"
#include <iostream>

using namespace omega;
using namespace std;

PortholeFunctionsBinder* PortholeGUI::functionsBinder;
vector<PortholeInterfaceType*> PortholeGUI::interfaces;
std::map<string, TiXmlElement* > PortholeGUI::interfacesMap;
std::map<string, PortholeElement*> PortholeGUI::elementsMap;

///////////////////////////////////////////////////////////////////////////////////////////////
PortholeGUI::PortholeGUI()
{
	xmlPrinter = new TiXmlPrinter();
}

///////////////////////////////////////////////////////////////////////////////////////////////
PortholeGUI::~PortholeGUI(){
	delete xmlPrinter;
	delete device;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void PortholeGUI::setDeviceSpecifications(int width, int height, string orientation){

	// Set the device
	this->device = new PortholeDevice();
	device->deviceWidth = width;
	device->deviceHeight = height;
	device->deviceOrientation = orientation;
	
	for(int i=0; i<interfaces.size(); i++){
		if (width > interfaces.at(i)->minWidth && 
			height > interfaces.at(i)->minHeight &&
			orientation.compare(interfaces.at(i)->orientation)==0)
			device->interfaceType = interfaces.at(i);
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////
string PortholeGUI::create(bool firstTime){

	int cameraIterator = 0;
	string result = "";

	string interfaceKey = device->interfaceType->id + device->interfaceType->orientation;
	TiXmlElement* root = interfacesMap[interfaceKey];

	if (root == NULL) return "Interface not available for this device";

	// Parse the GUI elemets disposition
	for (TiXmlElement* pChild = root->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement()){
		
		// Get element name: should correspond to id TODO check
		const char* id = pChild->Value();

		PortholeElement* element = elementsMap[id];

		string width="", height="";

		// Parse attributes
		TiXmlAttribute* pAttrib = pChild->ToElement()->FirstAttribute();
		while(pAttrib){

			string attribute = pAttrib->Name();
			StringUtils::toLowerCase(attribute);

			// Save id attribute
			if (strcmp(attribute.c_str(),"width")==0){
				width = pAttrib->Value();
			}

			// Save type attribute
			else if (strcmp(attribute.c_str(),"height")==0){
				height = pAttrib->Value();
			}

			// Next attribute
			pAttrib = pAttrib->Next();
		}

		/*
		*	GOOGLE MAPS
		*/
		// For googlemaps element, create a div element that will contain a googlemaps view
		// TODO Size of map canvas
		if (strcmp(element->type.c_str(),"googlemap")==0){
			element->htmlValue = "<div  style=\" padding : 5px \"><input id=\"searchTextField\" type=\"text\" style=\"width : 100%; height : 20px;\"></div>";
			element->htmlValue.append("<div id=\"map-canvas\" class=\"map_container\" style=\"width:");
			element->htmlValue.append(boost::lexical_cast<string>(device->deviceWidth)+"px; height:"+
							  boost::lexical_cast<string>(device->deviceHeight)+"px \" ");
			element->htmlValue.append("></div>");
		}

		/*
		*	CAMERA STREAM
		*/
		// Create a session camera
		else if (strcmp(element->type.c_str(),"camera_stream")==0){

			int cameraId = 0;

			// FIXME Not working
			if (strcmp(element->cameraType.c_str(),"default")==0){
				if (firstTime){
					createCustomCamera(true);
					// Update camera id on html
					cameraId = camerasIncrementalId;
				}
				else{
					// TODO
					cameraIterator++;
				}
			}
			// Custom camera case
			else {
				if (firstTime){
					createCustomCamera(false);
					// Update camera id on html
					cameraId = camerasIncrementalId;
				}
				else{
					// Update camera id on html
					cameraId = camerasId.at(cameraIterator);
					modCustomCamera(cameraId, 1.0);
					cameraIterator++;
				}
			}

			element->htmlValue = "<canvas id=\"camera-canvas\" class=\"camera_container\" data-camera_id = \"" +
								boost::lexical_cast<string>(cameraId) +
								"\"></canvas>";

		}

		// Create the HTML result for this element. embedded into a (width,size) div element
		// TODO Layouts Vertical/Horizontal/Grid/Relative
		result.append("<div style=\" width : "+ width +"; height : "+ height +"; \" >"+ element->htmlValue +"</div>");

	}

	return result;

}

///////////////////////////////////////////////////////////////////////////////////////////////
/* 
*	Camera creation function
*	followDefaultCamera: true if camera shold reflect the default camera position and orientation
*/
void PortholeGUI::createCustomCamera(bool followDefaultCamera){

	// Get the global engine
	Engine* myEngine = Engine::instance();

	// Initialize camera size
	// Workaround. This avoid a canvas drawing bug
	// Round down width to a multiple of 4.
	int width = (int)( IMAGE_QUALITY * device->deviceWidth / 4 ) * 4;
	int height = width*((float)device->deviceHeight/device->deviceWidth);
	//cout << "Width -> initially " << device->deviceWidth  << " chosen: " << width << endl;

	PixelData* sessionCanvas = new PixelData(PixelData::FormatRgb,  width,  height);

	uint flags = Camera::ForceMono | Camera::DrawScene | Camera::Offscreen;

	Camera* sessionCamera = myEngine->createCamera(flags);
	sessionCamera->setProjection(60, 1, 0.1f, 100);
	sessionCamera->setAutoAspect(true);

	// Initialize the camera position to be the same as the main camera.
	Camera* defaultCamera = myEngine->getDefaultCamera();
	sessionCamera->setPosition(defaultCamera->getPosition() + defaultCamera->getHeadOffset());
	sessionCamera->getOutput(0)->setReadbackTarget(sessionCanvas);
	sessionCamera->getOutput(0)->setEnabled(true);

	// Save the new Camera and PixelData objects
	PortholeCamera* camera = new PortholeCamera();
	camera->id = ++camerasIncrementalId;
	camera->camera =sessionCamera;
	camera->canvas = sessionCanvas;
	camera->canvasWidth = width;
	camera->canvasHeight = height;
	camera->size = IMAGE_QUALITY;
	camera->followDefault = followDefaultCamera;
	/*camera->oldusStreamSent = 0;*/
	sessionCameras[camera->id] = camera;
	camerasId.push_back(camera->id); 
}

///////////////////////////////////////////////////////////////////////////////////////////////
void PortholeGUI::modCustomCamera(int cameraId, float size){ 

	// Retrieve the camera to be modified
	PortholeCamera* portholeCamera = sessionCameras[cameraId];

	Camera* sessionCamera = portholeCamera->camera;

	// Get the global engine
	Engine* myEngine = Engine::instance();

	// Initialize camera size
	// Workaround. This avoid a canvas drawing bug
	// Round down width to a multiple of 4.
	int width = (int)((size * portholeCamera->size * device->deviceWidth) / 4) * 4;
	int height = width*((float)device->deviceHeight/device->deviceWidth);

	// Set new camera target
	portholeCamera->canvas = new PixelData(PixelData::FormatRgb, width, height);
	sessionCamera->getOutput(0)->setReadbackTarget(portholeCamera->canvas);
	sessionCamera->getOutput(0)->setEnabled(true);

	portholeCamera->size = portholeCamera->size * size;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*Recursive*/void PortholeGUI::searchNode(TiXmlElement* node){

	if ( !node ) return;

	// Parse attributes
	TiXmlAttribute* pAttrib = node->FirstAttribute();
	while(pAttrib){

		string attribute = pAttrib->Name();
		StringUtils::toLowerCase(attribute);

		// Save id attribute if not defined by user as cpp function
		if (HTML::isEvent(attribute) && !functionsBinder->isCppDefined(pAttrib->Value())){
			cout << "Adding script " << pAttrib->Value() << endl; 
			string key = "python_script"+boost::lexical_cast<string>(functionsBinder->scriptNumber)+"(event)";
			functionsBinder->addPythonScript(pAttrib->Value(), key); // Insert the new script
			pAttrib->SetValue(key.c_str());
		}

		// Next attribute
		pAttrib = pAttrib->Next();
	}

	// Check recursively
	for (TiXmlElement* pChild = node->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement()){
		searchNode(pChild);
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////
vector<string> PortholeGUI::findHtmlScripts(){

	vector<string> result;

	TiXmlNode* guiElements = xmlDoc->FirstChildElement()->FirstChildElement();

	searchNode(guiElements->ToElement());

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void PortholeGUI::parseXmlFile(char* xmlPath){
	omega::xmlDoc = new TiXmlDocument(xmlPath);
	bool loadOkay = xmlDoc->LoadFile();
	if (!loadOkay){
		printf("!!! Porthole: Failed to load XML file");
		abort();
	}

	// Recursive search for Python Scripts inside events handlers
	findHtmlScripts();

	TiXmlNode* guiElements = xmlDoc->FirstChildElement()->FirstChildElement();
	TiXmlPrinter* xmlPrinter = new TiXmlPrinter();

	// Parse the GUI elements
	for (TiXmlNode* pChild = guiElements->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement()){

		PortholeElement* element = new PortholeElement();

		// Parse attributes
		TiXmlAttribute* pAttrib = pChild->ToElement()->FirstAttribute();
		while(pAttrib){

			string attribute = pAttrib->Name();
			StringUtils::toLowerCase(attribute);

			// Save id attribute
			if (strcmp(attribute.c_str(),"id")==0){
				element->id = pAttrib->Value();
			}

			// Save type attribute
			else if (strcmp(attribute.c_str(),"type")==0){
				element->type = pAttrib->Value();
			}

			// Save camera type attribute
			else if (strcmp(attribute.c_str(),"camera")==0){
				element->cameraType = pAttrib->Value();
			}

			// Next attribute
			pAttrib = pAttrib->Next();
		}

		StringUtils::toLowerCase(element->type);
		StringUtils::toLowerCase(element->cameraType);

		// For HTML elements, just add all the content to the element
		if  (strcmp(element->type.c_str(),"html")==0){

			// Parse the GUI elements
			for (TiXmlNode* pHtmlChild = pChild->FirstChildElement(); pHtmlChild != 0; pHtmlChild = pHtmlChild->NextSiblingElement()){
				
				// TODO FIXME not inside div -> fail
				pHtmlChild->Accept( xmlPrinter );
				//cout << "Added: " << id << " -> " << xmlPrinter->CStr() << endl;
				element->htmlValue.append(xmlPrinter->CStr());
				// delete new line
				element->htmlValue.erase(std::remove(element->htmlValue.begin(), element->htmlValue.end(), '\n'), element->htmlValue.end());

			}

		}

		if(element->id.length() > 0 && element->type.length() > 0){
			elementsMap[element->id] = element;
		}

	}

	TiXmlNode* guiDisposition = guiElements->NextSiblingElement();

	// Parse the GUI elemets disposition
	// For each specified interface size
	for (TiXmlElement* pInterfaceChild = guiDisposition->FirstChildElement(); pInterfaceChild != 0; pInterfaceChild = pInterfaceChild->NextSiblingElement()){

			// Get element name
			string interfaceId = string(pInterfaceChild->Value());

			int minWidth=0, minHeight=0;

			// Parse attributes
			TiXmlAttribute* pAttrib = pInterfaceChild->FirstAttribute();
			while(pAttrib){

				string attribute = pAttrib->Name();
				StringUtils::toLowerCase(attribute);

				// Save id attribute
				if (strcmp(attribute.c_str(),"minwidth")==0){
					minWidth = pAttrib->IntValue();
				}

				// Save type attribute
				else if (strcmp(attribute.c_str(),"minheight")==0){
					minHeight = pAttrib->IntValue();
				}

				// Next attribute
				pAttrib = pAttrib->Next();
			}

		// For each orientation
		for (TiXmlElement* pOrientationChild = pInterfaceChild->FirstChildElement(); pOrientationChild != 0; pOrientationChild = pOrientationChild->NextSiblingElement()){
				
			// Get element name
			string orientation = string(pOrientationChild->Value());
			StringUtils::toLowerCase(orientation);

			// Check orientation and save node in the map
			if (orientation.compare("portrait")==0 || orientation.compare("port")==0){
				PortholeInterfaceType* interfaceType = new PortholeInterfaceType();
				interfaceType->minWidth = minWidth;
				interfaceType->minHeight = minHeight;
				interfaceType->id = interfaceId;
				interfaceType->orientation = "portrait";
				interfaces.push_back(interfaceType);
				cout << ">> Added interface:" << interfaceId << " " << orientation << " " << minWidth << " " << minHeight << endl;
				interfacesMap[interfaceId + orientation] = pOrientationChild;
			}
			else if (orientation.compare("landscape")==0 || orientation.compare("land")==0){
				PortholeInterfaceType* interfaceType = new PortholeInterfaceType();
				interfaceType->minWidth = minHeight;
				interfaceType->minHeight = minWidth;
				interfaceType->id = interfaceId;
				interfaceType->orientation = "landscape";
				interfaces.push_back(interfaceType);
				cout << ">> Added interface:" << interfaceId << " " << orientation << " " << minHeight << " " << minWidth << endl;
				interfacesMap[interfaceId + orientation] = pOrientationChild;
			}
		}	
	}

}