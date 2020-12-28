#version 330 core

out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;

struct Light {
    vec3 pos;
    vec3 color;

    float linear;
    float quadratic;
};

uniform Light light;
uniform vec3 view_pos;

void main() {
    // Retrieve data from g-buffer
    vec3 frag_pos = texture(g_position, tex_coords).rgb;
    vec3 normal = texture(g_normal, tex_coords).rgb;
    vec3 diffuse = texture(g_albedo_spec, tex_coords).rgb;
    float specular = 0.5;

    vec3 lighting = diffuse * 0.1;
    vec3 view_dir = normalize(view_pos - frag_pos);

    // Diffuse
    vec3 light_dir = normalize(light.pos - frag_pos);
    vec3 diff = max(dot(normal, light_dir), 0.0) * diffuse * light.color;
    // Specular
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float s = pow(max(dot(normal, halfway_dir), 0.0), 16.0);
    vec3 spec = light.color * s * specular;
    // Attenuation
    float distance = length(light.pos - frag_pos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * distance * distance);
    diff *= attenuation;
    spec *= attenuation;
    lighting += diff + spec;

    frag_color = vec4(lighting, attenuation);
}
