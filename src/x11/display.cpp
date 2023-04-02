#include "display.hpp"

#include <X11/Xlib.h>
#include <cassert>

namespace squint::x11 {
  void DisplayDeleter::operator()(Display* display) {
    XCloseDisplay(display);
  }

  DisplayPtr openDisplay() {
    const char* displayId = getenv("DISPLAY");
    if (!displayId) {
      displayId = ":0";
    }

    DisplayPtr display{XOpenDisplay(displayId)};
    assert(display);
    return display;
  }
}
