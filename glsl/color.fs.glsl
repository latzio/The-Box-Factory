#version 130

uniform vec4 u_color;

in vec3 v_normal;
in float v_illumination;

out vec4 o_fragcolor;

void main()
{
    vec3 color = u_color.rgb ;//* v_illumination;
    vec3 pigment = color;

    o_fragcolor = vec4(pigment.r, pigment.g, pigment.b, 1.0);
}
