#version 130

uniform sampler2D u_texture;
uniform vec4 u_color;
uniform float u_shininess;

in vec3 v_position;
in vec3 v_normal;
in vec2 v_tex;
in float v_illumination;

out vec4 o_fragcolor;

const vec3 c_ambient = vec3(.06, .05, .05);

const vec3 c_eye = vec3(0.0, 31.0, 25.0);
const vec3 c_light = vec3(10.0, 10.0, 2.0);

const vec3 c_highlightColor = vec3(0.2, 0.3, 0.2);

void main()
{
    vec3 color = u_color.rgb * v_illumination;
    vec3 texel = texture(u_texture, v_tex).rgb;

    vec3 diffuse = u_color.rgb * texel * v_illumination;

    vec3 lightVector = normalize(c_light - v_position);
    vec3 eyeVector = normalize(c_eye - v_position);

    vec3 specular = c_highlightColor * max(0.0, pow(dot(reflect(-lightVector, v_normal), eyeVector), u_shininess));

    o_fragcolor = vec4(c_ambient + diffuse + specular, 1.0);
}
