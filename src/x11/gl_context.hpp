#pragma once

#include <utility>
#include "display.hpp"

using Colormap = long unsigned int;
using Window = long unsigned int; 

namespace squint::x11 {
  struct GLContext {
    Display* display;
    void* context;
    Colormap colorMap;
    Window window;

    GLContext(Display* display, void* context, Colormap colorMap, Window window) noexcept
      : display{display}, context{context}, colorMap{colorMap}, window{window} {}
    ~GLContext();

    GLContext(const GLContext& other) = delete;
    GLContext& operator=(const GLContext& other) = delete;

    GLContext(GLContext&& other) noexcept {
      std::swap(*this, other);
    }

    GLContext& operator=(GLContext&& other) noexcept {
      std::swap(*this, other);
      return *this;
    }

    friend void swap(GLContext& first, GLContext& second) noexcept {
      using std::swap;
      std::swap(first.display, second.display);
      std::swap(first.context, second.context);
      std::swap(first.colorMap, second.colorMap);
      std::swap(first.window, second.window);
    }
  };

  GLContext initializeGLContext(const DisplayHandle&);
}
