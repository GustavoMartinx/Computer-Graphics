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
#define TEXTURE 0
#if TEXTURE
#include <SOIL/SOIL.h>
#endif
bool keys[256];

GLuint texture;

#define MAX_PARTICLES 1000
#define WCX 640
#define WCY 480
#define RAIN 0
#define SNOW 1
#define HAIL 2

float slowdown = 2.0;
float velocity = 0.0;
float zoom = -40.0;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.1;

int loop;
int fall;

// floor colors
float r = 0.0;
float g = 1.0;
float b = 0.0;
float ground_points[300][300][3];
float ground_colors[300][300][4];
float accum = -10.0;

typedef struct
{
   // Life
   bool alive; // is the particle alive?
   float life; // particle lifespan
   float fade; // decay
   // color
   float red;
   float green;
   float blue;
   // Position/direction
   float xpos;
   float ypos;
   float zpos;
   // Velocity/Direction, only goes down in y dir
   float vel;
   // Gravity
   float gravity;
} particles;

// Paticle System
particles par_sys[MAX_PARTICLES];

static char *Model_file = NULL; /* nome do arquivo do objeto */
static GLMmodel *Model;         /* modelo do objeto*/
static GLfloat Scale = 4.0;     /* fator de escala */
static GLint WinWidth = 1024, WinHeight = 768;
GLMmodel **Models;
GLfloat *PositionsX;
GLfloat *PositionsY;
GLfloat *PositionsZ;
GLfloat *RotationsX;
GLfloat *RotationsY;
GLfloat *RotationsZ;
GLfloat *ScalesX;
GLfloat *ScalesY;
GLfloat *ScalesZ;
static GLfloat LightPositionsX = 0;
static GLfloat LightPositionsY = 10;
static GLfloat LightPositionsZ = 100;
GLboolean animacao_bola_de_neve = GL_FALSE;
GLboolean animacao_bola_de_neve_estagio1 = GL_FALSE;
GLboolean animacao_bola_de_neve_estagio2 = GL_FALSE;
GLboolean animacao_bola_de_neve_estagio3 = GL_FALSE;
GLfloat vel_queda_bola_neve = 1.641861;
GLboolean animacao_porta = GL_FALSE;
GLboolean porta_indo = GL_TRUE;
GLfloat angulo_da_porta = 0;
unsigned char last_char = ' ';
GLboolean fé = GL_FALSE;

int n_models = 0;

GLdouble xPosCamera = 408, yPosCamera = 283, zPosCamera = 377;
volatile GLdouble xLookCamera = -0.867427, yLookCamera = -0.479646, zLookCamera = -0.481203;
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

static void read_model(char *Model_file,
                       GLfloat PosX, GLfloat PosY, GLfloat PosZ,
                       GLfloat RotX, GLfloat RotY, GLfloat RotZ,
                       GLfloat ScaleX, GLfloat ScaleY, GLfloat ScaleZ)
{
   float objScale;

   /* lendo o modelo */
   Models[n_models] = glmReadOBJ(Model_file);
   ScalesX[n_models] = ScaleX;
   ScalesY[n_models] = ScaleY;
   ScalesZ[n_models] = ScaleZ;
   objScale = glmUnitize(Models[n_models]);
   PositionsX[n_models] = PosX;
   PositionsY[n_models] = PosY;
   PositionsZ[n_models] = PosZ;
   RotationsX[n_models] = RotX;
   RotationsY[n_models] = RotY;
   RotationsZ[n_models] = RotZ;
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
   glClearColor(0.2, 0.2, 0.4, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glEnable(GL_NORMALIZE);
   glCullFace(GL_BACK);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   GLfloat luzAmbiente[4] = {0.1, 0.1, 0.1, 1.0};
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
   GLfloat luzDifusa[4] = {0.3, 0.3, 0.3, 1.0};
   GLfloat luzEspecular[4] = {0.5, 0.5, 0.5, 1.0};
   GLfloat posicoesLuz[4] = {LightPositionsX, LightPositionsY, LightPositionsZ, 1.0};
   glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
   glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
   GLfloat ambientColor[] = {0.1f, 0.1f, 0.1f, 0.0f};
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
   glShadeModel(GL_SMOOTH);

   GLint especMaterial = 80;
   // Define a concentração do brilho
   glMateriali(GL_FRONT, GL_SHININESS, especMaterial);



#if TEXTURE
   texture = SOIL_load_OGL_texture(
       "Textures/grass.jpg",
       SOIL_LOAD_AUTO,
       SOIL_CREATE_NEW_ID,
       SOIL_FLAG_TEXTURE_REPEATS);

   if (texture == 0)
   {
      printf("\nERRO\n");
   }
#endif
}



static void reshape(int width, int height)
{
   float ar = 0.5 * (float)width / (float)height; // razão de aspecto
   WinWidth = width;                              // largura da janela
   WinHeight = height;                            // atura da janela
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-ar, ar, -0.5, 0.5, 1, 1000.0);
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

   GLfloat especularidade[4] = {0.1, 0.1, 0.1, 1.0};
   GLfloat materialDiffuse[] = {1.0, 1.0f, 1.0f, 0.0f};
   for (int i = 0; i < n_models; i++)
   {
      
      glPushMatrix();
      glTranslatef(PositionsX[i], PositionsY[i], PositionsZ[i]);
      if (i == 1)
      {
         glRotatef(RotationsY[2], 0, 1, 0); // Para rodar a porta no lugar dela
         glTranslatef(0, 0, 12);            // Para sincronizar a porta com a casa
      }
      glRotatef(View.rotX, 1, 1, 0);
      glRotatef(View.rotY, 0, 1, 0);
      glRotatef(RotationsX[i], 1, 0, 0);
      glRotatef(RotationsY[i], 0, 1, 0);
      glRotatef(RotationsZ[i], 0, 0, 1);
      if (i == 1)
      {
         glTranslatef(-2, 0, 0);
         glRotatef(angulo_da_porta, 0, 1, 0); // Para fazer a animação da porta
         glTranslatef(2, 0, 0);               // Para girar em torno do batente
         // transladar a porta pra ficar no eixo do lado/ rodar e transladar de novo
      }
      glScalef(ScalesX[i], ScalesY[i], ScalesZ[i]);
      glmDrawVBO(Models[i]);
      glPopMatrix();
   }
   // -43, 12.6, 185
   
   glColor3f(1, 1, 1);
   glTranslatef(0, 0.75, 0);
   glutSolidSphere(0.75, 10, 10);
   glTranslatef(0, 1, 0);
   glutSolidSphere(0.45, 10, 10);
   glPushMatrix();

   // Draw eyes
   glTranslatef(0.2, 0, 0.5);
   glColor3f(0, 0, 1);
   glutSolidSphere(0.03, 10, 10);
   glTranslatef(-0.2, 0, 0);
   glutSolidSphere(0.03, 10, 10);

   glPopMatrix();
   glTranslatef(0.1, -0.05, 0.5);
   glColor3f(1, 0, 0);
   glutSolidCone(0.025, 0.25, 4, 4);

#if TEXTURE
   glPushMatrix();
   glEnable(GL_TEXTURE_2D);
   glColor3f(0.2f, 0.145f, 0.114f);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
#endif
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
   //          View.Rotating = GL_TRUE;s
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

void eventos()
{
   if (animacao_porta)
   {
      if (porta_indo)
      {
         angulo_da_porta += 1;
      }
      else // porta_voltando
      {
         angulo_da_porta -= 1;
      }
      if (angulo_da_porta > 0 || angulo_da_porta < -90)
      {
         animacao_porta = GL_FALSE;
      }
   }
   if (animacao_bola_de_neve)
   {
      if (animacao_bola_de_neve_estagio1)
      {
         vel_queda_bola_neve = 1.641861;
         PositionsX[0] += 0.269845;
         PositionsY[0] -= vel_queda_bola_neve;
         PositionsZ[0] += 0.577322;
         if (ScalesY[0] > 5)
         {
            ScalesX[0] += 0.1;
            ScalesY[0] += 0.1;
            ScalesZ[0] += 0.1;
         }
         else
         {
            ScalesX[0] += 0.25;
            ScalesY[0] += 0.25;
            ScalesZ[0] += 0.25;
         }
      }
      if (PositionsY[0] < 61 && animacao_bola_de_neve_estagio1)
      {
         animacao_bola_de_neve_estagio1 = GL_FALSE;
         animacao_bola_de_neve_estagio2 = GL_TRUE;
         vel_queda_bola_neve = -0.641861;
      }
      if (animacao_bola_de_neve_estagio2)
      {
         vel_queda_bola_neve += 0.1;
         PositionsX[0] += 0.269845 * 2.1;
         PositionsY[0] -= vel_queda_bola_neve;
         PositionsZ[0] += 0.577322 * 2.1;
      }
      if (PositionsY[0] < 15 && animacao_bola_de_neve_estagio2)
      {
         vel_queda_bola_neve = 0.2;
         animacao_bola_de_neve_estagio2 = GL_FALSE;
         animacao_bola_de_neve_estagio3 = GL_TRUE;
      }
      if (animacao_bola_de_neve_estagio3)
      {
         PositionsX[0] += 0.269845 * 2;
         PositionsY[0] -= 0.11;
         ScalesX[0] -= 0.05;
         ScalesY[0] -= 0.05;
         ScalesZ[0] -= 0.05;
         PositionsZ[0] += 0.577322 * 2;
      }
      if ((PositionsY[0] < 0 || ScalesY[0] < 0) && animacao_bola_de_neve_estagio3)
      {
         animacao_bola_de_neve = GL_FALSE;
         PositionsX[0] = -39.148174;
         PositionsY[0] = 128.500000;
         PositionsZ[0] = 39.043934;
         vel_queda_bola_neve = 1.641861;
         ScalesX[0] = 0;
         ScalesY[0] = 0;
         ScalesZ[0] = 0;
         animacao_bola_de_neve_estagio1 = GL_FALSE;
         animacao_bola_de_neve_estagio2 = GL_FALSE;
         animacao_bola_de_neve_estagio3 = GL_FALSE;
      }
   }
   if (keys['w'])
   {
      keys['W'] = false;
      // move forward
      xPosCamera = xPosCamera + 0.5 * xLookCamera;
      zPosCamera = zPosCamera + 0.5 * zLookCamera;
   }
   if (keys['a'])
   {
      keys['A'] = false;
      // move left
      xPosCamera = xPosCamera + 0.5 * zLookCamera;
      zPosCamera = zPosCamera - 0.5 * xLookCamera;
   }
   if (keys['s'])
   {
      keys['S'] = false;
      // move back
      xPosCamera = xPosCamera - 0.5 * xLookCamera;
      zPosCamera = zPosCamera - 0.5 * zLookCamera;
   }
   if (keys['d'])
   {
      keys['D'] = false;
      // move right
      xPosCamera = xPosCamera - 0.5 * zLookCamera;
      zPosCamera = zPosCamera + 0.5 * xLookCamera;
   }
   if (keys['W'])
   {
      keys['w'] = false;
      // fast move forward
      xPosCamera = xPosCamera + 2.0 * xLookCamera;
      zPosCamera = zPosCamera + 2.0 * zLookCamera;
   }
   if (keys['A'])
   {
      keys['a'] = false;
      // fast move left
      xPosCamera = xPosCamera + 2.0 * zLookCamera;
      zPosCamera = zPosCamera - 2.0 * xLookCamera;
   }
   if (keys['S'])
   {
      keys['s'] = false;
      // fast move back
      xPosCamera = xPosCamera - 2.0 * xLookCamera;
      zPosCamera = zPosCamera - 2.0 * zLookCamera;
   }
   if (keys['D'])
   {
      keys['d'] = false;
      // fast move right
      xPosCamera = xPosCamera - 2.0 * zLookCamera;
      zPosCamera = zPosCamera + 2.0 * xLookCamera;
   }

   // mover montanha
   if (fé)
   {
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
   }

   if (keys[' '])
   {
      yPosCamera = yPosCamera + 2.0;
   }
   else if (keys['q'] || keys['Q'])
   {
      if (keys['Q'])
         keys['q'] = false;
      if (keys['q'])
         keys['Q'] = false;
      yPosCamera = yPosCamera - 2.0;
   }
   glutPostRedisplay();
}

static void Keyboard(unsigned char key, int x, int y)
{
   if (last_char == 'f' && key == 'e')
   {
      fé = GL_TRUE;
   }
   if (last_char == 'l' && key == 'u')
   {
      GLfloat posicoesLuz[4] = {PositionsX[2], PositionsY[2], PositionsZ[2], 1.0};
      glLightfv(GL_LIGHT0, GL_POSITION, posicoesLuz);
   }
   if (key == 'o' || key == 'O') // debug snapshot
   {
      printf("Debug Info:\n");
      printf("xLookCamera: %f,yLookCamera: %f,zLookCamera: %f\n", xLookCamera, yLookCamera, zLookCamera);
      printf("xPosCamera = %f, yPosCamera = %f, zPosCamera = %f\n", xPosCamera, yPosCamera, zPosCamera);
   }
   if (key == 'b' || key == 'B')
   {
      if (!animacao_bola_de_neve)
      {
         ScalesX[0] = 1;
         ScalesY[0] = 1;
         ScalesZ[0] = 1;
         animacao_bola_de_neve = GL_TRUE;
         animacao_bola_de_neve_estagio1 = GL_TRUE;
      }
   }
   if (key == 'p' || key == 'P')
   {
      animacao_porta = GL_TRUE;
      if (porta_indo)
      {
         porta_indo = GL_FALSE;
      }
      else
      {
         porta_indo = GL_TRUE;
      }
   }
   if (key < 256)
   {
      keys[key] = true;
   }
   last_char = key;
   // printf("key: %c, mouseX:%d, mouseY:%d\n", key, x, y);
   //  printf("xLookCamera = %f, zLookCamera = %f, maior = %c\n", xLookCamera, zLookCamera, maior);
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
   Models = calloc(10, sizeof(GLMmodel *));
   ScalesX = calloc(10, sizeof(GLfloat));
   ScalesY = calloc(10, sizeof(GLfloat));
   ScalesZ = calloc(10, sizeof(GLfloat));
   PositionsX = calloc(10, sizeof(GLfloat));
   PositionsY = calloc(10, sizeof(GLfloat));
   PositionsZ = calloc(10, sizeof(GLfloat));
   RotationsX = calloc(10, sizeof(GLfloat));
   RotationsY = calloc(10, sizeof(GLfloat));
   RotationsZ = calloc(10, sizeof(GLfloat));
   glutInit(&argc, argv);
   glutInitWindowSize(WinWidth, WinHeight);

   // sudo apt-get install libsoil-dev
   // gcc -o app main.c glm.c glmdraw.c util/readtex.c util/shaderutil.c util/trackball.c -lGLU -lGL -lglut -lGLEW -lm -lSOIL
   // ./app
   // static char * Model_file0 = "untitled.obj";

   // static char * Model_file2 = "bed.obj";

   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutCreateWindow("Snowland");

   glewInit();

   DoFeatureChecks();

   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc(Keyboard);
   glutKeyboardUpFunc(KeyboardUp);
   glutMouseFunc(Mouse);
   glutMotionFunc(Motion);
   glutIdleFunc(eventos);

   InitViewInfo(&View);

   static char *Model_file1 = "Moon2K.obj";
   static char *Model_file2 = "../obj-development/color-door.obj";
   static char *Model_file3 = "../obj-development/cabana-roof-snow.obj";
   static char *Model_file4 = "../obj-development/montanha.obj";
   static char *Model_file5 = "../obj-development/base-globo.obj";
   static char *Model_file6 = "../obj-development/chao-neve.obj";
   static char *Model_file7 = "../obj-development/globo-black.obj";

   read_model(Model_file1,
              -39.148174, 128.5,
              39.043934, 0, 0, 0,
              0, 0, 0); // BOla de neve
   read_model(Model_file2,
              -43, 6.6, 185,
              0, 0, 0,
              100, 100, 100); // PORTA
   read_model(Model_file3,
              -43, 12.6, 185,
              0, 15, 0,
              100, 100, 100); // CASA
   read_model(Model_file4,
              -40, 63.5, 40,
              0, 0, 0,
              175, 175, 175); // montanha
   read_model(Model_file5,
              -23, -43.5, 130,
              0, 0, 0,
              300, 300, 300); // Base Globo
   read_model(Model_file6,
              -23, 1, 130,
              0, 0, 0,
              235, 100, 235); // Chão neve
   read_model(Model_file7,
              -23, 80, 130,
              0, 0, 0,
              250, 250, 250); // Globo

   // CONSTANTES DE SINCRONIZAÇÃO:
   //  SCALE:
   //  MONTANHA: 100 CASA: 100 PORTA: 99
   //  POSICAO: MONTANHA N SEI
   //  MONTANHA: 63.5 PRA TIRAR DO CHÃO
   //  CASA: 9.6 PRA TIRAR DO CHAO, -12 A MAIS QUE A PORTA PRA ENCAIXAR
   //  PORTA: 3.6 PRA TIRAR DO CHÃO, 2 PRA ELA GIRAR NO LADO DELA
   //  EX:
   //  read_model(Model_file2, 100, 0, 3.5, 0, 0, 0, 0);
   //  read_model(Model_file3, 100, 0, 9.500000, -12, 0, 0, 0);

   init();

   glutMainLoop();
   return 0;
}
