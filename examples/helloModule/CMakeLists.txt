add_library(helloModule SHARED helloModule.cpp)
set_target_properties(helloModule PROPERTIES FOLDER modules SUFFIX ".pyd")
target_link_libraries(helloModule cyclops omega omegaToolkit omegaOsg)
