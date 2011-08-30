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
add_definitions( -D_CRT_SECURE_NO_WARNINGS -DOENGINE_EXPORTING -DFTGL_LIBRARY_STATIC -DEQ_FABRIC_STATIC -DFREEGLUT_STATIC)

###################################################################################################
# Source files
set( srcs 
		# omega
		oengine/FontManager.cpp
		oengine/ImageUtils.cpp
		oengine/EngineClient.cpp
		oengine/Renderer.cpp
		oengine/Effect.cpp
		oengine/EffectManager.cpp
		oengine/DefaultRenderPass.cpp
		oengine/OverlayRenderPass.cpp
		oengine/LightingPass.cpp
		oengine/RenderToTexture.cpp
		oengine/Camera.cpp
		oengine/BoundingSphere.cpp
		oengine/DefaultMouseInteractor.cpp
		oengine/DefaultTwoHandsInteractor.cpp
		oengine/Mesh.cpp
		oengine/MeshData.cpp
		oengine/MeshManager.cpp
		oengine/Node.cpp
		oengine/Teapot.cpp
		oengine/SceneManager.cpp
		oengine/SceneQuery.cpp
		oengine/SceneNode.cpp
		oengine/PlyDataReader.cpp
		oengine/plyfile.cpp
		oengine/ReferenceBox.cpp
		
		# omega::ui
		oengine/ui/AbstractButton.cpp
		oengine/ui/Button.cpp
		oengine/ui/Container.cpp
		oengine/ui/Image.cpp
		oengine/ui/Label.cpp
		oengine/ui/DefaultSkin.cpp
		oengine/ui/Slider.cpp
		oengine/ui/UiManager.cpp
		oengine/ui/Widget.cpp
		oengine/ui/UserManagerPanel.cpp

		#omega::mvc
		oengine/mvc/Model.cpp
		oengine/mvc/ViewManager.cpp
		oengine/mvc/View.cpp
		oengine/mvc/ViewClient.cpp
		oengine/mvc/Controller.cpp
		oengine/mvc/DynamicController.cpp
		oengine/mvc/UiDynamicController.cpp
        )
		
###################################################################################################
# Headers
set( headers 
		# omega
		${OmegaLib_SOURCE_DIR}/include/oengine.h
		${OmegaLib_SOURCE_DIR}/include/oengine/oenginebase.h
		${OmegaLib_SOURCE_DIR}/include/oengine/FontManager.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ImageUtils.h
		${OmegaLib_SOURCE_DIR}/include/oengine/EngineClient.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Renderer.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Actor.h
		${OmegaLib_SOURCE_DIR}/include/oengine/BoundingSphere.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Camera.h
		${OmegaLib_SOURCE_DIR}/include/oengine/DefaultMouseInteractor.h
		${OmegaLib_SOURCE_DIR}/include/oengine/DefaultTwoHandsInteractor.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Mesh.h
		${OmegaLib_SOURCE_DIR}/include/oengine/MeshData.h
		${OmegaLib_SOURCE_DIR}/include/oengine/MeshManager.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Node.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Effect.h
		${OmegaLib_SOURCE_DIR}/include/oengine/EffectManager.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Renderable.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Teapot.h
		${OmegaLib_SOURCE_DIR}/include/oengine/SceneManager.h
		${OmegaLib_SOURCE_DIR}/include/oengine/SceneQuery.h
		${OmegaLib_SOURCE_DIR}/include/oengine/SceneNode.h
		${OmegaLib_SOURCE_DIR}/include/oengine/PlyDataReader.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ply.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ReferenceBox.h
		${OmegaLib_SOURCE_DIR}/include/oengine/RenderToTexture.h
		${OmegaLib_SOURCE_DIR}/include/oengine/RenderPass.h
		${OmegaLib_SOURCE_DIR}/include/oengine/DefaultRenderPass.h
		${OmegaLib_SOURCE_DIR}/include/oengine/OverlayRenderPass.h
		${OmegaLib_SOURCE_DIR}/include/oengine/LightingPass.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Light.h
		
		# omega::ui
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/AbstractButton.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/Button.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/Container.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/Image.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/Label.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/DefaultSkin.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/Slider.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/UIManager.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/Widget.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/WidgetFactory.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/UserManagerPanel.h

		#omega::mvc
		${OmegaLib_SOURCE_DIR}/include/oengine/mvc/Model.h
		${OmegaLib_SOURCE_DIR}/include/oengine/mvc/ViewManager.h
		${OmegaLib_SOURCE_DIR}/include/oengine/mvc/View.h
		${OmegaLib_SOURCE_DIR}/include/oengine/mvc/ViewClient.h
		${OmegaLib_SOURCE_DIR}/include/oengine/mvc/Controller.h
		${OmegaLib_SOURCE_DIR}/include/oengine/mvc/DynamicController.h
		${OmegaLib_SOURCE_DIR}/include/oengine/mvc/UiDynamicController.h
        ) 

###################################################################################################
source_group(oengine REGULAR_EXPRESSION oengine/)		
source_group(oengine::ui REGULAR_EXPRESSION oengine/ui/)		
source_group(oengine::mvc REGULAR_EXPRESSION oengine/mvc/)		

###################################################################################################
# Setup compile info
if(OMEGA_BUILD_STATIC)
	add_library( oengine ${srcs} ${headers})
else(OMEGA_BUILD_STATIC)
	add_library( oengine SHARED ${srcs} ${headers})
endif(OMEGA_BUILD_STATIC)

###################################################################################################
# Setup link info.
target_link_libraries( oengine omega freetype ftgl FreeImage)
if(OMEGA_USE_PYTHON)
	target_link_libraries( oengine ${PYTHON_LIBRARIES})
endif(OMEGA_USE_PYTHON)

###################################################################################################
# configure include header

