add_subdirectory(libname)
add_subdirectory(test)

# В начало CMakeLists.txt после project()
message(STATUS "Starting TIFF diagnostics...")

# Принудительно ищем TIFF с выводом логов
find_package(TIFF QUIET)
if(TIFF_FOUND)
    message(STATUS "TIFF found:")
    message(STATUS "  TIFF_INCLUDE_DIR = ${TIFF_INCLUDE_DIR}")
    message(STATUS "  TIFF_LIBRARIES = ${TIFF_LIBRARIES}")
    message(STATUS "  TIFF_VERSION = ${TIFF_VERSION}")
else()
    message(WARNING "TIFF not found! Searching in standard paths...")
    
    # Проверяем стандартные пути
    set(TIFF_PATHS 
        "C:/vcpkg/installed/x64-windows"
        "C:/libtiff"
        "$ENV{ProgramFiles}/tiff"
    )
    
    find_path(TIFF_INCLUDE_DIR tiff.h PATHS ${TIFF_PATHS} PATH_SUFFIXES include)
    find_library(TIFF_LIBRARY NAMES tiff libtiff PATHS ${TIFF_PATHS} PATH_SUFFIXES lib)

    if(TIFF_INCLUDE_DIR AND TIFF_LIBRARY)
        message(STATUS "Manually found TIFF:")
        message(STATUS "  Include dir: ${TIFF_INCLUDE_DIR}")
        message(STATUS "  Library: ${TIFF_LIBRARY}")
        set(TIFF_FOUND TRUE)
    else()
        message(FATAL_ERROR "TIFF not found in standard paths! Please install it via:\n  vcpkg install tiff:x64-windows")
    endif()
endif()

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