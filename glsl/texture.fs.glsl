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

    vec3 pigment = mix(color, texel, 0.85) * clamp(0.2, v_illumination * 10.0, 1.0);

    o_fragcolor = vec4(pigment.r, pigment.g, pigment.b, 1.0);
}
