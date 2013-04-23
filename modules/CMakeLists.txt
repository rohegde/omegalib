# In this section, add modules that require OpenSceneGraph support
if(OMEGA_BUILD_OSG_LIB)
	#templateModule
	add_subdirectory(templateModule)
	add_subdirectory(pointCloud)
endif()
