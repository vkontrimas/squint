#pragma once

#include "gl.hpp"
#include "../basic_handle.hpp"
#include <cassert>

namespace squint::gl {
  struct FramebufferDeleter {
    void operator()(GLuint framebuffer) {
      glDeleteFramebuffers(1, &framebuffer);
    }
  };
  using Framebuffer = BasicHandle<GLuint, FramebufferDeleter>;

  Framebuffer genFramebuffer() {
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    assert(framebuffer);
    return framebuffer;
  }
}
