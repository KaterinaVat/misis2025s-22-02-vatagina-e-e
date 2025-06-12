


add_executable(task0701 task07-01.cpp )
target_link_libraries(task0701 PUBLIC semcv)


add_executable(task0702 "task07-02.cpp")
target_link_libraries(task0702 PUBLIC semcv)



add_library(boundaries "boundaries.cpp")
target_link_libraries( boundaries PUBLIC ${OpenCV_LIBS})