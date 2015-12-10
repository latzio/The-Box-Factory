#version 130

uniform vec4 u_color;
uniform sampler2D u_texture;

in vec3 v_normal;
in vec2 v_tex;

out vec4 o_fragcolor;

void main()
{
    o_fragcolor = mix(u_color, texture(u_texture, v_tex), 0.99);
}
