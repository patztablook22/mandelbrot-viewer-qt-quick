cmake_minimum_required(VERSION 3.16)
project(program LANGUAGES CXX)

set(BIN_NAME mandelbrot)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        palettes.qrc
        instructions.h instructions.cpp
        renderer.h renderer.cpp
        worker.h worker.cpp
        palette.h palette.cpp
)

add_executable(${BIN_NAME} ${PROJECT_SOURCES})
install(TARGETS ${BIN_NAME} DESTINATION bin)

#target_compile_definitions(${PROJECT_NAME}
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${BIN_NAME}
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  Qt${QT_VERSION_MAJOR}::Multimedia
  Qt${QT_VERSION_MAJOR}::Concurrent
  Qt${QT_VERSION_MAJOR}::QuickControls2
)

