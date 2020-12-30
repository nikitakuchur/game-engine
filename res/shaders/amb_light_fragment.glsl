#version 330 core

out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;

struct Light {
    vec3 color;
};

uniform Light light;

void main() {
    vec3 diffuse = texture(g_albedo_spec, tex_coords).rgb;
    vec3 lighting = diffuse * light.color;
    frag_color = vec4(lighting, 1.0);
}
