cmake_minimum_required(VERSION 3.10)

project(C-SpaceMMORPG-Sector)

add_executable(C-SpaceMMORPG-Sector main.c)

set (CMAKE_BUILD_TYPE Debug)
add_subdirectory(GameManagement)

target_include_directories(C-SpaceMMORPG-Sector PUBLIC ../include)

#linking shared include folder
target_include_directories(C-SpaceMMORPG-Sector PUBLIC ../../SharedInclude)

target_link_libraries(C-SpaceMMORPG-Sector PRIVATE GameManagement)
