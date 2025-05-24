add_library(semcv semcv.hpp semcv.cpp)
target_link_libraries(semcv PUBLIC ${OpenCV_LIBS})

target_link_libraries(semcv PRIVATE nlohmann_json::nlohmann_json)