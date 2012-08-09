/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Donghi Daniele		d.donghi@gmail.com
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
/*
 * this is just an example of parsing handshake headers, you don't need this
 * in your code unless you will filter allowing connections by the header
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

		if (in && strcmp((char*)in, "/favicon.ico") == 0) {
			if (libwebsockets_serve_http_file(wsi,
			     (DATA_PATH+"/favicon.ico").c_str(), "image/x-icon"))
				fprintf(stderr, "Failed to send favicon\n");
			break;
		}

		/* send the script... when it runs it'll start websockets */

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

		// Ask for the first available slot on this channel
		libwebsocket_callback_on_writable(context, wsi);

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

		//cout << base64image << endl;
		//cout << "Size is: " << base64image.length() << endl;

		// Send the base64 image
		unsigned char* buf;
		buf = new unsigned char[LWS_SEND_BUFFER_PRE_PADDING + base64image.length() + LWS_SEND_BUFFER_POST_PADDING];
		unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];
		n = sprintf((char *)p, "%s",base64image.c_str());
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
		break;

	case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
		//dump_handshake_info((struct lws_tokens *)(long)user);
		/* you could return non-zero here and kill the connection */
		break;

	case LWS_CALLBACK_CLOSED:
		break;

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
		 * This broadcasts to all dumb-increment-protocol connections
		 * at 50Hz.
		 *
		 * We're just sending a character 'x', in these examples the
		 * callbacks send their own per-connection content.
		 *
		 * You have to send something with nonzero length to get the
		 * callback actions delivered.
		 *
		 * We take care of pre-and-post padding allocation.
		 */

		// WE DO NOT USE BROADCAST TO ALL DEVICES, EACH GET IT'S OWN CAMERA STREAM
		//if (((unsigned int)tv.tv_usec - oldus) > 20000) {
		//	libwebsockets_broadcast(
		//			&protocols[PROTOCOL_WEBSOCKET],
		//			&buf[LWS_SEND_BUFFER_PRE_PADDING], 1);
		//	oldus = tv.tv_usec;
		//}

		/*
		 * This example server does not fork or create a thread for
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

