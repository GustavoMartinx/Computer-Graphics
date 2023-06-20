/*

Programa para carregar modelos de objetos OBJ usando OpenGL e GLM

Observações:
- câmera esta posição padrão (na origem olhando pra a direção negativa de z)
- usando fonte de luz padrão (fonte de luz distante e na direção negativa de z)

*/

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


static char *Model_file = NULL;		/* nome do arquivo do objeto */
static GLMmodel *Model;             /* modelo do objeto*/
static GLfloat Scale = 4.0;			/* fator de escala */
static GLint WinWidth = 1024, WinHeight = 768;
GLMmodel ** Models;
static GLfloat *Scales;
int n_models = 0;

typedef struct{
   // Variáveis para controles de rotação
   float rotX, rotY, rotX_ini, rotY_ini;
   int x_ini,y_ini,bot;
   float Distance;
   /* Quando o mouse está se movendo */
   GLboolean Rotating, Translating;
   GLint StartX, StartY;
   float StartDistance;
} ViewInfo;

static ViewInfo View;

static void InitViewInfo(ViewInfo *view){
   view->Rotating = GL_FALSE;
   view->Translating = GL_FALSE;
   view->Distance = 12.0;
   view->StartDistance = 0.0;
}

static void read_model(char *Model_file) {
   float objScale;

   /* lendo o modelo */
   n_models += 1;
   Model = glmReadOBJ(Model_file);
   objScale = glmUnitize(Model);
   glmFacetNormals(Model);
   if (Model->numnormals == 0) {
      GLfloat smoothing_angle = 90.0;
      glmVertexNormals(Model, smoothing_angle);
   }

   glmLoadTextures(Model);
   glmReIndex(Model);
   glmMakeVBOs(Model);
}

static void init(void){
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glEnable(GL_NORMALIZE);
}


static void reshape(int width, int height) {
   float ar = 0.5 * (float) width / (float) height; //razão de aspecto
   WinWidth = width; //largura da janela
   WinHeight = height;  //atura da janela
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-ar, ar, -0.5, 0.5, 1.0, 300.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -3.0);
}

static void display(void){
   GLfloat rot[4][4];
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glPushMatrix();
      glTranslatef(0.0, 0.0, -View.Distance);
      glRotatef(View.rotX,1,0,0);
	  glRotatef(View.rotY,0,1,0);
      glScalef(Scale, Scale, Scale );
      glmDrawVBO(Model);
   glPopMatrix();
   glutSwapBuffers();
}

/**
 * Evento de Mouse
 */
#define SENS_ROT	5.0
static void Mouse(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON) { //mouse - botão da esquera rotaciona o objeto
        if (state == GLUT_DOWN) {
            View.x_ini = x;
            View.y_ini = y;
            View.rotX_ini = View.rotX;
            View.rotY_ini = View.rotY;
            View.Rotating = GL_TRUE;
        } else if (state == GLUT_UP) {
            View.Rotating = GL_FALSE;
        }
    } else if (button == GLUT_MIDDLE_BUTTON) {  //mouse - botão do meio aproxima ou afasta o objeto (translação)
        if (state == GLUT_DOWN) {
            View.StartX = x;
            View.StartY = y;
            View.StartDistance = View.Distance;
            View.Translating = GL_TRUE;
        } else if (state == GLUT_UP) {
            View.Translating = GL_FALSE;
        }
   }
}

/**
 * Evento de movimento do mouse
 */

static void Motion(int x, int y) {
   int i;
   if (View.Rotating) {
        int deltax = View.x_ini - x;
        int deltay = View.y_ini - y;
		// E modifica ângulos
		View.rotY = View.rotY_ini - deltax/SENS_ROT;
		View.rotX = View.rotX_ini - deltay/SENS_ROT;

      glutPostRedisplay();
   } else if (View.Translating) {
      float dz = 0.02 * (y - View.StartY);
      View.Distance = View.StartDistance + dz;
      glutPostRedisplay();
   }
}


static void DoFeatureChecks(void){
   if (!GLEW_VERSION_2_0) {
      /* check for individual extensions */
      if (!GLEW_ARB_texture_cube_map) {
         printf("Sorry, GL_ARB_texture_cube_map is required.\n");
         exit(1);
      }
      if (!GLEW_ARB_vertex_shader) {
         printf("Sorry, GL_ARB_vertex_shader is required.\n");
         exit(1);
      }
      if (!GLEW_ARB_fragment_shader) {
         printf("Sorry, GL_ARB_fragment_shader is required.\n");
         exit(1);
      }
      if (!GLEW_ARB_vertex_buffer_object) {
         printf("Sorry, GL_ARB_vertex_buffer_object is required.\n");
         exit(1);
      }
   }
   if (!ShadersSupported()) {
      printf("Sorry, GLSL is required\n");
      exit(1);
   }
}


int main(int argc, char** argv) {
   Models = calloc(5, sizeof(GLMmodel *));
   Scales = calloc(5, sizeof(GLfloat));
   glutInit(&argc, argv);
   glutInitWindowSize(WinWidth, WinHeight);

   // gcc -o app main.c glm.c glmdraw.c util/readtex.c util/shaderutil.c util/trackball.c -lGLU -lGL -lglut -lGLEW -lm 
   // ./app
   static char * Model_file1 = "Moon2K.obj";
   static char * Model_file2 = "bed.obj";

   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutCreateWindow("objview");

   glewInit();

   DoFeatureChecks();

   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutMouseFunc(Mouse);
   glutMotionFunc(Motion);

   InitViewInfo(&View);

   read_model(Model_file1);
   read_model(Model_file2);
   init();

   glutMainLoop();
   return 0;
}
