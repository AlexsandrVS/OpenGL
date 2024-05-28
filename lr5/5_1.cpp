#define STB_IMAGE_IMPLEMENTATION
#include "GL/glew.h"
#include "GL/glut.h"
#include "stb_image.h"
#include <vector>
#include <cmath>

unsigned int texture;

void generateCylinder(float radius, float height, int numSegments, std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<unsigned int>& indices) {
    float angleIncrement = 2.0f * M_PI / numSegments;

    // Нижняя крышка
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
    for (int i = 0; i < numSegments; ++i) {
        int nextIndex = (i + 1) % numSegments;

        // Верхний треугольник
        indices.push_back(i);
        indices.push_back(i + numSegments);
        indices.push_back(nextIndex + numSegments);

        // Нижний треугольник
        indices.push_back(i);
        indices.push_back(nextIndex + numSegments);
        indices.push_back(nextIndex);
    }

    // Верхняя крышка
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

     // Индексы для верхней и нижней крышек
    int baseIndex = vertices.size() / 3 - 2 * (numSegments + 1);
    for (int i = 1; i < numSegments; ++i) {
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + i);
        indices.push_back(baseIndex + i + 1);
    }
    indices.push_back(baseIndex);
    indices.push_back(baseIndex + numSegments);
    indices.push_back(baseIndex + 1);

    baseIndex += numSegments + 1;
    for (int i = 1; i < numSegments; ++i) {
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + i + 1);
        indices.push_back(baseIndex + i);
    }
    indices.push_back(baseIndex);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + numSegments);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1, 1, 1);
    glPushMatrix();
    glRotatef(60, 0, 1, 0); // Поворот на 60 градусов вокруг оси y
    glRotatef(45, 1, 0, 0); // Поворот на 45 градусов вокруг оси x

    std::vector<float> vertices;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    generateCylinder(0.5f, 1.0f, 100, vertices, texCoords, indices);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords.data());
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
    glutSwapBuffers();
}

void Game_Init() {
    int width, height, cnt;
    // Загрузим данные картинки
    unsigned char* data = stbi_load("1.jpg", &width, &height, &cnt, 0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("OpenGL");
    
    Game_Init();
    glutDisplayFunc(display);
    glutMainLoop();
}
