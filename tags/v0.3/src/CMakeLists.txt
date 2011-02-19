####################################################################################################################### 
# THE OMEGA LIB PROJECT
#---------------------------------------------------------------------------------------------------------------------
# Copyright 2010-2011							Electronic Visualization Laboratory, University of Illinois at Chicago
# Authors:										
#  Alessandro Febretti							febret@gmail.com
#---------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
# All rights reserved.
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
# following conditions are met:
# 
# Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
# disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
# and the following disclaimer in the documentation and/or other materials provided with the distribution. 
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
# INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
######################################################################################################################

#######################################################################################################################
# Directories needed for compilation
INCLUDE_DIRECTORIES(
  ${OmegaLib_SOURCE_DIR}/include/
  ${OmegaLib_SOURCE_DIR}/include/equalizer
)

#######################################################################################################################
# Options
SET(OMEGA_BUILD_STATIC false CACHE BOOL "Build omega as a static library")
IF(OMEGA_BUILD_STATIC)
	ADD_DEFINITIONS(-DOMEGA_STATIC)
ENDIF(OMEGA_BUILD_STATIC)

SET(OMEGA_USE_DISPLAY false CACHE BOOL "Enable display system support.")
IF(OMEGA_USE_DISPLAY)
	SET(OMEGA_BUILD_UTILITY_LIB false CACHE BOOL "Enable build of the omegalib utility library (outils)")
ENDIF(OMEGA_USE_DISPLAY)
IF(OMEGA_BUILD_UTILITY_LIB)
	SET(OMEGA_BUILD_DEMOS false CACHE BOOL "Enable build of demo applications included in Omegalib")
	SET(OMEGA_BUILD_PLAYGROUND false CACHE BOOL "Enable build of playground projects. Playgroud projects are experimental, and may not work. ")
ENDIF(OMEGA_BUILD_UTILITY_LIB)

#######################################################################################################################
IF(MSVC)
	# Exclude libcmt when linking in visual studio
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
	ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS)
ENDIF()

#######################################################################################################################
# Add subdirectiories
ADD_SUBDIRECTORY(3rdparty/libconfig)
ADD_SUBDIRECTORY(omega)
ADD_SUBDIRECTORY(oinputserver)

IF(OMEGA_BUILD_UTILITY_LIB)
	ADD_SUBDIRECTORY(outils)
ENDIF(OMEGA_BUILD_UTILITY_LIB)

IF(OMEGA_BUILD_DEMOS)
	ADD_SUBDIRECTORY(demos/meshviewer)
	ADD_SUBDIRECTORY(demos/ohello)
ENDIF(OMEGA_BUILD_DEMOS)

IF(OMEGA_BUILD_PLAYGROUND)
	ADD_SUBDIRECTORY(playground/vtkdemo)
ENDIF(OMEGA_BUILD_PLAYGROUND)
