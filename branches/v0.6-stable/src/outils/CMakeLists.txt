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
add_definitions( -D_CRT_SECURE_NO_WARNINGS -DOUTILS_EXPORTING -DFTGL_LIBRARY_STATIC -DEQ_FABRIC_STATIC -DFREEGLUT_STATIC)

###################################################################################################
# Source files
set( srcs 
		# omega
		omega/FontManager.cpp
		omega/ImageUtils.cpp
		omega/EngineClient.cpp
		omega/Renderer.cpp
		
		# omega::scene
		omega/scene/Camera.cpp
		omega/scene/BoundingSphere.cpp
		omega/scene/DefaultMouseInteractor.cpp
		omega/scene/DefaultTwoHandsInteractor.cpp
		omega/scene/Effect.cpp
		omega/scene/EffectManager.cpp
		omega/scene/Mesh.cpp
		omega/scene/MeshData.cpp
		omega/scene/MeshManager.cpp
		omega/scene/Node.cpp
		omega/scene/Teapot.cpp
		omega/scene/SceneManager.cpp
		omega/scene/SceneQuery.cpp
		omega/scene/SceneNode.cpp
		omega/scene/PlyDataReader.cpp
		omega/scene/plyfile.cpp
		omega/scene/ReferenceBox.cpp
		omega/scene/RenderToTexture.cpp
		omega/scene/DefaultRenderPass.cpp
		omega/scene/OverlayRenderPass.cpp
		omega/scene/LightingPass.cpp
		
		# omega::ui
		omega/ui/AbstractButton.cpp
		omega/ui/Button.cpp
		omega/ui/Container.cpp
		omega/ui/Image.cpp
		omega/ui/Label.cpp
		omega/ui/DefaultSkin.cpp
		omega/ui/Slider.cpp
		omega/ui/UIManager.cpp
		omega/ui/Widget.cpp
		omega/ui/UserManagerPanel.cpp

		#omega::mvc
		omega/mvc/Model.cpp
		omega/mvc/ViewManager.cpp
		omega/mvc/View.cpp
		omega/mvc/ViewClient.cpp
		omega/mvc/Controller.cpp
		omega/mvc/DynamicController.cpp
		omega/mvc/UIDynamicController.cpp

		#omega::script
		omega/script/ScriptInterpreter.cpp
		omega/script/ScriptModule.cpp
        )
		
###################################################################################################
# Headers
set( headers 
		# omega
		${OmegaLib_SOURCE_DIR}/include/omega/FontManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/ImageUtils.h
		${OmegaLib_SOURCE_DIR}/include/omega/EngineClient.h
		${OmegaLib_SOURCE_DIR}/include/omega/Renderer.h
		
		# omega::scene
		${OmegaLib_SOURCE_DIR}/include/omega/scene.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Actor.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/BoundingSphere.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Camera.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/DefaultMouseInteractor.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/DefaultTwoHandsInteractor.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Mesh.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/MeshData.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/MeshManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Node.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Effect.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/EffectManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Renderable.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Teapot.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SceneManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SceneQuery.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/SceneNode.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/PlyDataReader.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/ply.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/ReferenceBox.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/RenderToTexture.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/RenderPass.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/DefaultRenderPass.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/OverlayRenderPass.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/LightingPass.h
		${OmegaLib_SOURCE_DIR}/include/omega/scene/Light.h
		
		# omega::ui
		${OmegaLib_SOURCE_DIR}/include/omega/ui.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/AbstractButton.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Button.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Container.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Image.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Label.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/DefaultSkin.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Slider.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/UIManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/Widget.h
		${OmegaLib_SOURCE_DIR}/include/omega/ui/UserManagerPanel.h

		#omega::mvc
		${OmegaLib_SOURCE_DIR}/include/omega/mvc/Model.h
		${OmegaLib_SOURCE_DIR}/include/omega/mvc/ViewManager.h
		${OmegaLib_SOURCE_DIR}/include/omega/mvc/View.h
		${OmegaLib_SOURCE_DIR}/include/omega/mvc/ViewClient.h
		${OmegaLib_SOURCE_DIR}/include/omega/mvc/Controller.h
		${OmegaLib_SOURCE_DIR}/include/omega/mvc/DynamicController.h
		${OmegaLib_SOURCE_DIR}/include/omega/mvc/UIDynamicController.h

		#omega::script
		${OmegaLib_SOURCE_DIR}/include/omega/script/ScriptInterpreter.h
		${OmegaLib_SOURCE_DIR}/include/omega/script/ScriptModule.h
        ) 

if(OMEGA_USE_PYTHON)
	set( headers ${headers}
		omega/script/PythonInterpreterWrapper.h
		${OmegaLib_SOURCE_DIR}/include/omega/script/PythonInterpreter.h
		${OmegaLib_SOURCE_DIR}/include/omega/script/PythonModule.h)

			
	set( srcs ${srcs}
		omega/script/PythonInterpreter.cpp
		omega/script/PythonModule.cpp)
endif(OMEGA_USE_PYTHON)
        
###################################################################################################
source_group(omega REGULAR_EXPRESSION omega/)		
source_group(omega::ui REGULAR_EXPRESSION omega/ui/)		
source_group(omega::scene REGULAR_EXPRESSION omega/scene/)		
source_group(omega::mvc REGULAR_EXPRESSION omega/mvc/)		
source_group(omega::script REGULAR_EXPRESSION omega/script/)		

###################################################################################################
# Setup compile info
if(OMEGA_BUILD_STATIC)
	add_library( outils ${srcs} ${headers})
else(OMEGA_BUILD_STATIC)
	add_library( outils SHARED ${srcs} ${headers})
endif(OMEGA_BUILD_STATIC)

###################################################################################################
# Setup link info.
target_link_libraries( outils omega freetype ftgl FreeImage)
if(OMEGA_USE_PYTHON)
	target_link_libraries( outils ${PYTHON_LIBRARIES})
endif(OMEGA_USE_PYTHON)
