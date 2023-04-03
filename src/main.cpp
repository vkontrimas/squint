#include "gl/gl.hpp"
#include "x11/display.hpp"
#include "x11/screenshot.hpp"
#include "x11/gl_context.hpp"
#include "fx/pipeline.hpp"
#include "fx/set_image.hpp"
#include "fx/peek_image.hpp"
#include "fx/fullscreen_quad.hpp"
#include "fx/pretty_pixelate.hpp"

#include <X11/Xlib.h> // we depend on XImage rn...

#include <fstream>
#include <vector>

int main(int, char**) { 
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
  | squint::fx::PrettyPixelate{24, &fullscreenQuad, output->width, output->height}
  | squint::fx::PeekImage{outputBuffer.data()};

  std::fstream file {"test.raw", std::ios::binary | std::ios::out};
  file.write(outputBuffer.data(), outputBuffer.size());

  return 0; 
}
