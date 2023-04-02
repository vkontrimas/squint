#pragma once

#include "display.hpp"

namespace squint::x11 {
  struct GLContext {

  };

  GLContext initializeGLContext(const DisplayHandle&);
}
