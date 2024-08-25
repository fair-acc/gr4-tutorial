
cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE) # Mainly for FMT

project(gr4-oot-module CXX)
include(GNUInstallDirs)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(gr4-oot-module_options INTERFACE)
target_compile_features(gr4-oot-module_options INTERFACE cxx_std_23)

include(FetchContent)

FetchContent_Declare(
    graph-prototype
    GIT_REPOSITORY https://github.com/fair-acc/graph-prototype.git
    GIT_TAG c01c93ccfbaafe7ab0674910469d154c8d007288 # main as of 2024-08-25
)

FetchContent_Declare(
    ut
    GIT_REPOSITORY https://github.com/boost-ext/ut.git
    GIT_TAG v2.0.1 # latest tag as of 2023-12-19
)

FetchContent_MakeAvailable( graph-prototype ut )

add_library(gr-oot-blocklib INTERFACE)
target_link_libraries(gr-oot-blocklib INTERFACE gnuradio-core gr4-oot-module_options)
target_include_directories(gr-oot-blocklib INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/> $<INSTALL_INTERFACE:include/>)

if (ENABLE_TESTING)
    add_subdirectory(test)
endif ()
