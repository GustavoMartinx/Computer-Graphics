/*

Programa para carregar modelos de objetos OBJ usando OpenGL e GLM

Observações:
- câmera esta posição padrão (na origem olhando pra a direção negativa de z)
- usando fonte de luz padrão (fonte de luz distante e na direção negativa de z)

*/

#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <GL/glew.h>
#include "util/glut_wrap.h"
#include "glm.h"
#include "util/trackball.h"
#include "util/shaderutil.h"
bool keys[256];

#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "shader.h"

unsigned int texture1, texture2;
Shader *ourShader;

unsigned int VBO, VAO, EBO;

static char *Model_file = NULL; /* nome do arquivo do objeto */
static GLMmodel *Model;         /* modelo do objeto*/
static GLfloat Scale = 4.0;     /* fator de escala */
static GLint WinWidth = 1024, WinHeight = 768;
GLMmodel **Models;
static GLfloat *PositionsX;
static GLfloat *PositionsY;
static GLfloat *PositionsZ;
static GLfloat *Scales;
static GLfloat LightPositionsX = 0;
static GLfloat LightPositionsY = 0;
static GLfloat LightPositionsZ = 21;

int n_models = 0;

GLdouble xPosCamera = 0, yPosCamera = 0, zPosCamera = 5;
volatile GLdouble xLookCamera = 0, yLookCamera = 0, zLookCamera = -1;
GLdouble xUpCamera = 0, yUpCamera = 1, zUpCamera = 0;
int ultimomouseX, ultimomouseY = 0;
GLboolean movendoCamera = GL_FALSE;
typedef struct
{
   // Variáveis para controles de rotação
   float rotX, rotY, rotX_ini, rotY_ini;
   int x_ini, y_ini, bot;
   float Distance;
   /* Quando o mouse está se movendo */
   GLboolean Rotating, Translating;
   GLint StartX, StartY;
   float StartDistance;
} ViewInfo;

static ViewInfo View;

static void InitViewInfo(ViewInfo *view)
{
   view->Rotating = GL_FALSE;
   view->Translating = GL_FALSE;
   view->Distance = 12.0;
   view->StartDistance = 0.0;
}

static void read_model(char *Model_file, GLfloat Scale, GLfloat PosX, GLfloat PosY, GLfloat PosZ)
{
   float objScale;

   /* lendo o modelo */
   Models[n_models] = glmReadOBJ(Model_file);
   Scales[n_models] = Scale;
   objScale = glmUnitize(Models[n_models]);
   PositionsX[n_models] = PosX;
   PositionsY[n_models] = PosY;
   PositionsZ[n_models] = PosZ;
   glmFacetNormals(Models[n_models]);
   if (Models[n_models]->numnormals == 0)
   {
      GLfloat smoothing_angle = 90.0;
      glmVertexNormals(Models[n_models], smoothing_angle);
   }

   glmLoadTextures(Models[n_models]);
   glmReIndex(Models[n_models]);
   glmMakeVBOs(Models[n_models]);
   n_models += 1;
}

static void init(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glEnable(GL_NORMALIZE);
   glCullFace(GL_BACK);

   glEnable(GL_LIGHTING);
   GLfloat ambientColor[] = {0.1f, 0.1f, 1.0f, 0.0f};
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

   float vertices[] = {
       // positions          // colors           // texture coords
       0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
       0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
       -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
   };
   unsigned int indices[] = {
       0, 1, 3, // first triangle
       1, 2, 3  // second triangle
   };

   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // position attribute
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   // color attribute
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   // texture coord attribute
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
   glEnableVertexAttribArray(2);

   // texture 1
   // ---------
   glGenTextures(1, &texture1);
   glBindTexture(GL_TEXTURE_2D, texture1);
   // set the texture wrapping parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // set texture filtering parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   // load image, create texture and generate mipmaps
   int width, height, nrChannels;
   stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
   // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
   unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
   if (data)
   {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   }

   stbi_image_free(data);
   // texture 2
   // ---------
   glGenTextures(1, &texture2);
   glBindTexture(GL_TEXTURE_2D, texture2);
   // set the texture wrapping parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // set texture filtering parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   // load image, create texture and generate mipmaps
   data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
   if (data)
   {
      // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   }

   stbi_image_free(data);

   ourShader = Shader_create("./texture.vs", "./texture.fs");

   printf("\nourShader->ID: %d", Shader_getID(ourShader));

   // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
   // -------------------------------------------------------------------------------------------
   Shader_use(ourShader); // don't forget to activate/use the shader before setting uniforms!
   // either set it manually like so:
   glUniform1i(glGetUniformLocation(Shader_getID(ourShader), "texture1"), 0);
   // or set it via the texture class
   Shader_setInt(ourShader, "texture2", 1);

   // should be ambient light
   // does not work on our models: ambient light is configured in
   // glmdraw.c in line 408
}

static void reshape(int width, int height)
{
   float ar = 0.5 * (float)width / (float)height; // razão de aspecto
   WinWidth = width;                              // largura da janela
   WinHeight = height;                            // atura da janela
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-ar, ar, -0.5, 0.5, 1.0, 500.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -3.0);
}

static void display(void)
{
   GLfloat rot[4][4];
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(xPosCamera, yPosCamera, zPosCamera,
             xPosCamera + xLookCamera, yPosCamera + yLookCamera, zPosCamera + zLookCamera,
             xUpCamera, yUpCamera, zUpCamera);

   for (int i = 0; i < n_models; i++)
   {
      glPushMatrix();
      glTranslatef(PositionsX[i], PositionsY[i], PositionsZ[i]);
      glRotatef(View.rotX, 1, 0, 0);
      glRotatef(View.rotY, 0, 1, 0);
      glScalef(Scales[i], Scales[i], Scales[i]);
      glmDrawVBO(Models[i]);
      glPopMatrix();
   }

   // bind textures on corresponding texture units
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture1);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texture2);

   // render container
   Shader_use(ourShader);
   glBindVertexArray(VAO);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

   glutSwapBuffers();
}

/**
 * Evento de Mouse
 */
#define SENS_ROT 5.0
static void Mouse(int button, int state, int x, int y)
{
   //  if (button == GLUT_LEFT_BUTTON) { //mouse - botão da esquera rotaciona o objeto
   //      if (state == GLUT_DOWN) {
   //          View.x_ini = x;
   //          View.y_ini = y;
   //          View.rotX_ini = View.rotX;
   //          View.rotY_ini = View.rotY;
   //          View.Rotating = GL_TRUE;
   //      } else if (state == GLUT_UP) {
   //          View.Rotating = GL_FALSE;
   //      }
   //  } else if (button == GLUT_MIDDLE_BUTTON) {  //mouse - botão do meio aproxima ou afasta o objeto (translação)
   //      if (state == GLUT_DOWN) {
   //          View.StartX = x;
   //          View.StartY = y;
   //          View.StartDistance = View.Distance;
   //          View.Translating = GL_TRUE;
   //      } else if (state == GLUT_UP) {
   //          View.Translating = GL_FALSE;
   //      }
   // }
   if (button == GLUT_RIGHT_BUTTON)
   { // mouse - botão da direita rotaciona a camera
      if (state == GLUT_DOWN)
      {

         if (!movendoCamera)
         {
            ultimomouseX = x;
            ultimomouseY = y;
            // printf("x: %d, y: %d\n", x, y);
            // printf("xLookCamera: %f,yLookCamera: %f,zLookCamera: %f\n", xLookCamera, yLookCamera, zLookCamera);
         }
         movendoCamera = GL_TRUE;
      }
      else if (state == GLUT_UP)
      {
         movendoCamera = GL_FALSE;
      }
   }
}

static void Keyboard(unsigned char key, int x, int y)
{
   if (key == 'p') // debug snapshot
   {
      printf("Debug Info:\n");
      printf("xLookCamera: %f,yLookCamera: %f,zLookCamera: %f\n", xLookCamera, yLookCamera, zLookCamera);
      printf("xPosCamera = %f, yPosCamera = %f, zPosCamera = %f\n", xPosCamera, yPosCamera, zPosCamera);
   }
   else if (key == 'l')
   {
      LightPositionsX = xPosCamera;
      LightPositionsY = yPosCamera;
      LightPositionsZ = zPosCamera;
   }

   if (key < 256)
   {
      keys[key] = true;
   }
   // printf("key: %c, mouseX:%d, mouseY:%d\n", key, x, y);
   //  printf("xLookCamera = %f, zLookCamera = %f, maior = %c\n", xLookCamera, zLookCamera, maior);
   if (keys['w'])
   {
      // move forward
      xPosCamera = xPosCamera + xLookCamera;
      zPosCamera = zPosCamera + zLookCamera;
   }
   if (keys['a'])
   {
      // move left
      xPosCamera = xPosCamera + zLookCamera;
      zPosCamera = zPosCamera - xLookCamera;
   }
   if (keys['s'])
   {
      // move back
      xPosCamera = xPosCamera - xLookCamera;
      zPosCamera = zPosCamera - zLookCamera;
   }
   if (keys['d'])
   {
      // move right
      xPosCamera = xPosCamera - zLookCamera;
      zPosCamera = zPosCamera + xLookCamera;
   }

   // mover montanha
   if (keys['i'])
   {
      // move forward
      PositionsX[3] = PositionsX[3] + 2 * xLookCamera;
      PositionsZ[3] = PositionsZ[3] + 2 * zLookCamera;
   }
   if (keys['j'])
   {
      // move left
      PositionsX[3] = PositionsX[3] + 2 * zLookCamera;
      PositionsZ[3] = PositionsZ[3] - 2 * xLookCamera;
   }
   if (keys['k'])
   {
      // move back
      PositionsX[3] = PositionsX[3] - 2 * xLookCamera;
      PositionsZ[3] = PositionsZ[3] - 2 * zLookCamera;
   }
   if (keys['l'])
   {
      // move right
      PositionsX[3] = PositionsX[3] - 2 * zLookCamera;
      PositionsZ[3] = PositionsZ[3] + 2 * xLookCamera;
   }

   if (keys[' '])
   {
      yPosCamera = yPosCamera + 0.5;
   }
   else if (keys['q'])
   {
      yPosCamera = yPosCamera - 0.5;
   }
   glutPostRedisplay();
}

void KeyboardUp(unsigned char key, int x, int y)
{
   keys[key] = false;
}

/**
 * Evento de movimento do mouse
 */

static void Motion(int x, int y)
{
   int i;
   if (View.Rotating)
   {
      int deltax = View.x_ini - x;
      int deltay = View.y_ini - y;
      // E modifica ângulos
      View.rotY = View.rotY_ini - deltax / SENS_ROT;
      View.rotX = View.rotX_ini - deltay / SENS_ROT;

      glutPostRedisplay();
   }
   else if (View.Translating)
   {
      float dz = 0.02 * (y - View.StartY);
      View.Distance = View.StartDistance + dz;
      glutPostRedisplay();
   }
   if (movendoCamera)
   {
      if (!ultimomouseX && !ultimomouseY)
      {
         ultimomouseX = x;
         ultimomouseY = y;
      }
      // printf("x: %d, y: %d\n", x, y);
      // printf("xLookCamera: %f,yLookCamera: %f,zLookCamera: %f\n", xLookCamera, yLookCamera, zLookCamera);
      GLdouble deltaX = -(x - ultimomouseX);
      GLdouble deltaY = (y - ultimomouseY);
      GLdouble deltaToRad = (36 * (M_PI / 180) * 0.01);
      GLdouble angulo_de_mudanca = (deltaX * deltaToRad);

      zLookCamera = zLookCamera * cos(angulo_de_mudanca) - xLookCamera * sin(angulo_de_mudanca);
      xLookCamera = zLookCamera * sin(angulo_de_mudanca) + xLookCamera * cos(angulo_de_mudanca);

      yLookCamera = yLookCamera * cos((deltaY * deltaToRad)) - 1 * sin((deltaY * deltaToRad));

      if (yLookCamera > 2)
      {
         yLookCamera = 2;
      }
      if (yLookCamera < -2)
      {
         yLookCamera = -2;
      }

      ultimomouseX = x;
      ultimomouseY = y;
      glutPostRedisplay();
   }
}

static void DoFeatureChecks(void)
{
   if (!GLEW_VERSION_2_0)
   {
      /* check for individual extensions */
      if (!GLEW_ARB_texture_cube_map)
      {
         printf("Sorry, GL_ARB_texture_cube_map is required.\n");
         exit(1);
      }
      if (!GLEW_ARB_vertex_shader)
      {
         printf("Sorry, GL_ARB_vertex_shader is required.\n");
         exit(1);
      }
      if (!GLEW_ARB_fragment_shader)
      {
         printf("Sorry, GL_ARB_fragment_shader is required.\n");
         exit(1);
      }
      if (!GLEW_ARB_vertex_buffer_object)
      {
         printf("Sorry, GL_ARB_vertex_buffer_object is required.\n");
         exit(1);
      }
   }
   if (!ShadersSupported())
   {
      printf("Sorry, GLSL is required\n");
      exit(1);
   }
}

int main(int argc, char **argv)
{
   Models = (GLMmodel **)calloc(10, sizeof(GLMmodel *));
   Scales = (GLfloat *)calloc(10, sizeof(GLfloat));
   PositionsX = (GLfloat *)calloc(10, sizeof(GLfloat));
   PositionsY = (GLfloat *)calloc(10, sizeof(GLfloat));
   PositionsZ = (GLfloat *)calloc(10, sizeof(GLfloat));
   glutInit(&argc, argv);
   glutInitWindowSize(WinWidth, WinHeight);

   // gcc -o app main.c glm.c glmdraw.c util/readtex.c util/shaderutil.c util/trackball.c -lGLU -lGL -lglut -lGLEW -lm
   // ./app
   static char *Model_file1 = "Moon2K.obj";
   static char *Model_file2 = "bed.obj";
   static char *Model_file3 = "bobcat.obj";
   static char *Model_file4 = "../obj-development/montanha.obj";
   // vai crashar com menos de 4 objetos pq to movendo PositionsX[3] hardcoded antes
   // se tirar objs tira isso ai tbm

   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutCreateWindow("objview");

   glewInit();

   const GLubyte *version = glGetString(GL_VERSION);
   const GLubyte *renderer = glGetString(GL_RENDERER);
   const GLubyte *vendor = glGetString(GL_VENDOR);
   const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

   printf("OpenGL Version: %s\n", version);
   printf("Renderer: %s\n", renderer);
   printf("Vendor: %s\n", vendor);
   printf("GLSL Version: %s\n", glslVersion);

   DoFeatureChecks();

   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc(Keyboard);
   glutKeyboardUpFunc(KeyboardUp);
   glutMouseFunc(Mouse);
   glutMotionFunc(Motion);

   InitViewInfo(&View);

   read_model(Model_file1, 0.5, 40, 90, 40);
   read_model(Model_file2, 3, 5, 0, 0);
   read_model(Model_file3, 3, 10, 0, 0);
   read_model(Model_file4, 100, 40, 25, 40);

   init();

   glutMainLoop();
   return 0;
}
