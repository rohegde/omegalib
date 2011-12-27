include(ExternalProject)

if(OMEGA_OS_WIN)
	set(EXTLIB_NAME OpenSceneGraph-3.0.1-VS10-x86)
elseif(OMEGA_OS_LINUX)
	if(OMEGA_ARCH_32)
		set(EXTLIB_NAME OpenSceneGraph-3.0.1-linux-x86)
	else(OMEGA_ARCH_32)
		set(EXTLIB_NAME OpenSceneGraph-3.0.1-linux-x64)
	endif(OMEGA_ARCH_32)
endif(OMEGA_OS_WIN)

set(EXTLIB_TGZ ${CMAKE_BINARY_DIR}/${EXTLIB_NAME}.tar.gz)
set(EXTLIB_DIR ${CMAKE_BINARY_DIR}/${EXTLIB_NAME})
set(OSG_BINARY_DIR ${CMAKE_BINARY_DIR}/${EXTLIB_NAME})

# Uncomment this line to make omegalib use an external openscenegraph binary build
# set(EXTLIB_DIR D:/Workspace/3rdparty/OpenSceneGraph-3.0.1-VS10.0.30319-x86-debug-12741)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/${EXTLIB_NAME}.tar.gz)
  message(STATUS "Downloading OpenSceneGraph binary archive...")
  file(DOWNLOAD http://omegalib.googlecode.com/files/${EXTLIB_NAME}.tar.gz ${EXTLIB_TGZ} SHOW_PROGRESS)
endif(NOT EXISTS ${CMAKE_BINARY_DIR}/${EXTLIB_NAME}.tar.gz)

if(NOT EXISTS ${EXTLIB_DIR})
  message(STATUS "Extracting OpenSceneGraph...")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzfh
    ${EXTLIB_TGZ} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif(NOT EXISTS ${EXTLIB_DIR})

set(OSG_INCLUDES ${EXTLIB_DIR}/include)

set(OSG_COMPONENTS osg osgAnimation osgDB osgFX osgGA osgManipulator osgShadow osgSim osgTerrain osgText osgUtil osgVolume osgViewer osgWidget OpenThreads)

if(OMEGA_OS_WIN)
    foreach( C ${OSG_COMPONENTS} )
		set(${C}_LIBRARY ${EXTLIB_DIR}/lib/release/${C}.lib)
		set(${C}_LIBRARY_DEBUG ${EXTLIB_DIR}/lib/debug/${C}d.lib)
		set(${C}_INCLUDE_DIR ${OSG_INCLUDES})
		set(OSG_LIBS ${OSG_LIBS} optimized ${${C}_LIBRARY} debug ${${C}_LIBRARY_DEBUG})
	endforeach()

	# Copy the dlls into the target directories
	file(COPY ${EXTLIB_DIR}/bin/debug/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG} PATTERN "*.dll")
	file(COPY ${EXTLIB_DIR}/bin/release/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE} PATTERN "*.dll")

elseif(OMEGA_OS_LINUX)
    foreach( C ${OSG_COMPONENTS} )
		set(${C}_LIBRARY ${EXTLIB_DIR}/lib/release/lib${C}.so)
		set(${C}_LIBRARY_DEBUG ${EXTLIB_DIR}/lib/debug/lib${C}d.so)
		set(${C}_INCLUDE_DIR ${OSG_INCLUDES})
		set(OSG_LIBS ${OSG_LIBS} optimized ${${C}_LIBRARY} debug ${${C}_LIBRARY_DEBUG})
	endforeach()

	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		file(COPY ${EXTLIB_DIR}/lib/debug/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
		file(COPY ${EXTLIB_DIR}/lib/debug/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	else(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		file(COPY ${EXTLIB_DIR}/lib/release/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
		file(COPY ${EXTLIB_DIR}/lib/release/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
endif(OMEGA_OS_WIN)

include(${CMAKE_CURRENT_LIST_DIR}/UseOsgWorks.cmake)
# Add osgWorks to openscenegraph includes and libraries (this simplified inclusion in other projects.
# we consider osg and osgWorks as a single package.
set(OSG_INCLUDES ${OSG_INCLUDES} ${OSGWORKS_INCLUDES})
set(OSG_LIBS ${OSG_LIBS} ${OSGWORKS_LIBS})
