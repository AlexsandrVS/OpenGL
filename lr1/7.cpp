#include "GL/glut.h"
#include <iostream>

const int rows = 6; // Количество строк
const int cols = 6; // Количество столбцов

const int cellSize = 150; // Размер клетки

// Перечисление для цветов
enum CellColor {
    WHITE,
    RED,
    GREEN,
    BLUE,
    YELLOW
};

// Структура для представления клетки
struct Cell {
    int x; // Координата x левого верхнего угла клетки
    int y; // Координата y левого верхнего угла клетки
    int width; // Ширина клетки
    int height; // Высота клетки
    CellColor color; // Цвет клетки
};

Cell cells[rows][cols]; // Двумерный массив клеток

// Функция отрисовки сцены
void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Отрисовываем клетки
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Cell cell = cells[i][j];
            switch (cell.color) {
                case WHITE:
                    glColor3f(1.0f, 1.0f, 1.0f); // Белый цвет
                    break;
                case RED:
                    glColor3f(1.0f, 0.0f, 0.0f); // Красный цвет
                    break;
                case GREEN:
                    glColor3f(0.0f, 1.0f, 0.0f); // Зеленый цвет
                    break;
                case BLUE:
                    glColor3f(0.0f, 0.0f, 1.0f); // Синий цвет
                    break;
                case YELLOW:
                    glColor3f(1.0f, 1.0f, 0.0f); // Желтый цвет
                    break;
            }
            glBegin(GL_QUADS);
            glVertex2i(cell.x, cell.y);
            glVertex2i(cell.x + cell.width, cell.y);
            glVertex2i(cell.x + cell.width, cell.y - cell.height);
            glVertex2i(cell.x, cell.y - cell.height);
            glEnd();
        }
    }

    glFlush();
}

// Устанавливаем начальное состояние OpenGL
void SetupRC(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Черный цвет фона
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Инициализируем координаты и размеры каждой клетки
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cells[i][j].x = j * cellSize;
            cells[i][j].y = glutGet(GLUT_WINDOW_HEIGHT) - i * cellSize;
            cells[i][j].width = cellSize;
            cells[i][j].height = cellSize;
            cells[i][j].color = WHITE; // По умолчанию клетка белая
        }
    }
}

// Функция обработки событий мыши
void MouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Преобразуем координаты экрана в координаты окна
        y = glutGet(GLUT_WINDOW_HEIGHT) - y;
        // Проверяем, на какую клетку кликнули
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (x >= cells[i][j].x && x <= cells[i][j].x + cells[i][j].width &&
                    y >= cells[i][j].y - cells[i][j].height && y <= cells[i][j].y) {
                    // Меняем цвет клетки
                    switch (cells[i][j].color) {
                        case WHITE:
                            cells[i][j].color = RED;
                            break;
                        case RED:
                            cells[i][j].color = GREEN;
                            break;
                        case GREEN:
                            cells[i][j].color = BLUE;
                            break;
                        case BLUE:
                            cells[i][j].color = YELLOW;
                            break;
                        case YELLOW:
                            cells[i][j].color = WHITE;
                            break;
                    }
                    glutPostRedisplay(); // Перерисовываем сцену
                    return;
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(cols * cellSize, rows * cellSize);
    glutInitWindowPosition(100, 100); // Позиция окна
    glutCreateWindow("Clickable");
    glutDisplayFunc(RenderScene);
    glutMouseFunc(MouseFunc);
    SetupRC();
    glutMainLoop();
    return 0;
}
