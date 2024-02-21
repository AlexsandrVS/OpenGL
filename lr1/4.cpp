#include "GL/glut.h"

void RenderScene(void) {
    // Очищаем окно, используя текущий цвет очистки
    glClear(GL_COLOR_BUFFER_BIT);
    // В качестве текущего цвета рисования задает белый
    glColor3f(1.0f, 1.0f, 1.0f);
    // Рисует прямоугольник, закрашенный текущим цветом
    glRectf(-25.0f, 25.0f, 25.0f, -25.0f);
    // Очищает очередь текущих команд
    glFlush();
}

void SetupRC(void) {
    // Устанавливает в качестве цвета очистки розовый
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
}

void ChangeSize(GLsizei w, GLsizei h)
{
    GLfloat aspectRatio;
    // Предотвращает деление на нуль
    if (h == 0)
    h = 1;
    // Устанавливает поле просмотра с размерами окна
    glViewport(0, 0, w, h);
    // Обновляет систему координат
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    aspectRatio = (GLfloat)w / (GLfloat)h; if (w <= h)
    // Установка двумерной ортографической системы координат
    glOrtho(-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio,
    1.0, -1.0);
    else
    glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0,
    1.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("GLRect");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    SetupRC();
    glutMainLoop();
}