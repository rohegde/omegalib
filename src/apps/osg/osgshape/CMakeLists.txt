add_executable(osgshape osgshape.cpp)
target_link_libraries(osgshape omega oengine oosg)
set_target_properties(osgshape PROPERTIES FOLDER apps)

