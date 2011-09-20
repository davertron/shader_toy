#ifdef GL_ES
precision highp float;
#endif

uniform vec2 resolution;
uniform float time;

void main(){
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
