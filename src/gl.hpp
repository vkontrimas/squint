#pragma once

#include <GL/gl.h>

namespace squint {
  struct Gl {
    using glGenBuffersProc = void(GLsizei, GLuint*);
    glGenBuffersProc* glGenBuffers;
    using glDeleteBuffersProc = void(GLsizei, GLuint*);
    glDeleteBuffersProc* glDeleteBuffers;
    using glBindBufferProc = void(GLenum, GLuint);
    glBindBufferProc* glBindBuffer;
    using glBufferDataProc = void(GLenum, GLsizeiptr, const GLvoid*, GLenum);
    glBufferDataProc* glBufferData;
    using glCreateShaderProc = GLuint(GLenum);
    glCreateShaderProc* glCreateShader;
    using glDeleteShaderProc = void(GLuint);
    glDeleteShaderProc* glDeleteShader;
    using glShaderSourceProc = void(GLuint, GLsizei, const GLchar**, const GLint*);
    glShaderSourceProc* glShaderSource;
    using glCompileShaderProc = void(GLuint);
    glCompileShaderProc* glCompileShader;
    using glGetShaderivProc = void(GLuint, GLenum, GLint*);
    glGetShaderivProc* glGetShaderiv;
    using glCreateProgramProc = GLuint();
    glCreateProgramProc* glCreateProgram;
    using glDeleteProgramProc = void(GLuint);
    glDeleteProgramProc* glDeleteProgram;
    using glAttachShaderProc = void(GLuint, GLuint);
    glAttachShaderProc* glAttachShader;
    using glLinkProgramProc = void(GLuint);
    glLinkProgramProc* glLinkProgram;
    using glUseProgramProc = void(GLuint);
    glUseProgramProc* glUseProgram;
    using glGetProgramivProc = void(GLuint, GLenum, GLint*);
    glGetProgramivProc* glGetProgramiv;
    using glBindAttribLocationProc = void(GLuint, GLuint, const GLchar*);
    glBindAttribLocationProc* glBindAttribLocation;
    using glEnableVertexAttribArrayProc = void(GLuint);
    glEnableVertexAttribArrayProc* glEnableVertexAttribArray;
    using glVertexAttribPointerProc = void(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
    glVertexAttribPointerProc* glVertexAttribPointer;
    using glGetShaderInfoLogProc = void(GLuint, GLsizei, GLsizei*, GLchar*);
    glGetShaderInfoLogProc* glGetShaderInfoLog;
    using glGetProgramInfoLogProc = void(GLuint, GLsizei, GLsizei*, GLchar*);
    glGetProgramInfoLogProc* glGetProgramInfoLog;
    using glGenTexturesProc = void(GLsizei, GLuint*);
    glGenTexturesProc* glGenTextures;
    using glBindTextureProc = void(GLenum, GLuint);
    glBindTextureProc* glBindTexture;
    using glTexImage2DProc = void(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*);
    glTexImage2DProc* glTexImage2D;
    using glTexParameteriProc = void(GLenum, GLenum, GLint);
    glTexParameteriProc* glTexParameteri;
    using glGenRenderbuffersProc = void(GLsizei, GLuint*);
    glGenRenderbuffersProc* glGenRenderbuffers;
    using glBindRenderbufferProc = void(GLenum, GLuint);
    glBindRenderbufferProc* glBindRenderbuffer;
    using glRenderbufferStorageProc = void(GLenum, GLenum, GLsizei, GLsizei);
    glRenderbufferStorageProc* glRenderbufferStorage;
    using glGenFramebuffersProc = void(GLsizei, GLuint*);
    glGenFramebuffersProc* glGenFramebuffers;
    using glBindFramebufferProc = void(GLenum, GLuint);
    glBindFramebufferProc* glBindFramebuffer;
    using glFramebufferRenderbufferProc = void(GLenum, GLenum, GLenum, GLuint);
    glFramebufferRenderbufferProc* glFramebufferRenderbuffer;
    using glCheckFramebufferStatusProc = GLenum(GLenum);
    glCheckFramebufferStatusProc* glCheckFramebufferStatus;
    using glDebugMessageCallbackProc = void(GLDEBUGPROC, void*);
    glDebugMessageCallbackProc* glDebugMessageCallback;
    // Not strictly necessary but makes access into GL functions more consistent
    using glEnableProc = void(GLenum);
    glEnableProc* glEnable;

    using GetProcAddressFuncT = void* (const char*);

    static Gl load(GetProcAddressFuncT* getProcAddress) {
      Gl gl;
      gl.glGenBuffers = (glGenBuffersProc*)getProcAddress("glGenBuffers");
      gl.glDeleteBuffers = (glDeleteBuffersProc*)getProcAddress("glDeleteBuffers");
      gl.glBindBuffer = (glBindBufferProc*)getProcAddress("glBindBuffer");
      gl.glBufferData = (glBufferDataProc*)getProcAddress("glBufferData");
      gl.glCreateShader = (glCreateShaderProc*)getProcAddress("glCreateShader");
      gl.glDeleteShader = (glDeleteShaderProc*)getProcAddress("glDeleteShader");
      gl.glShaderSource = (glShaderSourceProc*)getProcAddress("glShaderSource");
      gl.glCompileShader = (glCompileShaderProc*)getProcAddress("glCompileShader");
      gl.glGetShaderiv = (glGetShaderivProc*)getProcAddress("glGetShaderiv");
      gl.glCreateProgram = (glCreateProgramProc*)getProcAddress("glCreateProgram");
      gl.glDeleteProgram = (glDeleteProgramProc*)getProcAddress("glDeleteProgram");
      gl.glAttachShader = (glAttachShaderProc*)getProcAddress("glAttachShader");
      gl.glLinkProgram = (glLinkProgramProc*)getProcAddress("glLinkProgram");
      gl.glUseProgram = (glUseProgramProc*)getProcAddress("glUseProgram");
      gl.glGetProgramiv = (glGetProgramivProc*)getProcAddress("glGetProgramiv");
      gl.glBindAttribLocation = (glBindAttribLocationProc*)getProcAddress("glBindAttribLocation");
      gl.glEnableVertexAttribArray = (glEnableVertexAttribArrayProc*)getProcAddress("glEnableVertexAttribArray");
      gl.glVertexAttribPointer = (glVertexAttribPointerProc*)getProcAddress("glVertexAttribPointer");
      gl.glGetShaderInfoLog = (glGetShaderInfoLogProc*)getProcAddress("glGetShaderInfoLog");
      gl.glGetProgramInfoLog = (glGetProgramInfoLogProc*)getProcAddress("glGetProgramInfoLog");
      gl.glGenTextures = (glGenTexturesProc*)getProcAddress("glGenTextures");
      gl.glBindTexture = (glBindTextureProc*)getProcAddress("glBindTexture");
      gl.glTexImage2D = (glTexImage2DProc*)getProcAddress("glTexImage2D");
      gl.glTexParameteri = (glTexParameteriProc*)getProcAddress("glTexParameteri");
      gl.glGenRenderbuffers = (glGenRenderbuffersProc*)getProcAddress("glGenRenderbuffers");
      gl.glBindRenderbuffer = (glBindRenderbufferProc*)getProcAddress("glBindRenderbuffer");
      gl.glRenderbufferStorage = (glRenderbufferStorageProc*)getProcAddress("glRenderbufferStorage");
      gl.glGenFramebuffers = (glGenFramebuffersProc*)getProcAddress("glGenFramebuffers");
      gl.glBindFramebuffer = (glBindFramebufferProc*)getProcAddress("glBindFramebuffer");
      gl.glFramebufferRenderbuffer = (glFramebufferRenderbufferProc*)getProcAddress("glFramebufferRenderbuffer");
      gl.glCheckFramebufferStatus = (glCheckFramebufferStatusProc*)getProcAddress("glCheckFramebufferStatus");
      gl.glDebugMessageCallback = (glDebugMessageCallbackProc*)getProcAddress("glDebugMessageCallback");
      gl.glEnable =(glEnableProc*)getProcAddress("glEnable");
      return gl;
    }
  };
}
