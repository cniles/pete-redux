#include "shader.h"
#define _DEBUG

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>

#define EZ_VERT 0x01
#define EZ_FRAG 0x02
#define EZ_PROG 0x04

using namespace std;

Shader::Shader() {
  glsl_vert = 0;
  glsl_frag = 0;
  glsl_prog = 0;
  error_flag = 0;
}

bool Shader::errors() {
  return error_flag;
}

string Shader::getCompileErrors() {
  stringstream errors;
  char error_buffer[512];
  if(error_flag & EZ_VERT) {
    errors << "There was a GLSL compile error in the vertex shader:\n";
    glGetShaderInfoLog(glsl_vert, 512, NULL, error_buffer);
    errors << error_buffer << '\n';
  }
  if(error_flag & EZ_FRAG) {
    errors << "There was a GLSL compile error in the fragment shader:\n";
    glGetShaderInfoLog(glsl_frag, 512, NULL, error_buffer);
    errors << error_buffer << '\n';
  }
  if(error_flag & EZ_PROG) {
    errors << "There was a GLSL linker error:\n";
    glGetProgramInfoLog(glsl_prog, 512, NULL, error_buffer);
    errors << error_buffer << '\n';
  }
  errors << endl;
  return errors.str();
}

Shader::Shader(const char* vert_file, const char* frag_file) {
  error_flag = 0;
  glsl_vert = glCreateShader(GL_VERTEX_SHADER);
  glsl_frag = glCreateShader(GL_FRAGMENT_SHADER);
  glsl_prog = glCreateProgram();
  glAttachShader(glsl_prog, glsl_vert);
  glAttachShader(glsl_prog, glsl_frag);

  const GLchar* tmp;
  string vert_src;
  string frag_src;

  loadSourceFile(vert_file, vert_src);
  loadSourceFile(frag_file, frag_src);

  tmp = static_cast<const GLchar*>(vert_src.c_str());
  glShaderSource(glsl_vert, 1, &tmp, NULL);
  glCompileShader(glsl_vert);

  tmp = static_cast<const GLchar*>(frag_src.c_str());
  glShaderSource(glsl_frag, 1, &tmp, NULL);
  glCompileShader(glsl_frag);

  GLint result;
  glGetShaderiv(glsl_vert, GL_COMPILE_STATUS, &result);
  if(result==GL_FALSE) {
    cerr << "Error compiling vertex shader, check errors!" << endl;
    error_flag |= EZ_VERT;
  }
  glGetShaderiv(glsl_frag, GL_COMPILE_STATUS, &result);
  if(result==GL_FALSE) {
    cerr << "Error compiling fragment shader, check errors!" << endl;
    error_flag |= EZ_FRAG;
  }
  if(error_flag==0) {
    glLinkProgram(glsl_prog);
    glGetProgramiv(glsl_prog, GL_LINK_STATUS, &result);
    if(result==GL_FALSE) {
      error_flag |= EZ_PROG;
    }
  }
}

void Shader::link() {
  GLint result;
  glLinkProgram(glsl_prog);
  glGetProgramiv(glsl_prog, GL_LINK_STATUS, &result);
  if(result==GL_FALSE) {
    cerr << "Failed link! Check errors!" << endl;
    error_flag |= EZ_PROG;
  }  
}

void Shader::bind() {
  glUseProgram(glsl_prog);
}

void Shader::unbind() {
  glUseProgram(0);
}

GLint Shader::bindAttribute(const string& name, GLuint index) {
  glBindAttribLocation(glsl_prog, index, static_cast<const GLchar*>(name.c_str()));
}

GLint Shader::getUniformLocation(const string& name) {
  map<string,GLint>::iterator loc_iter;
  if((loc_iter=locations.find(name))==locations.end()) {
    GLint loc = glGetUniformLocation(glsl_prog, (char*)name.c_str());
    loc_iter = locations.insert(pair<string,GLint>(name,loc)).first;
    if(loc==-1) {
      cerr << "Accessing invalid uniform variable '" << name << '\'' << endl;
      return -1;
    }
  }
  return loc_iter->second;
}

void Shader::loadSourceFile(const char* fname, string& src) {
  stringstream source;
  fstream src_in(fname, fstream::in);
  char in;
  while(src_in.get(in).good()) {
    source.put(in);
  }
  src_in.close();
  src = source.str();
}
