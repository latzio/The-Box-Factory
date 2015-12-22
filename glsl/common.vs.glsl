#version 130

uniform mat4 u_perspective;
uniform mat4 u_modelview;
uniform mat4 u_modelview_ivt;

uniform vec3 u_eye;// = vec3(0.0, 31.0, 25.0);
uniform vec3 u_light;// = vec3(10.0, 5.0, 5.0);

in vec3 a_position;
in vec3 a_normal;
in vec2 a_tex;

out vec3 v_position;
out vec2 v_tex;
out vec3 v_normal;
out vec3 v_toEye;
out vec3 v_toLight;
out float v_lambert;

void main()
{
   vec4 position = u_modelview * vec4(a_position, 1.0);
   vec4 normal = u_modelview_ivt * vec4(a_normal, 0.0);

   v_tex = a_tex;
   v_position = position.xyz;
   v_normal = normalize(normal.xyz);
   v_toEye = normalize(u_eye - position.xyz);
   v_toLight = normalize(u_light - position.xyz);
   v_lambert = max(dot(v_toLight, v_normal), 0.0);

   gl_Position = u_perspective * position;
}
