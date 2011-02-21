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
#include "omega/ui/Painter.h"
#include "omega/Texture.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
Painter::Painter():
	myTargetTexture(NULL),
	myDrawing(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::beginDraw()
{
	myDrawing = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::endDraw()
{
	myDrawing = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::drawRectGradient(Vector2i pos, Vector2i size, Widget::Orientation orientation, 
	Color startColor, Color endColor, float pc)
{
	int x = pos[0];
	int y = pos[1];
	int width = size[0];
	int height = size[1];

	float s = 0;

	glColor4fv(startColor.begin());
	if(orientation == Widget::Horizontal)
	{
		// draw full color portion
		s = int(height * pc);
		glRecti(x, y, x + width, y + s);
		y += s;
		height -= s;
		// draw gradient portion
		glBegin(GL_QUADS);
		glVertex2i(x, y);
		glVertex2i(x + width, y);
		glColor4fv(endColor.begin());
		glVertex2i(x + width, y + height);
		glVertex2i(x, y + height);
		glEnd(); 
	}
	else
	{
		// draw full color portion
		s = int(width * pc);
		glRecti(x, y, x + s, y + height);
		x += s;
		width -= s;
		// draw gradient portion
		glBegin(GL_QUADS);
		glVertex2i(x, y + height);
		glVertex2i(x, y);
		glColor4fv(endColor.begin());
		glVertex2i(x + width, y);
		glVertex2i(x + width, y + height);
		glEnd();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::drawRect(Vector2i pos, Vector2i size, Color color)
{
	int x = pos[0];
	int y = pos[1];
	int width = size[0];
	int height = size[1];

	glColor4fv(color.begin());
	glRecti(x, y, x + width, y + height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::drawRectOutline(Vector2i pos, Vector2i size, Color color)
{
	int x = pos[0];
	int y = pos[1];
	int width = size[0];
	int height = size[1];

	glColor4fv(color.begin());

	glBegin(GL_LINES);

	glVertex2f(x, y);
	glVertex2f(x + width, y);

	glVertex2f(x, y + height);
	glVertex2f(x + width, y + height);

	glVertex2f(x, y);
	glVertex2f(x, y + height);

	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);

	glEnd();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::drawText(const String& text, Font* font, const Vector2f& position, unsigned int align) 
{ 
	Vector2i rect = font->computeSize(text);
	float x, y;

	if(align & Font::HALeft) x = position[0];
	else if(align & Font::HARight) x = position[0] - rect[0];
	else x = position[0] - rect[0] / 2;

	if(align & Font::VATop) y = -position[1] - rect[1];
	else if(align & Font::VABottom) y = -position[1];
	else y = -position[1] - rect[1] / 2;

	font->render(text, x, y); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::drawRectTexture(Texture* texture, const Vector2i& position, const Vector2i size)
{
	glEnable(GL_TEXTURE_2D);
	texture->bind(GpuManager::TextureUnit0);

	float x = position[0];
	float y = position[1];

	float width = size[0];
	float height = size[1];

	glColor4ub(255, 255, 255, 255);

	glBegin(GL_TRIANGLE_STRIP);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(x, y);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(x + width, y);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(x, y + height);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(x + width, y + height);

	glEnd();

	texture->unbind();
	glDisable(GL_TEXTURE_2D);
}
