/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Implementatio of GfxU
 *********************************************************************************************************************/
#include "omega/SystemManager.h"
#include "omega/DisplaySystem.h"

#include "outk/gfx/GfxUtils.h"

using namespace omega;
using namespace outk::gfx;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void teapot(GLint grid, GLdouble scale, GLenum type);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::beginOverlayMode(const DrawContext& context)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, context.viewportHeight - 1, 0.0);
    glScalef(1.0, -1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, context.viewportWidth, 0, context.viewportHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::endOverlayMode()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::drawText(int x, int y, const char* s, Font f)
{
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
void GfxUtils::drawVGradient(Vector2i pos, Vector2i size, Vector4f startColor, Vector4f endColor, float pc)
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

////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::drawHGradient(Vector2i pos, Vector2i size, Vector4f startColor, Vector4f endColor, float pc)
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

////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::drawBox(Vector2i pos, Vector2i size, int dx, Color color)
{
	//strokeWeight(1);
	//stroke(c, sa);
	//line(x, y - dx, x + width, y - dx);
	//line(x, y + height + dx, x + width, y + height + dx);
	//line(x - dx, y, x - dx, y + height);
	//line(x + width + dx, y, x + width + dx, y + height);
}