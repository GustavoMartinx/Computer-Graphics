#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

static int width;
static int height;
volatile float spin;

void Timer(int value){
    spin += 1.0f;

    glutPostRedisplay();
    glutTimerFunc(20,Timer, 1);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);

    glViewport(0, 0, width / 2, height / 2); // Esquerda baixo
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3.0, 3.0, -3.0, 3.0,
            1, 50); // distancia da camera pro plano near, e pro far

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 4.5,  // x, y, z -  posição da câmera
              0.0, 0.0, 0.0,  // x, y, z -  pra onde a câmera aponta
              0.0, 1.0, 0.0); // x, y, z -  vetor view up

    glPushMatrix();
    glutWireTeapot(2);
    glPopMatrix();

    glViewport(width / 2, 0, width / 2, height / 2); // Direta baixo
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 1,
                   1, 50); // distancia da camera pro plano near, e pro far

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 4.5,  // x, y, z -  posição da câmera
              0.0, 0.0, 0.0,  // x, y, z -  pra onde a câmera aponta
              0.0, 1.0, 0.0); // x, y, z -  vetor view up

    glPushMatrix();
    glRotatef(spin, 0, 0, 1);
    glRotatef(45, 1, 0, 0);
    glutWireTeapot(1.8);
    glPopMatrix();

    glViewport(0, height / 2, width / 2, height / 2); // Esquerda cima
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3.0, 3.0, -3.0, 3.0,
            1, 50); // distancia da camera pro plano near, e pro far

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 4.5, 0.0,   // x, y, z -  posição da câmera
              0.0, 0.0, 0.0,   // x, y, z -  pra onde a câmera aponta
              0.0, 0.0, -1.0); // x, y, z -  vetor view up

    glPushMatrix();
    glutWireTeapot(2);
    glPopMatrix();

    glViewport(width / 2, height / 2, width / 2, height / 2); // Direita cima
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3.0, 3.0, -3.0, 3.0,
            1, 50); // distancia da camera pro plano near, e pro far

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-4.5, 0.0, 0.0, // x, y, z -  posição da câmera
              0.0, 0.0, 0.0,  // x, y, z -  pra onde a câmera aponta
              0.0, 1.0, 0.0); // x, y, z -  vetor view up

    glPushMatrix();
    glutWireTeapot(2);
    glPopMatrix();

    glFlush();
}

static void reshape(int w, int h)
{
    width = w;
    height = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 0);
    glutCreateWindow("Bule - Quatro projecoes");
    glClearColor(1, 1, 1, 1);
    glShadeModel(GL_FLAT);
    glutDisplayFunc(display);

    glutTimerFunc(0, Timer, 1);

    glutReshapeFunc(reshape);
    glutMainLoop();
    return EXIT_SUCCESS;
}
