#include "GL/glut.h"

// Переменные для хранения текущих координат курсора мыши
int mouseX = 0;
int mouseY = 0;

// Функция отрисовки сцены
void RenderScene(void) {
    GLfloat r = (GLfloat)mouseX / glutGet(GLUT_WINDOW_WIDTH);
    GLfloat g = (GLfloat)mouseY / glutGet(GLUT_WINDOW_HEIGHT);
    GLfloat b = 1.0f - r;

    // Очищаем окно новым цветом
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Принудительно выводим все команды в буфер OpenGL
    glFlush();
}

// Функция для отслеживания перемещения курсора мыши
void OnMouseMove(int x, int y) {
    mouseX = x;
    mouseY = glutGet(GLUT_WINDOW_HEIGHT) - y; // Инвертируем координаты y
    glutPostRedisplay(); // Вызываем перерисовку окна
}

// Устанавливаем начальное состояние OpenGL
void SetupRC(void) {
    // Устанавливаем цвет очистки окна в черный
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Функция изменения размеров окна
void ChangeSize(GLsizei w, GLsizei h) {
    // Обновляем матрицу проекции
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
    glutInitWindowSize(800, 600); // Задаем размеры окна
    glutCreateWindow("Rainbow Window");
    // Устанавливаем функцию отрисовки
    glutDisplayFunc(RenderScene);
    // Устанавливаем функцию изменения размеров окна
    glutReshapeFunc(ChangeSize);
    // Устанавливаем функцию отслеживания перемещения курсора мыши
    glutPassiveMotionFunc(OnMouseMove);
    // Устанавливаем начальное состояние OpenGL
    SetupRC();
    // Запускаем основной цикл GLUT
    glutMainLoop();

    return 0;
}
