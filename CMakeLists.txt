cmake_minimum_required(VERSION 3.10)

project(physics-emulator)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(physics-emulator main.cxx)

target_link_libraries(physics-emulator ${GTKMM_LIBRARIES})

