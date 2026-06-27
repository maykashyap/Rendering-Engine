#version 460 core

layout(location = 0) in vec2 aPos;

uniform mat4 m_viewProjection;

uniform mat4 m_transform;
uniform vec3 v_anchor;

void main()
{
  vec4 final = vec4(vec3(aPos, 0.0) - v_anchor, 1.0);
  gl_Position = m_viewProjection * (m_transform * final);
}
