add_executable(cv_task01 test1.cpp )
add_executable(cv_task02 test02.cpp)
target_link_libraries(cv_task01 PUBLIC ${OpenCV_LIBS})
target_link_libraries(cv_task02 PUBLIC ${OpenCV_LIBS})

