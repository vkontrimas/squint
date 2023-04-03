#include "pretty_pixelate.hpp"
#include "stage_context.hpp"
#include "fullscreen_quad.hpp"

#include <cmath>
#include <iostream>

extern const char squint_glsl_quad_vert[];
extern const char squint_glsl_average_frag[];
extern const char squint_glsl_upscale_frag[];

namespace squint::fx {
  PrettyPixelate::Shader::Shader(const char* fragSource) 
    : program{gl::compileProgram(squint_glsl_quad_vert, fragSource)},
      vertexLocation{glGetAttribLocation(*program, "position")},
      uvLocation{glGetAttribLocation(*program, "texCoord")} {}

  // TODO: Awkward that we have to take the width & height here when we get it in StageContext too...
  PrettyPixelate::PrettyPixelate(float pixelSize, FullscreenQuad* fullscreenQuad, int width, int height) 
    : pixelSize_{pixelSize},
      fullscreenQuad_{fullscreenQuad},
      downscaleFramebuffer_{gl::genFramebuffer()},
      downscaleTexture_{gl::genTexture()},
      average_{squint_glsl_average_frag},
      upscale_{squint_glsl_upscale_frag} {
    const int downscaledWidth = static_cast<int>(std::round(width / pixelSize));
    const int downscaledHeight = static_cast<int>(std::round(height / pixelSize));
    std::cout << "PPIX: orig w " << width << " h " << height
      << " downscaled w " << downscaledWidth << " h " << downscaledHeight << std::endl;

    glBindTexture(GL_TEXTURE_2D, *downscaleTexture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
      GL_TEXTURE_2D, 0,
      GL_RGBA8,
      downscaledWidth, downscaledHeight, 0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      nullptr
    );
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, *downscaleFramebuffer_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *downscaleTexture_, 0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
  }

  void PrettyPixelate::operator()(const StageContext& context) {
#if 1
    glBindFramebuffer(GL_FRAMEBUFFER, *downscaleFramebuffer_);
    glBindTexture(GL_TEXTURE_2D, context.backTexture);
    glUseProgram(*average_.program);
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    // fullscreenQuad_->draw(average_.vertexLocation, average_.uvLocation);
#endif

#if 1
    glBindFramebuffer(GL_FRAMEBUFFER, context.frontBuffer);
    glUseProgram(*upscale_.program);
    glBindTexture(GL_TEXTURE_2D, *downscaleTexture_);
    fullscreenQuad_->draw(average_.vertexLocation, average_.uvLocation);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
#endif
  }
}
