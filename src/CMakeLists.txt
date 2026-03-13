cmake_minimum_required(VERSION 3.16)

project(Aethera VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/home
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/login
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/register
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/profile
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/examhistory
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/settings
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
    main.cpp
    
    assets/icons.qrc
    assets/aethera.db

    core/database.cpp
    core/database.h
    core/usersession.cpp
    core/usersession.h

    ui/login/login.cpp
    ui/login/login.h
    ui/login/login.ui

    ui/register/register.cpp
    ui/register/register.h
    ui/register/register.ui

    ui/home/home.cpp
    ui/home/home.h
    ui/home/home.ui

    ui/sidebar/sidebar.cpp
    ui/sidebar/style.h

    ui/exam/exam.cpp
    ui/exam/exam.h
    ui/exam/exam.ui
    ui/exam/style.h

    ui/examhistory/examhistory.cpp
    ui/examhistory/examhistory.h
    ui/examhistory/examhistory.ui

    ui/settings/settings.cpp
    ui/settings/settings.h
    ui/settings/settings.ui

    ui/profile/profile.cpp
    ui/profile/profile.h
    ui/profile/profile.ui
)

qt_add_executable(Aethera ${PROJECT_SOURCES})

target_link_libraries(Aethera PRIVATE Qt6::Widgets Qt6::Sql)

target_include_directories(Aethera PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/login
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/register
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/home
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/sidebar
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/examhistory
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/settings
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/profile
)
