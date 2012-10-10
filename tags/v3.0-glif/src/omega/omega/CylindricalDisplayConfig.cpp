/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/CylindricalDisplayConfig.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////
bool CylindricalDisplayConfig::buildConfig(DisplayConfig& cfg, Setting& scfg)
{
	int numSides = cfg.numTiles.x();
	
	// Angle increment for each side (column)
	float sideAngleIncrement = Config::getFloatValue("sideAngleIncrement", scfg, 90);

	// Angle of side containing 0-index tiles.
	float sideAngleStart = Config::getFloatValue("sideAngleStart", scfg, -90);

	// Display system cylinder radius
	float radius = Config::getFloatValue("radius", scfg, 5);

	// Number of vertical tiles in each side
	int numSideTiles = cfg.numTiles.y();

	// Offset of center of bottom tile.
	float yOffset = cfg.referenceOffset.y();

	// Fill up the tile position / orientation data.
	// Compute the edge coordinates for all sides
	float curAngle = sideAngleStart;
	for(int horizIndex = 0; horizIndex < numSides; horizIndex ++)
	{
		float yPos = yOffset;
		for(int vertIndex = 0; vertIndex < numSideTiles; vertIndex ++)
		{
			DisplayTileConfig& tc = cfg.tiles[horizIndex][vertIndex];

			tc.yaw = curAngle;
			tc.pitch = 0;
			tc.center = Vector3f(
				sin(curAngle * Math::DegToRad) * radius,
				yPos,
				-1 * cos(curAngle * Math::DegToRad) * radius);

			yPos += cfg.tileSize.y();
		}
		curAngle += sideAngleIncrement;
	}

	return true;
}
