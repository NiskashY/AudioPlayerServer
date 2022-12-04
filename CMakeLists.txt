cmake_minimum_required(VERSION 3.23)
project(server_app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        REQUIRED
        Sql
)

add_executable(server_app
        main.cpp
        db_manipulation.h logging.h logging.cpp)

target_link_libraries(server_app
        Qt5::Core
        Qt5::Sql
)

install(TARGETS server_app
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})