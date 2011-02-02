/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#include "omega/Color.h"
#include "omega/GfxUtils.h"
#include "omega/SystemManager.h"
#include "omega/DisplaySystem.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void teapot(GLint grid, GLdouble scale, GLenum type);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLenum getLightGLEnum(int lightId)
{
	switch(lightId)
	{
	case 0:
		return GL_LIGHT0;
	case 1:
		return GL_LIGHT1;
	case 2:
		return GL_LIGHT2;
	case 3:
		return GL_LIGHT3;
	case 4:
		return GL_LIGHT4;
	case 5:
		return GL_LIGHT5;
	case 6:
		return GL_LIGHT6;
	case 7:
		return GL_LIGHT7;
	}
	return GL_LIGHT0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::setLightingEnabled(bool value)
{
	if(value)
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::setLightEnabled(int lightId, bool value)
{
	GLenum glid = getLightGLEnum(lightId);
	if(value)
	{
		glEnable(glid);
	}
	else
	{
		glDisable(glid);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::setLightPosition(int lightId, const Vector3f& pos)
{
	GLenum glid = getLightGLEnum(lightId);
	glLightfv(glid, GL_POSITION, pos.begin());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::setLightColor(int lightId, const Color& color)
{
	GLenum glid = getLightGLEnum(lightId);
	glLightfv(glid, GL_DIFFUSE, color.begin());
	glLightfv(glid, GL_SPECULAR, color.begin());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Ray GfxUtils::getViewRay(const Vector2f& pos)
{
	Matrix4f modelview;
	Matrix4f projection;
	GLint viewport[4];

	glGetFloatv( GL_MODELVIEW_MATRIX, modelview.begin() );
	glGetFloatv( GL_PROJECTION_MATRIX, projection.begin() );
	glGetIntegerv( GL_VIEWPORT, viewport );

	Recti vp;
	vp[0][0] = viewport[0];
	vp[0][1] = viewport[1];
	vp[1][0] = viewport[2];
	vp[1][1] = viewport[3];

	return Math::unproject(pos, modelview, projection, vp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::beginOverlayMode(const DrawContext& context)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, context.viewportWidth, 0, context.viewportHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, context.viewportHeight - 1, 0.0);
    glScalef(1.0, -1.0, 1.0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::endOverlayMode()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::drawText(int x, int y, const char* s, Font f)
{
#ifdef OMEGA_USE_DISPLAY_GLUT
	void* font = NULL;

	int iy = 0;
	switch(f)
	{
	case Helvetica12: 
		font = GLUT_BITMAP_HELVETICA_12; iy = 12;
		break;
	case Helvetica18: 
		font = GLUT_BITMAP_HELVETICA_18; iy = 18;
		break;
	}

	glRasterPos2f(x, y);
	int len = (int) strlen(s);
	for (int i = 0; i < len; i++) 
	{
		if(s[i] == '\n')
		{
			y += iy;
			glRasterPos2f(x, y);
		}
		else
		{
			glutBitmapCharacter(font, s[i]);
		}
	}
#else
	#error Glut support needed for this method
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::drawSolidTeapot(float scale)
{
  teapot(14, scale, GL_FILL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::drawWireTeapot(float scale)
{
  teapot(10, scale, GL_LINE);
}
  
////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::drawVGradient(Vector2i pos, Vector2i size, Color startColor, Color endColor, float pc)
{
	int x = pos.x();
	int y = pos.y();
	int width = size.x();
	int height = size.y();

	float h1 = int(height * pc);

	glColor4fv(startColor.begin());
	glRecti(x, y, x + width, y + h1);

	y += h1;
	height -= h1;

	glBegin(GL_QUADS);

	glVertex2i(x, y);
	glVertex2i(x + width, y);

	glColor4fv(endColor.begin());
	glVertex2i(x + width, y + height);
	glVertex2i(x, y + height);

	glEnd(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::drawHGradient(Vector2i pos, Vector2i size, Color startColor, Color endColor, float pc)
{
	int x = pos.x();
	int y = pos.y();
	int width = size.x();
	int height = size.y();

	float w1 = int(width * pc);

	glColor4fv(startColor.begin());
	glRecti(x, y, x + w1, y + height);

	x += w1;
	width -= w1;

	glBegin(GL_QUADS);

	glVertex2i(x, y + height);
	glVertex2i(x, y);

	glColor4fv(endColor.begin());
	glVertex2i(x + width, y);
	glVertex2i(x + width, y + height);

	glEnd();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::drawDRect(omega::Vector2f pos, omega::Vector2f size, int d)
{
	float x = pos[0];
	float y = pos[1];

	float w = size[0];
	float h = size[1];

	glBegin(GL_LINE_LOOP);

	glVertex2f(x, y - d);
	glVertex2f(x + w, y - d);
	glVertex2f(x + w + d, y);
	glVertex2f(x + w + d, y + h);
	glVertex2f(x + w, y + h + d);
	glVertex2f(x, y + h + d);
	glVertex2f(x - d, y + h);
	glVertex2f(x - d, y);
	glVertex2f(x, y - d);

	glEnd();
}
