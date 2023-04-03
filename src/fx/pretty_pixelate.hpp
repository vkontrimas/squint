#pragma once

#include "../gl/program.hpp"
#include "../gl/texture.hpp"
#include "../gl/framebuffer.hpp"

namespace squint::fx {
  struct StageContext;
  struct FullscreenQuad;

  class PrettyPixelate {
  public:
    PrettyPixelate(float pixelSize, FullscreenQuad*, int width, int height);

    void operator()(const StageContext&);

  private:
    float pixelSize_;
    FullscreenQuad* fullscreenQuad_;
    gl::Framebuffer downscaleFramebuffer_;
    gl::Texture downscaleTexture_;
    gl::Program averageProgram_;
    gl::Program upscaleProgram_;
  };
}
