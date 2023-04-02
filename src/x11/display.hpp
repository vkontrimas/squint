#pragma once

#include <memory>


using Display = struct _XDisplay;

namespace squint::x11 {
  struct DisplayDeleter {
    void operator()(Display*);
  };
  using DisplayHandle = std::unique_ptr<Display, DisplayDeleter>;

  DisplayHandle openDisplay();
}
