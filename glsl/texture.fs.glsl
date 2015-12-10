#version 130

uniform vec4 u_color;
uniform sampler2D u_texture;

in vec3 v_normal;
in vec2 v_tex;
in float v_illumination;

out vec4 o_fragcolor;

void main()
{
    vec3 color = u_color.rgb;
    vec3 texel = texture(u_texture, v_tex).rgb;

    vec3 pigment = mix(color, texel, 0.75);
    pigment = mix(pigment, pigment * v_illumination, 0.85);

    o_fragcolor = vec4(pigment.r, pigment.g, pigment.b, 1.0);
}
