#include "fast_pixelate.hpp"
#include "fullscreen_quad.hpp"
#include "stage_context.hpp"

#include <squint/glsl/fast_pixelate.frag.h>
#include <squint/glsl/quad.vert.h>

namespace squint::fx {
  FastPixelate::FastPixelate(float pixelSize, FullscreenQuad* fullscreenQuad) 
    : pixelSize_{pixelSize},
      fullscreenQuad_{fullscreenQuad},
      program_{gl::compileProgram(squint_glsl_quad_vert, squint_glsl_fast_pixelate_frag)},
      pixelSizeLocation_{glGetUniformLocation(*program_, "pixelSize")},
      vertexLocation_{glGetAttribLocation(*program_, "position")},
      uvLocation_{glGetAttribLocation(*program_, "texCoord")} {}

  void FastPixelate::operator()(const StageContext& context) {
    glUseProgram(*program_);
    glUniform1f(pixelSizeLocation_, pixelSize_);
    fullscreenQuad_->draw(vertexLocation_, uvLocation_);
    glUseProgram(0);
  }
}
