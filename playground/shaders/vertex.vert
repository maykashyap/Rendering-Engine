#version 460 core

layout(location = 0) in vec2 aPos;

out vec3 vertexColor;
uniform float aspectRatio;
uniform mat4 transform;
uniform vec3 anchor;

void main()
{
  vec4 final = vec4(vec3(aPos, 0.0) - anchor, 1.0);
  final = transform * final;
  final = vec4(final.x/aspectRatio, final.yzw);
  // temp code
  float scale = (2.0 * 25.0) / 600.0;
  gl_Position = vec4(final.x * scale, final.y * scale, final.zw);
}
