#version 130

uniform mat4 u_perspective;
uniform mat4 u_modelview;
uniform mat4 u_modelview_ivt;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_tex;

out vec3 v_normal;
out vec2 v_tex;
out float v_illumination;

void main()
{
   vec4 position = u_modelview * vec4(a_position, 1.0);
   vec4 normal = normalize(u_modelview_ivt * vec4(a_normal, 1.0));

   v_normal = normal.xyz;

   vec4 light = u_modelview * vec4(.0, 20.0, .0, 1.0);

   vec3 lightVector = normalize(light.xyz - position.xyz);
   //float intensity = smoothstep(0.0, 20.0, length(light));
   v_illumination = dot(v_normal, normalize(lightVector));
   v_tex = a_tex;

   gl_Position = u_perspective * position;
}
