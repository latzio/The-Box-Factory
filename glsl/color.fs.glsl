#version 130

uniform vec4 u_color;

in vec3 v_normal;
in float v_illumination;

out vec4 o_fragcolor;

void main()
{
    vec3 color = u_color.rgb;
    vec3 pigment = mix(color, color * v_illumination, 0.75);

    o_fragcolor = vec4(pigment.r, pigment.g, pigment.b, 1.0);
}
