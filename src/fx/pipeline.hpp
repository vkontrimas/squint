#pragma once

#include <utility>

namespace squint::fx {
  struct Pixelate {
    float pixels;

    void operator()(int buffer) {}
  };

  void GaussianBlur(int buffer) {}

  void ToImage(int buffer) {}


  class Pipeline final {
  public:
    static Pipeline fromScreenshot(const auto&) {
      return {};
    }

    void swapBuffers() {
      using std::swap;
      swap(front_, back_);
    }

    template<typename IntermediateStage>
    Pipeline& operator|(IntermediateStage&& stage) {
      stage(front_);
      swapBuffers();
      return *this;
    }

  private:
    int front_, back_;

    Pipeline() noexcept : front_{0}, back_{0} {}
  };
}
