#include "gl.hpp"

#include <cstdio>

namespace {
  void errorCallback(GLenum, GLenum type, GLuint, GLenum severity, GLsizei, const GLchar* message, const void*) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message);
  }
}

glGenBuffersProc* glGenBuffers = nullptr;
glDeleteBuffersProc* glDeleteBuffers = nullptr;
glBindBufferProc* glBindBuffer = nullptr;
glBufferDataProc* glBufferData = nullptr;
glCreateShaderProc* glCreateShader = nullptr;
glDeleteShaderProc* glDeleteShader = nullptr;
glShaderSourceProc* glShaderSource = nullptr;
glCompileShaderProc* glCompileShader = nullptr;
glGetShaderivProc* glGetShaderiv = nullptr;
glCreateProgramProc* glCreateProgram = nullptr;
glDeleteProgramProc* glDeleteProgram = nullptr;
glAttachShaderProc* glAttachShader = nullptr;
glDetachShaderProc* glDetachShader = nullptr;
glLinkProgramProc* glLinkProgram = nullptr;
glUseProgramProc* glUseProgram = nullptr;
glGetProgramivProc* glGetProgramiv = nullptr;
glGetUniformLocationProc* glGetUniformLocation = nullptr;
glUniform1fProc* glUniform1f = nullptr;
glBindAttribLocationProc* glBindAttribLocation = nullptr;
glGetAttribLocationProc* glGetAttribLocation = nullptr;
glEnableVertexAttribArrayProc* glEnableVertexAttribArray = nullptr;
glVertexAttribPointerProc* glVertexAttribPointer = nullptr;
glGetShaderInfoLogProc* glGetShaderInfoLog = nullptr;
glGetProgramInfoLogProc* glGetProgramInfoLog = nullptr;
glGenRenderbuffersProc* glGenRenderbuffers = nullptr;
glDeleteRenderbuffersProc* glDeleteRenderbuffers = nullptr;
glBindRenderbufferProc* glBindRenderbuffer = nullptr;
glRenderbufferStorageProc* glRenderbufferStorage = nullptr;
glGenFramebuffersProc* glGenFramebuffers = nullptr;
glDeleteFramebuffersProc* glDeleteFramebuffers = nullptr;
glBindFramebufferProc* glBindFramebuffer = nullptr;
glFramebufferRenderbufferProc* glFramebufferRenderbuffer = nullptr;
glFramebufferTexture2DProc* glFramebufferTexture2D = nullptr;
glCheckFramebufferStatusProc* glCheckFramebufferStatus = nullptr;

void squint::gl::loadGL(GetProcAddressFuncT* getProcAddress) {
  glGenBuffers = (glGenBuffersProc*)getProcAddress("glGenBuffers");
  glDeleteBuffers = (glDeleteBuffersProc*)getProcAddress("glDeleteBuffers");
  glBindBuffer = (glBindBufferProc*)getProcAddress("glBindBuffer");
  glBufferData = (glBufferDataProc*)getProcAddress("glBufferData");
  glCreateShader = (glCreateShaderProc*)getProcAddress("glCreateShader");
  glDeleteShader = (glDeleteShaderProc*)getProcAddress("glDeleteShader");
  glShaderSource = (glShaderSourceProc*)getProcAddress("glShaderSource");
  glCompileShader = (glCompileShaderProc*)getProcAddress("glCompileShader");
  glGetShaderiv = (glGetShaderivProc*)getProcAddress("glGetShaderiv");
  glCreateProgram = (glCreateProgramProc*)getProcAddress("glCreateProgram");
  glDeleteProgram = (glDeleteProgramProc*)getProcAddress("glDeleteProgram");
  glAttachShader = (glAttachShaderProc*)getProcAddress("glAttachShader");
  glDetachShader = (glDetachShaderProc*)getProcAddress("glDetachShader");
  glLinkProgram = (glLinkProgramProc*)getProcAddress("glLinkProgram");
  glUseProgram = (glUseProgramProc*)getProcAddress("glUseProgram");
  glGetProgramiv = (glGetProgramivProc*)getProcAddress("glGetProgramiv");
  glGetUniformLocation = (glGetUniformLocationProc*)getProcAddress("glGetUniformLocation");
  glUniform1f = (glUniform1fProc*)getProcAddress("glUniform1f");
  glBindAttribLocation = (glBindAttribLocationProc*)getProcAddress("glBindAttribLocation");
  glGetAttribLocation = (glGetAttribLocationProc*)getProcAddress("glGetAttribLocation");
  glEnableVertexAttribArray = (glEnableVertexAttribArrayProc*)getProcAddress("glEnableVertexAttribArray");
  glVertexAttribPointer = (glVertexAttribPointerProc*)getProcAddress("glVertexAttribPointer");
  glGetShaderInfoLog = (glGetShaderInfoLogProc*)getProcAddress("glGetShaderInfoLog");
  glGetProgramInfoLog = (glGetProgramInfoLogProc*)getProcAddress("glGetProgramInfoLog");
  glGenRenderbuffers = (glGenRenderbuffersProc*)getProcAddress("glGenRenderbuffers");
  glDeleteRenderbuffers = (glDeleteRenderbuffersProc*)getProcAddress("glDeleteRenderbuffers");
  glBindRenderbuffer = (glBindRenderbufferProc*)getProcAddress("glBindRenderbuffer");
  glRenderbufferStorage = (glRenderbufferStorageProc*)getProcAddress("glRenderbufferStorage");
  glGenFramebuffers = (glGenFramebuffersProc*)getProcAddress("glGenFramebuffers");
  glDeleteFramebuffers = (glDeleteFramebuffersProc*)getProcAddress("glDeleteFramebuffers");
  glBindFramebuffer = (glBindFramebufferProc*)getProcAddress("glBindFramebuffer");
  glFramebufferRenderbuffer = (glFramebufferRenderbufferProc*)getProcAddress("glFramebufferRenderbuffer");
  glFramebufferTexture2D = (glFramebufferTexture2DProc*)getProcAddress("glFramebufferTexture2D");
  glCheckFramebufferStatus = (glCheckFramebufferStatusProc*)getProcAddress("glCheckFramebufferStatus");

  // Set up error callback in debug build
  // TODO: Check if GL version supports it! This function is from OpenGL 4+
  // TODO: Move this to gl_context
#ifndef NDEBUG
  using glDebugMessageCallbackProc = void(GLDEBUGPROC, void*);
  glDebugMessageCallbackProc* glDebugMessageCallback =
    (glDebugMessageCallbackProc*)getProcAddress("glDebugMessageCallback");
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(errorCallback, 0);
#endif
}
