#include "gl_context.hpp"

#include "../gl.hpp"

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <cassert>

namespace {
  static bool contextCreationError = false;

  int errorHandlerForGLInit(Display*, XErrorEvent*) {
    contextCreationError = true;
    return 0;
  }
}

namespace squint::x11 {
  GLContext initializeGLContext(const DisplayHandle& display) {
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
    auto oldErrorHandler = XSetErrorHandler(&errorHandlerForGLInit);

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

    return {
      display.get(),
      glContext,
      colorMap,
      window
    };
  }

  GLContext::~GLContext() {
    glXMakeCurrent(display, 0, nullptr);
    glXDestroyContext(display, static_cast<GLXContext>(context));
    XDestroyWindow(display, window);
    XFreeColormap(display, colorMap);
  }
}
