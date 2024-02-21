#include <GL/glut.h>
#include <algorithm>
#include <vector>
#include <random>

constexpr int initialRows = 5;
constexpr int initialCols = 7;
constexpr int cellSize = 50;
int rows = initialRows;
int cols = initialCols;

enum CellColor {
    WHITE,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    NUM_COLORS 
};

struct Cell {
    int x;
    int y;
    int size;
    bool clicked;
    CellColor color;
};

std::vector<std::vector<Cell>> cells;

void SetupRC() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
    glMatrixMode(GL_MODELVIEW);
}

void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            switch (cells[i][j].color) {
                case WHITE:
                    glColor3f(1.0f, 1.0f, 1.0f);
                    break;
                case RED:
                    glColor3f(1.0f, 0.0f, 0.0f);
                    break;
                case GREEN:
                    glColor3f(0.0f, 1.0f, 0.0f);
                    break;
                case BLUE:
                    glColor3f(0.0f, 0.0f, 1.0f);
                    break;
                case YELLOW:
                    glColor3f(1.0f, 1.0f, 0.0f);
                    break;
            }
            glBegin(GL_QUADS);
            glVertex2i(cells[i][j].x, cells[i][j].y);
            glVertex2i(cells[i][j].x + cells[i][j].size, cells[i][j].y);
            glVertex2i(cells[i][j].x + cells[i][j].size, cells[i][j].y + cells[i][j].size);
            glVertex2i(cells[i][j].x, cells[i][j].y + cells[i][j].size);
            glEnd();
        }
    }

    glFlush();
}

void Resize(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    rows = height / cellSize;
    cols = width / cellSize;

    cells.resize(rows);
    for (int i = 0; i < rows; ++i) {
        cells[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            cells[i][j].x = j * cellSize;
            cells[i][j].y = i * cellSize;
            cells[i][j].size = cellSize;
            cells[i][j].clicked = false;

            // Генерируем случайный цвет для каждой клетки
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0, CellColor::NUM_COLORS - 1);
            cells[i][j].color = static_cast<CellColor>(dis(gen));
        }
    }
}

void MouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int row = y / cellSize;
        int col = x / cellSize;
        cells[row][col].clicked = !cells[row][col].clicked;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Resizable Grid");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(Resize);
    glutMouseFunc(MouseFunc);
    SetupRC();
    Resize(400, 300); // Вызываем Resize в начале для создания сетки с размерами окна по умолчанию
    glutMainLoop();
    return 0;
}
