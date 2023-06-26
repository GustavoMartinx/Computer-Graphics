#include "shader.h"

#include "glad.h"
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Shader
{
    unsigned int ID;
};

int hasExtension(const char* extension)
{
    const GLubyte* extensions = glGetString(GL_EXTENSIONS);
    if (extensions != NULL)
    {
        const char* search = (const char*)extensions;
        const char* match = strstr(search, extension);
        while (match != NULL)
        {
            // Ensure the match is a complete word
            const char* end = match + strlen(extension);
            if ((match == search || *(match - 1) == ' ') && (*end == ' ' || *end == '\0'))
            {
                return 1; // Extension found
            }
            search = end;
            match = strstr(search, extension);
        }
    }
    return 0; // Extension not found
}

struct Shader *Shader_create(char *vertexPath, char *fragmentPath)
{
    printf("\nSHADER_CREATE\n");

    struct Shader *shader = (struct Shader *)malloc(sizeof(struct Shader));

    // Read vertex shader file
    FILE *vShaderFile = fopen(vertexPath, "r");

    printf("\nvShaderFile: %p", vShaderFile);

    if (!vShaderFile)
    {
        printf("Failed to open vertex shader file: %s\n", vertexPath);
        free(shader);
        return NULL;
    }

    fseek(vShaderFile, 0, SEEK_END);

    printf("\nvShaderFileSize: %ld", ftell(vShaderFile));

    long int vShaderFileSize = ftell(vShaderFile);

    fseek(vShaderFile, 0, SEEK_SET);

    GLchar *vShaderCode = (GLchar *)malloc(vShaderFileSize + 1);
    fread(vShaderCode, vShaderFileSize, 1, vShaderFile);
    vShaderCode[vShaderFileSize] = '\0';

    printf("\nvShaderCode:\n\n%s", vShaderCode);

    fclose(vShaderFile);

    printf("\n");

    FILE *fShaderFile = fopen(fragmentPath, "r");

    printf("\nfShaderFile: %p", vShaderFile);

    if (!fShaderFile)
    {
        printf("Failed to open fragment shader file: %s\n", fragmentPath);
        free(vShaderCode);
        free(shader);
        return NULL;
    }

    fseek(fShaderFile, 0, SEEK_END);

    printf("\nfShaderFileSize: %ld", ftell(fShaderFile));

    long int fShaderFileSize = ftell(fShaderFile);

    fseek(fShaderFile, 0, SEEK_SET);

    GLchar *fShaderCode = (GLchar *)malloc(fShaderFileSize + 1);
    fread(fShaderCode, fShaderFileSize, 1, fShaderFile);
    fShaderCode[fShaderFileSize] = '\0';

    printf("\nfShaderCode:\n\n%s", fShaderCode);

    fclose(fShaderFile);

    if (!gladLoadGL())
    {
        printf("\nDEU RUIM\n");
        return NULL;
    }

    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    // {
    //     printf("\nDEU RUIM\n");
    //     return NULL;
    // }

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);

    printf("Vertex shader object: %u\n", vertex);

    glShaderSource(vertex, 1, (const GLchar **)&vShaderCode, NULL);
    glCompileShader(vertex);

    if (hasExtension("GL_ARB_fragment_shader"))
    {
        printf("GL_ARB_fragment_shader extension is supported\n");
    }
    else
    {
        printf("GL_ARB_fragment_shader extension is not supported\n");
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    printf("fragment shader object: %u\n", fragment);

    glShaderSource(fragment, 1, (const GLchar **)&fShaderCode, NULL);
    glCompileShader(fragment);

    // GLint compileStatus;
    // glGetShaderiv(fragment, GL_COMPILE_STATUS, &compileStatus);
    // if (compileStatus == GL_FALSE)
    // {
    //     GLint infoLogLength;
    //     glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &infoLogLength);

    //     GLchar *infoLog = (GLchar *)malloc(infoLogLength * sizeof(GLchar));
    //     glGetShaderInfoLog(fragment, infoLogLength, NULL, infoLog);

    //     printf("fragment shader compilation failed:\n%s\n", infoLog);

    //     free(infoLog);
    // }

    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, vertex);
    glAttachShader(shader->ID, fragment);
    glLinkProgram(shader->ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(vShaderCode);
    free(fShaderCode);

    printf("\nACABOU\n");

    return shader;
}

void Shader_use(struct Shader *shader)
{
    printf("\nSHADER_USE\n");
    glUseProgram(shader->ID);
}

void Shader_setBool(struct Shader *shader, char *name, int value)
{
    printf("\nSHADER_SETBOOL\n");
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void Shader_setInt(struct Shader *shader, char *name, int value)
{
    printf("\nSHADER_INT\n");
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void Shader_setFloat(struct Shader *shader, char *name, float value)
{
    printf("\nSHADER_FLOAT\n");
    glUniform1f(glGetUniformLocation(shader->ID, name), value);
}

unsigned int Shader_getID(struct Shader *shader)
{
    printf("\nSHADER_ID\n");
    if (shader)
    {
        return shader->ID;
    }
    return 0; // Or any appropriate default value
}
