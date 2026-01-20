cmake_minimum_required(VERSION 3.16)

project(MapGenerator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(APP_SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/MapWidget.cpp
)

set(APP_HEADERS
    src/MainWindow.h
    src/MapWidget.h
    src/Obstacle.h
)

# Handle Qt MOC automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(MapGenerator ${APP_SOURCES} ${APP_HEADERS})

target_link_libraries(MapGenerator PRIVATE Qt6::Widgets)
