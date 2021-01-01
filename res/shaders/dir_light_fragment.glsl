#version 330 core

out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;

uniform sampler2D shadow_map;
uniform mat4 light_space_mat;

struct Light {
    vec3 dir;
    vec3 color;
};

uniform Light light;
uniform vec3 view_pos;

float calculate_shadow(vec4 frag_pos_light_space, float bias) {
    // Perform perspective divide
    vec3 proj_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
    // Transform to [0,1] range
    proj_coords = proj_coords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range frag_pos_light as coords)
    float closest_depth = texture(shadow_map, proj_coords.xy).r;
    // Get depth of current fragment from light's perspective
    float current_depth = proj_coords.z;

    if (proj_coords.z > 1.0) {
        return 0.0;
    }

    // PCF
    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
    for (int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
            shadow += current_depth - bias > pcf_depth  ? 1.0 : 0.0;
        }
    }
    return shadow / 9.0;
}

void main() {
    // Retrieve data from g-buffer
    vec3 frag_pos = texture(g_position, tex_coords).rgb;
    vec3 normal = texture(g_normal, tex_coords).rgb;
    vec3 diffuse = texture(g_albedo_spec, tex_coords).rgb;
    float specular = 0.1;

    vec3 view_dir = normalize(view_pos - frag_pos);

    // Diffuse
    vec3 light_dir = normalize(-light.dir);
    vec3 diff = max(dot(normal, light_dir), 0.0) * diffuse * light.color;
    // Specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float s = pow(max(dot(normal, reflect_dir), 0.0), 16.0);
    vec3 spec = light.color * s * specular;

    // Calculate shadow
    float shadow = calculate_shadow(light_space_mat * vec4(frag_pos, 1.0), 0.01);

    vec3 lighting = (1.0 - shadow) * (diff + spec) * light.color;

    frag_color = vec4(lighting, 1.0);
}
