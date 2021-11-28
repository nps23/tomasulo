cmake_minimum_required(VERSION 3.5)

project(Tomasulo)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set (source_dir "${PROJECT_SOURCE_DIR}/src")
message("The source dir is: ${PROJECT_SOURCE_DIR}")

file (GLOB sources 
    "${source_dir}/*.cpp"
    "${source_dir}/structures/*.cpp")

file (GLOB header_files 
    "${source_dir}/*.h"
    "${source_dir}/structures/*.h")

file (GLOB text_files "${source_dir}/test/*.txt")

add_executable(Tomasulo ${sources} ${header_files} ${text_files})
set(CMAKE_SUPPRESS_REGENERATION true)
# hopefully remove the "ALL BUILDS" solution problem
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)
#set_property(GLOBAL PROPERTY VS_STARTUP_PROJECT "Tomasulo")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "Tomasulo")

message("${CMAKE_CURRENT_SOURCE_DIR} ---------  ${CMAKE_CURRENT_BINARY_DIR}")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/test/input.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src/test/)