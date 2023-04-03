#include "pretty_pixelate.hpp"

extern const char squint_glsl_average_frag[];
extern const char squint_glsl_upscale_frag[];

namespace squint::fx {
  PrettyPixelate::PrettyPixelate(float pixelSize, FullscreenQuad* fullscreenQuad) 
    : pixelSize_{pixelSize},
      fullscreenQuad_{fullscreenQuad} {}

  void PrettyPixelate::operator()(const StageContext&) {

  }
}
