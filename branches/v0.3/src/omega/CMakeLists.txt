#********************************************************************************************************************** 
# THE OMEGA LIB PROJECT
#---------------------------------------------------------------------------------------------------------------------
# Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
# Authors:										
#   Alessandro Febretti							febret@gmail.com
#---------------------------------------------------------------------------------------------------------------------
# [LICENSE NOTE]
#---------------------------------------------------------------------------------------------------------------------
# [SUMMARY OF FILE CONTENTS]
#*********************************************************************************************************************/

#######################################################################################################################
# Find required packages
# take away these and use libs included with project.
find_package(OpenGL)

#######################################################################################################################
# Compile definitions
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS -DLIBCONFIGXX_STATIC -DOMEGA_EXPORTING)

#######################################################################################################################
# Exclude libcmt when linking in visual studio
IF(MSVC)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
ENDIF()

#######################################################################################################################
# Source files
SET( srcs 
        osystem.cpp
		Color.cpp
		Config.cpp
		EqualizerDisplaySystem.cpp
        FontManager.cpp
        GfxUtils.cpp
#		GLUTDisplaySystem.cpp
        GpuBuffer.cpp
        GpuEnvironment.cpp
        GpuManager.cpp
        GpuProgram.cpp
		InputManager.cpp
		InputService.cpp
		Math.cpp
        Observer.cpp
		SystemManager.cpp
        teapot.cpp
        TextureManager.cpp
        Utils.cpp
		
		# omega::scene
		scene/Cube.cpp
		scene/Drawable.cpp
		scene/SceneManager.cpp
		scene/SceneNode.cpp
		
		# omega::ui
		ui/AbstractButton.cpp
		ui/Box.cpp
		ui/Button.cpp
		ui/Image.cpp
		ui/Label.cpp
		ui/PalladiumSkin.cpp
		ui/Slider.cpp
		ui/UIManager.cpp
		ui/Widget.cpp
        )
		
#######################################################################################################################
# Headers
SET( headers 
        ${OmegaLib_SOURCE_DIR}/include/omega.h
        ${OmegaLib_SOURCE_DIR}/include/omega/osystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Application.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Color.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Config.h
        ${OmegaLib_SOURCE_DIR}/include/omega/DisplaySystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/EqualizerDisplaySystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/FontManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/GfxUtils.h
#		${OmegaLib_SOURCE_DIR}/include/omega/GLUTDisplaySystem.h
        ${OmegaLib_SOURCE_DIR}/include/omega/GpuBuffer.h
        ${OmegaLib_SOURCE_DIR}/include/omega/GpuEnvironment.h
        ${OmegaLib_SOURCE_DIR}/include/omega/GpuManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/GpuProgram.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputEvent.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputService.h
        ${OmegaLib_SOURCE_DIR}/include/omega/InputFilter.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Math.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Observer.h
        ${OmegaLib_SOURCE_DIR}/include/omega/SystemManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/TextureManager.h
        ${OmegaLib_SOURCE_DIR}/include/omega/Utils.h
		
		# omega::scene
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Cube.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Drawable.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SceneManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SceneNode.h
		
		# omega::ui
		${OmegaLib_SOURCE_DIR}/include/omega/ui/AbstractButton.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Box.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Button.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Image.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Label.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/PalladiumSkin.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Slider.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/UIManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Widget.h
        ) 
        
#######################################################################################################################
# Enable / disable specific modules within omegalib
SET(OMEGA_USE_MOUSE true CACHE BOOL "Enable/disable mouse input support")
IF(OMEGA_USE_MOUSE)
	SET( srcs	 ${srcs} input/MouseService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/MouseService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_MOUSE)
ENDIF(OMEGA_USE_MOUSE)

SET(OMEGA_USE_MOCAP true CACHE BOOL "Enable/disable NaturalPoint MoCap support")
IF(OMEGA_USE_MOCAP)
	SET( srcs	 ${srcs} input/MoCapService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/MoCapService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_MOCAP)
ENDIF(OMEGA_USE_MOCAP)

SET(OMEGA_USE_OPTITRACK true CACHE BOOL "Enable/disable OptiTrack camera support")
IF(OMEGA_USE_OPTITRACK)
	SET( srcs	 ${srcs} input/OptiTrackService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/OptiTrackService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_OPTITRACK)
ENDIF(OMEGA_USE_OPTITRACK)

SET(OMEGA_USE_PQLABS true CACHE BOOL "Enable/disable PqLabs multitouch support")
IF(OMEGA_USE_PQLABS)
	SET( srcs	 ${srcs} input/PQService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/PQService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_PQLABS)
ENDIF(OMEGA_USE_PQLABS)

SET(OMEGA_USE_NETSERVICE true CACHE BOOL "Enable/disable Network input support")
IF(OMEGA_USE_NETSERVICE)
	SET( srcs	 ${srcs} input/NetService.cpp )
	SET( headers ${headers} ${OmegaLib_SOURCE_DIR}/include/omega/input/NetService.h )
	ADD_DEFINITIONS( -DOMEGA_USE_NETSERVICE)
ENDIF(OMEGA_USE_NETSERVICE)

#######################################################################################################################
SOURCE_GROUP(omega REGULAR_EXPRESSION omega/)		
SOURCE_GROUP(omega::input REGULAR_EXPRESSION omega/input/)		
SOURCE_GROUP(omega::ui REGULAR_EXPRESSION omega/ui/)		
SOURCE_GROUP(omega::scene REGULAR_EXPRESSION omega/scene/)		

#######################################################################################################################
# Setup compile info
ADD_LIBRARY( omega SHARED ${srcs} ${headers})

#######################################################################################################################
# Setup module-specific link info
IF(OMEGA_USE_MOCAP)
	TARGET_LINK_LIBRARIES( omega debug ${LIB_DEBUG}/NatNetLibStatic.lib optimized ${LIB_RELEASE}/NatNetLibStatic.lib)
ENDIF(OMEGA_USE_MOCAP)

IF(OMEGA_USE_OPTITRACK)
	TARGET_LINK_LIBRARIES( omega debug ${LIB_DEBUG}/optitrackuuid.lib optimized ${LIB_RELEASE}/optitrackuuid.lib)
ENDIF(OMEGA_USE_OPTITRACK)

IF(OMEGA_USE_PQLABS)
	TARGET_LINK_LIBRARIES( omega debug ${LIB_DEBUG}/PQMTClient.lib optimized ${LIB_RELEASE}/PQMTClient.lib)
ENDIF(OMEGA_USE_PQLABS)

#######################################################################################################################
# Setup link info.
TARGET_LINK_LIBRARIES( omega
	libconfig
	debug ${LIB_DEBUG}/pthreadVC2.lib optimized ${LIB_RELEASE}/pthreadVC2.lib
#	debug ${LIB_DEBUG}/rpcrt4.lib optimized ${LIB_RELEASE}/rpcrt4.lib
	debug ${LIB_DEBUG}/equalizer.lib optimized ${LIB_RELEASE}/equalizer.lib
	debug ${LIB_DEBUG}/ws2_32 optimized ${LIB_RELEASE}/ws2_32
	debug ${LIB_DEBUG}/glut32.lib optimized ${LIB_RELEASE}/glut32.lib
	debug ${LIB_DEBUG}/freetype2312_D.lib optimized ${LIB_RELEASE}/freetype2312.lib
	debug ${LIB_DEBUG}/ftgl_static_D.lib optimized ${LIB_RELEASE}/ftgl_static.lib
	debug ${LIB_DEBUG}/FreeImage.lib optimized ${LIB_RELEASE}/FreeImage.lib
	debug ${LIB_DEBUG}/OpenCL.lib optimized ${LIB_RELEASE}/OpenCL.lib
	${OPENGL_LIBRARY})
