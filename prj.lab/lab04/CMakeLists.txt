


add_library(task4 test4.cpp  "task04.h")
target_link_libraries(task4 PUBLIC ${OpenCV_LIBS})


add_executable(task0401 task04-01.cpp)
target_link_libraries(task0401 PUBLIC task4)


add_executable(task0402 task04-02.cpp)
target_link_libraries(task0402 PUBLIC task4)

add_executable(task0403 task04-03.cpp)
target_link_libraries(task0403 PUBLIC task4)