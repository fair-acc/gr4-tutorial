#include <fmt/core.h>
#include <gnuradio-4.0/Scheduler.hpp>
#include <gnuradio-4.0/testing/NullSources.hpp>
#include <gnuradio-4.0/basic/common_blocks.hpp>
#include <gnuradio-4.0/oot/oot_blocks.hpp>

void runFlowgraph() {
  gr::Graph graph{};
  auto& source = graph.emplaceBlock<gr::testing::CountingSource<float>>({{ "n_samples_max", 100U }});
  auto& scale  = graph.emplaceBlock<builtin_multiply<float>>({{ "factor", 3.0f }});
  auto& square = graph.emplaceBlock<gr::oot::ESquare>();
  auto& sink   = graph.emplaceBlock<gr::testing::CountingSink<float>>();

  auto result1 = graph.connect<"out">(source).to<"in">(scale);
  auto result2 = graph.connect<"out">(scale).to<"in">(square);
  auto result3 = graph.connect<"out">(square).to<"in">(sink);

  if (result1 == gr::ConnectionResult::FAILED || result2 == gr::ConnectionResult::FAILED || result3 == gr::ConnectionResult::FAILED) {
    fmt::print("Error: Failed to connect flowgraph!\n");
    return;
  }

  gr::scheduler::Simple<> scheduler{std::move(graph)};

  scheduler.runAndWait();

  fmt::print("Flowgraph processed {} samples!\n", sink.count);
}

/** Merged graphs can currently not be emplaced into the flowgraph
void runFlowgraphMerged() {
  gr::Graph graph{};
  auto& source = graph.emplaceBlock<gr::testing::CountingSource<float>>({{ "n_samples_max", 100U }});
  // TODO: fix emplacing mergedGraphs
  auto& merged = graph.emplaceBlock<gr::MergedGraph<builtin_multiply<float>, gr::oot::ASquare, 0, 0>>(builtin_multiply<float>({{ "factor", 3.0f }}), gr::oot::ASquare());
  auto& sink   = graph.emplaceBlock<gr::testing::CountingSink<float>>();

  auto result1 = graph.connect<"out">(source).to<"in">(merged);
  auto result2 = graph.connect<"out">(merged).to<"in">(sink);

  if (result1 == gr::ConnectionResult::FAILED || result2 == gr::ConnectionResult::FAILED) {
    fmt::print("Error: Failed to connect merged flowgraph!\n");
    return;
  }

  gr::scheduler::Simple<> scheduler{std::move(graph)};

  scheduler.runAndWait();

  fmt::print("Flowgraph with merged block processed {} samples!\n", sink.count);
}
*/

int main() {
  fmt::print("This is a simple GNURadio 4.0 application, which uses a graph which is known at compile-time and can therefore be optimized by the compiler\n");
  runFlowgraph();
  //runFlowgraphMerged();
}

