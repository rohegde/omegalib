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
#ifndef __DISPLAY_CONFIG__
#define __DISPLAY_CONFIG__

#include "osystem.h"
#include "SystemManager.h"
//#include "IEventListener.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class SystemManager;
	class DisplaySystem;
	class RenderTarget;
	class ApplicationBase;
	class ChannelImpl;
	class GpuContext;
	class Renderer;
	class Camera;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct DisplayTileConfig
	{
		enum StereoMode { Mono, Interleaved, SideBySide, Default };

		DisplayTileConfig(): drawStats(false), drawFps(false), disableScene(false), disableOverlay(false), stereoMode(Mono) {}

		StereoMode stereoMode;

		Vector2i index;
		Vector2i resolution;

		//! 2d offset of window content
		Vector2i offset;

		//! Window position (when autooffset is not used)
		Vector2i position;

		int device;
		Vector3f center;
		float yaw;
		float pitch;
		bool drawStats;
		bool drawFps;
		bool disableScene;
		bool disableOverlay;

		//! Name of camera attached to this tile. Can be empty or 'default' for default camera
		String cameraName;
		//! Reference to camera attached to this tile. Set during display system initialization
		Camera* camera;

		Vector3f topLeft;
		Vector3f bottomLeft;
		Vector3f bottomRight;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct DisplayNodeConfig
	{
		static const int MaxTiles = 64;
		int numTiles;
		String hostname;
		int port;
		bool isRemote;
		DisplayTileConfig* tiles[MaxTiles];
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Stores omegalib display configuration data.
	struct DisplayConfig
	{
	public:
		static void LoadConfig(Setting& s, DisplayConfig& cfg);

	public:
		// UGLY CONSTANTS.
		static const int MaxTiles = 64;
		static const int MaxNodes = 64;
		
		DisplayConfig(): disableConfigGenerator(false), latency(1), enableStencilInterleaver(false), enableSwapSync(true), forceMono(false) {}		
		bool disableConfigGenerator;

		// ! Configuration type enum
		enum ConfigType {
			//! Planar configuration: the display tiles are on the same plane. The user won't have to
			//! Specify position and rotation for each tile.
			ConfigPlanar, 
			//! Custom configuration. The user will have to manually specify position and rotation for each tile,
			//! or they will be provided by a custom DisplayConfigBuilder.
			ConfigCustom};
		//! Display configuration type.
		ConfigType type;

		//! Open a statistics window on the master node.
		bool displayStatsOnMaster;
		DisplayTileConfig statsTile;

		//! Number of horizontal / vertical tiles in the display system
		Vector2i numTiles;
		int latency;

		//! (Used only for planar configurtions) Index of the tile whose center will be used as the origin of the display system geometry.
		Vector2i referenceTile;
		//! Offset of reference tile center wrt world origin.
		Vector3f referenceOffset;

		//! Size of tile in meters.
		Vector2f tileSize;
		//! Size of tile bezel in meters.
		Vector2f bezelSize;

		//! Tile resolution in pixels.
		Vector2i tileResolution;

		//! When set to true, window positions will be computed automatically in a multiwindow setting.
		bool autoOffsetWindows;
		//! Offset of the first window in pixels (valid for multiwindow settings)
		Vector2i windowOffset;

		//! Global stereo mode. Will be used by tiles that specify 'Default" as their stereo mode.
		DisplayTileConfig::StereoMode stereoMode;
		//! Enable stencl-based interleaved stereo. Faster but requires use of stencil buffer.
		bool enableStencilInterleaver;

		//! Enable vsync on all tiles
		bool enableVSync;
		//! Enable swap sync on cluster displays
		bool enableSwapSync;
			 

		//! Enable fullscreen rendering.
		bool fullscreen;

		// Display fps on each tile.
		bool drawFps;

		bool renderOnMaster;

		//! When set to true, observer orientation will be kept aligned to each tile normal.
		//bool panopticStereoEnabled;

		//! Runtime setings
		//@{
		//! Runtime flag: when set to true, observer orientation will still use camera orientation even
		bool panopticStereoEnabled;
		//! Runtime flag:When set to true, all tiles will be forced to render in mono mode
		bool forceMono;
		//@}
		
		
		//! Tile configurations.
		DisplayTileConfig tiles[MaxTiles][MaxTiles];
		DisplayTileConfig specialTiles[MaxTiles];

		//! Total display resolution. Will be computed automatically during the setup process, users should leave this blank.
		Vector2i displayResolution;

		//! Number of nodes for a multimachine display system.
		int numNodes;
		//! Node configurations for a multimachine display system.
		DisplayNodeConfig nodes[MaxNodes];
		//! Interval in milliseconds between node launcher commands
		int launcherInterval; 
		//! Node launcher command.
		String nodeLauncher;
		//! Node killer command.
		String nodeKiller;
		//! Default port used to connect to nodes
		int basePort;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Provides custom display configurations
	class IDisplayConfigBuilder
	{
	public:
		virtual bool buildConfig(DisplayConfig& cfg, Setting& scfg) = 0;
	};
}; // namespace omega

#endif
