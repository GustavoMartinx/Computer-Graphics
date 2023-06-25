#ifndef SHADER_H
#define SHADER_H

typedef struct Shader Shader;

struct Shader *Shader_create(char *vertexPath, char *fragmentPath);
void Shader_use(struct Shader *shader);
void Shader_setBool(struct Shader *shader, char *name, int value);
void Shader_setInt(struct Shader *shader, char *name, int value);
void Shader_setFloat(struct Shader *shader, char *name, float value);
unsigned int Shader_getID(struct Shader *shader);

#endif
