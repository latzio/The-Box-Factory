#version 130

uniform vec4 u_color;

in vec3 v_normal;

out vec4 o_fragcolor;

void main()
{
  o_fragcolor = u_color;
}
