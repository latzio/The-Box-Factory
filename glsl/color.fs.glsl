#version 130

uniform vec4 u_ambient;
uniform vec4 u_diffuse;
uniform vec4 u_specular;
uniform float u_shininess;

in vec3 v_normal;
in vec3 v_toEye;
in vec3 v_toLight;
in float v_lambert;

out vec4 o_fragcolor;

void main()
{
    vec3 diffuse = u_diffuse.rgb * v_lambert;

    vec3 toEye = normalize(v_toEye);
    vec3 toLight = normalize(v_toLight);
    float shininess = max(0.0, pow(dot(reflect(-toLight, v_normal), v_toEye), u_shininess));
    vec3 specular = u_specular.rgb * shininess;

    o_fragcolor = vec4(u_ambient.rgb + diffuse + specular, 1.0);
}
