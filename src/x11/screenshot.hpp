#pragma once

#include "display.hpp"
#include "xhandle.hpp"

using XImage = struct _XImage;

namespace squint::x11 {
  XHandle<XImage> takeScreenshot(const squint::x11::DisplayHandle& display);
}
