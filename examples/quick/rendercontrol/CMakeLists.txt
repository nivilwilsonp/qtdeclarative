# Generated from rendercontrol.pro.

cmake_minimum_required(VERSION 3.14)
project(rendercontrol LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INSTALL_EXAMPLEDIR "examples/quick/rendercontrol")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Quick)
find_package(Qt6 COMPONENTS Qml)

add_qt_gui_executable(rendercontrol
    cuberenderer.cpp cuberenderer.h
    main.cpp
    window_multithreaded.cpp window_multithreaded.h
    window_singlethreaded.cpp window_singlethreaded.h
)
target_link_libraries(rendercontrol PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)


# Resources:
set(rendercontrol_resource_files
    "demo.qml"
)

qt6_add_resources(rendercontrol "rendercontrol"
    PREFIX
        "/rendercontrol"
    FILES
        ${rendercontrol_resource_files}
)

install(TARGETS rendercontrol
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
