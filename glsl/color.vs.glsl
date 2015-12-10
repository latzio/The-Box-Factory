#version 130

uniform mat4 u_perspective;
uniform mat4 u_modelview;
uniform mat4 u_modelview_ivt;

in vec3 a_position;
in vec3 a_normal;

out vec3 v_normal;
out float v_illumination;

void main()
{
   vec4 position = u_perspective * u_modelview * vec4(a_position, 1.0);
   vec4 normal = normalize(u_modelview_ivt * vec4(a_normal, 1.0));

   v_normal = normal.xyz;
   v_illumination = max(0.0, dot(v_normal, normalize(vec3(.5, 20.0, 1.5))));
   gl_Position = position;
}
