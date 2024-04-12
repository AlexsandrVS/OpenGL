#include "GL/glut.h"
#include "GL/gl.h"

GLfloat x1 = 0.0f;
GLfloat y1 = 0.0f;
GLfloat rsize = 40;
GLfloat scale = 1.0f;
GLfloat scaleCoef = 0.01f;

void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(x1, y1, 0.0f); // Переносим начало координат
    glTranslatef(rsize, -rsize, 0.0f); // Перемещаем начало координат в верхнюю правую вершину квадрата
    glScalef(scale, scale, 1.0f); // Масштабируем квадрат
    glTranslatef(-rsize, rsize, 0.0f); // Возвращаем начало координат обратно
    glBegin(GL_QUADS); // Начинаем определение последовательности вершин для квадрата
    glVertex2f(-rsize, -rsize);
    glVertex2f(rsize, -rsize); 
    glVertex2f(rsize, rsize);  
    glVertex2f(-rsize, rsize);
    glEnd();
    glPopMatrix();
    glutSwapBuffers();
}

void TimerFunction(int value)
{
    scale += scaleCoef;
    if (scale > 1.5f || scale < 1.0f) { // Если масштаб больше 1.5 или меньше 1.0, меняем направление масштабирования
        scaleCoef = -scaleCoef;
    }
    glutPostRedisplay();
    glutTimerFunc(10, TimerFunction, 1); // Устанавливаем таймер с интервалом 10 миллисекунд
}

void ChangeSize(GLsizei w, GLsizei h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) {
        gluOrtho2D(-100.0, 100.0, -100.0 * (GLfloat)h / (GLfloat)w, 100.0 * (GLfloat)h / (GLfloat)w);
    }
    else {
        gluOrtho2D(-100.0 * (GLfloat)w / (GLfloat)h, 100.0 * (GLfloat)w / (GLfloat)h, -100.0, 100.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Pulsating Square");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(50, TimerFunction, 1); // Устанавливаем таймер с интервалом 50 миллисекунд
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Устанавливаем цвет фона
    glutMainLoop();
    return 0;
}
