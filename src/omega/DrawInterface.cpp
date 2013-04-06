/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/DrawInterface.h"
#include "omega/DisplaySystem.h"
#include "omega/Texture.h"
#include "omega/glheaders.h"
#include "omega/SystemManager.h"

#include "FTGL/ftgl.h"

using namespace omega;


///////////////////////////////////////////////////////////////////////////////////////////////////
DrawInterface::DrawInterface():
	//myTargetTexture(NULL),
	myDrawing(false),
	myDefaultFont(NULL),
	myContext(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::beginDraw3D(const DrawContext& context)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glLoadMatrixd(context.modelview.data());

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadMatrixd(context.projection.data());

    glMatrixMode(GL_MODELVIEW);

	glViewport(context.viewport.x(), context.viewport.y(), context.viewport.width(), context.viewport.height());

	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	myDrawing = true;
	myContext = &context;

	//int maxVaryingFloats = 0;
	//glGetIntegerv(GL_MAX_VARYING_FLOATS, &maxVaryingFloats);
	//ofmsg("OpenGL capabilities: max varying floats = %1%", %maxVaryingFloats);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::beginDraw2D(const DrawContext& context)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

	int left = context.tile->offset[0];
	int right = left + context.tile->pixelSize[0];
	int top = context.tile->offset[1];
	int bottom = top + context.tile->pixelSize[1];

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(left, right, bottom, top, -1, 1);

    glMatrixMode(GL_MODELVIEW);

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	myDrawing = true;
	myContext = &context;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::endDraw()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
	glPopAttrib();
	myDrawing = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::setGlColor(const Color& col)
{
	glColor4f(
		col[0] * myBrush.color[0], 
		col[1] * myBrush.color[1],
		col[2] * myBrush.color[2],
		col[3] * myBrush.color[3]
	);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::pushTransform(const AffineTransform3& transform)
{
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixd(transform.data());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::popTransform()
{
    glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::drawRectGradient(Vector2f pos, Vector2f size, Orientation orientation, 
	Color startColor, Color endColor, float pc)
{
	int x = pos[0];
	int y = pos[1];
	int width = size[0];
	int height = size[1];

	float s = 0;

	setGlColor(startColor);
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
		setGlColor(endColor);
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
		setGlColor(endColor);
		glVertex2i(x + width, y);
		glVertex2i(x + width, y + height);
		glEnd();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::drawRect(Vector2f pos, Vector2f size, Color color)
{
	int x = pos[0];
	int y = pos[1];
	int width = size[0];
	int height = size[1];

	glColor4f(color[0], color[1], color[2], color[3]);
	glRecti(x, y, x + width, y + height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::drawRectOutline(Vector2f pos, Vector2f size, Color color)
{
	int x = pos[0];
	int y = pos[1];
	int width = size[0];
	int height = size[1];

	setGlColor(color);

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
void DrawInterface::drawText(const String& text, Font* font, const Vector2f& position, unsigned int align, Color color) 
{ 
	setGlColor(color);

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
void DrawInterface::drawRectTexture(Texture* texture, const Vector2f& position, const Vector2f size, uint flipFlags, const Vector2f& minUV, const Vector2f& maxUV)
{
	glEnable(GL_TEXTURE_2D);
	texture->bind(GpuContext::TextureUnit0);

	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);

	float x = position[0];
	float y = position[1];

	float width = size[0];
	float height = size[1];

	float minx = minUV.x();
	float miny = minUV.y();
	float maxx = maxUV.x();
	float maxy = maxUV.y();

	if((flipFlags & FlipX) == FlipX)
	{
		minx = 1;
		maxx = 0;
	}
	if((flipFlags & FlipY) == FlipY)
	{
		miny = 1;
		maxy = 0;
	}

	glBegin(GL_TRIANGLE_STRIP);

	glTexCoord2f(minx, maxy);
	glVertex2f(x, y);

	glTexCoord2f(maxx, maxy);
	glVertex2f(x + width, y);

	glTexCoord2f(minx, miny);
	glVertex2f(x, y + height);

	glTexCoord2f(maxx, miny);
	glVertex2f(x + width, y + height);

	glEnd();

	texture->unbind();
	glDisable(GL_TEXTURE_2D);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::drawCircleOutline(Vector2f position, float radius, const Color& color, int segments)
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	setGlColor(color);

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
void DrawInterface::drawCircle(Vector2f position, float radius, const Color& color, int segments)
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	setGlColor(color);

	float stp = Math::Pi * 2 / segments;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(position[0], position[1]);
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
void DrawInterface::drawWireSphere(const Color& color, int segments, int slices)
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	setGlColor(color);

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

///////////////////////////////////////////////////////////////////////////////////////////////////
Font* DrawInterface::createFont(omega::String fontName, omega::String filename, int size)
{
	Font::lock();
	//if(getFont(fontName))
	//{
	//	ofwarn("FontManager::createFont: font '%1%' already existing.", %fontName);
	//	return getFont(fontName);
	//}

	DataManager* dm = SystemManager::instance()->getDataManager();
	DataInfo info = dm->getInfo(filename);
	oassert(!info.isNull());
	oassert(info.local);

	FTFont* fontImpl = new FTTextureFont(info.path.c_str());

	//delete data;

	if(fontImpl->Error())
	{
		ofwarn("Font %1% failed to open", %filename);
		delete fontImpl;
		return NULL;
	}

	if(!fontImpl->FaceSize(size))
	{
		ofwarn("Font %1% failed to set size %2%", %filename %size);
		delete fontImpl;
	}

	Font* font = new Font(fontImpl);

	myFonts[fontName] = font;
	Font::unlock();
	return font;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Font* DrawInterface::getFont(omega::String fontName)
{
	if(myFonts.find(fontName) != myFonts.end())
		return myFonts[fontName];

	ofmsg("Creating Font %1%", %fontName);
	Vector<String> args = StringUtils::split(fontName);
	if(args.size() < 2)
	{
		owarn("Invalid font creation arguments");
		return NULL;
	}
	String fontFile = args[0];
	int fontSize = boost::lexical_cast<int>(args[1]);
	return createFont(fontName, fontFile, fontSize);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::fillTexture(TextureSource* texture)
{
	myBrush.texture = texture->getTexture(*myContext);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::textureFlip(uint flipflags)
{
	myBrush.flip = flipflags;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::textureRegion(float su, float sv, float eu, float ev)
{
	myBrush.startuv = Vector2f(su, sv);
	myBrush.enduv = Vector2f(eu, ev);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawInterface::rect(float x, float y, float width, float height)
{
	if(!myBrush.texture.isNull())
	{
		glColor4f(myBrush.color[0], myBrush.color[1], myBrush.color[2], myBrush.color[3]);
		drawRectTexture(myBrush.texture,
			Vector2f(x, y),
			Vector2f(width, height),
			myBrush.flip,
			myBrush.startuv,
			myBrush.enduv);
	}
}
