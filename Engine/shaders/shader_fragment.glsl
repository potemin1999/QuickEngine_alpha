#version 330

#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) out vec4 color;
layout(location = 1) out float depth;
layout(location = 2) out vec3 normal;

in vec2 uv;
in float v_Depth;

void main(){
  //gl_FragData[0] = vec4(1.0,0.0,0.0,1.0);
  color = vec4(uv,uv);//vec4(vec3(0.5 + uv.x*uv.y*0.5),1.0);
  //color = vec4(v_Depth,v_Depth,v_Depth,1.0);
  depth = v_Depth;
}