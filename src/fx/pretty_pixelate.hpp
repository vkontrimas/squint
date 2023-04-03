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

    struct Shader {
      gl::Program program;
      GLuint vertexLocation;
      GLuint uvLocation;

      Shader(const char* fragSource);
    };

    Shader average_;
    Shader upscale_;
  };
}
