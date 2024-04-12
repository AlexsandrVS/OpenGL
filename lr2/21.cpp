#include <GL/glut.h>
#include <cmath>

GLuint maskTextureID;

void createMaskTexture() {
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &maskTextureID);
    glBindTexture(GL_TEXTURE_2D, maskTextureID);

    // Черный круг с белым фоном
    const int texSize = 100;
    unsigned char texData[texSize * texSize * 4];
    for (int y = 0; y < texSize; ++y) {
        for (int x = 0; x < texSize; ++x) {
            int offset = (y * texSize + x) * 4;
            float distance = sqrt((x - texSize / 2) * (x - texSize / 2) + (y - texSize / 2) * (y - texSize / 2));
            if (distance <= texSize / 2) {
                texData[offset + 0] = 255;  // R
                texData[offset + 1] = 255;  // G
                texData[offset + 2] = 255;  // B
                texData[offset + 3] = 255;  // A
            } else {
                texData[offset + 0] = 0;
                texData[offset + 1] = 0;
                texData[offset + 2] = 0;
                texData[offset + 3] = 0;
            }
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSize, texSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glDisable(GL_TEXTURE_2D);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Нарисовать фон (прямоугольник на весь экран)
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, maskTextureID);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);  // Левый нижний угол
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);   // Правый нижний угол
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);    // Правый верхний угол
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);   // Левый верхний угол
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // Нарисовать маску (круг)
    createMaskTexture();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Mask Texture");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Черный цвет фона

    glutDisplayFunc(display);

    createMaskTexture(); // Создаем маску

    glutMainLoop();
    return 0;
}
