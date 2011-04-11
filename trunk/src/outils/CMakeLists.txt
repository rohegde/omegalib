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
ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS -DOUTILS_EXPORTING -DFTGL_LIBRARY_STATIC -DEQ_FABRIC_STATIC -DFREEGLUT_STATIC)

###################################################################################################
# Source files
SET( srcs 
		# omega
		omega/FontManager.cpp
		omega/ImageUtils.cpp
		omega/EngineClient.cpp
		
		# omega::scene
		omega/scene/Camera.cpp
		omega/scene/DefaultMouseInteractor.cpp
		omega/scene/DefaultTwoHandsInteractor.cpp
		omega/scene/Effect.cpp
		omega/scene/EffectManager.cpp
		omega/scene/Mesh.cpp
		omega/scene/MeshData.cpp
		omega/scene/MeshManager.cpp
		omega/scene/Node.cpp
		omega/scene/SimplePrimitive.cpp
		omega/scene/SceneManager.cpp
		omega/scene/SceneQuery.cpp
		omega/scene/SceneNode.cpp
		omega/scene/PlyDataReader.cpp
		omega/scene/plyfile.cpp
		omega/scene/ReferenceBox.cpp
		omega/scene/RenderToTexture.cpp
		
		# omega::ui
		omega/ui/AbstractButton.cpp
		omega/ui/Button.cpp
		omega/ui/Container.cpp
		omega/ui/Image.cpp
		omega/ui/Label.cpp
		omega/ui/Painter.cpp
		omega/ui/DefaultSkin.cpp
		omega/ui/Slider.cpp
		omega/ui/UIManager.cpp
		omega/ui/Widget.cpp
        )
		
###################################################################################################
# Headers
SET( headers 
		# omega
		${OmegaLib_SOURCE_DIR}/include/omega/FontManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/ImageUtils.h
		${OmegaLib_SOURCE_DIR}/include/omega/EngineClient.h
		
		# omega::scene
		${OmegaLib_SOURCE_DIR}/include/omega/scene.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Actor.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Camera.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/DefaultMouseInteractor.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/DefaultTwoHandsInteractor.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Mesh.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/MeshData.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/MeshManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Node.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Effect.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/EffectManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Drawable.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SimplePrimitive.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SceneManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SceneQuery.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SceneNode.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/PlyDataReader.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/ply.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/ReferenceBox.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/RenderToTexture.h
		
		# omega::ui
		${OmegaLib_SOURCE_DIR}/include/omega/ui.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/AbstractButton.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Button.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Container.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Image.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Label.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Painter.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/DefaultSkin.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Slider.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/UIManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Widget.h
        ) 
        
###################################################################################################
SOURCE_GROUP(omega REGULAR_EXPRESSION omega/)		
SOURCE_GROUP(omega::ui REGULAR_EXPRESSION omega/ui/)		
SOURCE_GROUP(omega::scene REGULAR_EXPRESSION omega/scene/)		

###################################################################################################
# Setup compile info
IF(OMEGA_BUILD_STATIC)
	ADD_LIBRARY( outils ${srcs} ${headers})
ELSE(OMEGA_BUILD_STATIC)
	ADD_LIBRARY( outils SHARED ${srcs} ${headers})
ENDIF(OMEGA_BUILD_STATIC)

###################################################################################################
if(MSVC)
	# Horrible horrible hack, ignore multiple symbol decinitions, but apparently 
	# needed to have VS2008 work with NstNetLib static...
	# See if you can remove this in the future.
    #set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /FORCE:MULTIPLE")
    #set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} /FORCE:MULTIPLE")
	#add_definitions(-D_CRT_SECURE_NO_WARNINGS /wd4244 /wd4018)
endif(MSVC)

###################################################################################################
# Setup link info.
TARGET_LINK_LIBRARIES( outils omega freetype ftgl FreeImage)
