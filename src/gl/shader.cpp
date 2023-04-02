#include "shader.hpp"
#include <cassert>
#include <iostream>

struct ShaderDeleter {
  void operator()(GLuint shader) {
    glDeleteShader(shader);
  }
};
using Shader = squint::BasicHandle<GLuint, ShaderDeleter>;

namespace {
  Shader compileShader(GLenum shaderType, const char* source) {
    assert(source);

    Shader shader = glCreateShader(shaderType);
    assert(*shader);

    glShaderSource(*shader, 1, &source, nullptr);
    glCompileShader(*shader);

    GLint compileSuccess;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compileSuccess);

    if (!compileSuccess) {
      constexpr GLsizei kLogLength = 1024;
      char log[kLogLength];
      glGetShaderInfoLog(*shader, kLogLength, nullptr, log);
      std::cerr << log << std::endl;
      std::abort();
    }

    return shader;
  }


  void linkProgram(GLuint program) {
    glLinkProgram(program);

    GLint linkSuccess;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);

    if (!linkSuccess) {
      constexpr GLsizei kLogLength = 1024;
      char log[kLogLength];
      glGetProgramInfoLog(program, kLogLength, nullptr, log);
      std::cerr << log << std::endl;
      std::abort();
    }
  }
}

namespace squint::gl {
  Program compileProgram(const char* vertexSource, const char* fragmentSource) {
    Shader fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    Shader vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);

    Program program = glCreateProgram();
    glAttachShader(*program, *vertexShader);
    glAttachShader(*program, *fragmentShader);
    linkProgram(*program);

    glDetachShader(*program, *vertexShader);
    glDetachShader(*program, *fragmentShader);
    return program;
  }
}
