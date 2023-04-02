#include "xhandle.hpp"

#include <X11/Xlib.h>

namespace squint::x11::detail {
  void XFreeDeleter::operator()(void* x11ObjectPtr) {
    XFree(x11ObjectPtr);
  }
}
