#pragma once

#include <utility>
#include <iostream>
#include <concepts>
#include <type_traits>

namespace squint::fx {
  template<typename Func>
  concept IntermediateStage = requires (Func f, int a, int b) {
    { f(a, b) } -> std::same_as<void>;
  };

  template<typename Func>
  concept TerminatorStage = std::is_invocable_v<Func, int, int> &&
                            !std::is_same_v<std::invoke_result_t<Func, int, int>, void>;

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

  int ToImage(int front, int back) {
    std::cout << "ToImage" << std::endl;
    return back;
  }

  int Test(int foo) {
    return foo;
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

    template<IntermediateStage Stage>
    Pipeline& operator|(Stage&& stage) {
      std::cout << "--------------------------------------------------------------------------------\n";
      std::cout << counter_++ << "| F: " << front_ << " B: " << back_ << std::endl;
      stage(front_, back_);
      std::cout << "--------------------------------------------------------------------------------" << std::endl;
      swapBuffers();
      return *this;
    }

    template<TerminatorStage Stage>
    auto operator|(Stage&& stage) {
      std::cout << "--------------------------------------------------------------------------------\n";
      std::cout << counter_++ << "| F: " << front_ << " B: " << back_ << " | TERMINATED" << std::endl;
      auto value = stage(front_, back_);
      std::cout << "--------------------------------------------------------------------------------" << std::endl;
      return value;
    }

  private:
    int front_, back_;
    int counter_;

    Pipeline() noexcept : front_{0}, back_{0}, counter_{0} {}
  };
  }
