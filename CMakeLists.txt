cmake_minimum_required(VERSION 3.12)
project(RAYCAST)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug) # Set the build type to Debug

# Set the path to your raylib installation
set(raylib_DIR "C:/raylib/raylib/cmake")

file(GLOB PROJECT_SOURCES src/*.cpp)
add_executable(RAYCAST ${PROJECT_SOURCES})

find_package(raylib 5.0 QUIET) # Let's find the installed raylib package

if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/master.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

target_link_libraries(RAYCAST raylib)

