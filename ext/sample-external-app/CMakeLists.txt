cmake_minimum_required(VERSION 2.8.1) 
project(SampleApp)

# UseOmegalib.cmake will add a new variable to your cmake build: OMEGA_ROOT_DIR. Make it point to your
# omegalib base directory (the one containing the bin, lib and include folders)
include(${SampleApp_SOURCE_DIR}/../UseOmegaLib.cmake)

# We need to explicitly include the openGL package for this sample since we are making openGL calls in its source code.
find_package(OpenGL)

# Add an executable using a single source file, and link it to the omega and outils libraries
add_executable(sample sample.cpp)

# Use the following is you want to use debug omega libraries for debug and release builds.
target_link_libraries(sample ${OPENGL_LIBRARY}
	${OMEGA_LIB_DEBUG})

# Use the following is you want to use omegalib and the omega utility library.
# target_link_libraries(sample ${OPENGL_LIBRARY}
#	${OMEGA_LIB_DEBUG} ${OUTILS_LIB_DEBUG})
	
# Use the following is you want to use separate libraries for debug and release builds.
# target_link_libraries(sample ${OPENGL_LIBRARY}
#	debug ${OMEGA_LIB_DEBUG} ${OUTILS_LIB_DEBUG}
# optimized ${OMEGA_LIB_RELEASE} ${OUTILS_LIB_RELEASE})

