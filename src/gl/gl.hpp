#pragma once

#include <GL/gl.h>

using glGenBuffersProc = void(GLsizei, GLuint*);
extern glGenBuffersProc* glGenBuffers;

using glDeleteBuffersProc = void(GLsizei, GLuint*);
extern glDeleteBuffersProc* glDeleteBuffers;

using glBindBufferProc = void(GLenum, GLuint);
extern glBindBufferProc* glBindBuffer;

using glBufferDataProc = void(GLenum, GLsizeiptr, const GLvoid*, GLenum);
extern glBufferDataProc* glBufferData;

using glCreateShaderProc = GLuint(GLenum);
extern glCreateShaderProc* glCreateShader;

using glDeleteShaderProc = void(GLuint);
extern glDeleteShaderProc* glDeleteShader;

using glShaderSourceProc = void(GLuint, GLsizei, const GLchar**, const GLint*);
extern glShaderSourceProc* glShaderSource;

using glCompileShaderProc = void(GLuint);
extern glCompileShaderProc* glCompileShader;

using glGetShaderivProc = void(GLuint, GLenum, GLint*);
extern glGetShaderivProc* glGetShaderiv;

using glCreateProgramProc = GLuint();
extern glCreateProgramProc* glCreateProgram;

using glDeleteProgramProc = void(GLuint);
extern glDeleteProgramProc* glDeleteProgram;

using glAttachShaderProc = void(GLuint, GLuint);
extern glAttachShaderProc* glAttachShader;

using glDetachShaderProc = void(GLuint, GLuint);
extern glDetachShaderProc* glDetachShader;

using glLinkProgramProc = void(GLuint);
extern glLinkProgramProc* glLinkProgram;

using glUseProgramProc = void(GLuint);
extern glUseProgramProc* glUseProgram;

using glGetProgramivProc = void(GLuint, GLenum, GLint*);
extern glGetProgramivProc* glGetProgramiv;

using glBindAttribLocationProc = void(GLuint, GLuint, const GLchar*);
extern glBindAttribLocationProc* glBindAttribLocation;

using glEnableVertexAttribArrayProc = void(GLuint);
extern glEnableVertexAttribArrayProc* glEnableVertexAttribArray;

using glVertexAttribPointerProc = void(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
extern glVertexAttribPointerProc* glVertexAttribPointer;

using glGetShaderInfoLogProc = void(GLuint, GLsizei, GLsizei*, GLchar*);
extern glGetShaderInfoLogProc* glGetShaderInfoLog;

using glGetProgramInfoLogProc = void(GLuint, GLsizei, GLsizei*, GLchar*);
extern glGetProgramInfoLogProc* glGetProgramInfoLog;

using glGenRenderbuffersProc = void(GLsizei, GLuint*);
extern glGenRenderbuffersProc* glGenRenderbuffers;

using glBindRenderbufferProc = void(GLenum, GLuint);
extern glBindRenderbufferProc* glBindRenderbuffer;

using glRenderbufferStorageProc = void(GLenum, GLenum, GLsizei, GLsizei);
extern glRenderbufferStorageProc* glRenderbufferStorage;

using glGenFramebuffersProc = void(GLsizei, GLuint*);
extern glGenFramebuffersProc* glGenFramebuffers;

using glBindFramebufferProc = void(GLenum, GLuint);
extern glBindFramebufferProc* glBindFramebuffer;

using glFramebufferRenderbufferProc = void(GLenum, GLenum, GLenum, GLuint);
extern glFramebufferRenderbufferProc* glFramebufferRenderbuffer;

using glCheckFramebufferStatusProc = GLenum(GLenum);
extern glCheckFramebufferStatusProc* glCheckFramebufferStatus;

namespace squint::gl {
  using GetProcAddressFuncT = void* (const char*);
  void loadGL(GetProcAddressFuncT* getProcAddress);
}
