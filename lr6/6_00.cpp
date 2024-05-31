#include "GL/glut.h"
// Растровый образ костра
GLubyte mask1[72] = {
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0x00, 0xFF, 0x00, 
    0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00,
};

//////////////////////////////////////////////////////////////////
// Функция выполняет всю необходимую инициализацию в контексте визуализации
void SetupRC() {
    // Черный фон
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
//////////////////////////////////////////////////////////
// Устанавливается система координат, согласованная с координатами
void ChangeSize(int w, int h) {
    // Предотвращает деление на нуль, когда окно слишком маленькое
    // (нельзя сделать окно нулевой ширины).
    if (h == 0)
        h = 1;
    glViewport(0, 0, w, h);
    // Система координат обновляется перед модификацией
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Псевдокоординаты окна
    gluOrtho2D(0.0, (GLfloat)w, 0.0f, (GLfloat)h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
/////////////////////////////////////////////////////////////////
// Вызывается для рисования сцены
void RenderScene(void) {
    int x = 0, y = 0;
    // Очищаем окно текущим цветом очистки
    glClear(GL_COLOR_BUFFER_BIT);
    // Устанавливается белый цвет
    glColor3f(1.0f, 1.0f, 1.0f);
    // Цикл из 16 строк и столбцов
    for (y = 0; y < 12; y++) {
        // Устанавливается растровое полож
// Устанавливается растровое положение данного "квадрата”
        glRasterPos2i(0, y * 24);
        for (x = 0; x < 12; x++)
        {
            glBitmap(24, 24, 0.0, 0.0, 24.0, 0.0, mask1);
        }
    }
    // Переключает буферы
    glutSwapBuffers();
}
/////////////////////////////////////////////////////////////
// Точка входа основной программы
int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(512, 512);
    glutCreateWindow("OpenGL Bitmaps");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    SetupRC();
    glutMainLoop();
    return 0;
}
