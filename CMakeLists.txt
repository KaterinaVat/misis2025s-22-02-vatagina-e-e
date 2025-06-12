cmake_minimum_required(VERSION 3.20)
project(misis2025s-improc)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_VERBOSE_MAKEFILE True)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)


set(OpenCV_ROOT "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV found: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")
message(STATUS "OpenCV include dir: ${OpenCV_INCLUDE_DIRS}")

include_directories(prj.lab)
include_directories(prj.cv)

add_subdirectory(prj.lab)
add_subdirectory(prj.cv)


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


if(OpenCV_DIR)
    get_filename_component(OPENCV_DLL_DIR "${OpenCV_DIR}/../bin" ABSOLUTE)

    file(GLOB OPENCV_DLLS
        "${OPENCV_DLL_DIR}/opencv_core*.dll"
        "${OPENCV_DLL_DIR}/opencv_highgui*.dll"
        "${OPENCV_DLL_DIR}/opencv_imgproc*.dll"
    )


    install(FILES ${OPENCV_DLLS}
        DESTINATION bin
    )
endif()


install(TARGETS detector program_test coins_number
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION bin
)

install(DIRECTORY libname/include/
    DESTINATION include/libname
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)


install(DIRECTORY prj.cv/examples/      
        DESTINATION share/examples  
)





