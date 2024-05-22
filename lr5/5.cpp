#include "GL/glew.h"
#include "GL/glut.h"
#include <cstring>
#include <iostream>

unsigned int texture;
float vertex[] = { -1, -1, 0,  1, -1, 0,  1, 1, 0,  -1, 1, 0 };
float texCoord[] = { 0, 0,  1, 0,  1, 1,  0, 1 };

// Default texture parameters
GLint wrapS = GL_REPEAT;
GLint wrapT = GL_REPEAT;
GLint minFilter = GL_NEAREST;
GLint magFilter = GL_NEAREST;

void applyTextureParameters() {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glColor3f(1, 1, 1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoord);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void Text_Init() {
    int width = 2, height = 2;

    struct { unsigned char r, g, b, a; } data[2][2];
    memset(data, 0, sizeof(data));

    // Set texture colors with visible alpha channel
    data[0][0].r = 255; data[0][0].g = 0;   data[0][0].b = 0;   data[0][0].a = 255;
    data[0][1].r = 0;   data[0][1].g = 255; data[0][1].b = 0;   data[0][1].a = 255;
    data[1][0].r = 0;   data[1][0].g = 0;   data[1][0].b = 255; data[1][0].a = 255;
    data[1][1].r = 255; data[1][1].g = 255; data[1][1].b = 0;   data[1][1].a = 255;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    applyTextureParameters();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void menu(int option) {
    switch (option) {
        case 1: wrapS = GL_REPEAT; break;
        case 2: wrapS = GL_CLAMP; break;
        case 3: wrapT = GL_REPEAT; break;
        case 4: wrapT = GL_CLAMP; break;
        case 5: minFilter = GL_NEAREST; break;
        case 6: minFilter = GL_LINEAR; break;
        case 7: magFilter = GL_NEAREST; break;
        case 8: magFilter = GL_LINEAR; break;
    }
    applyTextureParameters();
    glutPostRedisplay();
}

void createMenu() {
    int wrapSMenu = glutCreateMenu(menu);
    glutAddMenuEntry("GL_REPEAT", 1);
    glutAddMenuEntry("GL_CLAMP", 2);

    int wrapTMenu = glutCreateMenu(menu);
    glutAddMenuEntry("GL_REPEAT", 3);
    glutAddMenuEntry("GL_CLAMP", 4);

    int minFilterMenu = glutCreateMenu(menu);
    glutAddMenuEntry("GL_NEAREST", 5);
    glutAddMenuEntry("GL_LINEAR", 6);

    int magFilterMenu = glutCreateMenu(menu);
    glutAddMenuEntry("GL_NEAREST", 7);
    glutAddMenuEntry("GL_LINEAR", 8);

    glutCreateMenu(menu);
    glutAddSubMenu("Wrap S", wrapSMenu);
    glutAddSubMenu("Wrap T", wrapTMenu);
    glutAddSubMenu("Min Filter", minFilterMenu);
    glutAddSubMenu("Mag Filter", magFilterMenu);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Texture Parameters Menu");

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    Text_Init();
    glutDisplayFunc(display);
    createMenu();
    glutMainLoop();
    return 0;
}
