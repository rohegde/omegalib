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
add_definitions( -D_CRT_SECURE_NO_WARNINGS -DOENGINE_EXPORTING -DEQ_FABRIC_STATIC -DFREEGLUT_STATIC )

#set(OENGINE_USE_OBJ false CACHE BOOL "Enable support for obj format inside oengine.")
#set(OENGINE_USE_PLY true CACHE BOOL "Enable support for ply format inside oengine.")
set(OENGINE_USE_OBJ false)
set(OENGINE_USE_PLY true)

###################################################################################################
# Source files
set( srcs 
		oengine/BoundingSphere.cpp
		oengine/Box.cpp
		oengine/ControllerManipulator.cpp
		oengine/DefaultMouseInteractor.cpp
		oengine/DefaultRenderPass.cpp
		oengine/DefaultTwoHandsInteractor.cpp
		oengine/Effect.cpp
		oengine/glteapot.cpp
		oengine/LightingModule.cpp
		oengine/LightingPass.cpp
		oengine/Mesh.cpp
		oengine/MeshData.cpp
		oengine/OverlayRenderPass.cpp
		oengine/ReferenceBox.cpp
		oengine/RenderableSceneObject.cpp
		oengine/SceneEditorModule.cpp
		oengine/SceneRenderable.cpp
		oengine/TransparentRenderPass.cpp
		oengine/UiModule.cpp
		oengine/UiRenderPass.cpp
		# omega::ui
		oengine/ui/AbstractButton.cpp
		oengine/ui/Button.cpp
		oengine/ui/Container.cpp
		oengine/ui/Image.cpp
		oengine/ui/Label.cpp
		oengine/ui/DefaultSkin.cpp
		oengine/ui/Slider.cpp
		oengine/ui/Widget.cpp
		oengine/ui/UserManagerPanel.cpp
        )
		
###################################################################################################
# Headers
set( headers 
		${OmegaLib_SOURCE_DIR}/include/oengine/BoundingSphere.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Box.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ControllerManipulator.h
		${OmegaLib_SOURCE_DIR}/include/oengine/DefaultMouseInteractor.h
		${OmegaLib_SOURCE_DIR}/include/oengine/DefaultTwoHandsInteractor.h
		${OmegaLib_SOURCE_DIR}/include/oengine/DefaultRenderPass.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Effect.h
		${OmegaLib_SOURCE_DIR}/include/oengine/glteapot.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Light.h
		${OmegaLib_SOURCE_DIR}/include/oengine/LightingModule.h
		${OmegaLib_SOURCE_DIR}/include/oengine/LightingPass.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Mesh.h
		${OmegaLib_SOURCE_DIR}/include/oengine/MeshData.h
		${OmegaLib_SOURCE_DIR}/include/oengine/OverlayRenderPass.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ply.h
		${OmegaLib_SOURCE_DIR}/include/oengine/PlyDataReader.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ReferenceBox.h
		${OmegaLib_SOURCE_DIR}/include/oengine/RenderableSceneObject.h
		${OmegaLib_SOURCE_DIR}/include/oengine/SceneEditorModule.h
		${OmegaLib_SOURCE_DIR}/include/oengine/SceneRenderable.h
		${OmegaLib_SOURCE_DIR}/include/oengine/Teapot.h
		${OmegaLib_SOURCE_DIR}/include/oengine/TransparentRenderPass.h
		${OmegaLib_SOURCE_DIR}/include/oengine/UiModule.h
		${OmegaLib_SOURCE_DIR}/include/oengine/UiRenderPass.h
		# omega::ui
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/AbstractButton.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/Button.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/Container.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/Image.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/Label.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/DefaultSkin.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/Slider.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/Widget.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/WidgetFactory.h
		${OmegaLib_SOURCE_DIR}/include/oengine/ui/UserManagerPanel.h
        ) 

###################################################################################################
if(OENGINE_USE_OBJ)
	set( srcs ${srcs}
		oengine/ObjDataReader.cpp
		oengine/obj/obj.cpp
		oengine/obj/obj.hpp
		oengine/obj/obj_config.hpp
		oengine/obj/obj_parser.cpp
		oengine/obj/obj_parser.hpp	)
	set( headers ${headers}
		${OmegaLib_SOURCE_DIR}/include/oengine/ObjDataReader.h)
	add_definitions(-DOENGINE_USE_OBJ)
endif(OENGINE_USE_OBJ)

if(OENGINE_USE_PLY)
	set( srcs ${srcs}
		oengine/PlyDataReader.cpp
		oengine/plyfile.cpp)
	set( headers ${headers}
		${OmegaLib_SOURCE_DIR}/include/oengine/PlyDataReader.h)
	add_definitions(-DOENGINE_USE_PLY)
endif(OENGINE_USE_PLY)

###################################################################################################
source_group(oengine REGULAR_EXPRESSION oengine/)		
source_group(oengine::ui REGULAR_EXPRESSION oengine/ui/)		
source_group(oengine::mvc REGULAR_EXPRESSION oengine/mvc/)		

###################################################################################################
# Setup compile info
enable_precompiled_headers(oengine/precompiled.h srcs)
add_library( oengine SHARED ${srcs} ${headers})

###################################################################################################
# Setup link info.
target_link_libraries( oengine omega)
if(OMEGA_USE_PYTHON)
	target_link_libraries( oengine ${PYTHON_LIBRARIES})
endif(OMEGA_USE_PYTHON)

###################################################################################################
# configure include header

