#pragma once

#include "gl.hpp"
#include "../basic_handle.hpp"
#include <cassert>

namespace squint::gl {
  struct TextureDeleter {
    void operator()(GLuint texture) {
      glDeleteTextures(1, &texture);
    }
  };

  using Texture = BasicHandle<GLuint, TextureDeleter>;

  Texture genTexture() {
    GLuint texture;
    glGenTextures(1, &texture);
    assert(texture);
    return texture;
  }
}
