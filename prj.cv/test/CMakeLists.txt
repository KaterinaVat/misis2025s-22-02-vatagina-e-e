add_executable(program_test test1.cpp )
add_executable(coins_number test02.cpp)
target_link_libraries(program_test PUBLIC detector)
target_link_libraries(coins_number PUBLIC detector)

