if(OMEGA_OS_WIN)
	set(EXTLIB_NAME OpenSceneGraph-3.0.1-VS10-x86)
elseif(OMEGA_OS_LINUX)
	if(OMEGA_ARCH_32)
		set(EXTLIB_NAME OpenSceneGraph-3.0.1-linux-x86)
	else(OMEGA_ARCH_32)
		set(EXTLIB_NAME OpenSceneGraph-3.0.1-Linux-x64)
	endif(OMEGA_ARCH_32)
endif(OMEGA_OS_WIN)

set(EXTLIB_TGZ ${CMAKE_BINARY_DIR}/${EXTLIB_NAME}.tar.gz)
set(EXTLIB_DIR ${CMAKE_BINARY_DIR}/${EXTLIB_NAME})

# Uncomment this line to make omegalib use an external openscenegraph binary build
# set(EXTLIB_DIR D:/Workspace/3rdparty/OpenSceneGraph-3.0.1-VS10.0.30319-x86-debug-12741)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/${EXTLIB_NAME}.tar.gz)
  message(STATUS "Downloading OpenSceneGraph binary archive...")
  file(DOWNLOAD http://omegalib.googlecode.com/files/${EXTLIB_NAME}.tar.gz ${EXTLIB_TGZ} SHOW_PROGRESS)
endif(NOT EXISTS ${CMAKE_BINARY_DIR}/${EXTLIB_NAME}.tar.gz)

if(NOT EXISTS ${EXTLIB_DIR})
  message(STATUS "Extracting OpenSceneGraph...")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf
    ${EXTLIB_TGZ} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif(NOT EXISTS ${EXTLIB_DIR})

include_directories(${EXTLIB_DIR}/include)

if(OMEGA_OS_WIN)
	set(OSG_LIBS 
		optimized ${EXTLIB_DIR}/lib/release/osg.lib
		optimized ${EXTLIB_DIR}/lib/release/osgAnimation.lib
		optimized ${EXTLIB_DIR}/lib/release/osgDB.lib
		optimized ${EXTLIB_DIR}/lib/release/osgFX.lib
		optimized ${EXTLIB_DIR}/lib/release/osgGA.lib
		optimized ${EXTLIB_DIR}/lib/release/osgManipulator.lib
		optimized ${EXTLIB_DIR}/lib/release/osgParticle.lib
		optimized ${EXTLIB_DIR}/lib/release/osgPresentation.lib
		optimized ${EXTLIB_DIR}/lib/release/osgShadow.lib
		optimized ${EXTLIB_DIR}/lib/release/osgSim.lib
		optimized ${EXTLIB_DIR}/lib/release/osgTerrain.lib
		optimized ${EXTLIB_DIR}/lib/release/osgText.lib
		optimized ${EXTLIB_DIR}/lib/release/osgUtil.lib
		optimized ${EXTLIB_DIR}/lib/release/osgVolume.lib
		optimized ${EXTLIB_DIR}/lib/release/osgViewer.lib
		optimized ${EXTLIB_DIR}/lib/release/osgWidget.lib
		
		
		debug ${EXTLIB_DIR}/lib/debug/osgd.lib
		debug ${EXTLIB_DIR}/lib/debug/osgAnimationd.lib
		debug ${EXTLIB_DIR}/lib/debug/osgDBd.lib
		debug ${EXTLIB_DIR}/lib/debug/osgFXd.lib
		debug ${EXTLIB_DIR}/lib/debug/osgGAd.lib
		debug ${EXTLIB_DIR}/lib/debug/osgManipulatord.lib
		debug ${EXTLIB_DIR}/lib/debug/osgParticled.lib
		debug ${EXTLIB_DIR}/lib/debug/osgPresentationd.lib
		debug ${EXTLIB_DIR}/lib/debug/osgShadowd.lib
		debug ${EXTLIB_DIR}/lib/debug/osgSimd.lib
		debug ${EXTLIB_DIR}/lib/debug/osgTerraind.lib
		debug ${EXTLIB_DIR}/lib/debug/osgTextd.lib
		debug ${EXTLIB_DIR}/lib/debug/osgUtild.lib
		debug ${EXTLIB_DIR}/lib/debug/osgVolumed.lib
		debug ${EXTLIB_DIR}/lib/debug/osgViewerd.lib
		debug ${EXTLIB_DIR}/lib/debug/osgWidgetd.lib)
		
	# Copy the dlls into the target directories
	file(COPY ${EXTLIB_DIR}/bin/debug/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG} PATTERN "*.dll")
	file(COPY ${EXTLIB_DIR}/bin/release/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE} PATTERN "*.dll")
elseif(OMEGA_OS_LINUX)
endif(OMEGA_OS_WIN)
