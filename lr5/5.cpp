#include "GL/glew.h"
#include "GL/glut.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>

unsigned int textures[4];

float vertex[] = { -1, -1, 0, 1, -1, 0, 1, 1, 0, -1, 1, 0 };
float texCoord[] = { 0, 0, 1, 0, 1, 1, 0, 1 };

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    // 1st texture
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glPushMatrix();
    glTranslatef(-0.5f, 0.5f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();

    // 2nd texture
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glPushMatrix();
    glTranslatef(0.5f, 0.5f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();

    // 3rd texture
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glPushMatrix();
    glTranslatef(-0.5f, -0.5f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();

    // 4th texture
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glPushMatrix();
    glTranslatef(0.5f, -0.5f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();

    glutSwapBuffers();
}

void Text_Init() {
    // procedure to create texture
    int width, height;
    width = 2;
    height = 2;
    // create a 2x2 array of texels
    struct { unsigned char r, g, b, a; } data[2][2];
    memset(data, 0, sizeof(data));

    // Generate texture names. 
    glGenTextures(4, textures);
    for (int i = 0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        if (i == 0) {
            data[0][0].r = 255;
            data[0][0].b = 144;
            data[1][0].g = 100;
            data[0][1].g = 100;
            data[1][1].r = 144;
            data[1][1].b = 255;
        } else if (i == 1) {
            data[0][0].r = 100;
            data[0][0].b = 0;
            data[1][0].g = 200;
            data[0][1].g = 200;
            data[1][1].r = 100;
            data[1][1].b = 0;
        } else if (i == 2) {
            data[0][0].r = 55;
            data[0][0].b = 30;
            data[1][0].g = 25;
            data[0][1].g = 25;
            data[1][1].r = 55;
            data[1][1].b = 30;
        } else if (i == 3) {
            data[0][0].r = 200;
            data[0][0].b = 60;
            data[1][0].g = 190;
            data[0][1].g = 190;
            data[1][1].r = 200;
            data[1][1].b = 60;
        }

        // Basic texture settings
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    // Disable active texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void menu(int value) {
    std::cout << "Menu option selected: " << value << std::endl;
    
    if (value >= 1 && value <= 4) {
        for (int i = 0; i < 4; i++) {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            switch (value) {
                case 1:
                    std::cout << "Setting texture parameter GL_TEXTURE_MAG_FILTER to GL_LINEAR" << std::endl;
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    break;
                case 2:
                    std::cout << "Setting texture parameter GL_TEXTURE_WRAP_T to GL_CLAMP" << std::endl;
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                    break;
                case 3:
                    std::cout << "Setting texture parameter GL_TEXTURE_WRAP_S to GL_CLAMP" << std::endl;
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                    break;
                case 4:
                    std::cout << "Setting texture parameter GL_TEXTURE_MIN_FILTER to GL_LINEAR" << std::endl;
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    break;
            }
        }
    }
    glutPostRedisplay();
}


void createMenu() {
    glutCreateMenu(menu);
    glutAddMenuEntry("Parameter 1", 1);
    glutAddMenuEntry("Parameter 2", 2);
    glutAddMenuEntry("Parameter 3", 3);
    glutAddMenuEntry("Parameter 4", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("OpenGL");
    
    Text_Init();
    createMenu();
    glutDisplayFunc(display);
    glutMainLoop();
}