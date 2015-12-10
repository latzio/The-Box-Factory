#version 130

uniform mat4 u_perspective;
uniform mat4 u_modelview;
uniform mat4 u_modelview_ivt;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_tex;

out vec3 v_normal;
out vec2 v_tex;

void main()
{
   gl_Position = u_perspective * u_modelview * vec4(a_position, 1.0);
   vec4 normal = u_modelview_ivt * vec4(a_normal, 0.0);
   v_normal = normal.xyz;
   v_tex = a_tex;
}
