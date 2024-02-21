#include "GL/glut.h"

// Преобразуем программу. В центре окна с размерами 400 × 400 изобразим красный прямоугольник.//

// Функция для отрисовки сцены
void RenderScene(void) {
    // Очищаем буфер цвета текущим цветом очистки
    glClear(GL_COLOR_BUFFER_BIT);
    // Устанавливаем красный цвет для прямоугольника
    glColor3f(1.0f, 0.0f, 0.0f);
    // Рисуем прямоугольник в центре окна
    glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
    // Принудительно выводим все команды в буфер OpenGL
    glFlush();
}

// Устанавливаем состояние визуализации
void SetupRC(void) {
    // Устанавливаем цвет очистки в синий
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

// Точка входа основной программы
int main(int argc, char** argv) {
    // Инициализация GLUT и создание окна
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Simple2");

    // Устанавливаем функцию обратного вызова для отрисовки сцены
    glutDisplayFunc(RenderScene);
    // Устанавливаем начальное состояние визуализации
    SetupRC();
    // Запускаем главный цикл GLUT
    glutMainLoop();

    return 0;
}