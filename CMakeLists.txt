cmake_minimum_required(VERSION 3.23)
project(server_app)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

set (COMMON_INCLUDE ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDE})

find_package(Qt5 COMPONENTS
        Core
        REQUIRED
        Sql
        Network
        Multimedia
)

add_executable(server_app
        main.cpp
        ./include/db_manipulation.h
        ./include/logging.h
        ./include/tcp_server.h
        ./src/db_manipulation.cpp
        ./src/logging.cpp
        ./src/tcp_server.cpp
)

target_link_libraries(server_app
        Qt5::Sql
        Qt5::Network
        Qt5::Multimedia
)

install(TARGETS server_app
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
