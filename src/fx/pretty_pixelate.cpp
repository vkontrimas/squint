#include "pretty_pixelate.hpp"
#include "stage_context.hpp"
#include "fullscreen_quad.hpp"

#include <cmath>

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
      averageRadiusLocation_{glGetUniformLocation(*average_.program, "radius")},
      upscale_{squint_glsl_upscale_frag},
      downscaledWidth_{static_cast<int>(std::round(width / pixelSize))},
      downscaledHeight_{static_cast<int>(std::round(height / pixelSize))}{

    glBindTexture(GL_TEXTURE_2D, *downscaleTexture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
      GL_TEXTURE_2D, 0,
      GL_RGBA8,
      downscaledWidth_, downscaledHeight_, 0,
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
    glBindFramebuffer(GL_FRAMEBUFFER, *downscaleFramebuffer_);
    glViewport(0, 0, downscaledWidth_, downscaledHeight_);

    glBindTexture(GL_TEXTURE_2D, context.backTexture);
    glUseProgram(*average_.program);
    glUniform1i(averageRadiusLocation_, static_cast<GLint>(std::round(pixelSize_ / 2.0f)));
    fullscreenQuad_->draw(average_.vertexLocation, average_.uvLocation);

    glBindFramebuffer(GL_FRAMEBUFFER, context.frontBuffer);
    glViewport(0, 0, context.width, context.height);
    glUseProgram(*upscale_.program);
    glBindTexture(GL_TEXTURE_2D, *downscaleTexture_);
    fullscreenQuad_->draw(average_.vertexLocation, average_.uvLocation);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}
