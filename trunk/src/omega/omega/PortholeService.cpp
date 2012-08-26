/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
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

// Implementation of the HTML5 based interfaces service for handled devices

#include "omega/PortholeService.h"
#include "omega/Camera.h"
#include "omega/Application.h"
#include "omega/ImageUtils.h"
#include "vjson/json.h"

#include <iostream>

using namespace omega;
using namespace omicron;

enum demo_protocols {
	/* always first */
	PROTOCOL_HTTP = 0,
	PROTOCOL_WEBSOCKET,
	/* always last */
	DEMO_PROTOCOL_COUNT
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void ServerThread::sendHtmlElements(struct libwebsocket_context *context,
		struct libwebsocket *wsi){

		// TEST Just send the first node
		TiXmlNode* node = xmlDoc->FirstChild();
		node->Accept( xmlPrinter );

		string toSend = "{\"event_type\" : \"html_elements\", \"innerHTML\" : \"";
		toSend.append(omicron::StringUtils::replaceAll(xmlPrinter->CStr(),"\"","\\\""));
		toSend.append("\"}");

		cout << toSend << endl;

		// Send the html elements
		int n;
		unsigned char* buf;
		buf = new unsigned char[LWS_SEND_BUFFER_PRE_PADDING + toSend.length() + LWS_SEND_BUFFER_POST_PADDING];
		unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];
		n = sprintf((char *)p, "%s",toSend.c_str());
		n = libwebsocket_write(wsi, p, n, LWS_WRITE_TEXT);

		// Free the buffer
		delete[] buf;

		return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * May be used for filtering allowing connections by the header
 * content
 */
void ServerThread::dump_handshake_info(struct lws_tokens *lwst)
{
	//int n;
	static const char *token_names[WSI_TOKEN_COUNT] = {
		/*[WSI_TOKEN_GET_URI]		=*/ "GET URI",
		/*[WSI_TOKEN_HOST]		=*/ "Host",
		/*[WSI_TOKEN_CONNECTION]	=*/ "Connection",
		/*[WSI_TOKEN_KEY1]		=*/ "key 1",
		/*[WSI_TOKEN_KEY2]		=*/ "key 2",
		/*[WSI_TOKEN_PROTOCOL]		=*/ "Protocol",
		/*[WSI_TOKEN_UPGRADE]		=*/ "Upgrade",
		/*[WSI_TOKEN_ORIGIN]		=*/ "Origin",
		/*[WSI_TOKEN_DRAFT]		=*/ "Draft",
		/*[WSI_TOKEN_CHALLENGE]		=*/ "Challenge",

		/* new for 04 */
		/*[WSI_TOKEN_KEY]		=*/ "Key",
		/*[WSI_TOKEN_VERSION]		=*/ "Version",
		/*[WSI_TOKEN_SWORIGIN]		=*/ "Sworigin",

		/* new for 05 */
		/*[WSI_TOKEN_EXTENSIONS]	=*/ "Extensions",

		/* client receives these */
		/*[WSI_TOKEN_ACCEPT]		=*/ "Accept",
		/*[WSI_TOKEN_NONCE]		=*/ "Nonce",
		/*[WSI_TOKEN_HTTP]		=*/ "Http",
		/*[WSI_TOKEN_MUXURL]	=*/ "MuxURL",
	};

	//TESTING FUNCTION
	/*
	for (n = 0; n < WSI_TOKEN_COUNT; n++) {
		if (lwst[n].token == NULL)
			continue;
		fprintf(stderr, "    %s = %s\n", token_names[n], lwst[n].token);
	}
	*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/* this protocol server (always the first one) just knows how to do HTTP */
int ServerThread::callback_http(struct libwebsocket_context *context,
		struct libwebsocket *wsi,
		enum libwebsocket_callback_reasons reason, void *user,
							   void *in, size_t len){
	char client_name[128];
	char client_ip[128];

	switch (reason) {
	case LWS_CALLBACK_HTTP:
		//fprintf(stderr, "serving HTTP URI %s\n", (char *)in);

		/* Html page icon */
		if (in && strcmp((char*)in, "/favicon.ico") == 0) {
			if (libwebsockets_serve_http_file(wsi,
			     (DATA_PATH+"/favicon.ico").c_str(), "image/x-icon"))
				fprintf(stderr, "Failed to send favicon\n");
			break;
		}

		/* Multitouch scripts */
		else if (in && strcmp((char*)in, "/hammer.js") == 0) {
			if (libwebsockets_serve_http_file(wsi,
			     (DATA_PATH+"/hammer.js").c_str(), "application/javascript"))
				fprintf(stderr, "Failed to send hammer.js\n");
			break;
		}
		else if (in && strcmp((char*)in, "/jquery.hammer.js") == 0) {
			if (libwebsockets_serve_http_file(wsi,
			     (DATA_PATH+"/jquery.hammer.js").c_str(), "application/javascript"))
				fprintf(stderr, "Failed to send jquery.hammer.js\n");
			break;
		}
		else if (in && strcmp((char*)in, "/jquery.specialevent.hammer.js") == 0) {
			if (libwebsockets_serve_http_file(wsi,
			     (DATA_PATH+"/jquery.specialevent.hammer.js").c_str(), "application/javascript"))
				fprintf(stderr, "Failed to send jquery.specialevent.hammer.js\n");
			break;
		}
		else if (in && strcmp((char*)in, "/browser_detect.js") == 0) {
			if (libwebsockets_serve_http_file(wsi,
			     (DATA_PATH+"/browser_detect.js").c_str(), "application/javascript"))
				fprintf(stderr, "Failed to send browser_detect.js\n");
			break;
		}

		/* Function Binder Javascript */
		else if (in && strcmp((char*)in, "/porthole_functions_binder.js") == 0) {
			
			// Build Content TODO call a function
			string content = "function onClickTest(){ alert(\"Button Clicked\"); }";
			
			// Build Message = Header + Content
			char content_length[16];
			sprintf (content_length, "%d", content.length());
			string msg = "HTTP/1.0 200 OK\x0d\x0a"
							"Server: libwebsockets\x0d\x0a"
							"Content-Type: application/javascript\x0d\x0a"
							"Content-Length: ";
			msg.append(content_length);
			msg.append("\x0d\x0a\x0d\x0a");
			msg.append(content);

			// Send the Javascript file
			int n;
			unsigned char* p;
			p = new unsigned char[msg.length()];
			n = sprintf((char*)p,msg.c_str());
			libwebsocket_write(wsi, (unsigned char*)p, n, LWS_WRITE_HTTP);

			// Free the buffer
			delete[] p;

			break;
		}

		/* HTML page... when it runs it'll start websockets */
		if (libwebsockets_serve_http_file(wsi,
				  (DATA_PATH+"/index.html").c_str(), "text/html"))
			fprintf(stderr, "Failed to send HTTP file\n");
		break;

	/* On connection, we could add any filtering function
	*  Now it's: accept any
	*/
	case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:

		libwebsockets_get_peer_addresses((int)(long)user, client_name,
			     sizeof(client_name), client_ip, sizeof(client_ip));

		fprintf(stderr, "Received network connect from %s (%s)\n",
							client_name, client_ip);

		/* if we returned non-zero from here, we kill the connection */
		break;

	default:
		break;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/* websocket protocol */

// Struct of data to be passed across the entire session
struct per_session_data {
	Camera* sessionCamera;
	PixelData* sessionCanvasPixel;
	ByteArray* sessionPng;
	unsigned int oldus; // Last message sent timestamp
};

// Struct of a socket message: payload and its length
struct a_message {
	void *payload;
	size_t len;
};

// JSON simple print
#define IDENT(n) for (int i = 0; i < n; ++i) printf("    ")
inline void print(json_value *value, int ident = 0)
{
	IDENT(ident);
	if (value->name) printf("\"%s\" = ", value->name);
	switch(value->type)
	{
	case JSON_NULL:
		printf("null\n");
		break;
	case JSON_OBJECT:
	case JSON_ARRAY:
		printf(value->type == JSON_OBJECT ? "{\n" : "[\n");
		for (json_value *it = value->first_child; it; it = it->next_sibling)
		{
			print(it, ident + 1);
		}
		IDENT(ident);
		printf(value->type == JSON_OBJECT ? "}\n" : "]\n");
		break;
	case JSON_STRING:
		printf("\"%s\"\n", value->string_value);
		break;
	case JSON_INT:
		printf("%d\n", value->int_value);
		break;
	case JSON_FLOAT:
		printf("%f\n", value->float_value);
		break;
	case JSON_BOOL:
		printf(value->int_value ? "true\n" : "false\n");
		break;
	}
}

#define MSG_EVENT_TYPE "event_type"

#define MSG_EVENT_SPEC "device_spec"
#define MSG_WIDTH "width"
#define MSG_HEIGHT "height"
#define MSG_ORIENTATION "orientation"

#define MSG_EVENT_DRAG "drag"
#define MSG_DELTA_X "deltaX"
#define MSG_DELTA_Y "deltaY"

#define MSG_EVENT_PINCH "pinch"
#define MSG_DELTA_SCALE "scale"
#define MSG_DELTA_ROTATION "rotation"

struct recv_message{
    string event_type;
    float deltaX,deltaY;
	float scale; // This value ranges about {0,2;6}: >1 is zoom in, <1 is zoom out
	float deltaRotation;
	int width,height;
	string orientation;
};

// This is the function that handle the event received by the client,
// that has the per_session_data structure associated
inline void parse_json_message(json_value *value, per_session_data* data, recv_message* message){

    switch(value->type)
    {
    case JSON_NULL:
        printf("null\n");
        break;
    case JSON_OBJECT:
    case JSON_ARRAY:
        for (json_value *it = value->first_child; it; it = it->next_sibling)
        {
            parse_json_message(it, data, message);
        }
        break;
    case JSON_STRING:

        // Event type
        if (strcmp(value->name, MSG_EVENT_TYPE) == 0)
           message->event_type = value->string_value;

		// Orientation
		else if (strcmp(value->name, MSG_ORIENTATION) == 0)
            message->orientation = value->string_value;

        break;
    case JSON_INT:

		// Width and Height
		if (strcmp(value->name, MSG_WIDTH) == 0)
            message->width = value->int_value;
		else if (strcmp(value->name, MSG_HEIGHT) == 0)
            message->height = value->int_value;

        break;
    case JSON_FLOAT:

		// Delta X and Y
		if (strcmp(value->name, MSG_DELTA_X) == 0)
            message->deltaX = value->float_value;
		else if (strcmp(value->name, MSG_DELTA_Y) == 0)
            message->deltaY = value->float_value;

		// Scale and Rotation
		else if (strcmp(value->name, MSG_DELTA_SCALE) == 0)
			message->scale = value->float_value;
		else if (strcmp(value->name, MSG_DELTA_ROTATION) == 0)
			message->deltaRotation = value->float_value;

        break;
    case JSON_BOOL:
        break;
    }

}


inline void handle_message(per_session_data* data, recv_message* message){

    // cout << "MSG: " << message->event_type << endl;

	// Handle drag event
	if (strcmp(message->event_type.c_str(),MSG_EVENT_DRAG)==0){
		
		Engine* myEngine = Engine::instance();
		Camera* defaultCamera = myEngine->getDefaultCamera();
		//Vector3f myPosition = defaultCamera->getPosition();
		
		//Vector3f myPosition = data->sessionCamera->getPosition();
		//cout << "Initial position: x = " << myPosition[0] << " y = " << myPosition[1] << endl;
        
		// Change x,y coords
		//myPosition[0] += message->deltaX/100; // TODO check step
        //myPosition[1] += message->deltaY/100;
		//defaultCamera->setPosition(myPosition);
		//data->sessionCamera->setPosition(myPosition);

		// Change pitch and yaw
		Quaternion curOrientation = defaultCamera->getOrientation();
		// Create the YAW and PITCH rotation quaternion
		float yawAngle = message->deltaX * Math::DegToRad;
		float pitchAngle = message->deltaY * Math::DegToRad;
		Quaternion orientation = AngleAxis(yawAngle, Vector3f::UnitY()) * AngleAxis(pitchAngle, Vector3f::UnitX()) * AngleAxis(0, Vector3f::UnitZ());
		// Apply rotation
		defaultCamera->setOrientation(curOrientation * orientation);

		//cout << "Final position: x = " << myPosition[0] << " y = " << myPosition[1] << endl;


    }

	// Handle pinch event
	else if (strcmp(message->event_type.c_str(),MSG_EVENT_PINCH)==0){

		// ZOOM

		Engine* myEngine = Engine::instance();
		Camera* defaultCamera = myEngine->getDefaultCamera();
		Vector3f myPosition = defaultCamera->getPosition();

		//Vector3f myPosition = data->sessionCamera->getPosition();
		//cout << "Initial position: z = " << myPosition[2] << endl;
		// Zoom in
		if (message->scale > 1)
			myPosition[2] -= message->scale/10; // TODO check scale factor
		// Zoom out
		else if (message->scale < 1)
			myPosition[2] += 1.0 - message->scale;
		//cout << "Final position: z = " << myPosition[2] << endl;
		//data->sessionCamera->setPosition(myPosition);
		defaultCamera->setPosition(myPosition);


		// ROTATION

		Quaternion curOrientation = defaultCamera->getOrientation();
		// Create the ROLL rotation quaternion
		float rollAngle = message->deltaRotation / 5 * Math::DegToRad;
		Quaternion orientation = AngleAxis(0, Vector3f::UnitY()) * AngleAxis(0, Vector3f::UnitX()) * AngleAxis(rollAngle, Vector3f::UnitZ());
		// Apply rotation
		defaultCamera->setOrientation(curOrientation * orientation);

    }

}

int ServerThread::callback_websocket(struct libwebsocket_context *context,
			struct libwebsocket *wsi,
			enum libwebsocket_callback_reasons reason,
					       void *user, void *in, size_t len){

	int n;
	struct per_session_data *data = (per_session_data*) user;

	switch (reason) {

	/* On connection estabilished */
	case LWS_CALLBACK_ESTABLISHED:
	{
		fprintf(stderr, "callback_websocket: LWS_CALLBACK_ESTABLISHED\n");

		data->oldus = 0;

		/* Camera initialization */
		Engine* myEngine = Engine::instance();

		// TODO check dimensions
		data->sessionCanvasPixel = new PixelData(PixelData::FormatRgb, 840, 480);

		uint flags = Camera::ForceMono | Camera::DrawScene;

		data->sessionCamera = myEngine->createCamera(flags);
		data->sessionCamera->setProjection(60, 1, 0.1f, 100);
		data->sessionCamera->setAutoAspect(true);

		// Initialize the tablet camera position to be the same as the main camera.
		Camera* defaultCamera = myEngine->getDefaultCamera();
		data->sessionCamera->setPosition(defaultCamera->getPosition());

		data->sessionCamera->getOutput(0)->setReadbackTarget(data->sessionCanvasPixel);
		data->sessionCamera->getOutput(0)->setEnabled(true);

		break;
	}

	case LWS_CALLBACK_SERVER_WRITEABLE:
	{
		// Write at 50Hz, so just pass the token and return "ok" if it's too early for us
		struct timeval tv;
		gettimeofday(&tv, NULL);
		if (((unsigned int)tv.tv_usec - data->oldus) < 20000) {
			libwebsocket_callback_on_writable(context, wsi);
			return 0;
		}

		// Get camera image as PNG and base64 encode it, because only simple strings could be sent via websockets  
		data->sessionPng = ImageUtils::encode(data->sessionCanvasPixel, ImageUtils::FormatPng);
		std::string base64image = base64_encode(data->sessionPng->getData(),data->sessionPng->getSize());

		string toSend = "{\"event_type\" : \"stream\", \"base64image\" : \"";
		toSend.append(base64image.c_str());
		toSend.append("\"}");

		// Send the base64 image
		unsigned char* buf;
		buf = new unsigned char[LWS_SEND_BUFFER_PRE_PADDING + toSend.length() + LWS_SEND_BUFFER_POST_PADDING];
		unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];
		n = sprintf((char *)p, "%s",toSend.c_str());
		n = libwebsocket_write(wsi, p, n, LWS_WRITE_TEXT);
		if (n < 0) {
			fprintf(stderr, "ERROR writing to socket");
			return 1;
		}

		// Free the buffer
		delete[] buf;

		// Save new timestamp
		data->oldus = tv.tv_usec;

		libwebsocket_callback_on_writable(context, wsi);

		break;
	}

	case LWS_CALLBACK_RECEIVE:
	{
		//cout << (char *)in <<endl;
		recv_message message = {"",0,0,1,0};
		char *errorPos = 0;
		char *errorDesc = 0;
		int errorLine = 0;
		block_allocator allocator(1 << 10); // 1 KB per block
        
		json_value *root = json_parse((char*)in, &errorPos, &errorDesc, &errorLine, &allocator);
		if (root)
		{
            print(root);
            parse_json_message(root, data, &message);
            handle_message(data, &message);
		}

		// First message received is a device specs message
		if (strcmp(message.event_type.c_str(),MSG_EVENT_SPEC)==0){

			// TODO Send the Html elements to the browser based on device specifications
			sendHtmlElements(context,wsi);

			// Ask for the first available slot on this channel
			libwebsocket_callback_on_writable(context, wsi);
		}

		break;
	}
	case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
		//dump_handshake_info((struct lws_tokens *)(long)user);
		/* you could return non-zero here and kill the connection */
		break;

	case LWS_CALLBACK_CLOSED:
	{
		Engine* myEngine = Engine::instance();
		myEngine->destroyCamera(data->sessionCamera);
		delete data->sessionCanvasPixel;
		//delete data->sessionPng;
		break;
	}

	default:
		break;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/* list of supported protocols and callbacks */
struct libwebsocket_protocols protocols[] = {

	/* first protocol must always be HTTP handler */
	{
		"http-only",		/* name */
		ServerThread::callback_http,		/* callback */
		0			/* per_session_data_size */
	},

	/* websocket enabled protocol */
	{
		"porthole_websocket",
		ServerThread::callback_websocket,
		sizeof(struct per_session_data),
	},

	/* End of list */
	{
		NULL, NULL, 0	
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
ServerThread::ServerThread():
port(PORT), use_ssl(0), opts(0), n(0)
{

	// Set DATA FOLDER PATH
	string fullPath;
	DataManager::findFile("porthole/index.html", fullPath);

	DATA_PATH = fullPath.substr(0,fullPath.find_last_of("/\\"));

	// cout << "Path is: " << DATA_PATH << endl; 

	minterface="";

	//setPollPriority(Service::PollLast);

	if (!use_ssl){
		cert_path = "";
		key_path = "";
	}
	else{
		cert_path = (DATA_PATH+"/server.pem").c_str();
		key_path = (DATA_PATH+"/server.key.pem").c_str();
	}

	// Html spec parser
	xmlDoc = new TiXmlDocument( (DATA_PATH+"/porthello.xml").c_str() );
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ServerThread::threadProc(){

	// Buffer used to send/receive data using websockets
	unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 1024 +
						  LWS_SEND_BUFFER_POST_PADDING];

	cout << ">> !! Porthole initialization" << endl;

	// Initialize the websockets context
	context = libwebsocket_create_context(port, minterface, protocols,
				libwebsocket_internal_extensions,
				cert_path, key_path, -1, -1, opts);

	if (context == NULL) {
		fprintf(stderr, "libwebsocket init failed\n");
		// TODO Delete service from global service manager
		return;
	}

	// A dumb buffer to keep socket alive
	buf[LWS_SEND_BUFFER_PRE_PADDING] = 'x';

	//fprintf(stderr, " Using no-fork service loop\n");
	oldus = 0;
	n = 0;
	while (n >= 0) {
		struct timeval tv;

		gettimeofday(&tv, NULL);

		/*
		 * This server does not fork or create a thread for
		 * websocket service, it all runs in this single loop.  So,
		 * we have to give the websockets an opportunity to service
		 * "manually".
		 *
		 * If no socket is needing service, the call below returns
		 * immediately and quickly.  Negative return means we are
		 * in process of closing
		 */

		n = libwebsocket_service(context, 50);
	}

	// Destroy context when main loop ends
	libwebsocket_context_destroy(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PortholeService::PortholeService()
{
	portholeServer = new ServerThread();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PortholeService::~PortholeService(){
	portholeServer->stop();
	delete portholeServer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeService::start()
{
	portholeServer->start();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeService::setup(Setting& settings){

	cout << ">> !! Setup called" << endl;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeService::poll(){

//	cout << ">> !! Poll called" << endl;
	
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                         BASE64 ENC/DEC                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////
static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string ServerThread::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string ServerThread::base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

