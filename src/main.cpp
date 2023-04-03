#include "gl/gl.hpp"
#include "gl/program.hpp"
#include "gl/renderbuffer.hpp"
#include "gl/framebuffer.hpp"
#include "gl/texture.hpp"
#include "gl/buffer.hpp"
#include "x11/display.hpp"
#include "x11/screenshot.hpp"
#include "x11/gl_context.hpp"
#include "fx/pipeline.hpp"
#include "fx/set_image.hpp"
#include "fx/peek_image.hpp"
#include "fx/fullscreen_quad.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <fstream>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <unistd.h> // for sleep() only
#include <vector>

#include <squint/glsl/fast_pixelate.frag.h>
#include <squint/glsl/quad.vert.h>

int main(int, char**) { 
  // 🤢 this is ugly code, pls ignore
  const auto display = squint::x11::openDisplay();
  const auto output = squint::x11::takeScreenshot(display);
  const auto glContext = squint::x11::initializeGLContext(display);

  /*
   * FX PIPELINE EXPERIMENT
   */
  std::vector<char> outputBuffer(output->width * output->height * 4, 0);

  squint::fx::FullscreenQuad quad;

  squint::fx::Pipeline{output->width, output->height}
  | [](const squint::fx::StageContext&) {
    glClearColor(1.0, 0.0, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  | squint::fx::SetImage{output->width, output->height, output->data}
  | squint::fx::PeekImage{outputBuffer.data()};

  // Write image
  {
    std::fstream file {"test.raw", std::ios::binary | std::ios::out};
    file.write(outputBuffer.data(), outputBuffer.size());
  }
  /*
   * FAST PIXELATE EXPERIMENT
   */
  // Set up framebuffer
#if 0
  std::cout << output->width << " " << output->height << std::endl;
  auto colorRenderbuffer = squint::gl::genRenderbuffer(GL_RGBA8, output->width, output->height);

  auto framebuffer = squint::gl::genFramebuffer();
  glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, *colorRenderbuffer);
  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

  glViewport(0, 0, output->width, output->height);

  // Set up shader
  auto program = squint::gl::compileProgram(
    squint_glsl_quad_vert, squint_glsl_fast_pixelate_frag
  );

  GLuint screenLocation = glGetUniformLocation(*program, "screen");
  std::cout << "Screen loc: " << screenLocation << std::endl;

  GLuint pixelSizeLocation = glGetUniformLocation(*program, "pixelSize");
  std::cout << "Pixel size loc: " << pixelSizeLocation << std::endl;

  const GLuint vertexLocation = 0;
  glBindAttribLocation(*program, vertexLocation, "position");

  const GLuint uvLocation = 1;
  glBindAttribLocation(*program, uvLocation, "texCoord");

  // Set up texture
  auto texture = squint::gl::genTexture();
  glBindTexture(GL_TEXTURE_2D, *texture);
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

  auto vertexBuffer = squint::gl::genBuffer();
  glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(vertexLocation);
  glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, false, sizeof(GLfloat) * 4, (GLvoid*)(sizeof(GLfloat) * 0));

  glEnableVertexAttribArray(uvLocation);
  glVertexAttribPointer(uvLocation, 2, GL_FLOAT, false, sizeof(GLfloat) * 4, (GLvoid*)(sizeof(GLfloat) * 2));

  // Try drawing?
  glClearColor(1.0, 0.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

#if 1
  glUseProgram(*program);
  // For pixelated squares to not be cut off, kPixelSize must be
  // a common factor of the width and height of the screen.
  constexpr float kPixelSize = 24;
  glUniform1f(pixelSizeLocation, kPixelSize);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
#endif
#endif

  return 0; 
}
