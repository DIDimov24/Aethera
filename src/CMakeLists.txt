cmake_minimum_required(VERSION 3.16)

project(Aethera VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp

    core/database.cpp
    core/database.h
    core/usersession.cpp
    core/usersession.h

    ui/login/login.cpp
    ui/login/login.h
    ui/login/login.ui
    ui/login/register.cpp
    ui/login/register.h
    ui/login/register.ui

    ui/home/home.cpp
    ui/home/home.h
    ui/home/home.ui
)

qt_add_executable(Aethera ${PROJECT_SOURCES})

target_link_libraries(Aethera PRIVATE Qt6::Widgets)

target_include_directories(Aethera PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/login
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/home
)
