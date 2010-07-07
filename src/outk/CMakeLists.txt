#********************************************************************************************************************** 
# THE OMEGA LIB PROJECT
#---------------------------------------------------------------------------------------------------------------------
# Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
# Authors:										
#  [Author]									[Mail]
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
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS -DLIBCONFIGXX_STATIC)

#######################################################################################################################
# Exclude libcmt when linking in visual studio
IF(MSVC)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
ENDIF()

#######################################################################################################################
# Source files
SET( srcs 
        gfx/GfxUtils.cpp
        gfx/teapot.cpp
        )
		
#######################################################################################################################
# Headers
SET( headers 
        ${OmegaLib_SOURCE_DIR}/include/outk/gfx/GfxUtils.h
        ${OmegaLib_SOURCE_DIR}/include/outk.h
        ) 
        
#######################################################################################################################
SOURCE_GROUP(gfx REGULAR_EXPRESSION gfx/)		

#######################################################################################################################
# Setup compile info
ADD_LIBRARY( outk SHARED ${srcs} ${headers})

#######################################################################################################################
# Setup link info.
TARGET_LINK_LIBRARIES( outk
	omega
	debug ${LIB_DEBUG}/glut32.lib optimized ${LIB_RELEASE}/glut32.lib
	${OPENGL_LIBRARY})
