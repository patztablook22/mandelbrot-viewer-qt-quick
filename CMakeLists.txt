cmake_minimum_required(VERSION 3.16)

project(mandelbrot VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(
        Qt${QT_VERSION_MAJOR}
        COMPONENTS
        Core
        Quick
        Concurrent
        Multimedia
        QuickControls2
        REQUIRED
)


add_subdirectory(src)

set(CPACK_RESOURCE_FILE_LICENSE "LICENSE")
set(CPACK_RESOURCE_FILE_README "README.md")
include(CPack)
