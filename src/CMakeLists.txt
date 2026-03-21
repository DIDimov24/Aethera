cmake_minimum_required(VERSION 3.16)

project(Aethera VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/core/db
    ${CMAKE_CURRENT_SOURCE_DIR}/core/session
    ${CMAKE_CURRENT_SOURCE_DIR}/core/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/home
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/login
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/register
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/profile
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/settings
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam/session
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam/results
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam/difficulties
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam/subjects
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam/history
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam/review
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/statistics
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Network Charts)

set(PROJECT_SOURCES
    main.cpp

    assets/icons.qrc
    assets/aethera.db


    core/db/database.cpp
    core/db/database.h

    core/session/usersession.cpp
    core/session/usersession.h

    core/utils/utils.h
    core/utils/utils.cpp


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

    ui/exam/session/questionsBank.h
    ui/exam/session/session.cpp
    ui/exam/session/session.h
    ui/exam/session/session.ui
    ui/exam/session/style.h

    ui/exam/results/results.cpp
    ui/exam/results/results.h
    ui/exam/results/results.ui

    ui/exam/difficulties/difficulties.cpp
    ui/exam/difficulties/difficulties.h
    ui/exam/difficulties/difficulties.ui

    ui/exam/subjects/subjects.cpp
    ui/exam/subjects/subjects.h
    ui/exam/subjects/subjects.ui

    ui/exam/history/history.cpp
    ui/exam/history/history.h
    ui/exam/history/history.ui
    ui/exam/history/style.h

    ui/exam/review/review.cpp
    ui/exam/review/review.h
    ui/exam/review/review.ui
    ui/exam/review/style.h

    ui/settings/settings.cpp
    ui/settings/settings.h
    ui/settings/settings.ui

    ui/profile/profile.cpp
    ui/profile/profile.h
    ui/profile/profile.ui

    ui/statistics/statistics.cpp
    ui/statistics/statistics.h
    ui/statistics/statistics.ui
)

qt_add_executable(Aethera ${PROJECT_SOURCES})

target_link_libraries(Aethera PRIVATE Qt6::Widgets Qt6::Sql Qt6::Network Qt6::Charts)

target_include_directories(Aethera PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core/db
    ${CMAKE_CURRENT_SOURCE_DIR}/core/session
    ${CMAKE_CURRENT_SOURCE_DIR}/core/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/login
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/register
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/home
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/sidebar
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/settings
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/profile
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam/session
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam/results
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam/difficulties
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam/subjects
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam/history
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/exam/review
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/statistics
)
