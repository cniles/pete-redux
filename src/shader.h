#ifndef SHADER_H_
#define SHADER_H_

#define GL_GLEXT_PROTOTYPES

#include <string>
#include <map>
#include <SDL/SDL_opengl.h>

class Shader {
  GLuint glsl_vert;
  GLuint glsl_frag;
  GLuint glsl_prog;
  bool error_flag;
  void loadSourceFile(const char* file, std::string&);
  std::map<std::string, GLint> locations;
 public:
  Shader();
  Shader(const char* vert_file, const char* frag_file);
  bool errors();
  std::string getCompileErrors();
  void bind();
  void unbind();
  GLint bindAttribute(const std::string& name, GLuint index);
  void link();
  GLint getUniformLocation(const std::string& name);
};

#endif
