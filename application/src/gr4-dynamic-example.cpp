#include <fmt/core.h>
#include <gnuradio-4.0/Scheduler.hpp>
#include <gnuradio-4.0/testing/NullSources.hpp>
#include <gnuradio-4.0/basic/common_blocks.hpp>
#include <gnuradio-4.0/oot/oot_blocks.hpp>

struct TestContext {
    explicit TestContext(std::vector<std::filesystem::path> paths) : loader(registry, std::move(paths)) {}
    gr::BlockRegistry registry;
    gr::PluginLoader  loader;
};

int main() {
  gr::Graph graph{};
  TestContext pluginContext(std::vector<std::filesystem::path>{ ",/gr4-oot-dir/" });
  gr::registerBlock<gr::oot::BSquare, float>(pluginContext.registry); // register a source defined block into the blocklib

  auto &source = graph.emplaceBlock("BSquare", "double", {}, pluginContext.loader); // load builtin block
  auto &block = graph.emplaceBlock("BlockFromSo", "double", {}, pluginContext.loader); // load from .so file

  graph.connect(source, 0, block, 0);
  gr::scheduler::Simple<> scheduler{std::move(graph)};
  scheduler.runAndWait();

  fmt::print("Dynamic Flowgraph processed samples!\n");
}

