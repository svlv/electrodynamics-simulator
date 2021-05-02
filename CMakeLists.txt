cmake_minimum_required(VERSION 3.10)

project(elfield)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)

find_package(Boost REQUIRED COMPONENTS system)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} "src")

file(GLOB_RECURSE SRC "src/*.cpp")
file(GLOB_RECURSE INC "src/*.hpp")

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES} Boost::system)

add_custom_target(format COMMAND clang-format -style=file -i ${SRC} ${INC})
