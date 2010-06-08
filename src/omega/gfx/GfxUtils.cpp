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
#include "gfx/GfxUtils.h"
#include "SystemManager.h"
#include "DisplaySystem.h"

using namespace omega;
using namespace omega::gfx;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::Begin2DCoords(int width, int height)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, height - 1, 0.0);
    glScalef(1.0, -1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);

    glMatrixMode(GL_MODELVIEW);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::Begin2DCoords()
{
	SystemManager* sys = SystemManager::GetInstance();
	DisplaySystem* disp = sys->GetDisplaySystem();

	int width = disp->GetValue(DisplaySystem::LocalWidth);
	int height = disp->GetValue(DisplaySystem::LocalHeight);

	Begin2DCoords(width, height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::End2DCoords()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GfxUtils::Print(int x, int y, const char* s, Font f)
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
