#include <cassert>
#include <cstdlib>
#include <X11/Xlib.h>

namespace {
  const char* getDisplay() {
    constexpr const char* kDefaultDisplay = ":0";
    const auto display = getenv("DISPLAY");
    return display ? display : kDefaultDisplay;
  }
}

int main(int, char**) { 
  Display* display = XOpenDisplay(getDisplay());
  assert(display);

  return 0; 
}
