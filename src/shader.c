#include "shader.h"

#include <glad/glad.h>
#include <stdio.h>
#include <malloc.h>

uint32_t shader_create(uint32_t type, const char *source) {
    uint32_t fragment = glCreateShader(type);
    glShaderSource(fragment, 1, &source, NULL);
    glCompileShader(fragment);
    // Check for shader compile errors
    int success;
    char log[512];
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, log);
        printf("COMPILATION_FAILED\n %s", log);
    }
    return fragment;
}

uint32_t shader_load(uint32_t type, const char *filename) {
    FILE *file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    uint32_t length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *str = malloc((length + 1) * sizeof(char));
    fread(str, 1, length, file);
    fclose(file);
    str[length] = '\0';
    uint32_t shader = shader_create(type, str);
    free(str);
    return shader;
}

uint32_t shader_create_program(uint32_t vertex, uint32_t fragment) {
    uint32_t program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    // Check for linking errors
    int success;
    char log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, log);
        printf("LINKING_FAILED\n %s", log);
    }
    return program;
}

uint32_t shader_create_program_f(const char *vertex_filename, const char *fragment_filename) {
    uint32_t vertex_shader = shader_load(GL_VERTEX_SHADER, vertex_filename);
    uint32_t fragment_shader = shader_load(GL_FRAGMENT_SHADER, fragment_filename);

    uint32_t shader_program = shader_create_program(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

void shader_set_int(uint32_t program, const char *name, int value) {
    uint32_t loc = glGetUniformLocation(program, name);
    glUniform1i(loc, value);
}

void shader_set_float(uint32_t program, const char *name, float value) {
    uint32_t loc = glGetUniformLocation(program, name);
    glUniform1f(loc, value);
}

void shader_set_vec3(uint32_t program, const char *name, vec3 v) {
    uint32_t loc = glGetUniformLocation(program, name);
    glUniform3fv(loc, 1, v);
}

void shader_set_mat4(uint32_t program, const char *name, mat4 m) {
    uint32_t loc = glGetUniformLocation(program, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const float *) m);
}
