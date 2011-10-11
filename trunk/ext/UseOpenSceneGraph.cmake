if(OMEGA_OS_WIN)
	set(EXTLIB_NAME OpenSceneGraph-3.0.0-VS10.0.30319-x86-release-12681)
elseif(OMEGA_OS_LINUX)
	set(EXTLIB_NAME OpenSceneGraph-3.0.0-linux)
endif(OMEGA_OS_WIN)

set(EXTLIB_TGZ ${CMAKE_SOURCE_DIR}/ext/${EXTLIB_NAME}.tar.gz)
set(EXTLIB_DIR ${CMAKE_BINARY_DIR}/${EXTLIB_NAME})

if(NOT EXISTS ${EXTLIB_DIR})
  message(STATUS "Extracting OpenSceneGraph...")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf
    ${EXTLIB_TGZ} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif(NOT EXISTS ${EXTLIB_DIR})

include_directories(${EXTLIB_DIR}/include)

if(OMEGA_OS_WIN)
	set(OSG_LIBS 
		${EXTLIB_DIR}/lib/osg.lib
		${EXTLIB_DIR}/lib/osgAnimation.lib
		${EXTLIB_DIR}/lib/osgDB.lib
		${EXTLIB_DIR}/lib/osgFX.lib
		${EXTLIB_DIR}/lib/osgGA.lib
		${EXTLIB_DIR}/lib/osgManipulator.lib
		${EXTLIB_DIR}/lib/osgParticle.lib
		${EXTLIB_DIR}/lib/osgPresentation.lib
		${EXTLIB_DIR}/lib/osgShadow.lib
		${EXTLIB_DIR}/lib/osgSim.lib
		${EXTLIB_DIR}/lib/osgTerrain.lib
		${EXTLIB_DIR}/lib/osgText.lib
		${EXTLIB_DIR}/lib/osgUtil.lib
		${EXTLIB_DIR}/lib/osgVolume.lib
		${EXTLIB_DIR}/lib/osgViewer.lib
		${EXTLIB_DIR}/lib/osgWidget.lib)
		
	# Copy the dlls into the target directories
	file(COPY ${EXTLIB_DIR}/bin/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG} PATTERN "*.dll")
	file(COPY ${EXTLIB_DIR}/bin/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE} PATTERN "*.dll")
elseif(OMEGA_OS_LINUX)
endif(OMEGA_OS_WIN)
