#version 330

#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

uniform int u_SuperSampling;
uniform int u_Vignette;

layout(location = 0) out vec4 color;
layout(location = 1) out float depth;

in vec2 uv;
in vec2 pixelSize;
in vec2 screenSize;

float mainsample_weight = 1.0;
float sample_weight = 0.5;
float sample_sum = 3;
bool multisampling = false;
bool fxaa = true;
uniform sampler2D ColorTex;
//uniform sampler2D DepthTex;

#define FXAA_EDGE_THRESHOLD_MIN = 1/24;

vec4 color_fxaa(sampler2D texture,vec2 uv){
  vec4 color = vec4(1.0);
  float FXAA_SPAN_MAX = 8.0; //8
  float FXAA_REDUCE_MUL = 1.0/8.0; //8.0
  float FXAA_REDUCE_MIN = 1.0/128.0; //128.0
  vec3 rgbNW=texture2D(texture,uv+(vec2(-pixelSize.x,-pixelSize.y))).xyz;
  vec3 rgbNE=texture2D(texture,uv+(vec2( pixelSize.x,-pixelSize.y))).xyz;
  vec3 rgbSW=texture2D(texture,uv+(vec2(-pixelSize.x, pixelSize.y))).xyz;
  vec3 rgbSE=texture2D(texture,uv+(vec2( pixelSize.x, pixelSize.y))).xyz;
  vec3 rgbM=texture2D(texture,uv).xyz;
  vec3 luma=vec3(0.299, 0.587, 0.114);
  float lumaNW = dot(rgbNW, luma);
  float lumaNE = dot(rgbNE, luma);
  float lumaSW = dot(rgbSW, luma);
  float lumaSE = dot(rgbSE, luma);
  float lumaM  = dot(rgbM,  luma);
  float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
  float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
  vec2 dir;
  dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
  dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
  float dirReduce = max( (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
  float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
  dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
            max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),dir * rcpDirMin)) / vec2(1920,1080);
  vec3 rgbA = (1.0/2.0) * (
      texture2D(texture, uv.xy + dir * (1.0/3.0 - 0.5)).xyz +
      texture2D(texture, uv.xy + dir * (2.0/3.0 - 0.5)).xyz);
  vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
      texture2D(texture, uv.xy + dir * (0.0/3.0 - 0.5)).xyz +
      texture2D(texture, uv.xy + dir * (3.0/3.0 - 0.5)).xyz);
  float lumaB = dot(rgbB, luma);
  if((lumaB < lumaMin) || (lumaB > lumaMax)){
        color.xyz=rgbA;
  }else{
        color.xyz=rgbB;
  }
  return color;
}



vec4 color_msaa(sampler2D texture,vec2 uv){
  vec4 color = vec4(1.0);
  color = mainsample_weight*texture2D(texture,uv);
  color+= sample_weight*texture2D(texture, vec2(uv.x+pixelSize.x,uv.y             ));
  color+= sample_weight*texture2D(texture, vec2(uv.x            ,uv.y+pixelSize.x ));
  color+= sample_weight*texture2D(texture, vec2(uv.x-pixelSize.x,uv.y             ));
  color+= sample_weight*texture2D(texture, vec2(uv.x            ,uv.y-pixelSize.x ));
  color/=sample_sum;
  return color;
}



vec4 color_simple(sampler2D texture,vec2 uv){
  return texture2D(texture,uv);
}



vec4 get_color(sampler2D texture,vec2 uv){
  if (fxaa){
    return color_fxaa(ColorTex,uv);
  }else if (multisampling){
    return color_msaa(ColorTex,uv);
  }else{
    return color_simple(ColorTex,uv);
  }
}



vec4 get_supersampled_color(sampler2D texture,vec2 uv_ss){

  vec4 color_lt = get_color(texture,uv_ss+vec2(0          ,0));
  vec4 color_rt = get_color(texture,uv_ss+vec2(pixelSize.x,0));
  vec4 color_lb = get_color(texture,uv_ss+vec2(0          ,pixelSize.y));
  vec4 color_rb = get_color(texture,uv_ss+vec2(pixelSize.x,pixelSize.y));

  return (color_rb+color_lb+color_rt+color_lt)/4;
}


vec4 get_vignette_color(vec4 color,vec2 vignette_uv,float strength){
  return color*(1-max(strength*(length(vignette_uv)-1),0));
}


void main(){
  if (u_SuperSampling==1)
    color = get_color(ColorTex,uv);
  else
    color = get_supersampled_color(ColorTex,uv);

  //vec2 vignette_uv = vec2(2*uv.x-1,2*uv.y-1);
  if (u_Vignette==1)
    color = get_vignette_color(color,vec2(2*uv.x-1,2*uv.y-1),0.5);
    


  //color = texture2D(ColorTex,uv);
  //color = vec4(texture(in_ColorTex,uv).rgb,1.0);      //vec4(1,0,0,1);
  //color = vec4(uv,0,0);
  //color = texture2D(ColorTex,uv) + vec4(uv,0,0);
  depth = 0.5;
}