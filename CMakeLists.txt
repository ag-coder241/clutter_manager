cmake_minimum_required(VERSION 3.16)
project(digital_clutter_manager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dcm
    src/main.cpp
    src/controller/controller.cpp
    src/scanner/FileScanner.cpp
    src/storage/Database.cpp
    src/analysis/ClutterAnalysis.cpp
)

target_include_directories(dcm PRIVATE src)
find_package(SQLite3 REQUIRED)
target_link_libraries(dcm SQLite::SQLite3)

