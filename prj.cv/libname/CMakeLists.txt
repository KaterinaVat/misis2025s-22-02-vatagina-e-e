add_library(detector "include/libname.hpp" libname.cpp)
target_link_libraries(detector PUBLIC ${OpenCV_LIBS})