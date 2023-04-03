#pragma once

#include "../gl/gl.hpp"
#include "stage_context.hpp"

namespace squint::fx {
  struct SetImage {
    int width, height;
    char* data;

    void operator()(const StageContext& context) {
      assert(context.width == width);
      assert(context.height == height);

      glBindTexture(GL_TEXTURE_2D, context.frontTexture);
      glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0, 0,
        context.width, context.height,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
      );
    }
  };
}
