add_library(gr-app-blocklib INTERFACE)
target_link_libraries(gr-app-blocklib INTERFACE gnuradio-core)
target_include_directories(gr-app-blocklib INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/> $<INSTALL_INTERFACE:include/>)

if (ENABLE_TESTING)
    add_subdirectory(test)
endif ()
