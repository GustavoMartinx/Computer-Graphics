#include "shader.h"
#include "glad.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Shader
{
    unsigned int ID;
};

struct Shader *Shader_create(char *vertexPath, char *fragmentPath)
{
    struct Shader *shader = (struct Shader *)malloc(sizeof(struct Shader));

    // Read vertex shader file
    FILE *vShaderFile = fopen(vertexPath, "r");
    if (!vShaderFile)
    {
        printf("Failed to open vertex shader file: %s\n", vertexPath);
        free(shader);
        return NULL;
    }

    fseek(vShaderFile, 0, SEEK_END);
    long vShaderFileSize = ftell(vShaderFile);
    fseek(vShaderFile, 0, SEEK_SET);

    GLchar *vShaderCode = (GLchar *)malloc(vShaderFileSize + 1);
    fread(vShaderCode, vShaderFileSize, 1, vShaderFile);
    vShaderCode[vShaderFileSize] = '\0';

    fclose(vShaderFile);

    // Read fragment shader file
    FILE *fShaderFile = fopen(fragmentPath, "r");
    if (!fShaderFile)
    {
        printf("Failed to open fragment shader file: %s\n", fragmentPath);
        free(vShaderCode);
        free(shader);
        return NULL;
    }

    fseek(fShaderFile, 0, SEEK_END);
    long fShaderFileSize = ftell(fShaderFile);
    fseek(fShaderFile, 0, SEEK_SET);

    GLchar *fShaderCode = (GLchar *)malloc(fShaderFileSize + 1);
    fread(fShaderCode, fShaderFileSize, 1, fShaderFile);
    fShaderCode[fShaderFileSize] = '\0';

    fclose(fShaderFile);

    // Compile shaders and create program
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const GLchar**)&vShaderCode, NULL);
    glCompileShader(vertex);

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const GLchar**)&fShaderCode, NULL);
    glCompileShader(fragment);

    // shader Program
    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, vertex);
    glAttachShader(shader->ID, fragment);
    glLinkProgram(shader->ID);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(vShaderCode);
    free(fShaderCode);

    return shader;
}

void Shader_use(struct Shader *shader)
{
    glUseProgram(shader->ID);
}

void Shader_setBool(struct Shader *shader, char *name, int value)
{
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void Shader_setInt(struct Shader *shader, char *name, int value)
{
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void Shader_setFloat(struct Shader *shader, char *name, float value)
{
    glUniform1f(glGetUniformLocation(shader->ID, name), value);
}

unsigned int Shader_getID(struct Shader *shader)
{
    if (shader)
    {
        return shader->ID;
    }
    return 0; // Or any appropriate default value
}
