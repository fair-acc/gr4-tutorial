cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE) # Mainly for FMT

project(gr4-example-apps CXX)
include(GNUInstallDirs)

if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang") # set default C++ STL to Clang's libc++ when using Clang
    add_compile_options(-stdlib=libc++ -fcolor-diagnostics)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++")
endif ()

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(gr4-example-app_options INTERFACE)
target_compile_features(gr4-example-app_options INTERFACE cxx_std_23)

include(FetchContent)

FetchContent_Declare(
    graph-prototype
    GIT_REPOSITORY https://github.com/fair-acc/graph-prototype.git
    GIT_TAG c01c93ccfbaafe7ab0674910469d154c8d007288 # main as of 2024-08-25
)

FetchContent_Declare(
    sample-oot-module
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/../oot-module/
)

FetchContent_Declare(
    ut
    GIT_REPOSITORY https://github.com/boost-ext/ut.git
    GIT_TAG v2.0.1 # latest tag as of 2023-12-19
)

FetchContent_MakeAvailable( graph-prototype sample-oot-module ut )

add_subdirectory(app-blocklib)

add_executable(gr4-static-example src/gr4-static-example.cpp)
target_link_libraries( gr4-static-example
  PRIVATE gr4-example-app_options
          gnuradio-core   # gr4 core runtime
          gr-basic        # gr4 basic blocks
          gr-testing      # gr4 testing blocks
          gr-oot-blocklib # blocklib from the out of tree blocklib in this repository
          gr-app-blocklib # in-project blocklib
          )

add_executable(gr4-dynamic-example src/gr4-dynamic-example.cpp)
target_link_libraries( gr4-dynamic-example
  PRIVATE gr4-example-app_options
          gnuradio-core   # gr4 core runtime
          gr-basic        # gr4 basic blocks
          gr-testing      # gr4 testing blocks
          gr-oot-blocklib # blocklib from the out of tree blocklib in this repository
          gr-app-blocklib # in-project blocklib
          )

