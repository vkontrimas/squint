#pragma once

namespace squint::fx {
  struct StageContext;
  struct FullscreenQuad;

  class PrettyPixelate {
  public:
    PrettyPixelate(float pixelSize, FullscreenQuad*);

    void operator()(const StageContext&);

  private:
    float pixelSize_;
    FullscreenQuad* fullscreenQuad_;
  };
}
