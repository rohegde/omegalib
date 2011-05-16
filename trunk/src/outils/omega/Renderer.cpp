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
#include "omega/Renderer.h"
#include "omega/Texture.h"
#include "omega/FontManager.h"
#include "omega/glheaders.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderer::Renderer():
	myTargetTexture(NULL),
	myDrawing(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::beginDraw3D(const DrawContext& context)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glLoadMatrixf(context.modelview.data());

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadMatrixf(context.projection.data());

    glMatrixMode(GL_MODELVIEW);

	glViewport(context.viewport.x(), context.viewport.y(), context.viewport.width(), context.viewport.height());

	glPushAttrib(GL_ENABLE_BIT);

	myDrawing = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::beginDraw2D(const DrawContext& context)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, context.viewport.height() - 1, 0.0);
    glScalef(1.0, -1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, context.viewport.width(), 0, context.viewport.height(), -1, 1);

    glMatrixMode(GL_MODELVIEW);

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	myDrawing = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::endDraw()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
	glPopAttrib();
	myDrawing = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::pushTransform(const AffineTransform3& transform)
{
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(transform.data());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::popTransform()
{
    glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::drawRectGradient(Vector2f pos, Vector2f size, Orientation orientation, 
	Color startColor, Color endColor, float pc)
{
	int x = pos[0];
	int y = pos[1];
	int width = size[0];
	int height = size[1];

	float s = 0;

	glColor4fv(startColor.data());
	if(orientation == Horizontal)
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
		glColor4fv(endColor.data());
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
		glColor4fv(endColor.data());
		glVertex2i(x + width, y);
		glVertex2i(x + width, y + height);
		glEnd();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::drawRect(Vector2f pos, Vector2f size, Color color)
{
	int x = pos[0];
	int y = pos[1];
	int width = size[0];
	int height = size[1];

	glColor4fv(color.data());
	glRecti(x, y, x + width, y + height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::drawRectOutline(Vector2f pos, Vector2f size, Color color)
{
	int x = pos[0];
	int y = pos[1];
	int width = size[0];
	int height = size[1];

	glColor4fv(color.data());

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
void Renderer::drawText(const String& text, Font* font, const Vector2f& position, unsigned int align) 
{ 
	Vector2f rect = font->computeSize(text);
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
void Renderer::drawRectTexture(Texture* texture, const Vector2f& position, const Vector2f size)
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

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::drawCircleOutline(Vector2f position, float radius, const Color& color, int segments)
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4fv(color.data());

	float stp = Math::Pi * 2 / segments;
	glBegin(GL_LINE_LOOP);
	for(float t = 0; t < 2 * Math::Pi; t+= stp)
	{
		float ptx = Math::sin(t) * radius + position[0];
		float pty = Math::cos(t) * radius + position[1];
		glVertex2f(ptx, pty);
	}
	glEnd();
	glPopAttrib();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::drawWireSphere(const Color& color, int segments, int slices)
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4fv(color.data());

	float stp = Math::Pi * 2 / segments;
	float stp2 = Math::Pi / (slices + 1);
	for(float g = 0; g <= Math::Pi; g+= stp2)
	{
		glBegin(GL_LINE_LOOP);
		for(float t = 0; t < 2 * Math::Pi; t+= stp)
		{
			float ptx = Math::sin(t) * Math::sin(g);
			float pty = Math::cos(t) * Math::sin(g);
			float ptz = Math::cos(g);
			glVertex3f(ptx, pty, ptz);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		for(float t = 0; t < 2 * Math::Pi; t+= stp)
		{
			float ptz = Math::sin(t) * Math::sin(g);
			float pty = Math::cos(t) * Math::sin(g);
			float ptx = Math::cos(g);
			glVertex3f(ptx, pty, ptz);
		}
		glEnd();
	}

	glPopAttrib();
}
