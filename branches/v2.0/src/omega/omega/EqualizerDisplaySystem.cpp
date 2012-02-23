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

#include "omega/EqualizerDisplaySystem.h"
#include "omega/SystemManager.h"
#include "omega/MouseService.h"
#include "omega/GpuManager.h"

#include "eqinternal/eqinternal.h"

#ifdef WIN32
#include <windows.h> // needed for Sleep 
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

using namespace omega;
using namespace co::base;
using namespace std;

extern omega::Vector2i sCanvasSize;
extern omega::Vector2i sCanvasChannels;
extern ChannelImpl* sCanvasChannelPointers[ConfigImpl::MaxCanvasChannels][ConfigImpl::MaxCanvasChannels];

struct TileConfig
{
	Vector2i index;
	int device;
};

struct NodeConfig
{
	static const int MaxTiles = 64;
	int numTiles;
	String hostname;
	bool isRemote;
	TileConfig* tiles[MaxTiles];
};

struct EqualizerConfig
{
	static const int MaxTiles = 64;
	static const int MaxNodes = 64;
	enum ConfigType {ConfigPlanar, ConfigCylindrical};
	ConfigType type;
	Vector2i numTiles;
	Vector2i referenceTile;
	Vector3f referenceOffset;
	Vector2f tileSize;
	float columnYawIncrement;
	bool autoOffsetWindows;
	Vector2i tileResolution;
	Vector2i windowOffset;
	TileConfig tiles[MaxTiles][MaxTiles];
	int numNodes;
	NodeConfig nodes[MaxNodes];
	bool interleaved;
	bool fullscreen;

	String nodeLauncher;
	int nodePort;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void parseConfig(const Setting& scfg, EqualizerConfig& cfg)
{
	String cfgType = Config::getStringValue("type", scfg, "ConfigPlanar");
	if(cfgType == "ConfigPlanar") cfg.type = EqualizerConfig::ConfigPlanar;
	else cfg.type = EqualizerConfig::ConfigCylindrical;

	cfg.numTiles = Config::getVector2iValue("numTiles", scfg);
	cfg.referenceTile = Config::getVector2iValue("referenceTile", scfg);
	cfg.referenceOffset = Config::getVector3fValue("referenceOffset", scfg);
	cfg.tileSize = Config::getVector2fValue("tileSize", scfg);
	cfg.columnYawIncrement = Config::getFloatValue("columnYawIncrement", scfg);
	cfg.autoOffsetWindows = Config::getBoolValue("autoOffsetWindows", scfg);
	cfg.tileResolution = Config::getVector2iValue("tileResolution", scfg);
	cfg.windowOffset = Config::getVector2iValue("windowOffset", scfg);
	cfg.interleaved = Config::getBoolValue("interleaved", scfg);
	cfg.fullscreen = Config::getBoolValue("fullscreen", scfg);

	cfg.nodeLauncher = Config::getStringValue("nodeLauncher", scfg);
	cfg.nodePort = Config::getIntValue("nodePort", scfg);

	const Setting& sTiles = scfg["tiles"];
	cfg.numNodes = 0;
	for(int i = 0; i < sTiles.getLength(); i++)
	{
		const Setting& sTileHost = sTiles[i];
		NodeConfig& ncfg = cfg.nodes[cfg.numNodes];
		ncfg.hostname = sTileHost.getName();
		ncfg.numTiles = 0;
		if(ncfg.hostname != "local")
		{
			ncfg.isRemote = true;
		}
		else
		{
			ncfg.isRemote = false;
		}

		for(int j = 0; j < sTileHost.getLength(); j++)
		{
			const Setting& sTile = sTileHost[j];
			// Parse tile index.
			std::vector<std::string> args = StringUtils::split(String(sTile.getName()), "xt");
			Vector2i index = Vector2i(atoi(args[0].c_str()), atoi(args[1].c_str()));

			TileConfig& tc = cfg.tiles[index[0]][index[1]];
			tc.index = index;
			//tc.interleaved = Config::getBoolValue("interleaved", sTile);
			tc.device = Config::getIntValue("device", sTile);
			ncfg.tiles[ncfg.numTiles] = &tc;
			ncfg.numTiles++;
		}
		cfg.numNodes++;
	}
}

#define L(line) indent + line + "\n"
#define START_BLOCK(string, name) string += indent + name + "\n" + indent + "{\n"; indent += "\t";
#define END_BLOCK(string) indent = indent.substr(0, indent.length() - 1); string += indent + "}\n";


///////////////////////////////////////////////////////////////////////////////////////////////////
String EqualizerDisplaySystem::generateEqConfig(const String& cfgPath)
{
	EqualizerConfig eqcfg;

	String indent = "";


	Config* cfg = new Config(cfgPath);
	cfg->load();
	parseConfig(cfg->getRootSetting()["config"], eqcfg);

	String result = L("#Equalizer 1.0 ascii");

	START_BLOCK(result, "global");

	result += 
		L("EQ_CONFIG_FATTR_EYE_BASE 0.06") +
		L("EQ_WINDOW_IATTR_PLANES_STENCIL ON");

	END_BLOCK(result);

	START_BLOCK(result, "server");

	// START_BLOCK(result, "connection");
	// result +=
		// L("type TCPIP") +
		// L("hostname \"orion.evl.uic.edu\"") +
		// L("port 24000");
	// END_BLOCK(result)

	START_BLOCK(result, "config");

	for(int n = 0; n < eqcfg.numNodes; n++)
	{
		NodeConfig& nc = eqcfg.nodes[n];
		if(nc.isRemote)
		{
			START_BLOCK(result, "node");
			START_BLOCK(result, "connection");
			result +=
				L("type TCPIP") +
				L("hostname \"" + nc.hostname + "\"") +
				L(ostr("port %1%", %eqcfg.nodePort));
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
			TileConfig& tc = *nc.tiles[i];
			if(eqcfg.autoOffsetWindows)
			{
				winX = tc.index[0] * eqcfg.tileResolution[0] + eqcfg.windowOffset[0];
				winY = tc.index[1] * eqcfg.tileResolution[1] + eqcfg.windowOffset[1];
			}
			
			String tileName = ostr("%1%x%2%", %tc.index[0] %tc.index[1]);
			String viewport = ostr("viewport [%1% %2% %3% %4%]", %winX %winY %eqcfg.tileResolution[0] %eqcfg.tileResolution[1]);

			String tileCfg = "";
			START_BLOCK(tileCfg, "pipe");
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
			TileConfig& tc = eqcfg.tiles[x][y];
			String segmentName = ostr("%1%x%2%", %x %y);
			String viewport = ostr("viewport [%1% %2% %3% %4%]", %tileViewportX %tileViewportY %tileViewportWidth %tileViewportHeight);

			float tw = eqcfg.tileSize[0];
			float th = eqcfg.tileSize[1];

			Vector3f topLeft = canvasTopLeft + Vector3f(x * tw, -(y * th), 0);
			Vector3f bottomLeft = topLeft + Vector3f(0, -th, 0);
			Vector3f bottomRight = topLeft + Vector3f(tw, -th, 0);
			
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


	for(int x = 0; x < eqcfg.numTiles[0]; x++)
	{
		for(int y = 0; y < eqcfg.numTiles[1]; y++)
		{
			TileConfig& tc = eqcfg.tiles[x][y];
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

	FILE* f = fopen("./_eqcfg.eqc", "w");
	fputs(result.c_str(), f);
	fclose(f);

	// SUPAMEGAHACK!!
	for(int n = 0; n < eqcfg.numNodes; n++)
	{
		NodeConfig& nc = eqcfg.nodes[n];

		if(nc.hostname != "local")
		{
			String executable = StringUtils::replaceAll(eqcfg.nodeLauncher, "%c", SystemManager::instance()->getApplication()->getName());
			String cmd = ostr("%1% %2%@%3%:%4%", %executable %SystemManager::instance()->getAppConfig()->getFilename() %nc.hostname %eqcfg.nodePort);
			olaunch(cmd);
			Sleep(2000);
		}
	}


	return "./_eqcfg.eqc";
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
void EqualizerDisplaySystem::setup(Setting& setting) 
{
	myDebugMouse = Config::getBoolValue("debugMouse", setting, false);
	myDisplayConfig = Config::getStringValue("config", setting);
	mySetting = &setting;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::setupEqInitArgs(int& numArgs, const char** argv)
{
	SystemManager* sys = SystemManager::instance();
	const char* appName = sys->getApplication()->getName();

	DataManager* dm = sys->getDataManager();
	DataInfo cfgInfo = dm->getInfo(myDisplayConfig);

	oassert(!cfgInfo.isNull())
	oassert(cfgInfo.local);

	String cfgPath;
	if(SystemManager::instance()->isMaster())
	{
		argv[0] = appName;
		argv[1] = "--eq-config";

		if(StringUtils::endsWith(cfgInfo.path, ".cfg"))
		{
			cfgPath = generateEqConfig(cfgInfo.path);
			// argh, strdup
			argv[2] = strdup(cfgPath.c_str());
		}
		else
		{
			// argh, strdup
			argv[2] = strdup(cfgInfo.path.c_str());
		}
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
	//Log::level = LOG_INFO;
	mySys = sys;

	//finishInitialize(cfg);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::finishInitialize(ConfigImpl* config)
{
	myConfig = config;
	//int numObservers = myConfig->getObservers().size();
	//for( unsigned int i = 0; i < numObservers; i++) myObservers.push_back(new Observer());

	myObservers.push_back(new Observer());

	//initLayers();
	initObservers();

	omsg(":: Equalizer initialization DONE ::");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::initObservers()
{
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::initLayers()
{
	if(mySetting->exists("views"))
	{
		Setting& stViews = (*mySetting)["views"];
		for(int i = 0; i < stViews.getLength(); i++)
		{
			Setting& stView = stViews[i];

			// Set draw statistics flag
			ViewImpl* view = myConfig->findView(stView.getName());
			if(view != NULL)
			{
				if(stView.exists("drawStatistics"))
					view->drawStatistics(stView["drawStatistics"]);
				if(stView.exists("drawFps"))
					view->drawFps(stView["drawFps"]);
			}

			// Set enabled layers
			if(stView.exists("layer"))
			{
				String layer = stView["layer"];
				setLayer(stView.getName(), Layer::fromString(layer));
			}
			else
			{
				ofwarn("Config: no layer section defined in config/views/%1%", %stView.getName());
			}
		}
	}
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
		if( myConfig->init( 0 ))
		{
			omsg(":: Equalizer display system startup DONE ::");

			uint32_t spin = 0;
			while( myConfig->isRunning( ))
			{
				myConfig->startFrame( spin );
				myConfig->finishFrame();
				spin++;
			}
		
			// 5. exit config
			myConfig->exit();
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::setLayer(const char* viewName, Layer::Enum layer) 
{
	if(!myConfig)
	{
		oerror("EqualizerDisplaySystem::setLayerEnabled - must be called AFTER EqualizerDisplaySystem::initialize");
		return;
	}

	ViewImpl* view = myConfig->findView(viewName);
	if(view != NULL)
	{
		view->setLayer(layer);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Layer::Enum EqualizerDisplaySystem::getLayer(const char* viewName) 
{ 
	if(!myConfig)
	{
		oerror("EqualizerDisplaySystem::GetLayerEnabled - must be called AFTER EqualizerDisplaySystem::initialize");
		return Layer::Null;
	}

	ViewImpl* view = myConfig->findView(viewName);
	if(view != NULL)
	{
		return view->getLayer();
	}
	return Layer::Null;
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
	return sCanvasSize;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Ray EqualizerDisplaySystem::getViewRay(Vector2i position)
{
	int channelWidth = (sCanvasSize[0] / sCanvasChannels[0]);
	int channelHeight = (sCanvasSize[1] / sCanvasChannels[1]);

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

	ChannelImpl* ch = sCanvasChannelPointers[channelX][channelY];
	if(ch != NULL)
	{
		const DrawContext& dc = ch->getLastDrawContext();
	    //ofmsg("Channel pixel X,Y: %1% %2%", %x %y);
		return Math::unproject(Vector2f(x, y), dc.modelview, dc.projection, dc.viewport);
	}
	return Ray();
}

