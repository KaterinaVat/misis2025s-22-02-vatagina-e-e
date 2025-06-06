cmake_minimum_required(VERSION 3.20)
project(misis2025s-improc)

# Настройка vcpkg (добавьте в самом начале)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "Vcpkg target triplet")

# Базовые настройки
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Пути для выходных файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
foreach(CFG Debug Release RelWithDebInfo)
    string(TOUPPER ${CFG} CFG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFG_UPPER} ${CMAKE_CURRENT_SOURCE_DIR}/bin.${CFG})
endforeach()

# Поиск зависимостей через vcpkg
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)
find_package(TIFF REQUIRED)  # Явно добавляем TIFF
find_package(nlohmann_json 3.11.3 REQUIRED)

# Подпроекты
add_subdirectory(prj.lab)
add_subdirectory(prj.cv)

# Установка (исправленные пути)
if(INSTALL_TARGET)
    # Установка OpenCV DLL
    if(OpenCV_FOUND AND TARGET OpenCV::core)
        install(FILES $<TARGET_RUNTIME_DLLS:OpenCV::core> DESTINATION bin)
    endif()

    # Установка основных целей
    install(TARGETS detector cv_task01 cv_task02
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
    )

    # Установка заголовков (исправленный путь)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/prj.cv/libname/include")
        install(DIRECTORY prj.cv/libname/include/ DESTINATION include)
    endif()

    # Установка примеров
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/prj.cv/examples")
        install(DIRECTORY prj.cv/examples/ DESTINATION examples)
    endif()
endif()