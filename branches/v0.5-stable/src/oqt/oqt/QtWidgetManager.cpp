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
#include <QApplication>
#include <QUiLoader>
#include <QPainter>
#include <QImage>
#include <QFile>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "oqt/QtWidget.h"
#include "oqt/QtWidgetManager.h"
#include "omega/scene.h"
#include "omega/SystemManager.h"
#include "omega/DataManager.h"
#include "omega/StringUtils.h"

using namespace oqt;

///////////////////////////////////////////////////////////////////////////////////////////////////
QtWidgetManager::QtWidgetManager(): 
	//myEngine(engine),
	myFocusedWidget(NULL)
{
	char* argv = "";
	int argcp = 1;
	myQApp = onew(QApplication)(argcp, &argv);

	myWidgetScene = onew(QGraphicsScene)(this);
	myWidgetView = onew(QGraphicsView)(myWidgetScene);
	myWidgetView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
		
	// for debugging, show Qt's window with
	// mWidgetView->show();

	// We need to manually tell the scene that a visible view is watching.
	// A QGraphicsView doesn't do that when it is not visible as 
	// a widget on screen.
	QEvent wsce(QEvent::WindowActivate);
	QApplication::sendEvent(myWidgetScene, &wsce);
		
	connect(myWidgetScene, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(setUiDirty()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
QtWidgetManager::~QtWidgetManager()
{
	QEvent wsce(QEvent::WindowDeactivate);
	QApplication::sendEvent(myWidgetScene, &wsce);
		
	// Note: For ~QGraphicsScene to be able to run, qApp must still be valid.
	odelete(myWidgetView);
	odelete(myWidgetScene);
	odelete(myQApp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//QtWidget* QtWidgetManager::createWidget(const String& name, Container* container, QWidget* widget)
//{
//	oassert(container != NULL);
//	oassert(widget != NULL);
//
//	QtWidget* w = onew(QtWidget)(this, name);
//	w->setWidget(widget);
//	container->addChild(w);
//
//	return w;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
QtWidget* QtWidgetManager::loadUiFile(const String& filename)
{
	QUiLoader uiLoader;
	
	DataManager* dm = SystemManager::instance()->getDataManager();
	DataInfo cfgInfo = dm->getInfo(filename);
	if(!cfgInfo.isNull())
	{
		QFile file(cfgInfo.path.c_str());
		file.open(QFile::ReadOnly);
	
		QWidget* generatedWidget = uiLoader.load(&file, NULL);
		generatedWidget->setAttribute(Qt::WA_TranslucentBackground);

	
		file.close();

		QtWidget* w = onew(QtWidget)(this, filename);
		w->setWidget(generatedWidget);

		return w;
	}
	else
	{
		ofwarn("PythonInterpreter: script not found: %1%", %filename);
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void QtWidgetManager::setActiveWidget(QWidget *aWidget)
{
	oassert(myWidgetScene);
		
	if (myTopLevelWidget && myTopLevelWidget != aWidget)
	{
		if (myFocusedWidget)
		{
			QEvent foe(QEvent::FocusOut);
			QApplication::sendEvent(myFocusedWidget, &foe);
			myFocusedWidget = NULL;
		}

		myWidgetScene->clear();
		myTopLevelWidget = NULL;
	}
	
	myWidgetScene->addWidget(aWidget);
	myTopLevelWidget = aWidget;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void QtWidgetManager::renderIntoTexture(Texture* aTexture)
{
	oassert(aTexture != NULL);

	myWidgetView->setGeometry(QRect(0, 0, aTexture->getWidth(), aTexture->getHeight()));

	byte* texData = aTexture->getData();

	// render into texture buffer
	QImage textureImg(texData, aTexture->getWidth(), aTexture->getHeight(), QImage::Format_ARGB32);
	textureImg.fill(0);
		
	QPainter painter(&textureImg);
	myWidgetView->render(&painter, QRect(QPoint(0, 0), myWidgetView->size()), QRect(QPoint(0, 0), myWidgetView->size()));

	aTexture->setDirty();
	aTexture->refresh();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void QtWidgetManager::setUiDirty(bool aDirty)
{
	myUiDirty = aDirty;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void QtWidgetManager::initialize()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool QtWidgetManager::handleEvent(const Event& evt, UpdateContext& context)
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool QtWidgetManager::handleEvent(const Event& evt, DrawContext& context)
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void QtWidgetManager::update(const UpdateContext& context)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void QtWidgetManager::draw(const DrawContext& context)
{
}
