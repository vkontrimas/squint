#pragma once

#include "gl.hpp"
#include "../basic_handle.hpp"
#include <cassert>

namespace squint::gl {
  struct RenderbufferDeleter {
    void operator()(GLuint renderbuffer) {
      glDeleteRenderbuffers(1, &renderbuffer);
    }
  };
  using Renderbuffer = BasicHandle<GLuint, RenderbufferDeleter>;

  Renderbuffer genRenderbuffer() {
    GLuint renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    assert(renderbuffer);
    return renderbuffer;
  }
}
