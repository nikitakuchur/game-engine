#version 330 core

layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_albedo_spec;

in vec3 frag_pos;
in vec3 normal;
in vec2 tex_coords;

uniform vec4 color = vec4(1.0);
uniform sampler2D texture_diffuse;
//uniform sampler2D texture_specular;

void main() {
    // Store the fragment position vector in the first g-buffer texture
    g_position = frag_pos;
    // Also store the per-fragment normals into the g-buffer
    g_normal = normalize(normal);
    // And the diffuse per-fragment color
    g_albedo_spec.rgb = texture(texture_diffuse, tex_coords).rgb * color.rgb;
    // Store specular intensity in g_albedo_spec's alpha component
    //g_albedo_spec.a = texture(texture_specular, tex_coords).r;
    g_albedo_spec.a = 1.0;
}
