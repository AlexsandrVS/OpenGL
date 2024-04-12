#include "GL/glut.h"
#include "GL/gl.h"

double rotate_y = 0;
double rotate_x = 0;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Поворачиваем на угол
	glRotatef(rotate_x, 1, 0, 0);
	glRotatef(rotate_y, 0, 1, 0);

	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.5, 0.0);
	glVertex3f(0.0, 0.5, 0.0);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.5, 1.0);
	glVertex3f(0.0, 0.5, 0.0);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.5, 0.0);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1, 1, 0);
	glVertex3f(0.0, 0.5, 0.0);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glEnd();
	
	glutSwapBuffers();
}


void TimerFunction(int value) {
    glRotated(270,0,0,1);
    rotate_x = 0;
    rotate_y = 0;
    glutPostRedisplay();  // Пометка окна на перерисовку
    // glutTimerFunc(50, TimerFunction, 0); // 10 милисикунд
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);  // Инициализация GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(500, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Cube"); 
    glEnable(GL_DEPTH_TEST);  // Включение буфера глубины для правильного отображения глубины объектов
    glutDisplayFunc(display);
    glutTimerFunc(50, TimerFunction, 0);  // Установка функции таймера
    glutMainLoop();
    return 0;
}