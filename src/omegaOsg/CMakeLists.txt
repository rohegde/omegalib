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
# Compile definitions
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS -DOOSG_EXPORTING)

###################################################################################################
# Source files
SET( srcs 
		omegaOsg/OsgModule.cpp
		omegaOsg/OsgRenderPass.cpp
		omegaOsg/OsgSceneObject.cpp
		omegaOsg/SceneView.cpp
		omegaOsg/ReaderFreeImage.cpp
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
        ) 
        
###################################################################################################
source_group(omegaOsg REGULAR_EXPRESSION omegaOsg/)		

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
	omega ${OSG_LIBS})
