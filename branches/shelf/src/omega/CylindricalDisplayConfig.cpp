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
#include "omega/CylindricalDisplayConfig.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////
bool CylindricalDisplayConfig::buildConfig(DisplayConfig& cfg, Setting& scfg)
{
	Vector2i numTiles = Config::getVector2iValue("numTiles", scfg);
	cfg.tileGridSize = numTiles;

	cfg.canvasPixelSize = numTiles.cwiseProduct(cfg.tileResolution);
	ofmsg("canvas pixel size: %1%", %cfg.canvasPixelSize);

	int numSides = numTiles.x();
	
	// Angle increment for each side (column)
	float sideAngleIncrement = Config::getFloatValue("sideAngleIncrement", scfg, 90);

	// Angle of side containing 0-index tiles.
	float sideAngleStart = Config::getFloatValue("sideAngleStart", scfg, -90);

	// Display system cylinder radius
	float radius = Config::getFloatValue("radius", scfg, 5);

	// Number of vertical tiles in each side
	int numSideTiles = numTiles.y();

	// Offset of center of bottom tile.
	float yOffset = cfg.referenceOffset.y();

	float tileViewportWidth = 1.0f / numTiles[0];
	float tileViewportHeight = 1.0f / numTiles[1];
	float tileViewportX = 0.0f;
	float tileViewportY = 0.0f;

	// Fill up the tile position / orientation data.
	// Compute the edge coordinates for all sides
	float curAngle = sideAngleStart;
	for(int x = 0; x < numSides; x ++)
	{
		float yPos = yOffset;
		for(int y = 0; y < numSideTiles; y ++)
		{
			// Use the indices to create a tile name in the form t<X>x<Y> (i.e. t1x0).
			// This is kind of hacking, because it forces tile names to be in that form for cylindrical configurations, 
			// but it works well enough.
			String tileName = ostr("t%1%x%2%", %x %y);
			if(cfg.tiles.find(tileName) == cfg.tiles.end())
			{
				ofwarn("CylindricalDisplayConfig::buildConfig: could not find tile '%1%'", %tileName);
			}
			else
			{
				DisplayTileConfig* tc = cfg.tiles[tileName];
				cfg.tileGrid[x][y] = tc;
				
				tc->enabled = true;
				tc->isInGrid = true;
				tc->gridX = x;
				tc->gridY = y;
				
				tc->yaw = curAngle;
				tc->pitch = 0;
				tc->center = Vector3f(
					sin(curAngle * Math::DegToRad) * radius,
					yPos,
					-1 * cos(curAngle * Math::DegToRad) * radius);

				// Save the tile viewport
				//tc->viewport = Vector4f(tileViewportX, tileViewportY, tileViewportWidth, tileViewportHeight);

				cfg.computeTileCorners(tc);
			}
			yPos += cfg.tileSize.y();
			tileViewportY += tileViewportHeight;
		}
		curAngle += sideAngleIncrement;
		tileViewportY = 0.0f;
		tileViewportX += tileViewportWidth;
	}

	return true;
}
