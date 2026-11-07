cmake_minimum_required(VERSION 3.21)

project(Lab8_AirQuality LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SqliteOrm_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/share/SqliteOrm")
set(unofficial-sqlite3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/share/unofficial-sqlite3")

find_package(SqliteOrm CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_executable(Lab8_AirQuality
    Lab8_AirQuality.cpp
    include/models.h
    include/database.h
    include/sensor_repository.h
    include/measurement_repository.h
    src/database.cpp
    src/sensor_repository.cpp
    src/measurement_repository.cpp
    src/sqlite_comparison.cpp
)

target_include_directories(Lab8_AirQuality PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(Lab8_AirQuality PRIVATE
    sqlite_orm::sqlite_orm
    unofficial::sqlite3::sqlite3
)

add_custom_command(TARGET Lab8_AirQuality POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/sqlite3.dll"
    "$<TARGET_FILE_DIR:Lab8_AirQuality>"
)