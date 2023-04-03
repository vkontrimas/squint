#pragma once

#include "../gl/gl.hpp"

namespace squint::fx {
  struct StageContext {
    GLuint frontTexture;
    GLuint backTexture;
  };
}
