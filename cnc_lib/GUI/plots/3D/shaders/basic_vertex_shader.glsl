#version 300 es

uniform mat4 local_proj;
uniform mat4 view_proj;

in vec4 glColor;
in vec3 glVertex;

out vec4 frgColor;

void main()
{
  gl_Position = view_proj*(local_proj*vec4(glVertex,1.0 ));
  //gl_Position = (local_proj*vec4(glVertex,1.0 ));
  //gl_Position = vec4(glVertex,1.0);
  frgColor = glColor;
}
