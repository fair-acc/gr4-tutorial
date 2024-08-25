# GNURadio 4.0 based example applications

This contains some small gnuradio application examples.

- `app_static.cpp`: An example that uses a static graph that is known at compile time to do some processing.
- `app_loadgrc.cpp`: A small example that loads a `.grc` file and runs it.
- `blocklib/include/gr-demo-app-block.hpp`: demonstrates how to include a custom block directly into
   your application source tree. This is then used alongside the blocks from `gnuradio-basic` and `sample-oot-blocklib`.

## Build system

The project is a regular cmake project that depends on
 - `gnuradio-core` for the gnuradio runtime
 - `gnuradio-basic` for some basic blocks
 - `sample-oot-blocklib` optional dependency on the out of tree module demonstrated in this repository

 Q: probably we should have separate cmake dependencies for the header-only use vs the runtime library (for the core as well as the block libraries).

These dependencies are fetched using cmake's FetchConent mechanism. This allows us to depend on specific versions.
For packaging or user installation, CMake can be instructed to use a locally installed package instead fetching and building the specified version.

