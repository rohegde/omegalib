/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "outk/ui/Image.h"
#include "outk/ui/UIManager.h"

using namespace omega;
using namespace outk::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Image::Image(omega::String name):
	Widget(name),
	myTexture(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Image::~Image()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Image::draw()
{
	Widget::draw();

	if(myTexture != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myTexture->getGLTexture());

		float x = myPosition.x();
		float y = myPosition.y();

		float width = mySize.x();
		float height = mySize.y();

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
		glDisable(GL_TEXTURE_2D);
	}
}
