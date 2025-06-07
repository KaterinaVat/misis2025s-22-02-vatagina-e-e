add_library(semcv semcv.hpp semcv.cpp)
target_include_directories(semcv PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(semcv PUBLIC ${OpenCV_LIBS})

