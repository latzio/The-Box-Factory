#version 130

uniform sampler2D u_texture;
uniform vec4 u_color;
uniform float u_shininess;

in vec3 v_normal;
in vec2 v_tex;
in float v_illumination;

out vec4 o_fragcolor;

const vec3 c_ambient = vec3(.3, .2, .2);

void main()
{
    vec3 color = u_color.rgb * v_illumination;
    vec3 texel = texture(u_texture, v_tex).rgb;

    vec3 pigment = mix(c_ambient, color * texel, 0.9);

    o_fragcolor = vec4(pigment, 1.0);
}
