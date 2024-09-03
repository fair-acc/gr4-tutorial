# GNURadio 4.0 based example applications

This contains some small gnuradio application examples.

- `app_static.cpp`: An example that uses a static graph that is known at compile time to do some processing.
- `app_dynamic.cpp`: A small example that dynamically instantiates some blocks and runs them in a flowgraph.
                     While this compiles and shows the API, it will fail at runtime,
                     because the dynamic library to load from is not compiled in this project.
- `blocklib/include/gr-demo-app-block.hpp`: demonstrates how to include a custom block directly into
   your application source tree. This is then used alongside the blocks from `gnuradio-basic` and `sample-oot-blocklib`.
   Currently there are no local blocks implemented here.

## Build system

The project is a regular cmake project that depends on
 - `gnuradio-core` for the gnuradio runtime
 - `gnuradio-basic` for some basic blocks
 - `sample-oot-blocklib` optional dependency on the out of tree module demonstrated in this repository

These dependencies are fetched using cmake's FetchConent mechanism. This allows us to depend on specific versions.
For packaging or user installation, CMake can be instructed to use a locally installed package instead fetching and building the specified version.

