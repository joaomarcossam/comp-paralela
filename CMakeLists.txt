cmake_minimum_required(VERSION 3.10)
project(open_mpi_test)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(MPI REQUIRED)

include_directories(/usr/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.c")
add_executable(main ${SOURCES})

target_compile_options(main PRIVATE -pg)
target_link_options(main PRIVATE -pg)

if(PARALLEL)
    target_compile_definitions(main PRIVATE PARALLEL)
endif()

if(LOG_CODE_ENABLED)
    target_compile_definitions(main PRIVATE LOG_CODE_ENABLED)
endif()

if(PRINT_RESULT)
    target_compile_definitions(main PRIVATE PRINT_RESULT)
endif()

target_link_libraries(main MPI::MPI_C m)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")