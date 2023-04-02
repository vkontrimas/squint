#pragma once

#include <memory>

namespace squint::x11 {
  namespace detail {
    struct XFreeDeleter {
      void operator()(void* x11ObjectPtr);
    };
  }

  template<typename X11Object>
  using XHandle = std::unique_ptr<X11Object, detail::XFreeDeleter>;
}
