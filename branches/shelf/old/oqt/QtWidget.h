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
#ifndef __OQT_WIDGET_H__
#define __OQT_WIDGET_H__

#define QT_NO_KEYWORDS  // Avoid double definition warning of foreach macro
#include <QWidget>

#include "oqt/oqtbase.h"
#include "omega/Texture.h"
#include "omega/ui/Widget.h"

namespace oqt
{
	using namespace omega;
	using namespace omega::ui;

	class QtWidgetManager;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OQT_API QtWidget: public Widget
	{
	public:
		QtWidget(QtWidgetManager* owner, UIManager* mng);
		~QtWidget();

		QWidget* getWidget();
		void setWidget(QWidget* value);

	protected:
		virtual void renderContent();

	private:
		QtWidgetManager* myOwner;
		QWidget* myWidget;
		Texture* myTexture;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline QWidget* QtWidget::getWidget()
	{ return myWidget; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline void QtWidget::setWidget(QWidget* value)
	{ myWidget = value; }
};
#endif