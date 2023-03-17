#include <cassert>
#include <cstdlib>
#include <memory>
#include <X11/Xlib.h>

namespace {
  const char* getDisplay() {
    constexpr const char* kDefaultDisplay = ":0";
    const auto display = getenv("DISPLAY");
    return display ? display : kDefaultDisplay;
  }
}

struct DisplayDeleter {
  void operator()(Display* display) {
    XCloseDisplay(display);
  }
};

using DisplayPtr = std::unique_ptr<Display, DisplayDeleter>;

int main(int, char**) { 
  DisplayPtr display{XOpenDisplay(getDisplay())};
  assert(display);

  return 0; 
}
