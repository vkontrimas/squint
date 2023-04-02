#pragma once

#include <memory>


using Display = struct _XDisplay;

namespace squint::x11 {
  namespace detail {
    struct DisplayDeleter {
      void operator()(Display*);
    };
  }
  using DisplayHandle = std::unique_ptr<Display, detail::DisplayDeleter>;

  DisplayHandle openDisplay();
}
