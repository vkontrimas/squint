#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <fstream>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <unistd.h> // for sleep() only

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

static bool contextCreationError = false;
int customXErrorHandlerForGLInit(Display*, XErrorEvent*) {
  contextCreationError = true;
  return 0;
}

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
  // Verify GLX version
  int glxMajor, glxMinor;
  auto success = glXQueryVersion(display.get(), &glxMajor, &glxMinor);
  assert(success); assert(glxMajor >= 1 && glxMinor >= 3);

  // Grab framebuffer config info
  const static int visualAttributes[] = {
    GLX_X_RENDERABLE, True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
    // TODO: allowing for float RGB would allow more precision before we downrender to final image
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_ALPHA_SIZE, 8,
    GLX_DEPTH_SIZE, 24, // TODO: technically we don't care about the depth size
    GLX_STENCIL_SIZE, 8, // TODO: nor do we care about stencil size?
    GLX_DOUBLEBUFFER, True, // TODO: nor double buffer (we draw once?)
    // TODO: maybe we just enable these?
    // GLX_SAMPLE_BUFFERS, 1,
    // GLX_SAMPLES, 4,
    None
  };

  int framebufferConfigCount;
  GLXFBConfig* framebufferConfigs = glXChooseFBConfig(
    display.get(), DefaultScreen(display.get()), visualAttributes, &framebufferConfigCount
  ); 
  assert(framebufferConfigs);

  int bestIndex = -1;
  int bestSamples = -1;
  for (int i = 0; i < framebufferConfigCount; ++i) {
    XVisualInfo* visualInfo = glXGetVisualFromFBConfig(display.get(), framebufferConfigs[i]);

    if (visualInfo) {
      int sampleBuffers;
      glXGetFBConfigAttrib(display.get(), framebufferConfigs[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
      int samples;
      glXGetFBConfigAttrib(display.get(), framebufferConfigs[i], GLX_SAMPLES, &samples);
      if (bestIndex  == -1 || (sampleBuffers && samples > bestSamples)) {
        bestIndex = i;
        bestSamples = samples;
      }
    }

    XFree(visualInfo);
  }
  assert(bestIndex > -1);
  assert(bestSamples > -1);
  auto framebufferConfig = framebufferConfigs[bestIndex];
  XFree(framebufferConfigs);

  // Get visual
  XVisualInfo* visualInfo = glXGetVisualFromFBConfig(display.get(), framebufferConfig);
  Colormap colorMap = XCreateColormap(
    display.get(), RootWindow(display.get(), visualInfo->screen), visualInfo->visual, AllocNone
  );
  XSetWindowAttributes windowAttribs;
  windowAttribs.colormap = colorMap;
  windowAttribs.background_pixmap = None;
  windowAttribs.border_pixel = 0;
  windowAttribs.event_mask = StructureNotifyMask;
  Window window = XCreateWindow(
    display.get(),
    RootWindow(display.get(), visualInfo->screen),
    0, 0, 100, 100, 0, visualInfo->depth, InputOutput,
    visualInfo->visual,
    CWBorderPixel | CWColormap | CWEventMask, &windowAttribs
  );
  XFree(visualInfo);
  assert(window);
  XStoreName(display.get(), window, "foo");
  XMapWindow(display.get(), window);

  // Create GL context
  // TODO: X error handler?
  // TODO: In the future we should check GLX_ARB_create_context extension is supported
  //       for now assuming it is.
  using glXCreateContextAttribsARBProc = GLXContext (*)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
  glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 
    (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

  using glGenBuffersProc = void (*)(GLsizei, GLuint*);
  glGenBuffersProc glGenBuffers = (glGenBuffersProc)glXGetProcAddressARB((const GLubyte*)"glGenBuffers");

  using glBindBufferProc = void (*)(GLenum, GLuint);
  glBindBufferProc glBindBuffer = (glBindBufferProc)glXGetProcAddressARB((const GLubyte*)"glBindBuffer");

  using glBufferDataProc = void (*)(GLenum, GLsizeiptr, const GLvoid*, GLenum);
  glBufferDataProc glBufferData = (glBufferDataProc)glXGetProcAddressARB((const GLubyte*)"glBufferData");

  contextCreationError = false;
  auto oldErrorHandler = XSetErrorHandler(&customXErrorHandlerForGLInit);

  // TODO: Robustness. For now assuming we're creating GL 3.0 context
  int contextAttributes[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
    GLX_CONTEXT_MINOR_VERSION_ARB, 0,
    None
  };

  GLXContext glContext = glXCreateContextAttribsARB(
    display.get(),
    framebufferConfig,
    0,
    True,
    contextAttributes
  );


  XSync(display.get(), False);
  assert(!contextCreationError && glContext);
  XSetErrorHandler(oldErrorHandler);

  assert(glXIsDirect(display.get(), glContext));

  // Try drawing?
  glXMakeCurrent(display.get(), window, glContext);

  const GLfloat vertices[3 * 4] {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
  };

  GLuint vertexBuffer;
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glClearColor(1.0, 0.5, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glXSwapBuffers(display.get(), window);
  sleep(5);

  // Cleanup
  glXMakeCurrent(display.get(), 0, nullptr);
  glXDestroyContext(display.get(), glContext);
  XDestroyWindow(display.get(), window);
  XFreeColormap(display.get(), colorMap);

  return 0; 
}
