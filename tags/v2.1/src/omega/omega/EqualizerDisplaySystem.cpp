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
#include <omegaGl.h>

#include "eqinternal/eqinternal.h"

#include "omega/EqualizerDisplaySystem.h"
#include "omega/SystemManager.h"
#include "omega/MouseService.h"
#include "omega/GpuManager.h"
#include "omega/CylindricalDisplayConfig.h"

using namespace omega;
using namespace co::base;
using namespace std;

// TODO: move thi to osystem (ogetcwd)
#ifdef OMEGA_OS_WIN
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

// Uncomment to enable swap barrier on compounds (kinda buggy atm)
//#define EQ_USE_SWAP_BARRIER

#define OMEGA_EQ_TMP_FILE "./_eqcfg.eqc"

#define L(line) indent + line + "\n"
#define START_BLOCK(string, name) string += indent + name + "\n" + indent + "{\n"; indent += "\t";
#define END_BLOCK(string) indent = indent.substr(0, indent.length() - 1); string += indent + "}\n";

///////////////////////////////////////////////////////////////////////////////////////////////////
void exitConfig()
{
	EqualizerDisplaySystem* ds = (EqualizerDisplaySystem*)SystemManager::instance()->getDisplaySystem();
	ds->exitConfig();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
EqualizerDisplaySystem::EqualizerDisplaySystem():
	mySys(NULL),
	myConfig(NULL),
	myNodeFactory(NULL),
	mySetting(NULL),
	myDebugMouse(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
EqualizerDisplaySystem::~EqualizerDisplaySystem()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::exitConfig()
{
	SystemManager::instance()->postExitRequest();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::generateEqConfig()
{
	DisplayConfig& eqcfg = myDisplayConfig;
	String indent = "";

	Vector2f lcdSize = eqcfg.tileSize - eqcfg.bezelSize;

	String result = L("#Equalizer 1.0 ascii");

	START_BLOCK(result, "global");

	result += 
		L("EQ_CONFIG_FATTR_EYE_BASE 0.06") +
		L("EQ_WINDOW_IATTR_PLANES_STENCIL ON");

	END_BLOCK(result);

	START_BLOCK(result, "server");

	START_BLOCK(result, "config");
	result += L("latency 0");

	for(int n = 0; n < eqcfg.numNodes; n++)
	{
		DisplayNodeConfig& nc = eqcfg.nodes[n];
		if(nc.isRemote)
		{
			int port = eqcfg.basePort + nc.port;
			START_BLOCK(result, "node");
			START_BLOCK(result, "connection");
			result +=
				L("type TCPIP") +
				L("hostname \"" + nc.hostname + "\"") +
				L(ostr("port %1%", %port));
			END_BLOCK(result);
			START_BLOCK(result, "attributes");
			result +=
				L("thread_model LOCAL_SYNC");
			END_BLOCK(result);
		}
		else
		{
			START_BLOCK(result, "appNode");
			result += L("attributes { thread_model LOCAL_SYNC }");
		}


		int winX = eqcfg.windowOffset[0];
		int winY = eqcfg.windowOffset[1];

		// Write pipes section
		for(int i = 0; i < nc.numTiles; i++)
		{
			DisplayTileConfig& tc = *nc.tiles[i];
			if(eqcfg.autoOffsetWindows)
			{
				winX = tc.index[0] * eqcfg.tileResolution[0] + eqcfg.windowOffset[0];
				winY = tc.index[1] * eqcfg.tileResolution[1] + eqcfg.windowOffset[1];
			}
			
			String tileName = ostr("%1%x%2%", %tc.index[0] %tc.index[1]);
			String viewport = ostr("viewport [%1% %2% %3% %4%]", %winX %winY %eqcfg.tileResolution[0] %eqcfg.tileResolution[1]);

			String tileCfg = "";
			START_BLOCK(tileCfg, "pipe");
				tileCfg +=
					L("port = 0") +
					L(ostr("device = %1%", %tc.device));
			START_BLOCK(tileCfg, "window");
			tileCfg +=
				L("name \"" + tileName + "\"") +
				L(viewport) +
				L("channel { name \"" + tileName + "\"}");
			if(eqcfg.fullscreen)
			{
				START_BLOCK(tileCfg, "attributes");
				tileCfg +=
					L("hint_fullscreen ON") +
					L("hint_decoration OFF");
				END_BLOCK(tileCfg);
			}
			END_BLOCK(tileCfg)
			END_BLOCK(tileCfg)
			result += tileCfg;
		}

		// end of node
		END_BLOCK(result);
	}

	// observer
	result += 
		L("observer { name \"observer0\" }");

	// layout
	START_BLOCK(result, "layout");
	result += 
		L("name \"layout\"");
	START_BLOCK(result, "view");

	result += 
		L("name \"main\"") +
		L("observer \"observer0\"") +
		L("viewport [0 0 1 1]");
	
	END_BLOCK(result)
	END_BLOCK(result)

	START_BLOCK(result, "canvas");
	result += 
		L("layout \"layout\"");

	float tileViewportWidth = 1.0f / eqcfg.numTiles[0];
	float tileViewportHeight = 1.0f / eqcfg.numTiles[1];
	float tileViewportX = 0.0f;
	float tileViewportY = 0.0f;

	Vector3f canvasTopLeft = Vector3f(
		-eqcfg.referenceTile[0] * eqcfg.tileSize[0] - eqcfg.tileSize[0] / 2,
		eqcfg.referenceTile[1] * eqcfg.tileSize[1] + eqcfg.tileSize[1] / 2,
		0) + eqcfg.referenceOffset;



	for(int x = 0; x < eqcfg.numTiles[0]; x++)
	{
		for(int y = 0; y < eqcfg.numTiles[1]; y++)
		{
			DisplayTileConfig& tc = eqcfg.tiles[x][y];
			String segmentName = ostr("%1%x%2%", %x %y);
			String viewport = ostr("viewport [%1% %2% %3% %4%]", %tileViewportX %tileViewportY %tileViewportWidth %tileViewportHeight);

			//float tw = eqcfg.tileSize[0];
			//float th = eqcfg.tileSize[1];

			// Use with and height of lcd panel size without bezels.
			float tw = lcdSize[0];
			float th = lcdSize[1];

			Vector3f topLeft;
			Vector3f bottomLeft;
			Vector3f bottomRight;
			Vector3f center;

			if(eqcfg.type == DisplayConfig::ConfigPlanar)
			{
				// Compute the display corners for a planar display configuration
				topLeft = canvasTopLeft + Vector3f(x * tw, -(y * th), 0);
				bottomLeft = topLeft + Vector3f(0, -th, 0);
				bottomRight = topLeft + Vector3f(tw, -th, 0);

				center = (topLeft + bottomRight) / 2;
			}
			else
			{
				// Compute the display corners for custom display geometries
				center = tc.center;
				Quaternion orientation = AngleAxis(tc.yaw * Math::DegToRad, Vector3f::UnitY()) * AngleAxis(tc.pitch * Math::DegToRad, Vector3f::UnitX());
				// Define the default display up and right vectors
				Vector3f up = Vector3f::UnitY();
				Vector3f right = Vector3f::UnitX();

				// Compute the tile corners using the display center and oriented normal.
				up = orientation * up;
				right = orientation * right;

				// Reorient Z.
				right.z() = - right.z();

				topLeft = center + (up * th / 2) - (right * tw / 2);
				bottomLeft = center - (up * th / 2) - (right * tw / 2);
				bottomRight = center - (up * th / 2) + (right * tw / 2);
			}

			// Save the corners back to the tile display config structure.
			tc.bottomLeft = bottomLeft;
			tc.topLeft = topLeft;
			tc.bottomRight = bottomRight;
			tc.center = center;

			String tileCfg = "";
			START_BLOCK(tileCfg, "segment");
			tileCfg +=
					L("name \"" + segmentName + "\"") +
					L("channel \"" + segmentName + "\"") +
					L(viewport);
			START_BLOCK(tileCfg, "wall");
			tileCfg +=
				L("bottom_left " + ostr("[ %1% %2% %3% ]", %bottomLeft[0] %bottomLeft[1] %bottomLeft[2])) +
				L("bottom_right " + ostr("[ %1% %2% %3% ]", %bottomRight[0] %bottomRight[1] %bottomRight[2])) +
				L("top_left " + ostr("[ %1% %2% %3% ]", %topLeft[0] %topLeft[1] %topLeft[2]));
			END_BLOCK(tileCfg)
			END_BLOCK(tileCfg)
			result += tileCfg;

			tileViewportY += tileViewportHeight;
		}
		tileViewportY = 0.0f;
		tileViewportX += tileViewportWidth;
	}

	END_BLOCK(result);

	// compound
	START_BLOCK(result, "compound")
#ifdef EQ_USE_SWAP_BARRIER
	START_BLOCK(result, "swapbarrier")
	result +=
		L("name \"defaultbarrier\"");
	END_BLOCK(result)
#endif

	for(int x = 0; x < eqcfg.numTiles[0]; x++)
	{
		for(int y = 0; y < eqcfg.numTiles[1]; y++)
		{
			DisplayTileConfig& tc = eqcfg.tiles[x][y];
			String segmentName = ostr("%1%x%2%", %x %y);

			if(eqcfg.interleaved)
			{
				String tileCfg = "";
				START_BLOCK(tileCfg, "compound");
				tileCfg += 
					L("channel ( segment \"" + segmentName + "\" layout \"layout\" view 0 )") +
					L("eye [LEFT RIGHT]") +
					L("attributes { stereo_mode PASSIVE }");
				START_BLOCK(tileCfg, "compound");
				tileCfg += 
					L("eye [RIGHT]") +
					L("attributes { stereo_mode PASSIVE }") +
					L("channel \"" + segmentName + "\"") +
					L("pixel [ 0 0 1 2 ]") +
					L("outputframe { name \"" + segmentName + "r\" type texture }");
				END_BLOCK(tileCfg);
				START_BLOCK(tileCfg, "compound");
				tileCfg += 
					L("eye [LEFT]") +
					L("attributes { stereo_mode PASSIVE }") +
					L("channel \"" + segmentName + "\"") +
					L("pixel [ 0 1 1 2 ]") +
					L("outputframe { name \"" + segmentName + "l\" type texture }");
				END_BLOCK(tileCfg);
				tileCfg +=
					L("inputframe { name \"" + segmentName + "r\" }") +
					L("inputframe { name \"" + segmentName + "l\" }");
				END_BLOCK(tileCfg);

				result += tileCfg;
			}
			else
			{
				String tileCfg = "\t\tchannel ( segment \"" + segmentName + "\" layout \"layout\" view 0 )\n";
				result += tileCfg;
			}
		}
	}

	// end compound
	END_BLOCK(result)

	// end config
	END_BLOCK(result)

	// end server
	END_BLOCK(result)

	FILE* f = fopen(OMEGA_EQ_TMP_FILE, "w");
	fputs(result.c_str(), f);
	fclose(f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::setup(Setting& scfg) 
{
	mySetting = &scfg;
	DisplayConfig& cfg = myDisplayConfig;

	myDrawFps = Config::getBoolValue("drawFps", scfg);
	myDrawStatistics = Config::getBoolValue("drawStatistics", scfg);

	String cfgType = Config::getStringValue("geometry", scfg, "ConfigPlanar");
	if(cfgType == "ConfigPlanar") cfg.type = DisplayConfig::ConfigPlanar;
	else cfg.type = DisplayConfig::ConfigCustom;

	cfg.numTiles = Config::getVector2iValue("numTiles", scfg);
	cfg.referenceTile = Config::getVector2iValue("referenceTile", scfg);
	cfg.referenceOffset = Config::getVector3fValue("referenceOffset", scfg);
	cfg.tileSize = Config::getVector2fValue("tileSize", scfg);
	cfg.bezelSize = Config::getVector2fValue("bezelSize", scfg);
	cfg.autoOffsetWindows = Config::getBoolValue("autoOffsetWindows", scfg);
	cfg.tileResolution = Config::getVector2iValue("tileResolution", scfg);
	cfg.windowOffset = Config::getVector2iValue("windowOffset", scfg);
	cfg.interleaved = Config::getBoolValue("interleaved", scfg);
	cfg.fullscreen = Config::getBoolValue("fullscreen", scfg);

	cfg.displayResolution = cfg.tileResolution.cwiseProduct(cfg.numTiles);
	ofmsg("Total display resolution: %1%", %cfg.displayResolution);

	cfg.nodeLauncher = Config::getStringValue("nodeLauncher", scfg);
	cfg.nodeKiller = Config::getStringValue("nodeKiller", scfg);
	cfg.basePort = Config::getIntValue("basePort", scfg);

	cfg.launcherInterval = Config::getIntValue("launcherInterval", scfg, 500);

	const Setting& sTiles = scfg["tiles"];
	cfg.numNodes = 0;

	bool renderOnMaster = false;

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
			renderOnMaster = true;
			ncfg.isRemote = false;
		}
		ncfg.port = Config::getIntValue("port", sTileHost);

		for(int j = 0; j < sTileHost.getLength(); j++)
		{
			const Setting& sTile = sTileHost[j];
			if(sTile.getType() == Setting::TypeGroup)
			{
				// Parse tile index.
				std::vector<std::string> args = StringUtils::split(String(sTile.getName()), "xt");
				Vector2i index = Vector2i(atoi(args[0].c_str()), atoi(args[1].c_str()));

				DisplayTileConfig& tc = cfg.tiles[index[0]][index[1]];
				tc.index = index;
				//tc.interleaved = Config::getBoolValue("interleaved", sTile);
				tc.device = Config::getIntValue("device", sTile);

				tc.center = Config::getVector3fValue("center", sTile, Vector3f::Zero());
				tc.yaw = Config::getFloatValue("yaw", sTile, 0);
				tc.pitch = Config::getFloatValue("pitch", sTile, 0);

				tc.resolution = cfg.tileResolution;
				tc.offset = tc.index.cwiseProduct(tc.resolution);

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

	SystemManager::instance()->setRenderOnMaster(renderOnMaster);

	if(SystemManager::instance()->isMaster())
	{
		generateEqConfig();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::setupEqInitArgs(int& numArgs, const char** argv)
{
	SystemManager* sys = SystemManager::instance();
	const char* appName = sys->getApplication()->getName();
	if(SystemManager::instance()->isMaster())
	{
		argv[0] = appName;
		argv[1] = "--eq-config";
		argv[2] = OMEGA_EQ_TMP_FILE;
		numArgs = 3;
	}
	else
	{
		argv[0] = appName;
		argv[1] = "--eq-client";
		argv[2] = "--eq-listen";
		argv[3] = SystemManager::instance()->getHostname().c_str();
		numArgs = 4;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::initialize(SystemManager* sys)
{
	glewInit();
	Log::level = LOG_WARN;
	mySys = sys;

	//atexit(::exitConfig);

	// Launch application instances on secondary nodes.
	if(SystemManager::instance()->isMaster())
	{
		// Make sure to kill previous instances on all nodes.
		//killCluster();

		for(int n = 0; n < myDisplayConfig.numNodes; n++)
		{
			DisplayNodeConfig& nc = myDisplayConfig.nodes[n];

			if(nc.hostname != "local")
			{
				String executable = StringUtils::replaceAll(myDisplayConfig.nodeLauncher, "%c", SystemManager::instance()->getApplication()->getName());
				executable = StringUtils::replaceAll(executable, "%h", nc.hostname);
				
				// Substitute %d with current working directory
				char cCurrentPath[FILENAME_MAX];
				GetCurrentDir(cCurrentPath, sizeof(cCurrentPath));
				executable = StringUtils::replaceAll(executable, "%d", cCurrentPath);
				
				int port = myDisplayConfig.basePort + nc.port;
				String cmd = ostr("%1% %2%@%3%:%4%", %executable %SystemManager::instance()->getAppConfig()->getFilename() %nc.hostname %port);
				olaunch(cmd);
				osleep(myDisplayConfig.launcherInterval);
			}
		}
	}

	myObservers.push_back(new Observer());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::killCluster() 
{
	if(SystemManager::instance()->isMaster())
	{
		for(int n = 0; n < myDisplayConfig.numNodes; n++)
		{
			DisplayNodeConfig& nc = myDisplayConfig.nodes[n];

			if(nc.hostname != "local")
			{
				if(myDisplayConfig.nodeKiller != "")
				{
					String executable = StringUtils::replaceAll(myDisplayConfig.nodeKiller, "%c", SystemManager::instance()->getApplication()->getName());
					executable = StringUtils::replaceAll(executable, "%h", nc.hostname);
					olaunch(executable);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::finishInitialize(ConfigImpl* config)
{
	myConfig = config;
	ofmsg("Initializing %1% observer(s).", %myObservers.size());
	if(mySetting->exists("observers"))
	{
		Setting& stObservers = (*mySetting)["observers"];
		for(int i = 0; i < stObservers.getLength(); i++)
		{
			Setting& stObserver = stObservers[i];
			Observer* obs = getObserver(i);
			obs->load(stObserver);
		}
	}
	omsg(":: Equalizer initialization DONE ::");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::run()
{
	bool error = false;
	const char* argv[4];
	int numArgs = 0;
	setupEqInitArgs(numArgs, (const char**)argv);
	myNodeFactory = new EqualizerNodeFactory();
	if( !eq::init( numArgs, (char**)argv, myNodeFactory ))
	{
		oerror("Equalizer init failed");
	}
	
	myConfig = static_cast<ConfigImpl*>(eq::getConfig( numArgs, (char**)argv ));
	
	// If this is the master node, run the master loop.
	if(myConfig && mySys->isMaster())
	{
		omsg(":: Equalizer display system startup ::");
		if( myConfig->init())
		{
			omsg(":: Equalizer display system startup DONE ::");

			uint32_t spin = 0;
			bool exitRequestProcessed = false;
			while( myConfig->isRunning( ))
			{
				myConfig->startFrame( spin );
				myConfig->finishFrame();
				spin++;
				if(SystemManager::instance()->isExitRequested()
					&& !exitRequestProcessed)
				{
					exitRequestProcessed = true;
					myConfig->exit();
				}
			}
		}
		else
		{
			oerror("Config initialization failed!");
			error = true;
		}

		eq::releaseConfig( myConfig );
	}
	else
	{
		oerror("Cannot get config");
		error = true;
	}    

	eq::exit();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::cleanup()
{
	delete myNodeFactory;
	SharedDataServices::cleanup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Observer* EqualizerDisplaySystem::getObserver(int observerId)
{
	oassert(myObservers.size() > observerId);
	return myObservers[observerId];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Vector2i EqualizerDisplaySystem::getCanvasSize()
{
	return myDisplayConfig.displayResolution;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Ray EqualizerDisplaySystem::getViewRay(Vector2i position)
{
	int channelWidth = myDisplayConfig.tileResolution[0];
	int channelHeight = myDisplayConfig.tileResolution[1];
	int displayWidth = myDisplayConfig.displayResolution[0];
	int displayHeight = myDisplayConfig.displayResolution[1];

	if(position[0] < 0 || position[0] > displayWidth || 
		position[1] < 0 || position[1] > displayHeight)
	{
		ofwarn("EqualizerDisplaySystem::getViewRay: position out of bounds (%1%)", %position);
		return Ray();
	}

	int channelX = position[0] / channelWidth;
	int channelY = position[1] / channelHeight;

	int x = position[0] % channelWidth;
	int y = position[1] % channelHeight;

	return getViewRay(Vector2i(x, y), channelX, channelY);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Ray EqualizerDisplaySystem::getViewRay(Vector2i position, int channelX, int channelY)
{
	int x = position[0];
	int y = position[1];

	DisplayTileConfig& dtc = myDisplayConfig.tiles[channelX][channelY];
	Observer* o = myObservers.at(0);
	Vector3f head = o->getHeadPosition();

	float px = (float)x / dtc.resolution[0];
	float py = 1 - (float)y / dtc.resolution[1];

	Vector3f& vb = dtc.bottomLeft;
	Vector3f& va = dtc.topLeft;
	Vector3f& vc = dtc.bottomRight;

	Vector3f vba = va - vb;
	Vector3f vbc = vc - vb;

	Vector3f p = vb + vba * py + vbc * px;
	Vector3f direction = p - head;

	
	p = o->getWorldOrientation() * p;
	p += o->getWorldPosition();
	
	direction = o->getWorldOrientation() * direction;
	direction.normalize();

	//ofmsg("channel: %1%,%2% pixel:%3%,%4% pos: %5% dir %6%", %channelX %channelY %x %y %p %direction);

	return Ray(p, direction);
}

bool EqualizerDisplaySystem::getViewRayFromEvent(const Event& evt, Ray& ray, bool normalizedPointerCoords)
{
	if(evt.getServiceType() == Service::Wand)
	{
		Observer* o = myObservers.at(0);
		Vector3f pos = evt.getPosition() + o->getWorldPosition();
		Vector3f dir = evt.getOrientation() * (-Vector3f::UnitZ());
		ray.setOrigin(pos);
		ray.setDirection(dir);

		return true;
	}
	else if(evt.getServiceType() == Service::Pointer)
	{
		// If the pointer already contains ray information just return it.
		if(evt.getExtraDataType() == Event::ExtraDataVector3Array &&
			evt.getExtraDataItems() >= 2)
		{
			ray.setOrigin(evt.getExtraDataVector3(0));
			ray.setDirection(evt.getExtraDataVector3(1));
		}
		else
		{
			Vector3f pos = evt.getPosition();
			// The pointer did not contain ray information: generate a ray now.
			if(normalizedPointerCoords)
			{
				pos[0] = pos[0] *  myDisplayConfig.displayResolution[0];
				pos[1] = pos[1] *  myDisplayConfig.displayResolution[1];
			}

			ray = getViewRay(Vector2i(pos[0], pos[1]));
		}
		return true;
	}
	return false;
}

