#include "OpenGLShader.h"
#include <fstream>
#include <stdexcept>

using namespace Engine::Backend;

static std::string readFile(std::string_view path) {
  std::ifstream file{std::string(path)};
  if (!file.is_open())
    throw std::runtime_error("Failed to open shader file: " +
                             std::string(path));
  return std::string{std::istreambuf_iterator<char>(file),
                     std::istreambuf_iterator<char>()};
}

static GLuint compileShader(GLenum type, const std::string &source) {
  const char *src = source.c_str();
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    glDeleteShader(shader);
    throw std::runtime_error(
        std::string(type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") +
        " SHADER COMPILE ERROR:\n" + infoLog);
  }
  return shader;
}

OpenGLShader::OpenGLShader(std::string_view vertexPath,
                           std::string_view fragmentPath) {
  std::string vertexCode = readFile(vertexPath);
  std::string fragmentCode = readFile(fragmentPath);

  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

  // Linking
  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  int success;
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(ID, 512, nullptr, infoLog);
    glDeleteProgram(ID);
    throw std::runtime_error(std::string("SHADER LINKING ERROR:\n") + infoLog);
  }
}

OpenGLShader::~OpenGLShader() {
  if (ID != 0)
    glDeleteProgram(ID);
}

// I dont know why i would even need to move Shaders like this but claude told
// me to put these here so i obey
OpenGLShader::OpenGLShader(OpenGLShader &&other) noexcept : ID(other.ID) {
  other.ID = 0; // Prevent double-delete
}

OpenGLShader &OpenGLShader::operator=(OpenGLShader &&other) noexcept {
  if (this != &other) {
    if (ID != 0)
      glDeleteProgram(ID);
    ID = other.ID;
    other.ID = 0;
  }
  return *this;
}

void OpenGLShader::use() const { glUseProgram(ID); }

void OpenGLShader::setBool(std::string_view name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.data()), static_cast<int>(value));
}

void OpenGLShader::setInt(std::string_view name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.data()), value);
}

void OpenGLShader::setFloat(std::string_view name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.data()), value);
}
