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

file (GLOB input_text_files
    "${source_dir}/test*.txt")

add_executable(Tomasulo ${sources} ${header_files})

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "Tomasulo")

message("Copying all input text files from ${CMAKE_CURRENT_SOURCE_DIR} to ${CMAKE_CURRENT_BINARY_DIR}")
file(COPY ${input_text_files}
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src/test/)