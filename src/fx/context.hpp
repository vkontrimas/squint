#pragma once

#include <utility>
#include "../gl/framebuffer.hpp"
#include "..//gl/texture.hpp"

namespace squint::fx {
  class Context final {
  public:

    void swapBuffers() {
      using std::swap;
      /*swap(frontBuffer_, backBuffer_);
      swap(fronTexture_, backTexture_);*/
      swap(front_, back_);
    }

    Context& operator|(Context& next) {
      return *this;
    }


  private:
    int front_, back_;
    /*
    Framebuffer frontBuffer_;
    Framebuffer backBuffer_;
    Texture frontTexture_;
    Texture backTexture_;
    */
  };
}
