#pragma once

#include "gl.hpp"
#include "../basic_handle.hpp"

namespace squint::gl {
  struct ProgramDeleter {
    void operator()(GLuint program) {
      glDeleteProgram(program);
    }
  };
  using Program = BasicHandle<GLuint, ProgramDeleter>;

  Program compileProgram(const char* vertexSource, const char* fragmentSource);
}
