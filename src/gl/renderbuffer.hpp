#pragma once

#include "gl.hpp"
#include "../basic_handle.hpp"
#include <cassert>

namespace squint::gl {
  namespace detail {
    struct RenderbufferDeleter {
      void operator()(GLuint renderbuffer) {
        glDeleteRenderbuffers(1, &renderbuffer);
      }
    };
  }
  using Renderbuffer = BasicHandle<GLuint, detail::RenderbufferDeleter>;

  Renderbuffer genRenderbuffer() {
    GLuint renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    assert(renderbuffer);
    return renderbuffer;
  }

  Renderbuffer genRenderbuffer(GLenum format, int width, int height) {
    auto buffer = genRenderbuffer(); 
    glBindRenderbuffer(GL_RENDERBUFFER, *buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    return buffer;
  }
}
