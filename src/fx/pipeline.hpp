#pragma once

#include <utility>
#include <iostream>

namespace squint::fx {
  struct Pixelate {
    float pixels;

    void operator()(int& front, int back) {
      std::cout << "Pixelate" << std::endl;
      front = back + 2;
    }
  };

  void GaussianBlur(int& front, int back) {
    std::cout << "GaussianBlur" << std::endl;
    front = back * 2;
  }

  void ToImage(int& front, int back) {
    std::cout << "ToImage" << std::endl;
    front = back;
  }


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
      std::cout << "--------------------------------------------------------------------------------\n";
      std::cout << counter_++ << "| F: " << front_ << " B: " << back_ << std::endl;
      stage(front_, back_);
      std::cout << "--------------------------------------------------------------------------------" << std::endl;
      swapBuffers();
      return *this;
    }

  private:
    int front_, back_;
    int counter_;

    Pipeline() noexcept : front_{0}, back_{0}, counter_{0} {}
  };
}
