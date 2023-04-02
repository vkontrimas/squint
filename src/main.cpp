#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <fstream>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <GL/glx.h>
#include <unistd.h> // for sleep() only
#include <vector>

#include "gl.hpp"

#include <squint/glsl/test.frag.h>
#include <squint/glsl/test.vert.h>

namespace {
  const char* getDisplay() {
    constexpr const char* kDefaultDisplay = ":0";
    const auto display = getenv("DISPLAY");
    return display ? display : kDefaultDisplay;
  }

  void openglErrorCallback(GLenum, GLenum type, GLuint, GLenum severity, GLsizei, const GLchar* message, const void*) {
      fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message);
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
    0, 0, 1, 1, 0, visualInfo->depth, InputOutput,
    visualInfo->visual,
    CWBorderPixel | CWColormap | CWEventMask, &windowAttribs
  );
  XFree(visualInfo);
  assert(window);
  XStoreName(display.get(), window, "foo");

  using glXCreateContextAttribsARBProc = GLXContext(Display*, GLXFBConfig, GLXContext, Bool, const int*);
  glXCreateContextAttribsARBProc* glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc*)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

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

  // Make context current
  glXMakeCurrent(display.get(), window, glContext);

  // Load the rest of GL
  squint::loadGL((squint::GetProcAddressFuncT*)glXGetProcAddressARB);

  // Set up error callback
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(openglErrorCallback, 0);

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

  glXMakeCurrent(display.get(), 0, nullptr);
  glXDestroyContext(display.get(), glContext);
  XDestroyWindow(display.get(), window);
  XFreeColormap(display.get(), colorMap);

  return 0; 
}
