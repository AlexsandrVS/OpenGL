#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "tga.h" // Предполагается, что tga.h и tga.cpp включены в проект

GLuint textures[2];

void CreateTexture(GLuint textureArray[], const char* strFileName, int textureID) {
    if (!strFileName)
        return;

    tImageTGA* pImage = LoadTGA(strFileName);

    if (pImage == NULL)
        exit(0);

    glGenTextures(1, &textureArray[textureID]);
    glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

    int textureType = GL_RGB;
    if (pImage->channels == 4)
        textureType = GL_RGBA;

    gluBuild2DMipmaps(GL_TEXTURE_2D, pImage->channels, pImage->sizeX,
                      pImage->sizeY, textureType, GL_UNSIGNED_BYTE, pImage->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    if (pImage) {
        if (pImage->data) {
            free(pImage->data);
        }
        free(pImage);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);

    // Отображение первой текстуры в левом верхнем углу
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex2f(1.0,  0.0);
        glTexCoord2f(1.0, 1.0); glVertex2f( 0.0,  0.0);
        glTexCoord2f(1.0, 0.0); glVertex2f( 0.0, -1.0);
        glTexCoord2f(0.0, 0.0); glVertex2f(1.0, -1.0);
    glEnd();

    // Отображение второй текстуры в правом нижнем углу
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex2f( 0.0,  1.0);
        glTexCoord2f(1.0, 1.0); glVertex2f( -1.0,  1.0);
        glTexCoord2f(1.0, 0.0); glVertex2f( -1.0,  0.0);
        glTexCoord2f(0.0, 0.0); glVertex2f( 0.0,  0.0);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // Отображение синего квадрата в центре
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
        glVertex2f(-0.1, -0.1);
        glVertex2f( 0.1, -0.1);
        glVertex2f( 0.1,  0.1);
        glVertex2f(-0.1,  0.1);
    glEnd();

    glutSwapBuffers();
}

void init() {
    CreateTexture(textures, "11.tga", 1);
    CreateTexture(textures, "22.tga", 0);

    glClearColor(1.0, 1.0, 1.0, 1.0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("OpenGL");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();
    return 0;
}
