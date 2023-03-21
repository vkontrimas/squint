#include <cassert>
#include <cstdlib>
#include <memory>
#include <fstream>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <GL/gl.h>
#include <GL/glx.h>

namespace {
  const char* getDisplay() {
    constexpr const char* kDefaultDisplay = ":0";
    const auto display = getenv("DISPLAY");
    return display ? display : kDefaultDisplay;
  }
}

struct DisplayDeleter {
  void operator()(Display* display) {
    XCloseDisplay(display);
  }
};
using DisplayPtr = std::unique_ptr<Display, DisplayDeleter>;

int main(int, char**) { 
  // 🤢 this is ugly code, pls ignore

  DisplayPtr display{XOpenDisplay(getDisplay())};
  assert(display);

  /*
   * X11 screenshot experiment
   */
  Window rootWindow = XDefaultRootWindow(display.get());
  assert(rootWindow);

  XWindowAttributes rootAttribs;
  XGetWindowAttributes(display.get(), rootWindow, &rootAttribs);
  XRenderPictFormat* sourceFormat = XRenderFindVisualFormat(display.get(), rootAttribs.visual);
  XRenderPictureAttributes sourceAttribs;
  sourceAttribs.subwindow_mode = IncludeInferiors; // Not entirely sure what this does

  Picture source = XRenderCreatePicture(display.get(), rootWindow, sourceFormat, CPSubwindowMode, &sourceAttribs);
  assert(source);

  Pixmap pixmap = XCreatePixmap(display.get(), rootWindow, rootAttribs.width, rootAttribs.height, 32);
  assert(pixmap);

  // TODO: check if destinationFormat matches what i3 expects
  XRenderPictFormat* destinationFormat = XRenderFindStandardFormat(display.get(), PictStandardARGB32);
  XRenderPictureAttributes destinationAttribs;
  Picture destination = XRenderCreatePicture(display.get(), pixmap, destinationFormat, 0, &destinationAttribs);
  assert(destination);

  XRenderColor black { 0x0000, 0x0000, 0x0000, 0x0000 };
  XRenderFillRectangle(display.get(), PictOpSrc, destination, &black, 0, 0, rootAttribs.width, rootAttribs.height);
  XRenderComposite(display.get(), PictOpSrc, source, 0, destination, 0, 0, 0, 0, 0, 0, rootAttribs.width, rootAttribs.height);

  XImage* output = XGetImage(display.get(), pixmap, 0, 0, rootAttribs.width, rootAttribs.height, AllPlanes, ZPixmap);
  assert(output);

  // write file out as test
  std::fstream file {"test.raw", std::ios::binary | std::ios::out};
  file.write(output->data, output->width * output->height * 4);

  /*
   * OPENGL CONTEXT EXPERIMENT
   */
  const static int visualAttributes[] = {
    GLX_X_RENDERABLE, True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
    // TODO: allowing for float RGB would allow more precision before we downrender to final image
    GLX_RED_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_ALPHA_SIZE, 8,
    GLX_DEPTH_SIZE, 24, // TODO: technically we don't care about the depth size
    GLX_STENCIL_SIZE, 8, // TODO: nor do we care about stencil size?
    GLX_DOUBLEBUFFER, True, // TODO: nor double buffer (we draw once?)
    // TODO: maybe we just enable these?
    // GLX_SAMPLE_BUFFERS, 1,
    // GLX_SAMPLES, 4
    None
  };

  return 0; 
}
