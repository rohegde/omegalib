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
#include "meshviewer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerUI::initialize(MeshViewerClient* client)
{
	myClient = client;

	UIManager* ui = client->getEngine()->getUIManager();
	ui->setEventHandler(this);

	//! Load and set default font.
	FontManager* fm = client->getEngine()->getFontManager();
	Font* defaultFont = fm->getFont("default");
	ui->setDefaultFont(defaultFont);

	WidgetFactory* wf = ui->getWidgetFactory();
	Container* root = ui->getRootContainer();
	root->setLayout(Container::LayoutVertical);

	Container* entityButtons = wf->createContainer("entities", root, Container::LayoutHorizontal);

	myUserUI = wf->createContainer("userUI", root, Container::LayoutHorizontal);
	myUserUI->setVerticalAlign(Container::AlignBottom);

	// Setup ui layout using from config file sections.
	Config* cfg = SystemManager::instance()->getAppConfig();
	if(cfg->exists("config/ui/entityButtons"))
	{
		entityButtons->load(cfg->lookup("config/ui/entityButtons"));
	}
	if(cfg->exists("config/ui/root"))
	{
		root->load(cfg->lookup("config/ui/root"));
	}

	// Add buttons for each entity
	for(int i = 0; i < myClient->getNumEntities(); i++)
	{
		Entity* e = myClient->getEntity(i);
		Button* btn = wf->createButton(e->getName(), entityButtons);
		//btn->setAutosize(true);
		myEntityButtons.push_back(btn);
	}

#ifdef OMEGA_USE_OPENNI
	depthImage = wf->createImage("kinectDepthBuffer", entityButtons);
	depthImage->setHeight(240);
	depthImage->setWidth(320);
#endif

	texture = NULL;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerUI::handleUIEvent(const UIEvent& evt)
{
	for(int i = 0; i < myClient->getNumEntities(); i++)
	{
		if(myEntityButtons[i] == evt.source)
		{
			myClient->setVisibleEntity(i);
			break;
		}
	}
	for(int j = 0; j < myUserUI->getNumChildren(); j++)
	{
		Widget* w = myUserUI->getChildByIndex(j);
		if(w == evt.source)
		{
			int userId = atoi(w->getName().c_str());
			ofmsg("Active User id: %1%", %userId);
			myClient->setActiveUser(userId);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef OMEGA_USE_OPENNI
void MeshViewerUI::updateKinectTexture(OpenNIService* svc)
{
	if( texture == NULL ) 
	{
		texture = new Texture();
		texture->initialize((byte*)svc->getDepthImageData(), svc->getImageDataWidth(), svc->getImageDataHeight());
		depthImage->setTexture(texture);
	}
	
	texture->setDirty();
	texture->refresh();
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerUI::onTraceUser(int userId)
{
	WidgetFactory* wf = myClient->getEngine()->getUIManager()->getWidgetFactory();
	String name = oformat("%1%", %userId);
	String label = oformat("User %1%", %userId);
	Button* btn = wf->createButton(name, myUserUI);
	btn->setText(label);

	// Set button color through user id
#ifdef OMEGA_USE_OPENNI
	OpenNIService* svc = myClient->getServiceManager()->findService<OpenNIService>("OpenNIService");
	Color col = svc->getUserColor(userId);
	btn->setColor(col.scale(0.2f));
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerUI::onUntraceUser(int userId)
{
	String name = oformat("user %1%", %userId);
	myUserUI->removeChild(name);
}
