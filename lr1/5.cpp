#include "GL/glut.h"

// Функция отрисовки сцены
void RenderScene(void) {
    // Очищаем окно зеленым цветом
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Принудительно выводим все команды в буфер OpenGL
    glFlush();
}
 
void SetupRC(void) {
    // Устанавливаем цвет очистки окна в зеленый
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei w, GLsizei h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Устанавливаем ортографическую проекцию с учетом новых размеров окна
    glOrtho(0.0f, w, 0.0f, h, -1.0f, 1.0f);
    // Устанавливаем текущую матрицу в модельно-видовую
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    // Инициализация GLUT и создание окна
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    // Устанавливаем позицию окна и его размеры
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 300);
    glutCreateWindow("q5");
    // Устанавливаем функцию отрисовки
    glutDisplayFunc(RenderScene);
    // Устанавливаем функцию изменения размеров окна
    glutReshapeFunc(ChangeSize);
    // Устанавливаем начальное состояние OpenGL
    SetupRC();
    // Запускаем основной цикл GLUT
    glutMainLoop();

    return 0;
}
