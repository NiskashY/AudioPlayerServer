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
        Network
)

add_executable(server_app
        main.cpp
        db_manipulation.h
        db_manipulation.cpp

        logging.h
        logging.cpp

        tcp_server.h
        tcp_server.cpp
)

target_link_libraries(server_app
        Qt5::Core
        Qt5::Sql
        Qt5::Network
)

install(TARGETS server_app
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})