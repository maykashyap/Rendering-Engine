#include "OpenGLTexture.h"
#include <cstdint>
#include <string_view>

using namespace Engine;

Backend::OpenGLTexture::OpenGLTexture(std::string_view texturePath) {

  // unsigned int texture; glGenTextures(1, &texture);
  // glBindTexture(GL_TEXTURE_2D, texture);
  // // texture options
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // // (s, t, r) = (x, y, z)
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // // load and gen the texture
  // int width, height, nrChannels;
  // unsigned char *data =
  //     stbi_load(TEXTURE_SOURCE_PATH, &width, &height, &nrChannels, 0);
  // if (data) {
  //   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
  //                GL_UNSIGNED_BYTE, data);
  //   glGenerateMipmap(GL_TEXTURE_2D);
  //
  // } else
  //   puts("ERR::TEXTURE_FAILED_TO_BE_LOADED");
  // stbi_image_free(data);
}

Backend::OpenGLTexture::~OpenGLTexture() {}

void Backend::OpenGLTexture::bind(uint32_t location) const {}
