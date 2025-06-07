add_subdirectory(libname)
add_subdirectory(test)



if(WIN32)
    set(OpenCV_RUNTIME_PATH "${OpenCV_DIR}/../../bin")
else()
    set(OpenCV_RUNTIME_PATH "${OpenCV_DIR}/../../lib")
endif()

if(INSTALL_TARGET)
    install(DIRECTORY "${OpenCV_RUNTIME_PATH}/"
      DESTINATION bin
      FILES_MATCHING PATTERN "*opencv_core*"
    )
    install(DIRECTORY "${OpenCV_RUNTIME_PATH}/"
      DESTINATION bin
      FILES_MATCHING PATTERN "*opencv_highgui*"
    )
    install(DIRECTORY "${OpenCV_RUNTIME_PATH}/"
      DESTINATION bin
      FILES_MATCHING PATTERN "*opencv_imgproc*"
    )
endif()

# Исполняемые файлы
install(TARGETS detector cv_task01 cv_task02
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION bin
)

# Заголовочные файлы библиотеки
install(DIRECTORY libname/include/
    DESTINATION include/libname
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Примеры с фотографиями
install(DIRECTORY example/
    DESTINATION share/${PROJECT_NAME}/examples
)

# Тесты
install(DIRECTORY test/
    DESTINATION share/${PROJECT_NAME}/test
    PATTERN "CMakeLists.txt" EXCLUDE
)

# CMake-конфиги
install(DIRECTORY cmake/
    DESTINATION share/${PROJECT_NAME}/cmake
)

# Документация
install(FILES LICENSE README.md
    DESTINATION share/doc/${PROJECT_NAME}
)