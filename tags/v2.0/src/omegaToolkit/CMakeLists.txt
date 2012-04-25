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
add_definitions( -D_CRT_SECURE_NO_WARNINGS -DOTK_EXPORTING -DEQ_FABRIC_STATIC -DFREEGLUT_STATIC )

#set(omegaToolkit_USE_OBJ false CACHE BOOL "Enable support for obj format inside omegaToolkit.")
#set(omegaToolkit_USE_PLY true CACHE BOOL "Enable support for ply format inside omegaToolkit.")
set(OTK_USE_OBJ false)
set(OTK_USE_PLY true)

###################################################################################################
# Source files
set( srcs 
		omegaToolkit/ToolkitUtils.cpp
		omegaToolkit/Billboard.cpp
		omegaToolkit/BoundingSphere.cpp
		omegaToolkit/Box.cpp
		omegaToolkit/ControllerManipulator.cpp
		omegaToolkit/DefaultMouseInteractor.cpp
		omegaToolkit/DefaultTwoHandsInteractor.cpp
		omegaToolkit/Effect.cpp
		omegaToolkit/glteapot.cpp
		omegaToolkit/Mesh.cpp
		omegaToolkit/MeshData.cpp
		omegaToolkit/omegaToolkitPythonApi.cpp
		omegaToolkit/ReferenceBox.cpp
		omegaToolkit/RenderableSceneObject.cpp
		omegaToolkit/SceneEditorModule.cpp
		omegaToolkit/SceneRenderable.cpp
		omegaToolkit/UiModule.cpp
		omegaToolkit/UiRenderPass.cpp
		omegaToolkit/UiScriptCommand.cpp
		
		# omega::ui
		omegaToolkit/ui/AbstractButton.cpp
		omegaToolkit/ui/BareboneSkin.cpp
		omegaToolkit/ui/Button.cpp
		omegaToolkit/ui/Container.cpp
		omegaToolkit/ui/Image.cpp
		omegaToolkit/ui/Label.cpp
		omegaToolkit/ui/MenuManager.cpp
		omegaToolkit/ui/DefaultSkin.cpp
		omegaToolkit/ui/Slider.cpp
		omegaToolkit/ui/Widget.cpp
		omegaToolkit/ui/UserManagerPanel.cpp
        )
		
###################################################################################################
# Headers
set( headers 
		${CMAKE_SOURCE_DIR}/include/omegaToolkit.h
		
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ToolkitUtils.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/Billboard.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/BoundingSphere.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/Box.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ControllerManipulator.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/DefaultMouseInteractor.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/DefaultTwoHandsInteractor.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/Effect.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/glteapot.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/Mesh.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/MeshData.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ply.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/PlyDataReader.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ReferenceBox.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/RenderableSceneObject.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/SceneEditorModule.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/SceneRenderable.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/Teapot.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/UiModule.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/UiRenderPass.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/UiScriptCommand.h
		
		# omega::ui
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ui/AbstractButton.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ui/BareboneSkin.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ui/Button.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ui/Container.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ui/Image.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ui/Label.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ui/MenuManager.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ui/DefaultSkin.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ui/Slider.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ui/Widget.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ui/WidgetFactory.h
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ui/UserManagerPanel.h
        ) 

###################################################################################################
if(OTK_USE_OBJ)
	set( srcs ${srcs}
		omegaToolkit/ObjDataReader.cpp
		omegaToolkit/obj/obj.cpp
		omegaToolkit/obj/obj.hpp
		omegaToolkit/obj/obj_config.hpp
		omegaToolkit/obj/obj_parser.cpp
		omegaToolkit/obj/obj_parser.hpp	)
	set( headers ${headers}
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/ObjDataReader.h)
	add_definitions(-DOTK_USE_OBJ)
endif(OTK_USE_OBJ)

if(OTK_USE_PLY)
	set( srcs ${srcs}
		omegaToolkit/PlyDataReader.cpp
		omegaToolkit/plyfile.cpp)
	set( headers ${headers}
		${CMAKE_SOURCE_DIR}/include/omegaToolkit/PlyDataReader.h)
	add_definitions(-DOTK_USE_PLY)
endif(OTK_USE_PLY)

###################################################################################################
source_group(omegaToolkit REGULAR_EXPRESSION omegaToolkit/)		
source_group(omegaToolkit::ui REGULAR_EXPRESSION omegaToolkit/ui/)		
source_group(omegaToolkit::mvc REGULAR_EXPRESSION omegaToolkit/mvc/)		

###################################################################################################
# Setup compile info
enable_precompiled_headers(omegaToolkit/precompiled.h srcs)
add_library( omegaToolkit SHARED ${srcs} ${headers})

###################################################################################################
# Setup link info.
target_link_libraries( omegaToolkit omega)

###################################################################################################
# configure include header

