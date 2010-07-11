#********************************************************************************************************************** 
# THE OMEGA LIB PROJECT
#---------------------------------------------------------------------------------------------------------------------
# Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
# Authors:										
#  Alessandro Febretti							febret@gmail.com
#---------------------------------------------------------------------------------------------------------------------
# [LICENSE NOTE]
#---------------------------------------------------------------------------------------------------------------------
# outk - Omega Utility Toolkit
#*********************************************************************************************************************/

#######################################################################################################################
# Find required packages
# take away these and use libs included with project.
find_package(OpenGL)

#######################################################################################################################
# Compile definitions
ADD_DEFINITIONS( -DOUTK_EXPORTING -D_CRT_SECURE_NO_WARNINGS -DLIBCONFIGXX_STATIC -DFTGL_LIBRARY_STATIC)

#######################################################################################################################
# Exclude libcmt when linking in visual studio
IF(MSVC)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
ENDIF()

#######################################################################################################################
# Source files
SET( srcs 
        gfx/FontManager.cpp
        gfx/GfxUtils.cpp
        gfx/teapot.cpp
        ui/AbstractButton.cpp
        ui/Box.cpp
        ui/Button.cpp
        ui/Label.cpp
        ui/UIManager.cpp
        ui/Widget.cpp
        )
		
#######################################################################################################################
# Headers
SET( headers 
        ${OmegaLib_SOURCE_DIR}/include/outk/gfx/FontManager.h
        ${OmegaLib_SOURCE_DIR}/include/outk/gfx/GfxUtils.h
        ${OmegaLib_SOURCE_DIR}/include/outk/ui/AbstractButton.h
        ${OmegaLib_SOURCE_DIR}/include/outk/ui/Box.h
        ${OmegaLib_SOURCE_DIR}/include/outk/ui/Button.h
        ${OmegaLib_SOURCE_DIR}/include/outk/ui/Label.h
        ${OmegaLib_SOURCE_DIR}/include/outk/ui/UIEvent.h
        ${OmegaLib_SOURCE_DIR}/include/outk/ui/UIManager.h
        ${OmegaLib_SOURCE_DIR}/include/outk/ui/Widget.h
        ${OmegaLib_SOURCE_DIR}/include/outk.h
        ) 
        
#######################################################################################################################
SOURCE_GROUP(gfx REGULAR_EXPRESSION gfx/)		

#######################################################################################################################
SOURCE_GROUP(ui REGULAR_EXPRESSION ui/)		

#######################################################################################################################
# Setup compile info
ADD_LIBRARY( outk SHARED ${srcs} ${headers})

#######################################################################################################################
# Setup link info.
TARGET_LINK_LIBRARIES( outk
	omega
	debug ${LIB_DEBUG}/freetype2312_D.lib optimized ${LIB_RELEASE}/freetype2312.lib
	debug ${LIB_DEBUG}/ftgl_static_D.lib optimized ${LIB_RELEASE}/ftgl_static.lib
	debug ${LIB_DEBUG}/glut32.lib optimized ${LIB_RELEASE}/glut32.lib
	${OPENGL_LIBRARY})
