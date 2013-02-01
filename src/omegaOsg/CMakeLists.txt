###################################################################################################
# THE OMEGA LIB PROJECT
#-------------------------------------------------------------------------------------------------
# Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
# Authors:										
#  Alessandro Febretti		febret@gmail.com
#-------------------------------------------------------------------------------------------------
# Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
include(${CMAKE_SOURCE_DIR}/external/UseFbx.cmake)

###################################################################################################
# Compile definitions
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS -DOOSG_EXPORTING)

###################################################################################################
# Source files
SET( srcs 
		OsgModule.cpp
		OsgRenderPass.cpp
		OsgSceneObject.cpp
		SceneView.cpp
		ReaderFreeImage.cpp
		
		fbx/fbxRAnimation.cpp
		fbx/fbxRCamera.cpp
		fbx/fbxRLight.cpp
		fbx/fbxRMesh.cpp
		fbx/fbxRNode.cpp
		fbx/ReaderWriterFBX.cpp
		fbx/WriterCompareTriangle.cpp
		fbx/WriterNodeVisitor.cpp
		fbx/fbxMaterialToOsgStateSet.cpp
		)
		
###################################################################################################
# Headers
SET( headers 
		# omega
		${CMAKE_SOURCE_DIR}/include/omegaOsg/oosgbase.h
		${CMAKE_SOURCE_DIR}/include/omegaOsg/OsgModule.h
		${CMAKE_SOURCE_DIR}/include/omegaOsg/OsgRenderPass.h
		${CMAKE_SOURCE_DIR}/include/omegaOsg/OsgSceneObject.h
		${CMAKE_SOURCE_DIR}/include/omegaOsg/SceneView.h
		${CMAKE_SOURCE_DIR}/include/omegaOsg/ReaderFreeImage.h
		
		fbx/fbxMaterialToOsgStateSet.h
		fbx/fbxReader.h
		fbx/ReaderWriterFBX.h
		fbx/WriterCompareTriangle.h
		fbx/WriterNodeVisitor.h
        ) 
        
###################################################################################################
set(OMEGAOSG_USE_INVENTOR false CACHE BOOL "Enables Inventor (.vrml, .iv) support for omegaOsg. Needs the Coin3d library installed")
if(OMEGAOSG_USE_INVENTOR)
    find_package(Inventor)
	add_definitions(-DCOIN_NOT_DLL)
    if(INVENTOR_FOUND)
		set( srcs  ${srcs}
				Inventor/ConvertToInventor.cpp
				Inventor/ConvertFromInventor.cpp
				Inventor/PendulumCallback.cpp
				Inventor/ReaderWriterIV.cpp
				Inventor/ShuttleCallback.cpp
			)
		set( headers  ${headers}
				Inventor/ConvertToInventor.h
				Inventor/ConvertFromInventor.h
				Inventor/PendulumCallback.h
				Inventor/ReaderWriterIV.h
				Inventor/ShuttleCallback.h
			)
		#ADD_DEFINITIONS(-DCOIN_DLL)
		include_directories(${INVENTOR_INCLUDE_DIR})
	endif()
endif()

###################################################################################################
source_group(omegaOsg REGULAR_EXPRESSION omegaOsg/)		
source_group(fbx REGULAR_EXPRESSION omegaOsg/fbx/)		
source_group(Inventor REGULAR_EXPRESSION omegaOsg/Inventor/)		

IF(WIN32)
    SET(TARGET_EXTERNAL_LIBRARIES wininet)
endif(WIN32)

IF(APPLE)
    FIND_LIBRARY(SYS_CONFIG_LIBRARY SystemConfiguration )
    MARK_AS_ADVANCED(SYS_CONFIG_LIBRARY)
    SET(TARGET_EXTERNAL_LIBRARIES iconv ${SYS_CONFIG_LIBRARY})
endif(APPLE)

###################################################################################################
# Setup compile info
if(OMEGA_BUILD_STATIC)
	add_library( omegaOsg ${srcs} ${headers})
else(OMEGA_BUILD_STATIC)
	add_library( omegaOsg SHARED ${srcs} ${headers})
endif(OMEGA_BUILD_STATIC)

###################################################################################################
# Setup link info.
target_link_libraries(omegaOsg 
	omega ${OSG_LIBS} ${FBX_LIBNAME} ${TARGET_EXTERNAL_LIBRARIES})

if(OMEGAOSG_USE_INVENTOR)
    if(INVENTOR_FOUND)
		target_link_libraries(omegaOsg ${INVENTOR_LIBRARY})
	endif()
endif()