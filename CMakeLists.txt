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

add_executable(Tomasulo ${sources} ${header_files})
set(CMAKE_SUPPRESS_REGENERATION true)

# cmake_minimum_required(VERSION 3.10)

# # set the project name
# # Maps to a solution file
# project(Tomasulo)

# # Turn on the ability to create folders to organize projects
# set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# # Set the compiler flags and options. Visual Studio warning set to level 4
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
# #set (source_dir "${PROJECT_SOURCE_DIR}/src/")

# message("cxx flags" ${CMAKE_CXX_FLAGS})

# # sub dirs where we have more CMakeLists.txt files
# add_subdirectory(src)

# add_executable(tomasulo main.cpp)