/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Implementatio of GfxUtils
 *********************************************************************************************************************/
#include "omega/Color.h"
#include "omega/GfxUtils.h"
#include "omega/SystemManager.h"
#include "omega/DisplaySystem.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void teapot(GLint grid, GLdouble scale, GLenum type);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::getViewRay(float viewX, float viewY, omega::Vector3f* origin, omega::Vector3f* direction)
{
	GLdouble modelview[16];
	GLdouble projection[16];
	GLint viewport[4];

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	double mx1, my1, mz1, mx2, my2, mz2;
	gluUnProject(viewX, viewport[3] - viewY, 0, modelview, projection, viewport, &mx1, &my1, &mz1);
	gluUnProject(viewX, viewport[3] - viewY, 1, modelview, projection, viewport, &mx2, &my2, &mz2);

	origin->x() = mx1;
	origin->y() = my1;
	origin->z() = mz1;

	direction->x() = (mx1 - mx2);
	direction->y() = (my1 - my2);
	direction->z() = (mz1 - mz2);

	direction->normalize();
}

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
void GfxUtils::drawVGradient(Vector2i pos, Vector2i size, Color startColor, Color endColor, float pc)
{
	int x = pos.x();
	int y = pos.y();
	int width = size.x();
	int height = size.y();

	float h1 = int(height * pc);

	glColor4ubv(startColor);
	glRecti(x, y, x + width, y + h1);

	y += h1;
	height -= h1;

	glBegin(GL_QUADS);

	glVertex2i(x, y);
	glVertex2i(x + width, y);

	glColor4ubv(endColor);
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

	glColor4ubv(startColor);
	glRecti(x, y, x + w1, y + height);

	x += w1;
	width -= w1;

	glBegin(GL_QUADS);

	glVertex2i(x, y + height);
	glVertex2i(x, y);

	glColor4ubv(endColor);
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
