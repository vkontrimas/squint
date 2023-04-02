#include "display.hpp"

#include <X11/Xlib.h>
#include <cassert>

namespace squint::x11 {
  namespace detail {
    void DisplayDeleter::operator()(Display* display) {
      XCloseDisplay(display);
    }
  }

  DisplayHandle openDisplay() {
    const char* displayId = getenv("DISPLAY");
    if (!displayId) {
      displayId = ":0";
    }

    DisplayHandle display{XOpenDisplay(displayId)};
    assert(display);
    return display;
  }
}
