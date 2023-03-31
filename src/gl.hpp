#pragma once

#include <GL/gl.h>

namespace squint {
  struct Gl {
    template<typename FuncT>
    struct Function {
      using FuncPtrT = FuncT*;
      FuncPtrT ptr;
    };

    Function<void(GLsizei, GLuint*)> glGenBuffers;
  };
}
