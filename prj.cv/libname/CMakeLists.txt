add_library(detector "include/libname.hpp" libname.cpp)
#target_link_libraries(detector PUBLIC ${OpenCV_LIBS})


set_property(TARGET detector PROPERTY CXX_STANDARD 20)
target_include_directories(detector PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )
TARGET_LINK_LIBRARIES (detector ${OpenCV_LIBS})
