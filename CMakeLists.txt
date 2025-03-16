cmake_minimum_required(VERSION 3.21)
project(warnings-viewer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

find_package(Qt6 6.5 NO_MODULE REQUIRED COMPONENTS Widgets)

add_executable(
    warnings-viewer
    main.cpp
    clazychecks.cpp
    mainwindow.cpp
    settings.cpp
    settingswindow.cpp
    tab.cpp
    warningmodel.cpp
    warningproxymodel.cpp
    warningtypefilterproxymodel.cpp
    resources.qrc
)

target_link_libraries(
    warnings-viewer Qt6::Widgets
)
