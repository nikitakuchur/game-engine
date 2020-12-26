#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

out vec3 frag_pos;
out vec3 normal;
out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 world_pos = model * vec4(a_pos, 1.0);
    frag_pos = world_pos.xyz;

    mat3 normal_matrix = transpose(inverse(mat3(model)));
    normal = normal_matrix * a_normal;

    tex_coords = a_tex_coords;

    gl_Position = projection * view * world_pos;
}
