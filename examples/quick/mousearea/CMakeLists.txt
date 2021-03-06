# Generated from mousearea.pro.

cmake_minimum_required(VERSION 3.14)
project(mousearea LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INSTALL_EXAMPLEDIR "examples/quick/mousearea")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Quick)
find_package(Qt6 COMPONENTS Qml)

add_qt_gui_executable(mousearea
    main.cpp
)
target_link_libraries(mousearea PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)


# Resources:
set(mousearea_resource_files
    "mousearea-wheel-example.qml"
    "mousearea.qml"
)

qt6_add_resources(mousearea "mousearea"
    PREFIX
        "/mousearea"
    FILES
        ${mousearea_resource_files}
)

install(TARGETS mousearea
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
