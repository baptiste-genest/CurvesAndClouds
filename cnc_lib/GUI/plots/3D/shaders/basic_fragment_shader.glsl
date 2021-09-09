#version 300 es
precision mediump float;

in vec4 frgColor;
out vec4 glFragColor;

void main()
{
  glFragColor = frgColor;
  //glFragColor = vec4(1,0,0,1);
}
