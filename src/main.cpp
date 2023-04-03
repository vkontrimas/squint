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
#include "fx/fast_pixelate.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <fstream>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <unistd.h> // for sleep() only
#include <vector>

int main(int, char**) { 
  // 🤢 this is ugly code, pls ignore
  const auto display = squint::x11::openDisplay();
  const auto output = squint::x11::takeScreenshot(display);
  const auto glContext = squint::x11::initializeGLContext(display);

  std::vector<char> outputBuffer(output->width * output->height * 4, 0);

  squint::fx::FullscreenQuad fullscreenQuad;

  squint::fx::Pipeline{output->width, output->height}
  | [](const squint::fx::StageContext&) {
    glClearColor(1.0, 0.0, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  | squint::fx::SetImage{output->width, output->height, output->data}
  | squint::fx::FastPixelate{24, &fullscreenQuad}
  | squint::fx::PeekImage{outputBuffer.data()};

  std::fstream file {"test.raw", std::ios::binary | std::ios::out};
  file.write(outputBuffer.data(), outputBuffer.size());

  return 0; 
}
