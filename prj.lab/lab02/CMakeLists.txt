add_executable(task02 task02.cpp)
add_executable(myTest test.cpp)

target_link_libraries(task02 semcv)
target_link_libraries(myTest semcv)