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
#include "omega/CylindricalDisplayConfig.h"

using namespace omega;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayConfig::LoadConfig(Setting& scfg, DisplayConfig& cfg)
{
	//myDrawFps = Config::getBoolValue("drawFps", scfg);
	//myDrawStatistics = Config::getBoolValue("drawStatistics", scfg);

	String cfgType = Config::getStringValue("geometry", scfg, "ConfigPlanar");
	if(cfgType == "ConfigPlanar") cfg.type = DisplayConfig::ConfigPlanar;
	else cfg.type = DisplayConfig::ConfigCustom;

	cfg.numTiles = Config::getVector2iValue("numTiles", scfg);
	cfg.referenceTile = Config::getVector2iValue("referenceTile", scfg);
	cfg.referenceOffset = Config::getVector3fValue("referenceOffset", scfg);
	cfg.tileSize = Config::getVector2fValue("tileSize", scfg);
	cfg.bezelSize = Config::getVector2fValue("bezelSize", scfg);
	ofmsg("Bezel size: %1%", %cfg.bezelSize);
	
	cfg.autoOffsetWindows = Config::getBoolValue("autoOffsetWindows", scfg);
	cfg.tileResolution = Config::getVector2iValue("tileResolution", scfg);
	cfg.windowOffset = Config::getVector2iValue("windowOffset", scfg);

	cfg.latency = Config::getIntValue("latency", scfg);
	
	String sm = Config::getStringValue("stereoMode", scfg, "default");
	StringUtils::toLowerCase(sm);
	if(sm == "default") cfg.stereoMode = DisplayTileConfig::Default;
	else if(sm == "mono") cfg.stereoMode = DisplayTileConfig::Mono;
	else if(sm == "interleaved") cfg.stereoMode = DisplayTileConfig::Interleaved;
	else if(sm == "sidebyside") cfg.stereoMode = DisplayTileConfig::SideBySide;

	cfg.enableStencilInterleaver = Config::getBoolValue("enableStencilInterleaver", scfg);
	cfg.fullscreen = Config::getBoolValue("fullscreen", scfg);

	cfg.panopticStereoEnabled = Config::getBoolValue("panopticStereoEnabled", scfg);

	cfg.disableConfigGenerator = Config::getBoolValue("disableConfigGenerator", scfg, false);
	
	cfg.displayResolution = cfg.tileResolution.cwiseProduct(cfg.numTiles);
	ofmsg("Total display resolution: %1%", %cfg.displayResolution);

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
		cfg.statsTile.resolution = Vector2i(480, 860);
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
				std::vector<std::string> args = StringUtils::split(String(sTile.getName()), "xt");
				Vector2i index = Vector2i(atoi(args[0].c_str()), atoi(args[1].c_str()));

				DisplayTileConfig& tc = cfg.tiles[index[0]][index[1]];

				String sm = Config::getStringValue("stereoMode", sTile, "default");
				StringUtils::toLowerCase(sm);
				if(sm == "default") tc.stereoMode = DisplayTileConfig::Default;
				else if(sm == "mono") tc.stereoMode = DisplayTileConfig::Mono;
				else if(sm == "interleaved") tc.stereoMode = DisplayTileConfig::Interleaved;
				else if(sm == "sidebyside") tc.stereoMode = DisplayTileConfig::SideBySide;
				
				tc.drawFps = drawFps;
				tc.index = index;
				//tc.interleaved = Config::getBoolValue("interleaved", sTile);
				tc.device = Config::getIntValue("device", sTile);

				tc.center = Config::getVector3fValue("center", sTile, Vector3f::Zero());
				tc.yaw = Config::getFloatValue("yaw", sTile, 0);
				tc.pitch = Config::getFloatValue("pitch", sTile, 0);

				tc.position = Config::getVector2iValue("position", sTile);
				tc.disableScene = Config::getBoolValue("disableScene", sTile);

				if(sTile.exists("resolution"))
				{
					tc.offset = Config::getVector2iValue("resolution", sTile);
				}
				else
				{
					tc.resolution = cfg.tileResolution;
				}

				if(sTile.exists("offset"))
				{
					tc.offset = Config::getVector2iValue("offset", sTile);
				}
				else
				{
					tc.offset = tc.index.cwiseProduct(tc.resolution);
				}

				// Custom camera
				tc.cameraName = Config::getStringValue("cameraName", sTile, "");

				ncfg.tiles[ncfg.numTiles] = &tc;
				ncfg.numTiles++;
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
}
