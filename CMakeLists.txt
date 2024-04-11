cmake_minimum_required(VERSION 3.7...3.28)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(
        "SpaceRecycle"
        VERSION 1.0
        DESCRIPTION "Game made with raylib"
        LANGUAGES CXX
       )
   
set (CMAKE_CXX_STANDARD 20)

set(SRC_DIR "src")
set(BUILD_DIR "build")
set(BIN_DIR "bin")
set(INC_DIRS "include")

##########################################################################################
# Add dependencies with FetchContent
##########################################################################################

include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

function(add_git_dependency libName gitURL gitTag)
    FetchContent_Declare(
        ${libName}
        GIT_REPOSITORY ${gitURL}
        GIT_TAG        ${gitTag}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
	    FIND_PACKAGE_ARGS NAMES ${libName} # tell cmake to make it available via find_package
    )
    FetchContent_MakeAvailable(${libName})
    # target_compile_options(${libName} PRIVATE "-w")
endfunction()

# Add Raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

set(RAYLIB_VER 5.0)
find_package(raylib ${RAYLIB_VER} QUIET)
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    add_git_dependency(
        raylib
        https://github.com/raysan5/raylib.git
        ${RAYLIB_VER}
    )
endif()

# Add Raylib-CPP
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    if (NOT DEFINED RAYLIB_CPP_VERSION)
        set(RAYLIB_CPP_VERSION v5.0.1)
    endif()

    add_git_dependency(
        raylib_cpp
        https://github.com/RobLoach/raylib-cpp.git
        ${RAYLIB_CPP_VERSION}
    )
endif()

##########################################################################################
# Project executable setup
##########################################################################################

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${BIN_DIR}")

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${SRC_DIR}/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${INC_DIRS}")

target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    raylib_cpp
    )

##########################################################################################
# Project build settings
##########################################################################################

set(CMAKE_BUILD_TYPE "Debug")

# Setting ASSETS_PATH
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0 -DDEBUG")

    # Set the asset path macro to the absolute path on the dev machine
    target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
else()
    # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable
    target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets/")
endif()
