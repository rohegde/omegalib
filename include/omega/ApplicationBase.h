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
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

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

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct DisplayTileConfig
	{
		DisplayTileConfig(): drawStats(false), drawFps(false), disableScene(false) {}

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
		// UGLY CONSTANTS.
		static const int MaxTiles = 64;
		static const int MaxNodes = 64;
		
		DisplayConfig(): disableConfigGenerator(false), latency(1) {}
		
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

		//! Enable interleaved stereo rendering.
		bool interleaved;
		//! Enable fullscreen rendering.
		bool fullscreen;

		//! When set to true, observer orientation will be kept aligned to each tile normal.
		bool orientObserverToTile;
		
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

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Contains information about the context in which an update operation is taking place
	struct UpdateContext
	{
		uint64 frameNum;
		float time;
		float dt;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Contains information about the current frame.
	struct FrameInfo
	{
		FrameInfo(uint64 frame, GpuContext* context): frameNum(frame), gpuContext(context) {}
		uint64 frameNum;
		GpuContext* gpuContext;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Contains information about the context in which a drawing operation is taking place
	struct DrawContext
	{
		enum Eye { EyeLeft , EyeRight, EyeCyclop };
		enum Task { SceneDrawTask, OverlayDrawTask };
		uint64 frameNum; // TODO: Substitute with frameinfo
		AffineTransform3 modelview;
		Transform3 projection;
		//! The pixel viewport coordinates of this context with respect to the owner window of the context.
		Rect viewport;
		//! The eye being rendered for this context.
		Eye eye;
		//! The current draw task.
		Task task;
		//! Information about the drawing channel associated with this context.
		//ChannelInfo* channel;
		const DisplayTileConfig* tile;
		RenderTarget* drawBuffer;
		GpuContext* gpuContext;
	};

	class ServerBase;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API RendererBase: public ReferenceType
	{
	friend class DisplaySystem;
	friend class ServerBase;
	public:
		RendererBase(ServerBase* app);
		virtual ~RendererBase(); 

		GpuContext* getGpuContext() { return myGpuContext; } 
		void setGpuContext(GpuContext* ctx) { myGpuContext = ctx; } 

		virtual void initialize() {}
		virtual void finalize() {}

		virtual void draw(const DrawContext& context) {}
		virtual void startFrame(const FrameInfo& context) {}
		virtual void finishFrame(const FrameInfo& context) {}

		ServerBase* getEngine() { return myServer; }
		SystemManager*  getSystemManager()  { return SystemManager::instance(); }
		ServiceManager*   getServiceManager()   { return SystemManager::instance()->getServiceManager(); }
		DisplaySystem*  getDisplaySystem() { return SystemManager::instance()->getDisplaySystem(); }

	private:
		ServerBase* myServer;
		GpuContext* myGpuContext;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ServerBase: public ReferenceType, public IEventListener
	{
	friend class RendererBase;
	public:
		ServerBase(ApplicationBase* app): myApplication(app) {}
		virtual ~ServerBase() {}

		virtual void initialize() {}
		virtual void dispose() {}
		virtual void update(const UpdateContext& context) {}
		virtual void handleEvent(const Event& evt) { }

		SystemManager*  getSystemManager()  { return SystemManager::instance(); }
		ApplicationBase* getApplication() { return myApplication; }
		DisplaySystem*  getDisplaySystem() { return SystemManager::instance()->getDisplaySystem(); }
		int getCanvasWidth(); 
		int getCanvasHeight();

	private:
		void addClient(RendererBase* cli);

	private:
		ApplicationBase* myApplication;
		List<RendererBase*> myClients;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ApplicationBase
	{
	public:
		static const int MaxLayers = 16;

	public:
		virtual const char* getName() { return "OmegaLib " OMEGA_VERSION; }

		//! Called once for entire application initialization tasks.
		virtual void initialize() {}
	};
}; // namespace omega

#endif
