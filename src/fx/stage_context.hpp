#pragma once

#include "../gl/gl.hpp"

namespace squint::fx {
  struct StageContext {
    int width, height;
    GLuint frontTexture;
    GLuint backTexture;
  };
}
