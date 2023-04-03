#pragma once

#include "../gl/buffer.hpp"

#include <iostream>

namespace squint::fx {
  class FullscreenQuad {
  public:
    FullscreenQuad() : vertexBuffer_{gl::genBuffer()} {
      glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer_);
      const GLfloat vertices[] {
        // X     Y     U     V
        -1.0f, -1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  0.0f,  1.0f
      };
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void draw(GLuint vertexLocation, GLuint uvLocation) const {
      glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer_);

      glEnableVertexAttribArray(vertexLocation);
      glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, false, sizeof(GLfloat) * 4, (GLvoid*)(sizeof(GLfloat) * 0));

      glEnableVertexAttribArray(uvLocation);
      glVertexAttribPointer(uvLocation, 2, GL_FLOAT, false, sizeof(GLfloat) * 4, (GLvoid*)(sizeof(GLfloat) * 2));

      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glDisableVertexAttribArray(vertexLocation);
      glDisableVertexAttribArray(uvLocation);
    }

  private:
    gl::Buffer vertexBuffer_;
  };
}
