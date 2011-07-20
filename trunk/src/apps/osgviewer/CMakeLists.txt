###################################################################################################
# THE OMEGA LIB PROJECT
#-------------------------------------------------------------------------------------------------
# Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
# Authors:										
#  Alessandro Febretti		febret@gmail.com
#-------------------------------------------------------------------------------------------------
# Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
# All rights reserved.
# Redistribution and use in source and binary forms, with or without modification, are permitted 
# provided that the following conditions are met:
# 
# Redistributions of source code must retain the above copyright notice, this list of conditions 
# and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
# notice, this list of conditions and the following disclaimer in the documentation and/or other 
# materials provided with the distribution. 
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
# USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
###################################################################################################

###################################################################################################
find_package(OpenSceneGraph 2.9.8 COMPONENTS osgDB osgUtil)
if(OSG_LIBRARY MATCHES "OSG_LIBRARY-NOTFOUND")
  message(STATUS "Skipping osgviewer example (OSG not found)")	 
  return()
endif()
if(${OPENSCENEGRAPH_VERSION} VERSION_LESS "2.9.8")
  message(STATUS "Skipping osgviewer (OSG ${OPENSCENEGRAPH_VERSION} found, need >= 2.9.8)")
  return()
endif()


include_directories(SYSTEM ${OPENSCENEGRAPH_INCLUDE_DIRS})

###################################################################################################
# Source files
set(SRCS 
		SceneView.cpp
		osgviewer.cpp)
    
###################################################################################################
# Headers
set(HEADERS 
		SceneView.h
		osgviewer.h) 
        
###################################################################################################
# Setup compile info
add_executable(osgviewer ${SRCS} ${HEADERS})

###################################################################################################
# Setup link info.
target_link_libraries(osgviewer
	omega 
	outils
    oosg
	${OPENSCENEGRAPH_LIBRARIES})
