#version 130

uniform sampler2D u_shadow;

uniform mat4 u_lightTransform;

uniform vec4 u_ambient;
uniform vec4 u_diffuse;
uniform vec4 u_specular;
uniform float u_shininess;

in vec3 v_position;
in vec3 v_normal;
in vec3 v_toEye;
in vec3 v_toLight;
in float v_lambert;

out vec4 o_fragment;

void main()
{
    vec3 diffuse = u_diffuse.rgb * v_lambert;
    vec3 ambient = u_diffuse.rgb * u_ambient.rgb;

    vec3 toEye = normalize(v_toEye);
    vec3 toLight = normalize(v_toLight);
    vec3 normal = normalize(v_normal);

    float shininess = max(0.0, pow(max(0.0, dot(reflect(-toLight, normal), toEye)), u_shininess));
    vec3 specular = u_specular.rgb * shininess;

    o_fragment = vec4(ambient + diffuse + specular, 1.0);
}
