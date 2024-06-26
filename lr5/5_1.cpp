#define STB_IMAGE_IMPLEMENTATION
#include "GL/glew.h"
#include "GL/glut.h"
#include "stb_image.h"
#include <vector>
#include <cmath>

unsigned int textureSide; // Текстура для боковой поверхности
int numSegments = 100;    // Глобальная переменная для количества сегментов

float rotateX = 45.0f;    // Угол поворота вокруг оси X
float rotateY = 60.0f;    // Угол поворота вокруг оси Y

void generateCylinder(float radius, float height, int numSegments, std::vector<float>& vertices, std::vector<float>& texCoords) {
    float angleIncrement = 2.0f * M_PI / numSegments;

    // Нижняя крышка
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-height / 2);

    texCoords.push_back(0.5f);
    texCoords.push_back(0.5f);

    for (int i = 0; i <= numSegments; ++i) {
        float angle = angleIncrement * i;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(-height / 2);

        texCoords.push_back(0.5f + 0.5f * cosf(angle));
        texCoords.push_back(0.5f + 0.5f * sinf(angle));
    }

    // Боковая поверхность
    for (int i = 0; i <= numSegments; ++i) {
        float angle = angleIncrement * i;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);

        // Нижняя часть
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(-height / 2);

        texCoords.push_back(static_cast<float>(i) / numSegments);
        texCoords.push_back(0.0f);

        // Верхняя часть
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(height / 2);

        texCoords.push_back(static_cast<float>(i) / numSegments);
        texCoords.push_back(1.0f);
    }

    // Верхняя крышка
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(height / 2);

    texCoords.push_back(0.5f);
    texCoords.push_back(0.5f);

    for (int i = 0; i <= numSegments; ++i) {
        float angle = angleIncrement * i;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(height / 2);

        texCoords.push_back(0.5f + 0.5f * cosf(angle));
        texCoords.push_back(0.5f + 0.5f * sinf(angle));
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    // Применяем текстуру для боковой поверхности
    glBindTexture(GL_TEXTURE_2D, textureSide);
    glColor3f(1, 1, 1);

    std::vector<float> vertices;
    std::vector<float> texCoords;
    generateCylinder(0.5f, 1.0f, numSegments, vertices, texCoords);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords.data());

    // Поворот цилиндра
    glPushMatrix();
    glRotatef(rotateY, 0, 1, 0); // Поворот вокруг оси y
    glRotatef(rotateX, 1, 0, 0); // Поворот вокруг оси x

    // Отрисовка нижней крышки цилиндра
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2);

    // Отрисовка боковой поверхности цилиндра
    glDrawArrays(GL_TRIANGLE_STRIP, numSegments + 2, (numSegments + 1) * 2);

    // Применяем текстуру для верхней крышки
    glBindTexture(GL_TEXTURE_2D, textureSide);

    // Отрисовка верхней крышки цилиндра
    glDrawArrays(GL_TRIANGLE_FAN, (numSegments + 1) * 2 + numSegments + 2, numSegments + 2);

    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glutSwapBuffers();
}

void Game_Init() {
    int width, height, cnt;

    unsigned char* data = stbi_load("1.jpg", &width, &height, &cnt, 0);
    glGenTextures(1, &textureSide);
    glBindTexture(GL_TEXTURE_2D, textureSide);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            rotateX -= 5.0f;
            break;
        case 's':
            rotateX += 5.0f;
            break;
        case 'a':
            rotateY -= 5.0f;
            break;
        case 'd':
            rotateY += 5.0f;
            break;
    }
    glutPostRedisplay(); // Обновить экран
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("OpenGL");

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    Game_Init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard); // Установка функции обработки клавиатуры
    glutMainLoop();
}