#define STB_IMAGE_IMPLEMENTATION

#include "GL/glew.h"
#include "GL/glut.h"
#include "stb_image.h"
#include <math.h>

static GLfloat zPos = -62.0f;
unsigned int texture[5];

void ProcessMenu(int value)
{
    GLint iLoop;
    for (iLoop = 0; iLoop < 4; iLoop++)
    {
        glBindTexture(GL_TEXTURE_2D, texture[iLoop]);
        switch (value)
        {
        case 0:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
        case 1:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        case 2:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            break;
        case 3:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            break;
        case 4:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            break;
        case 5:
        default:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        }
    }
    glutPostRedisplay();
}

void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    int width, height, cnt;
    glGenTextures(7, texture);

    // Texture 1
    unsigned char* data1 = stbi_load("1.jpg", &width, &height, &cnt, 0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data1);

    // Texture 2
    unsigned char* data2 = stbi_load("2.jpg", &width, &height, &cnt, 0);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data2);

    // Texture 3
    unsigned char* data3 = stbi_load("3.jpg", &width, &height, &cnt, 0);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data3);

    // Texture 4
    unsigned char* data4 = stbi_load("4.jpg", &width, &height, &cnt, 0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data4);

    // Texture 5
    unsigned char* data5 = stbi_load("5.jpg", &width, &height, &cnt, 0);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data5);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data5);

    // Texture 5
    unsigned char* data6 = stbi_load("6.jpg", &width, &height, &cnt, 0);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data6);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data6);

    // Texture 5
    unsigned char* data7 = stbi_load("7.jpg", &width, &height, &cnt, 0);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data7);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data7);
}

void SpecialKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
        zPos += 1.0f;
    if (key == GLUT_KEY_DOWN)
        zPos -= 1.0f;
    glutPostRedisplay();
}

void ChangeSize(int w, int h)
{
    GLfloat fAspect;
    if (h == 0)
        h = 1;
    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Produce the perspective projection
    gluPerspective(90.0f, fAspect, 1, 120);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void RenderScene(void)
{
    GLfloat z;
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, zPos);

    for (z = 50.0f; z > 1.0f; z -= 10)
	{
       
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, -10.0f, z);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(10.0f, 0.0f, z);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(10.0f, 0.0f, z-10.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0.0f, -10.0f, z-10.0f);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, -10.0f, z);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-10.0f, 0.0f, z);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-10.0f, 0.0f, z-10.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0.0f, -10.0f, z-10.0f);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-10.0f, 10.0f, z);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-10.0f, 0.0f, z);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-10.0f, 0.0f, z-10.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-10, 10.0f, z-10.0f);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-10.0f, 10.0f, z);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-0.0f, 10.0f, z);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-0.0f, 10.0f, z-10.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-10, 10.0f, z-10.0f);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, 10.0f, z);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(10.0f, 0.0f, z);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(10.0f, 0.0f, z-10.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0.0f, 10.0f, z-10.0f);
        glEnd();
    }
    glBindTexture(GL_TEXTURE_2D, texture[5]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-10.0f, 10.0f, 30);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-0.0f, 10.0f, 30);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-0.0f, 10.0f, 30-10.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-10, 10.0f, 30-10.0f);
        glEnd();

        // glBindTexture(GL_TEXTURE_2D, texture[5]);
        // glBegin(GL_QUADS);
        // glTexCoord2f(0.0f, 1.0f);
        // glVertex3f(0, -10.0f, 0);
        // glTexCoord2f(1.0f, 1.0f);
        // glVertex3f(10, 0.0f, 0);
        // glTexCoord2f(1.0f, 0.0f);
        // glVertex3f(0, 10.0f, 0);
        // glTexCoord2f(0.0f, 0.0f);
        // glVertex3f(-10, 0.0f, 0);
        // glEnd();

        glBindTexture(GL_TEXTURE_2D, texture[6]);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.2f, 0.0f);
        glVertex3f(0.0f, -10.0f, 0.0f);
        glTexCoord2f(0.0f, 0.75f);
        glVertex3f(10, 0, 0.0f);
        glTexCoord2f(0.5f, 1.0f);
        glVertex3f(0, 10, 0.0f);
        glTexCoord2f(1.0f, 0.75f);
        glVertex3f(-10, 10, 0.0f);
        glTexCoord2f(0.8f, 0.0f);
        glVertex3f(-10, 0, 0.0f); 
        glEnd();


    glPopMatrix();
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("OpenGL");
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(RenderScene);

    // Setup menu
    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("GL_NEAREST", 0);
    glutAddMenuEntry("GL_LINEAR", 1);
    glutAddMenuEntry("GL_NEAREST_MIPMAP_NEAREST", 2);
    glutAddMenuEntry("GL_NEAREST_MIPMAP_LINEAR", 3);
    glutAddMenuEntry("GL_LINEAR_MIPMAP_NEAREST", 4);
    glutAddMenuEntry("GL_LINEAR_MIPMAP_LINEAR", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    SetupRC();
    glutMainLoop();
    return 0;
}
