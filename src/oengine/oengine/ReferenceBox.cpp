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
#include "oengine/ReferenceBox.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
ReferenceBox::ReferenceBox()
{
	myPrimaryLineColor = Color(0, 0, 0, 1);
	myPrimaryLineInterval = 0.5f;

	//myPrimaryLineColor = Color(0.8, 0.8, 1, 1);

	mySideColor[Back]   =  Color(0.3f, 0.2f, 0.2f, 0.2f);
	mySideColor[Front]  =  Color(0.3f, 0.2f, 0.2f, 0.2f);
	mySideColor[Left]   =  Color(0.2f, 0.3f, 0.2f, 0.2f);
	mySideColor[Right]  =  Color(0.2f, 0.3f, 0.2f, 0.2f);
	mySideColor[Bottom] =  Color(0.2f, 0.2f, 0.3f, 1.0f);
	mySideColor[Top]    =  Color(0.2f, 0.2f, 0.3f, 1.0f);

	for(int i = 0; i < NumSides; i++) mySideEnabled[i] = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ReferenceBox::render(SceneNode* node, RenderState* state)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	if(state->isFlagSet(RenderPass::RenderOpaque))
	{
		int gridLinesX = (int)(mySize[0] / myPrimaryLineInterval);
		int gridLinesY = (int)(mySize[1] / myPrimaryLineInterval);
		int gridLinesZ = (int)(mySize[2] / myPrimaryLineInterval);

		drawReferencePlane(myBox.getCorner(AlignedBox3::FAR_LEFT_TOP), myBox.getCorner(AlignedBox3::FAR_RIGHT_BOTTOM), AxisZ, mySideColor[Back]);
		drawReferenceGrid(myBox.getCorner(AlignedBox3::FAR_LEFT_TOP), myBox.getCorner(AlignedBox3::FAR_RIGHT_BOTTOM), AxisX, myPrimaryLineColor, gridLinesX);
		drawReferenceGrid(myBox.getCorner(AlignedBox3::FAR_LEFT_TOP), myBox.getCorner(AlignedBox3::FAR_RIGHT_BOTTOM), AxisY, myPrimaryLineColor, gridLinesY);

		drawReferencePlane(myBox.getCorner(AlignedBox3::FAR_LEFT_TOP), myBox.getCorner(AlignedBox3::NEAR_LEFT_BOTTOM), AxisX, mySideColor[Left]);
		drawReferenceGrid(myBox.getCorner(AlignedBox3::FAR_LEFT_TOP), myBox.getCorner(AlignedBox3::NEAR_LEFT_BOTTOM), AxisY, myPrimaryLineColor, gridLinesY);
		drawReferenceGrid(myBox.getCorner(AlignedBox3::FAR_LEFT_TOP), myBox.getCorner(AlignedBox3::NEAR_LEFT_BOTTOM), AxisZ, myPrimaryLineColor, gridLinesZ);

		drawReferencePlane(myBox.getCorner(AlignedBox3::FAR_RIGHT_TOP), myBox.getCorner(AlignedBox3::NEAR_RIGHT_BOTTOM), AxisX, mySideColor[Right]);
		drawReferenceGrid(myBox.getCorner(AlignedBox3::FAR_RIGHT_TOP), myBox.getCorner(AlignedBox3::NEAR_RIGHT_BOTTOM), AxisY, myPrimaryLineColor, gridLinesY);
		drawReferenceGrid(myBox.getCorner(AlignedBox3::FAR_RIGHT_TOP), myBox.getCorner(AlignedBox3::NEAR_RIGHT_BOTTOM), AxisZ, myPrimaryLineColor, gridLinesZ);

		drawReferencePlane(myBox.getCorner(AlignedBox3::FAR_LEFT_BOTTOM), myBox.getCorner(AlignedBox3::NEAR_RIGHT_BOTTOM), AxisY, mySideColor[Bottom]);
		drawReferenceGrid(myBox.getCorner(AlignedBox3::FAR_LEFT_BOTTOM), myBox.getCorner(AlignedBox3::NEAR_RIGHT_BOTTOM), AxisX, myPrimaryLineColor, gridLinesX);
		drawReferenceGrid(myBox.getCorner(AlignedBox3::FAR_LEFT_BOTTOM), myBox.getCorner(AlignedBox3::NEAR_RIGHT_BOTTOM), AxisZ, myPrimaryLineColor, gridLinesZ);

		drawReferencePlane(myBox.getCorner(AlignedBox3::FAR_LEFT_TOP), myBox.getCorner(AlignedBox3::NEAR_RIGHT_TOP), AxisY, mySideColor[Top]);
		drawReferenceGrid(myBox.getCorner(AlignedBox3::FAR_LEFT_TOP), myBox.getCorner(AlignedBox3::NEAR_RIGHT_TOP), AxisX, myPrimaryLineColor, gridLinesX);
		drawReferenceGrid(myBox.getCorner(AlignedBox3::FAR_LEFT_TOP), myBox.getCorner(AlignedBox3::NEAR_RIGHT_TOP), AxisZ, myPrimaryLineColor, gridLinesZ);
	}
	glEnable(GL_LIGHTING);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ReferenceBox::drawReferencePlane(
	const Vector3f& min, const Vector3f& max, Axis normal, const Color& color)
{
	Vector3f v1;
	Vector3f v2;
	Vector3f v3;
	Vector3f v4;

	switch(normal)
	{
	case AxisX:
		v1 = Vector3f(min[0], min[1], min[2]);
		v2 = Vector3f(min[0], max[1], min[2]);
		v3 = Vector3f(min[0], min[1], max[2]);
		v4 = Vector3f(min[0], max[1], max[2]);
		break;
	case AxisY:
		v1 = Vector3f(min[0], min[1], min[2]);
		v2 = Vector3f(min[0], min[1], max[2]);
		v3 = Vector3f(max[0], min[1], min[2]);
		v4 = Vector3f(max[0], min[1], max[2]);
		break;
	case AxisZ:
		v1 = Vector3f(min[0], min[1], min[2]);
		v2 = Vector3f(min[0], max[1], min[2]);
		v3 = Vector3f(max[0], min[1], min[2]);
		v4 = Vector3f(max[0], max[1], min[2]);
		break;
	}

	glColor4fv(color.data());
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(v1.data());
	glVertex3fv(v2.data());
	glVertex3fv(v3.data());
	glVertex3fv(v4.data());
	glEnd();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ReferenceBox::drawReferenceGrid(
	const Vector3f& min, const Vector3f& max, Axis normal, const Color& color, int lines)
{
	glDisable(GL_DEPTH_TEST);

	float vmin;
	float vmax;
	float vstep;

	switch(normal)
	{
	case AxisX:
		vmin = min[0];
		vmax = max[0];
		vstep = (vmax - vmin) / lines;
		break;
	case AxisY:
		vmin = min[1];
		vmax = max[1];
		vstep = (vmax - vmin) / lines;
		break;
	case AxisZ:
		vmin = min[2];
		vmax = max[2];
		vstep = (vmax - vmin) / lines;
		break;
	}

	if(vmin > vmax)
	{
		float tmp = vmin;
		vmin = vmax;
		vmax = tmp;
		vstep = -vstep;
	}


	glColor4fv(color.data());
	glBegin(GL_LINES);
	for(float v = vmin; v < vmax; v+= vstep)
	{
		switch(normal)
		{
		case AxisX:
			glVertex3f(v, min[1], min[2]);
			glVertex3f(v, max[1], max[2]);
			break;
		case AxisY:
			glVertex3f(min[0], v, min[2]);
			glVertex3f(max[0], v, max[2]);
			break;
		case AxisZ:
			glVertex3f(min[0], min[1], v);
			glVertex3f(max[0], max[1], v);
			break;
		}
	}
	glEnd();

	glEnable(GL_DEPTH_TEST);
}
