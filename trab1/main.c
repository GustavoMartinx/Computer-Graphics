#include <GL/glut.h>

char mode = 't';
GLfloat tetha = 0.0;
GLfloat translateOffsetX = 5.0;
GLfloat translateOffsetY = 5.0;
GLfloat scale = 1;


int init(void){
    glClearColor(0.0, 0.0, 0.0, 0.0);     //define a cor de fundo

    glMatrixMode(GL_PROJECTION);          //carrega a matriz de projeção
    gluOrtho2D(0.0,100.0,0.0,100.0);      //define projeção ortogonal 2D

}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);         //desenha o fundo (limpa a janela)
    
    glPushMatrix();
    glTranslatef(translateOffsetX, translateOffsetY, 0.0);
    glScalef(scale, scale, 1);
    glRotatef(tetha, 0.0, 0.0, 1);
    glColor3f(0.0,0.0,1.0);               //altera o atributo de cor
    glBegin(GL_QUADS);                    //desenha um quadrado
        glVertex2i(-5,-5);
        glVertex2i(-5, 5);
        glVertex2i(5, 5);
        glVertex2i(5, -5);
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
        else if(key == GLUT_KEY_DOWN) {
            translateOffsetY -= 2.0;
        }
        else if(key == GLUT_KEY_RIGHT) {
            translateOffsetX += 2.0;
        }
        else if(key == GLUT_KEY_LEFT) {
            translateOffsetX -= 2.0;
        }
    } else if(mode == 'r') {

        if(key == GLUT_KEY_RIGHT) {
            tetha += 1.0;
        }
        else if(key == GLUT_KEY_LEFT) {
            tetha -= 1.0;
        }
    } else if(mode == 's') {

        if(key == GLUT_KEY_UP) {
            scale += 1;
        }
        else if(key == GLUT_KEY_DOWN) {
            scale -= 1;
        }
        if(scale < 0) scale = 0.1
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {

    glutInit(&argc,argv);                                     //inicializa o GLUT
    glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB);               //configura o modo de display
    glutInitWindowSize(500,500);                              //configura a largura e altura da janela de exibição
    glutInitWindowPosition(100,310);
    glutCreateWindow("Atividade Prática 1");           //cria a janela de exibição
    glutSpecialFunc(TeclasEspeciais);
    glutKeyboardFunc(GerenciaTeclado);
    
    init();                          //executa função de inicialização
    glutDisplayFunc(display);        //estabelece a função "display" como a função callback de exibição.
    glutMainLoop();                  //mostre tudo e espere o usuário fecha-la
    return 0;
}