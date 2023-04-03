#pragma once

#include "../gl/program.hpp"

namespace squint::fx {
  struct FullscreenQuad;
  struct StageContext;

  class FastPixelate {
  public:
    // For pixelated squares to not be cut off, pixelSize must be
    // a common factor of the width and height of the screen.
    FastPixelate(float pixelSize, FullscreenQuad* fullscreenQuad);

    void operator()(const StageContext& context);

  private:
    float pixelSize_;
    FullscreenQuad* fullscreenQuad_;
    gl::Program program_;
    GLuint pixelSizeLocation_;
    GLuint vertexLocation_;
    GLuint uvLocation_;
  };
}
