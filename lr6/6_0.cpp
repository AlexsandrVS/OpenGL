#include "GL/glut.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

const int rows = 6; // Количество строк
const int cols = 6; // Количество столбцов

const int cellSize = 150; // Размер клетки

// Перечисление для цветов
enum CellColor {
    WHITE,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    MASK1,
    MASK2
};

void ApplyMask(CellColor maskColor);

// Структура для представления клетки
struct Cell {
    int x; // Координата x левого верхнего угла клетки
    int y; // Координата y левого верхнего угла клетки
    int width; // Ширина клетки
    int height; // Высота клетки
    CellColor color; // Цвет клетки
};

Cell cells[rows][cols]; // Двумерный массив клеток

GLubyte mask1[24*20] = {
    0x00, 0x00, 0x00, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x0F, 0xFF, 0xF0, 
    0x00, 0x00, 0x00
};

GLubyte mask2[16*12] = {
    0x01, 0xFC, 
    0x03, 0xFE, 
    0x03, 0x7C, 
    0x02, 0x0E, 
    0x03, 0x15, 
    0x03, 0x2B, 
    0x00, 0x21, 
    0x00, 0x2D, 
    0x00, 0x33, 
    0x00, 0x23, 
    0x00, 0x00, 
    0x00, 0x00
};

// Функция отрисовки сцены
void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Затем в функции RenderScene() вы можете использовать:
float randomRed = static_cast<float>(rand()) / RAND_MAX;     // Генерация случайного значения для красного цвета
float randomGreen = static_cast<float>(rand()) / RAND_MAX;   // Генерация случайного значения для зеленого цвета
float randomBlue = static_cast<float>(rand()) / RAND_MAX;    // Генерация случайного значения для синего цвета

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
                case MASK1:
                    // glColor3f(1.0f, 0.0f, 0.0f); // Цвет маски
                    // glEnable(GL_POLYGON_STIPPLE); // Включаем маску
                    // glPolygonStipple(mask1);      // Задаем маску

                    // glBegin(GL_QUADS);            // Начинаем рисовать четырехугольники
                    // glVertex2f(cell.x, cell.y);                   // Вершина 1
                    // glVertex2f(cell.x + cell.width, cell.y);      // Вершина 2
                    // glVertex2f(cell.x + cell.width, cell.y - cell.height); // Вершина 3
                    // glVertex2f(cell.x, cell.y - cell.height);     // Вершина 4
                    // glEnd();                       // Заканчиваем рисовать четырехугольники

                    // glDisable(GL_POLYGON_STIPPLE); // Выключаем маску
                    // break;
                    glColor3f(randomRed, randomGreen, randomBlue);               // Установка случайного цвета
                    // Отрисовываем текст в центре клетки
                    glRasterPos2i(cell.x + cell.width / 2 - 3, cell.y - cell.height / 2 + 2);
                    glBitmap(24, 20, 0, 0, 0, 0, mask1);
                    continue;

                case MASK2:
                    // glColor3f(0.0f, 1.0f, 1.0f); // Цвет маски
                    // glEnable(GL_POLYGON_STIPPLE); // Включаем маску
                    // glPolygonStipple(mask2);      // Задаем маску

                    // glBegin(GL_QUADS);            // Начинаем рисовать четырехугольники
                    // glVertex2f(cell.x, cell.y);                   // Вершина 1
                    // glVertex2f(cell.x + cell.width, cell.y);      // Вершина 2
                    // glVertex2f(cell.x + cell.width, cell.y - cell.height); // Вершина 3
                    // glVertex2f(cell.x, cell.y - cell.height);     // Вершина 4
                    // glEnd();                       // Заканчиваем рисовать четырехугольники

                    // glDisable(GL_POLYGON_STIPPLE); // Выключаем маску
                   glColor3f(randomRed, randomGreen, randomBlue);               // Установка случайного цвета
                    // Отрисовываем текст в центре клетки
                    glRasterPos2i(cell.x + cell.width / 2 - 3, cell.y - cell.height / 2 + 2);
                    glBitmap(16, 12, 0, 0, 0, 0, mask2);
                    continue;
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // цвет фона
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
                            cells[i][j].color = MASK2;
                            break;
                        case MASK2:
                            cells[i][j].color = MASK1;
                            break;
                        case MASK1:
                            cells[i][j].color = WHITE;
                            break;
                    }
                    
                    glutPostRedisplay(); // Перерисовываем сцену после изменения цвета клетки
                    return; // Завершаем функцию после изменения цвета клетки
                }
            }
        }
    }
}

void KeyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'h':
        case 'H':
            ApplyMask(MASK1); // Применить маску MASK1
            break;
        case 'j':
        case 'J':
            ApplyMask(MASK2); // Применить маску MASK2
            break;
    }
}

void ApplyMask(CellColor maskColor) {
    // Находим текущую цветовую маску, чтобы применить ее к клетке
    CellColor currentColorMask = WHITE;
    // Проходим по всем клеткам
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (cells[i][j].color == MASK1 || cells[i][j].color == MASK2) {
                currentColorMask = cells[i][j].color;
                break;
            }
        }
        if (currentColorMask != WHITE) {
            break;
        }
    }
    // Применяем маску к клетке того же цвета, что и текущая маска
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (cells[i][j].color == currentColorMask) {
                cells[i][j].color = maskColor;
            }
        }
    }
    glutPostRedisplay(); // Перерисовываем сцену после применения маски
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(nullptr)));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(cols * cellSize, rows * cellSize);
    glutInitWindowPosition(100, 100); // Позиция окна
    glutCreateWindow("OpenGL");
    glutDisplayFunc(RenderScene);
    glutMouseFunc(MouseFunc);
    glutKeyboardFunc(KeyboardFunc); // Регистрируем обработчик клавиатуры
    SetupRC();
    glutMainLoop();
    return 0;
}