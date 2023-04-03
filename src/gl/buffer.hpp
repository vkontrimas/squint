#pragma once

#include "gl.hpp"
#include "../basic_handle.hpp"
#include <cassert>

namespace squint::gl {
  namespace detail {
    struct BufferDeleter {
      void operator()(GLuint buffer) {
        glDeleteBuffers(1, &buffer);
      }
    };
  }

  using Buffer = BasicHandle<GLuint, detail::BufferDeleter>;

  inline Buffer genBuffer() {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    assert(buffer);
    return buffer;
  }
}
