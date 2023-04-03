#include "pipeline.hpp"

#include <utility>
#include <cassert>

namespace squint::fx {
  Pipeline::Pipeline(int width, int height) 
    : width_{width}, height_{height},
      frontBuffer_{gl::genFramebuffer()},
      backBuffer_{gl::genFramebuffer()},
      frontTexture_{gl::genTexture()},
      backTexture_{gl::genTexture()} {

    const auto initializeTexture = [&](GLuint texture) {
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(
        GL_TEXTURE_2D, 0,
        GL_RGBA8,
        width_, height_, 0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr
      );
    };

    initializeTexture(*backTexture_);
    glBindFramebuffer(GL_FRAMEBUFFER, *backBuffer_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *backTexture_, 0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    initializeTexture(*frontTexture_);
    glBindFramebuffer(GL_FRAMEBUFFER, *frontBuffer_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *frontTexture_, 0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glViewport(0, 0, width_, height_);
  }

  void Pipeline::prepareNextStage() {
    using std::swap;
    swap(frontBuffer_, backBuffer_);
    swap(frontTexture_, backTexture_);
    glBindFramebuffer(GL_FRAMEBUFFER, *frontBuffer_);
    glViewport(0, 0, width_, height_);
  }
}
