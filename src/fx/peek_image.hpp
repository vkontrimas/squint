#pragma once

#include "stage_context.hpp"

namespace squint::fx {
  struct PeekImage {
    char* output;

    void operator()(const StageContext& context) {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, context.backBuffer);
      glBlitFramebuffer(
        0, 0, context.width, context.height,
        0, 0, context.width, context.height,
        GL_COLOR_BUFFER_BIT, GL_LINEAR
      );
      glReadPixels(
        0, 0, context.width, context.height,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        output
      );
    }
  };
}
