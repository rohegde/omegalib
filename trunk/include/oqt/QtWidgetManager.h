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
 *-------------------------------------------------------------------------------------------------
 * Part of code taken from Cutexture
 * Copyright (c) 2010 Markus Weiland, Kevin Lang
 *************************************************************************************************/
#ifndef __QT_SERVER_H__
#define __QT_SERVER_H__

#include <QObject>
#include <QApplication>

#include "oqt/oqtbase.h"
#include "oqt/QtWidget.h"
#include "omega/EngineClient.h"
#include "omega/Texture.h"
#include "omega/ui/Container.h"

class QGraphicsView;
class QGraphicsScene;

namespace oqt
{
	using namespace omega;
	using namespace omega::ui;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OQT_API QtWidgetManager: public QObject
	{
	public:
		QtWidgetManager();
		~QtWidgetManager();

		//EngineClient* getEngine();

		//QtWidget* createWidget(const String& name, Container* container, QWidget* widget);
		QtWidget* loadUiFile(const String& filename);
		void setActiveWidget(QWidget *aWidget);
		void setUiDirty(bool aDirty);
		void renderIntoTexture(Texture* aTexture);

		void initialize();
		bool handleEvent(const Event& evt, UpdateContext& context);
		bool handleEvent(const Event& evt, DrawContext& context);
		void update(const UpdateContext& context);
		void draw(const DrawContext& context);

	private:
		QApplication* myQApp;

		//EngineClient* myEngine;

		/** Scene which contains all the user interface widgets
		 * as QGraphicsWidget items. */
		QGraphicsScene* myWidgetScene;

		/** View which visualizes the scene containing UI widgets. */
		QGraphicsView* myWidgetView;

		/** Top-level widget in the graphics scene. */
		QWidget* myTopLevelWidget;

		/** Pointer to the widget currently possessing keyboard focus. 
		 * Null if no focus set. */
		QWidget* myFocusedWidget;

		/** Indicates if the UI texture needs to be updated due to a  
		 * change in mWidgetScene. */
		bool myUiDirty;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//inline EngineClient* QtClient::getEngine()
	//{ return myEngine; }
};
#endif