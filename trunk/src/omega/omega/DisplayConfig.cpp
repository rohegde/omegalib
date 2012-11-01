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
#include "omega/DisplayConfig.h"
#include "omega/Engine.h"
#include "omega/CylindricalDisplayConfig.h"
#include "omega/PlanarDisplayConfig.h"
#include "omega/SageManager.h"

using namespace omega;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayConfig::LoadConfig(Setting& scfg, DisplayConfig& cfg)
{
	//myDrawFps = Config::getBoolValue("drawFps", scfg);
	//myDrawStatistics = Config::getBoolValue("drawStatistics", scfg);

	String cfgType = Config::getStringValue("geometry", scfg, "ConfigPlanar");

	//cfg.numTiles = Config::getVector2iValue("numTiles", scfg);
	cfg.referenceTile = Config::getVector2iValue("referenceTile", scfg);
	cfg.referenceOffset = Config::getVector3fValue("referenceOffset", scfg);
	cfg.tileSize = Config::getVector2fValue("tileSize", scfg);
	cfg.bezelSize = Config::getVector2fValue("bezelSize", scfg);
	ofmsg("Bezel size: %1%", %cfg.bezelSize);
	
	cfg.tileResolution = Config::getVector2iValue("tileResolution", scfg);
	cfg.windowOffset = Config::getVector2iValue("windowOffset", scfg);

	cfg.latency = Config::getIntValue("latency", scfg);
	
	String sm = Config::getStringValue("stereoMode", scfg, "default");
	StringUtils::toLowerCase(sm);
	if(sm == "default") cfg.stereoMode = DisplayTileConfig::Default;
	else if(sm == "mono") cfg.stereoMode = DisplayTileConfig::Mono;
	else if(sm == "interleaved") cfg.stereoMode = DisplayTileConfig::Interleaved;
	else if(sm == "sidebyside") cfg.stereoMode = DisplayTileConfig::SideBySide;

	cfg.fullscreen = Config::getBoolValue("fullscreen", scfg);

	// deprecated
	cfg.panopticStereoEnabled = Config::getBoolValue("panopticStereoEnabled", scfg);

	cfg.disableConfigGenerator = Config::getBoolValue("disableConfigGenerator", scfg, false);
	
	//cfg.displayResolution = cfg.tileResolution.cwiseProduct(cfg.numTiles);
	//ofmsg("Total display resolution: %1%", %cfg.displayResolution);

	cfg.nodeLauncher = Config::getStringValue("nodeLauncher", scfg);
	cfg.nodeKiller = Config::getStringValue("nodeKiller", scfg);
	cfg.basePort = Config::getIntValue("basePort", scfg);

	cfg.launcherInterval = Config::getIntValue("launcherInterval", scfg, 500);

	const Setting& sTiles = scfg["tiles"];
	cfg.numNodes = 0;

	cfg.displayStatsOnMaster = Config::getBoolValue("displayStatsOnMaster", scfg, false);
	if(cfg.displayStatsOnMaster)
	{
		cfg.statsTile.offset = Vector2i(0, 0);
		cfg.statsTile.pixelSize = Vector2i(480, 860);
		cfg.statsTile.drawStats = true;
	}

	bool drawFps = Config::getBoolValue("drawFps", scfg, false);

	cfg.enableVSync= Config::getBoolValue("enableVSync", scfg, false);
	cfg.enableSwapSync = Config::getBoolValue("enableSwapSync", scfg, true);

	for(int i = 0; i < sTiles.getLength(); i++)
	{
		const Setting& sTileHost = sTiles[i];
		DisplayNodeConfig& ncfg = cfg.nodes[cfg.numNodes];
		ncfg.hostname = sTileHost.getName();
		String alternHostname = Config::getStringValue("hostname", sTileHost);
		if(alternHostname != "") ncfg.hostname = alternHostname;
		ncfg.numTiles = 0;
		if(ncfg.hostname != "local")
		{
			ncfg.isRemote = true;
		}
		else
		{
			cfg.renderOnMaster = true;
			ncfg.isRemote = false;
		}
		ncfg.port = Config::getIntValue("port", sTileHost);

		for(int j = 0; j < sTileHost.getLength(); j++)
		{
			const Setting& sTile = sTileHost[j];
			if(sTile.getType() == Setting::TypeGroup)
			{
				DisplayTileConfig* tc = new DisplayTileConfig();
				cfg.tiles[sTile.getName()] = tc;
				tc->name = sTile.getName();

				String sm = Config::getStringValue("stereoMode", sTile, "default");
				StringUtils::toLowerCase(sm);
				if(sm == "default") tc->stereoMode = DisplayTileConfig::Default;
				else if(sm == "mono") tc->stereoMode = DisplayTileConfig::Mono;
				else if(sm == "interleaved") tc->stereoMode = DisplayTileConfig::Interleaved;
				else if(sm == "sidebyside") tc->stereoMode = DisplayTileConfig::SideBySide;
				
				tc->drawFps = drawFps;
				//tc.index = index;
				//tc.interleaved = Config::getBoolValue("interleaved", sTile);
				tc->device = Config::getIntValue("device", sTile);

				tc->center = Config::getVector3fValue("center", sTile, Vector3f::Zero());
				tc->yaw = Config::getFloatValue("yaw", sTile, 0);
				tc->pitch = Config::getFloatValue("pitch", sTile, 0);

				tc->position = Config::getVector2iValue("position", sTile);
				tc->disableScene = Config::getBoolValue("disableScene", sTile);

				tc->offscreen = Config::getBoolValue("offscreen", sTile, false);

				tc->viewport = Config::getVector4fValue("viewport", sTile, tc->viewport);

				// If the tile config contains a size entry use it, oterwise use the default tile and bezel size data
				if(sTile.exists("size"))
				{
					tc->size = Config::getVector2fValue("size", sTile);
				}
				else
				{
					tc->size = cfg.tileSize - cfg.bezelSize;
				}

				if(sTile.exists("resolution"))
				{
					tc->pixelSize = Config::getVector2iValue("resolution", sTile);
				}
				else
				{
					tc->pixelSize = cfg.tileResolution;
				}

				if(sTile.exists("offset"))
				{
					tc->offset = Config::getVector2iValue("offset", sTile);
				}
				else
				{
					std::vector<std::string> args = StringUtils::split(String(sTile.getName()), "xt");
					Vector2i index = Vector2i(atoi(args[0].c_str()), atoi(args[1].c_str()));
					tc->offset = index.cwiseProduct(tc->pixelSize);
				}

				// Custom camera
				tc->cameraName = Config::getStringValue("cameraName", sTile, "");

				// Compute default values for this tile corners. These values may be overwritted by display config generators applied later on.
				cfg.computeTileCorners(tc);

				ncfg.tiles[ncfg.numTiles] = tc;
				ncfg.numTiles++;
				cfg.numTiles++;
			}
		}
		cfg.numNodes++;
	}

	// Parse cylindrical display configurations.
	if(cfgType == "ConfigCylindrical")
	{
		CylindricalDisplayConfig cdc;
		cdc.buildConfig(cfg, scfg);
	}
	else if(cfgType == "ConfigPlanar")
	{
		PlanarDisplayConfig cdc;
		cdc.buildConfig(cfg, scfg);
	}

	// If SAGE support is available and we have a sage configuration section, pass it to the SAGE manager.
#ifdef OMEGA_USE_SAGE
	if(scfg.exists("sage"))
	{
		Engine::instance()->getSageManager()->setup(scfg["sage"]);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayConfig::computeTileCorners(DisplayTileConfig* tc)
{
	float tw = tc->size[0];
	float th = tc->size[1];

	// Compute the display corners for custom display geometries
	Quaternion orientation = AngleAxis(tc->yaw * Math::DegToRad, Vector3f::UnitY()) * AngleAxis(tc->pitch * Math::DegToRad, Vector3f::UnitX());
	// Define the default display up and right vectors
	Vector3f up = Vector3f::UnitY();
	Vector3f right = Vector3f::UnitX();

	// Compute the tile corners using the display center and oriented normal.
	up = orientation * up;
	right = orientation * right;

	// Reorient Z.
	right.z() = - right.z();

	tc->topLeft = tc->center + (up * th / 2) - (right * tw / 2);
	tc->bottomLeft = tc->center - (up * th / 2) - (right * tw / 2);
	tc->bottomRight = tc->center - (up * th / 2) + (right * tw / 2);
}
