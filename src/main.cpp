#include "gl.hpp"
#include "x11/display.hpp"
#include "x11/screenshot.hpp"
#include "x11/gl_context.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <fstream>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <unistd.h> // for sleep() only
#include <vector>

#include <squint/glsl/test.frag.h>
#include <squint/glsl/test.vert.h>

int main(int, char**) { 
  // 🤢 this is ugly code, pls ignore
  const auto display = squint::x11::openDisplay();
  const auto output = squint::x11::takeScreenshot(display);
  const auto glContext = squint::x11::initializeGLContext(display);

  // Set up framebuffer
  std::cout << output->width << " " << output->height << std::endl;
  GLuint colorRenderbuffer;
  glGenRenderbuffers(1, &colorRenderbuffer);
  assert(colorRenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, output->width, output->height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  assert(framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

  glViewport(0, 0, output->width, output->height); // TODO: Move to Gl

  // Set up shader
  const auto compileShader = [=](GLenum shaderType, const char* source) {
    assert(source);

    GLuint shader = glCreateShader(shaderType);
    assert(shader);

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compileSuccess;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);

    if (!compileSuccess) {
      constexpr GLsizei kLogLength = 1024;
      char log[kLogLength];
      glGetShaderInfoLog(shader, kLogLength, nullptr, log);
      std::cout << log << std::endl;
      std::abort();
    }

    return shader;
  };

  const auto linkProgram = [=](GLuint program) {
    glLinkProgram(program);

    GLint linkSuccess;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);

    if (!linkSuccess) {
      constexpr GLsizei kLogLength = 1024;
      char log[kLogLength];
      glGetProgramInfoLog(program, kLogLength, nullptr, log);
      std::cout << log << std::endl;
      std::abort();
    }
  };

  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, squint_glsl_test_frag);
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, squint_glsl_test_vert);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  linkProgram(program);

  const GLuint vertexLocation = 0;
  glBindAttribLocation(program, vertexLocation, "position");

  const GLuint uvLocation = 1;
  glBindAttribLocation(program, uvLocation, "texCoord");

  // Set up texture
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(
    GL_TEXTURE_2D, 0,
    GL_RGBA8,
    output->width, output->height, 0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    output->data
  );

  // Set up vertex buffer
  const GLfloat vertices[] {
    // X     Y     U     V
    -1.0f, -1.0f,  0.0f,  0.0f,
     1.0f, -1.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  0.0f,  1.0f
  };

  GLuint vertexBuffer;
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(vertexLocation);
  glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, false, sizeof(GLfloat) * 4, (GLvoid*)(sizeof(GLfloat) * 0));

  glEnableVertexAttribArray(uvLocation);
  glVertexAttribPointer(uvLocation, 2, GL_FLOAT, false, sizeof(GLfloat) * 4, (GLvoid*)(sizeof(GLfloat) * 2));

  // Try drawing?
  glClearColor(1.0, 0.0, 1.0, 1.0); // TODO: Move to Gl
  glClear(GL_COLOR_BUFFER_BIT); // TODO: Move to Gl

#if 1
  glUseProgram(program);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // TODO: Move to Gl
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
#endif

  // Write image to file
  std::vector<char> buffer(output->width * output->height * 4, 0);
  glReadPixels(0, 0, output->width, output->height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data()); // TODO: Move to Gl
  {
    std::fstream file {"test.raw", std::ios::binary | std::ios::out};
    file.write(buffer.data(), buffer.size());
  }

  // Cleanup
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteProgram(program);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  return 0; 
}
