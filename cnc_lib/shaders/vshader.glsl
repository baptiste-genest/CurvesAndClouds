#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;

attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec3 a_color;
attribute float a_value;

varying vec3 v_normal;
varying vec3 v_position;
varying vec3 v_color;
varying float v_value;

void main()
{
    gl_Position = mvp_matrix * a_position;

    v_normal = a_normal;
    v_color = a_color;
    v_position = a_position.xyz;
    v_value = a_value;
}
