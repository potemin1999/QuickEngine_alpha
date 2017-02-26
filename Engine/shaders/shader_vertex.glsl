#version 330

#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_norm;

uniform mat4 u_MVP;
out vec2 uv;
out float v_Depth;

void main(){
  //gl_PointSize = 10.0;
  vec4 pos = u_MVP * vec4(a_pos,1.0) ;
  gl_Position = vec4(pos.x,pos.y,pos.z,pos.w);
  uv = a_uv;
  v_Depth = pos.w;
}