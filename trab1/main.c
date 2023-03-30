#include <GL/glut.h>

char mode = 't';
GLfloat tetha = 0.0;
GLfloat translateOffsetX = 0.0;
GLfloat translateOffsetY = 0.0;
GLfloat scale = 1;


int init(void){
    glClearColor(0.0, 0.0, 0.0, 0.0);     //define a cor de fundo

    glMatrixMode(GL_PROJECTION);          //carrega a matriz de projeção
    gluOrtho2D(0.0,200.0,0.0,150.0);      //define projeção ortogonal 2D

}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);         //desenha o fundo (limpa a janela)
    
    glPushMatrix();
    glTranslatef(translateOffsetX, translateOffsetY, 0.0);
    glTranslatef(5, 5, 0.0);
    glScalef(scale, scale, 1);
    glRotatef(tetha, 0.0, 0.0, 1);
    glTranslatef(-5, -5, 0.0);
    glColor3f(0.0,0.0,1.0);               //altera o atributo de cor
    glBegin(GL_QUADS);                    //desenha um quadrado
        glVertex2i(0, 0);
        glVertex2i(0, 10);
        glVertex2i(10, 10);
        glVertex2i(10, 0);
    glEnd();

    glPopMatrix();
    glFlush();                            //desenha os comandos não executados
}

// Função callback chamada para gerenciar eventos de teclado
void GerenciaTeclado(unsigned char key, int x, int y)
{
    switch (key) {
 
            case 'r':// muda o modo para rotação
                    mode = 'r';
                    break;

            case 't':// muda o modo para tanslação
                    mode = 't';
                    break;

            case 's':// muda o modo para scala
                    mode = 's';
                    break;
    }
    glutPostRedisplay();
}

void TeclasEspeciais(int key, int x, int y) {

    if(mode == 't') {

        if(key == GLUT_KEY_UP) {
            translateOffsetY += 2.0;
        }
        if(key == GLUT_KEY_DOWN) {
            translateOffsetY -= 2.0;
        }
        if(key == GLUT_KEY_RIGHT) {
            translateOffsetX += 2.0;
        }
        if(key == GLUT_KEY_LEFT) {
            translateOffsetX -= 2.0;
        }
    } else if(mode == 'r') {

        if(key == GLUT_KEY_RIGHT) {
            tetha += 1.0;
        }
        if(key == GLUT_KEY_LEFT) {
            tetha -= 1.0;
        }
    } else if(mode == 's') {

        if(key == GLUT_KEY_RIGHT) {
            scale += 1.0;
        }
        if(key == GLUT_KEY_LEFT) {
            scale -= 1.0;
        }
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {

    glutInit(&argc,argv);                                     //inicializa o GLUT
    glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB);               //configura o modo de display
    glutInitWindowSize(400,300);                              //configura a largura e altura da janela de exibição
    glutInitWindowPosition(5,310);
    glutCreateWindow("Um programa OpenGL Exemplo");           //cria a janela de exibição
    glutSpecialFunc(TeclasEspeciais);
    glutKeyboardFunc(GerenciaTeclado);
    
    init();                          //executa função de inicialização
    glutDisplayFunc(display);        //estabelece a função "display" como a função callback de exibição.
    glutMainLoop();                  //mostre tudo e espere o usuário fecha-la
    return 0;
}