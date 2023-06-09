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

using glGetUniformLocationProc = GLint (GLuint program, const GLchar* name);
extern glGetUniformLocationProc* glGetUniformLocation;

using glUniform1fProc = void (GLint location, GLfloat v0);
extern glUniform1fProc* glUniform1f; 

using glUniform1iProc = void (GLint location, GLint v0);
extern glUniform1iProc* glUniform1i; 

using glBindAttribLocationProc = void(GLuint, GLuint, const GLchar*);
extern glBindAttribLocationProc* glBindAttribLocation;

using glGetAttribLocationProc = GLint(GLuint, const GLchar*);
extern glGetAttribLocationProc* glGetAttribLocation;

using glEnableVertexAttribArrayProc = void(GLuint);
extern glEnableVertexAttribArrayProc* glEnableVertexAttribArray;

using glDisableVertexAttribArrayProc = void(GLuint);
extern glDisableVertexAttribArrayProc* glDisableVertexAttribArray;

using glVertexAttribPointerProc = void(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
extern glVertexAttribPointerProc* glVertexAttribPointer;

using glGetShaderInfoLogProc = void(GLuint, GLsizei, GLsizei*, GLchar*);
extern glGetShaderInfoLogProc* glGetShaderInfoLog;

using glGetProgramInfoLogProc = void(GLuint, GLsizei, GLsizei*, GLchar*);
extern glGetProgramInfoLogProc* glGetProgramInfoLog;

using glGenRenderbuffersProc = void(GLsizei, GLuint*);
extern glGenRenderbuffersProc* glGenRenderbuffers;

using glDeleteRenderbuffersProc = void(GLsizei, GLuint*);
extern glDeleteRenderbuffersProc* glDeleteRenderbuffers;

using glBindRenderbufferProc = void(GLenum, GLuint);
extern glBindRenderbufferProc* glBindRenderbuffer;

using glRenderbufferStorageProc = void(GLenum, GLenum, GLsizei, GLsizei);
extern glRenderbufferStorageProc* glRenderbufferStorage;

using glGenFramebuffersProc = void(GLsizei, GLuint*);
extern glGenFramebuffersProc* glGenFramebuffers;

using glDeleteFramebuffersProc = void(GLsizei, GLuint*);
extern glDeleteFramebuffersProc* glDeleteFramebuffers;

using glBindFramebufferProc = void(GLenum, GLuint);
extern glBindFramebufferProc* glBindFramebuffer;

using glFramebufferRenderbufferProc = void(GLenum, GLenum, GLenum, GLuint);
extern glFramebufferRenderbufferProc* glFramebufferRenderbuffer;

using glFramebufferTexture2DProc = void(GLenum, GLenum, GLenum, GLuint, GLint);
extern glFramebufferTexture2DProc* glFramebufferTexture2D;

using glCheckFramebufferStatusProc = GLenum(GLenum);
extern glCheckFramebufferStatusProc* glCheckFramebufferStatus;

using glBlitFramebufferProc = void(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
extern glBlitFramebufferProc* glBlitFramebuffer;

namespace squint::gl {
  using GetProcAddressFuncT = void* (const char*);
  void loadGL(GetProcAddressFuncT* getProcAddress);
}
