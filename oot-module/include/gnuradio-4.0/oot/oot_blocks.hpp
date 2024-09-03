#include<algorithm>
#include<numeric>
#include<gnuradio-4.0/Block.hpp>
#include <gnuradio-4.0/HistoryBuffer.hpp>

namespace gr::oot {
/**
 * A) The simplest possible bloxk implementation
 */
class ASquare : public gr::Block<ASquare> {
public:
  PortIn<float> in;
  PortOut<float> out;

  float processOne(float inValue) {
    return inValue * inValue;
  }
};

/**
 * B) Block templated by type
 */
template<typename T>
requires (std::is_arithmetic_v<T>)
class BSquare : public gr::Block<BSquare<T>> {
public:
  PortIn<T> in;
  PortOut<T> out;

  T processOne(T inValue) {
    return inValue * inValue;
  }
};

/**
 * C) using SIMD to exploit CPU instructions
 */
template<typename T>
requires (std::is_arithmetic_v<T>)
class CSquare : public gr::Block<CSquare<T>> {
public:
  PortIn<T> in;
  PortOut<T> out;

  template<gr::meta::t_or_simd<T> V>
  auto processOne(V inValue) {
    return inValue * inValue;
  }
};

/**
 * D) HistoryBuffer
 */
class DAverage: public gr::Block<DAverage> {
public:
  PortIn<float>       in;
  PortOut<float>      out;

  gr::HistoryBuffer<float> hist{ 8 };

  float processOne(float inValue) {
    hist.push_back(inValue);
    return std::accumulate(hist.begin(), hist.end(), 0.f)/8.f;
  }
};

/**
 * E) processBulk
 */
class ESquare : public gr::Block<ESquare> {
public:
  PortIn<float> in;
  PortOut<float> out;

  gr::work::Status processBulk(std::span<const float> inSpan, std::span<float> outSpan) {
    for (std::size_t i=0; i < inSpan.size(); i++) {
      outSpan[i] = inSpan[i] * inSpan[i];
    }
    return gr::work::Status::OK;
  }
};

/**
 * F) processBulk - optional consume/produce
 * A block that always only consumes half of the data available at its input
 */
class FSquare : public gr::Block<FSquare> {
public:
  PortIn<float> in;
  PortOut<float> out;

  gr::work::Status processBulk(const ConsumableSpan auto &inSpan, PublishableSpan auto &outSpan) {
    for (std::size_t i=0; i < inSpan.size()/2; i++) {
      outSpan[i] = inSpan[i] * inSpan[i];
    }
    inSpan.consume(inSpan.size()/2);
    inSpan.produce(inSpan.size()/2);
    return gr::work::Status::OK;
  }
};

/**
 * G) dynamic number of ports
 */
template<typename T>
requires (std::is_arithmetic_v<T>)
struct GCosSinBlock : public gr::Block<GCosSinBlock<T>> {
  std::vector<PortIn<T>>       in;  // runtime variable number of input ports
  std::array<PortOut<T>, 2>    out; // fixed number (=2) output ports

  template<gr::ConsumableSpan TInSpan, gr::PublishableSpan TOutSpan>
  gr::work::Status processBulk(const std::span<TInSpan> &ins, std::span<TOutSpan> &outs) {
    for (int u=0; u < ins.size(); u++) {
      for (int i=0; i < ins[u].size(); i++) {
        outs[0][i] += sin(ins[u][i]);
        outs[1][i] += cos(ins[u][i]);
      }
    }
    return gr::work::Status::OK;
  }
};

/**
 * H) Settings
 */
class HSquare: public gr::Block<HSquare> {
public:
  PortIn<float>       in;
  PortOut<float>      out;

  float ceil = 49.f;
  void settingsChanged(const property_map & /*old*/,
                        const property_map & /*updated*/) noexcept {
    // check if the new settings are valid and update devices
  }
  float processOne(float inValue) {
    return std::min(ceil, inValue * inValue);
  }
};

/**
 * I) Annotations for Block/Port/Setting Documentation
 */
class ISquare: public gr::Block<ISquare,  Doc<R""(
@brief A super cool block doing important processing

This block returns the power of 2 of the input clipped to a configurable value.
)"">> {
public:
  PortIn<float>       in;
  PortOut<float>      out;

  Annotated<float, "ceil", Visible, Doc<"The result of the power of 2 will be clipped at this value">, Unit<"V">> ceil = 49.f;

  float processOne(float inValue) {
    return std::min(ceil.value, inValue * inValue);
  }
};

/**
 * J) Tag handling
 */
class JAnswers: public gr::Block<JAnswers> {
public:
  PortIn<int>       in;
  PortOut<int>      out;

  int nQuestions = 0;

  int processOne(int inValue) {
    if (inValue == 23) {
      nQuestions++;
      fmt::print("got {}th Question\n", nQuestions);
      out.publishTag({{"nQuestions", nQuestions}}, -1);
    } else if (inValue == 42) {
      fmt::print("got an answer\n");
      publishEoS(); // shutting down as we found the answer
    }
    return inValue;
  }
};

}

ENABLE_REFLECTION(gr::oot::ASquare, in, out);
ENABLE_REFLECTION_FOR_TEMPLATE_FULL((typename T), (gr::oot::BSquare<T>), in, out);
ENABLE_REFLECTION_FOR_TEMPLATE_FULL((typename T), (gr::oot::CSquare<T>), in, out);
ENABLE_REFLECTION(gr::oot::DAverage, in, out);
ENABLE_REFLECTION(gr::oot::ESquare, in, out);
ENABLE_REFLECTION(gr::oot::FSquare, in, out); // optional consume
ENABLE_REFLECTION_FOR_TEMPLATE_FULL((typename T), (gr::oot::GCosSinBlock<T>), in, out);
ENABLE_REFLECTION(gr::oot::HSquare, in, out, ceil);
ENABLE_REFLECTION(gr::oot::ISquare, in, out, ceil);
ENABLE_REFLECTION(gr::oot::JAnswers, in, out, nQuestions); // tag handling

