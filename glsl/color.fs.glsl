#version 130

uniform vec4 u_color;
uniform float u_shininess;

in vec3 v_normal;
in float v_illumination;

out vec4 o_fragcolor;

const vec3 c_ambient = vec3(.3, .2, .2);

void main()
{
    vec3 color = u_color.rgb * v_illumination;
    vec3 pigment = mix(c_ambient, color, 0.9);

    o_fragcolor = vec4(pigment, 1.0);
}
