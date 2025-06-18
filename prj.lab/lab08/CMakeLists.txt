
add_library(task8 task.cpp)
target_link_libraries(task8 semcv)


add_executable(task8_test "task08-01.cpp")
target_link_libraries(task8_test task8)

