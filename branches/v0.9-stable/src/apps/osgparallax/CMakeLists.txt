add_executable(osgparallax osgparallax.cpp)
target_link_libraries(osgparallax omega oengine oosg)
set_target_properties(osgparallax PROPERTIES FOLDER apps)