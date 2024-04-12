#include "GL/glut.h"
#include "math.h"
#include <GL/gl.h>

void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    GLubyte fire[128] = {   0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x0F, 0xFF, 0xFF, 0xF0,
                            0x0F, 0xFF, 0xFF, 0xF0,
                            0x0F, 0xFF, 0xFF, 0xF0,
                            0x0F, 0xFF, 0xFF, 0xF0,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0xFF, 0xFF, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00 };

    glBegin(GL_QUADS);	
    glColor3f(1.0, 0.6, 0.0);
    glVertex2f(-200.f, -25.f);
    glVertex2f(200.0f, -25.0f);
    glColor3f(1.f, 0.f, 1.f);
    glVertex2f(200.f, 100.f);
    glVertex2f(-200.f, 100.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.707f, 0.f, 0.707f);
    glVertex2f(0.f, 0.f);
    glVertex2f(0.f, 0.f);
    glColor3f(0.05f, 0.704f, 0.606f);
    glVertex2f(100.f, 100.f);
    glVertex2f(-100.f, 100.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-10, 5, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-65, 5, 1.0);
    glColor3f(0.0, 0, 1.0);
    glVertex3f(-30, 20, 1.0);
    glEnd();


    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-15, -5, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-75, -5, 1.0);
    glColor3f(0.0, 0, 1.0);
    glVertex3f(-45, 30, 1.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-20, -15, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-110, -15, 1.0);
    glColor3f(0.0, 0, 1.0);
    glVertex3f(-65, 40, 1.0);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(fire);
    glBegin(GL_TRIANGLES);
    // glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-30, -25, 0.0);
    // glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-140, -25, 1.0);
    // glColor3f(0.0, 0, 1.0);
    glVertex3f(-85, 50, 1.0);
    glEnd();
    glDisable(GL_POLYGON_STIPPLE);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(10, 5, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(65, 5, 1.0);
    glColor3f(0.0, 0, 1.0);
    glVertex3f(30, 20, 1.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(15, -5, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(75, -5, 1.0);
    glColor3f(0.0, 0, 1.0);
    glVertex3f(45, 30, 1.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(20, -15, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(110, -15, 1.0);
    glColor3f(0.0, 0, 1.0);
    glVertex3f(65, 40, 1.0);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(fire);
    glBegin(GL_TRIANGLES);
    // glColor3f(1.0, 0.0, 0.0);
    glVertex3f(30, -25, 0.0);
    // glColor3f(0.0, 1.0, 0.0);
    glVertex3f(140, -25, 1.0);
    // glColor3f(0.0, 0, 1.0);
    glVertex3f(85, 50, 1.0);
    glEnd();
    glDisable(GL_POLYGON_STIPPLE);

    glBegin(GL_QUADS);
    glColor3f(0.707f, 0.f, 0.707f);
    glVertex2f(0.f, 0.f);
    glVertex2f(0.f, 0.f);
    glColor3f(0.05f, 0.704f, 0.606f);
    glVertex2f(-50.f, -25.f);
    glVertex2f(50.f, -25.0f);
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float red = fabs(cos((i + 120) * 3.1415926 / 180));
        float green = fabs(cos((i + 240) * 3.1415926 / 180));
        float blue = fabs(cos(i * 3.1415926 / 180));
        
        glColor3f(red, green, blue);
        float angle = i * 3.1415926 / 180;
        float x = 25.0 * cos(angle) + 0.0;
        float y = 25.0 * sin(angle) + 20;
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 0.5, 0.3);
    glVertex2f(-200.f, -25.f);
    glVertex2f(200.0f, -25.0f);
    glColor3f(0.05f, 0.704f, 0.606f);
    glVertex2f(200.f, -100.f);
    glVertex2f(-200.f, -100.0f);
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.707f, 0.1f, 0.707f);
    glVertex3f(-140.f, -100.f, 0.f);
    glVertex3f(140.f, -100.f, 0.f);
    glColor3f(0.05f, 0.704f, 0.606f);
    glVertex3f(-50, -25, 0);
    glVertex3f(50, -25, 0);
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.707f, 0.3f, 0.707f);
    glVertex3f(-90.f, -100.f, 0.f);
    glVertex3f(90.f, -100.f, 0.f);
    glColor3f(0.05f, 0.704f, 0.606f);
    glVertex3f(-40, -25, 0);
    glVertex3f(40, -25, 0);
    glEnd();

    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(-3.f, -100.f, 0.f);
    glVertex3f(3.f, -100.f, 0.f);
    glColor3f(0.05f, 0.704f, 0.606f);
    glVertex3f(-0.50, -25, 0);
    glVertex3f(0.50, -25, 0);
    glEnd();

    glFlush();
}

void SetupRC(void) {
    glClearColor(0.f, 0.f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei w, GLsizei h) {
    GLfloat aspectRatio;
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    aspectRatio = (GLfloat)w / (GLfloat)h;
    if (w <= h)
        glOrtho(-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio, 1.0, -1.0);
    else
        glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("images");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    SetupRC();
    glutMainLoop();

    return 0;
}
