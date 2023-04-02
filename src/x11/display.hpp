#pragma once

#include <memory>


using Display = struct _XDisplay;

namespace squint::x11 {
  struct DisplayDeleter {
    void operator()(Display*);
  };
  using DisplayPtr = std::unique_ptr<Display, DisplayDeleter>;

  DisplayPtr openDisplay();
}
