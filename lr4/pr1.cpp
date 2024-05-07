#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

float mouseX, mouseY;
float angle = 0.0f;
bool showShadow = true;
bool enRotate = true;

class Model {
private:
    class Face {
    public:
        int edge;
        int* vertices;
        int* texcoords;
        int normal;

        Face(int edge, int* vertices, int* texcoords, int normal = -1) {
            this->edge = edge;
            this->vertices = vertices;
            this->texcoords = texcoords;
            this->normal = normal;
        }
    };
    vector<GLfloat*> vertices;
    vector<GLfloat*> texcoords;
    vector<GLfloat*> normals;
    vector<Face> faces;
    GLuint list;
public:
    void load(const char* filename) {
        string line;
        vector<string> lines;
        ifstream in(filename);
        if (!in.is_open()) {
            cout << "Cannot load model " << filename << endl;
            return;
        }
        while (!in.eof()) {
            getline(in, line);
            lines.push_back(line);
        }
        in.close();
        float a, b, c;
        for (string& line : lines) {
            if (line[0] == 'v') {
                if (line[1] == ' ') {
                    sscanf(line.c_str(), "v %f %f %f", &a, &b, &c);
                    vertices.push_back(new float[3] {a, b, c});
                }
                else if (line[1] == 't') {
                    sscanf(line.c_str(), "vt %f %f", &a, &b);
                    texcoords.push_back(new float[2] {a, b});
                }
                else {
                    sscanf(line.c_str(), "vn %f %f %f", &a, &b, &c);
                    normals.push_back(new float[3] {a, b, c});
                }
            }
            else if (line[0] == 'f') {
                int v0, v1, v2, v3, t0, t1, t2, t3, n;
                sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n, &v3, &t3, &n);

                if (v3 != 0) {
                    int* v = new int[4] {v0 - 1, v1 - 1, v2 - 1, v3 - 1};
                    faces.push_back(Face(4, v, NULL, n - 1));
                }
                else {
                    int* v = new int[3] {v0 - 1, v1 - 1, v2 - 1};
                    faces.push_back(Face(3, v, NULL, n - 1));
                }
            }
        }
        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        for (Face& face : faces) {
            if (face.normal != -1)
                glNormal3fv(normals[face.normal]);
            else
                glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBegin(GL_POLYGON);
            for (int i = 0; i < face.edge; i++)
                glVertex3fv(vertices[face.vertices[i]]);
            glEnd();
            if (face.normal == -1)
                glEnable(GL_LIGHTING);
        }
        glEndList();
        cout << "Model: " << filename << endl;
        cout << "Vertices: " << vertices.size() << endl;
        cout << "Texcoords: " << texcoords.size() << endl;
        cout << "Normals: " << normals.size() << endl;
        cout << "Faces: " << faces.size() << endl;
        for (float* f : vertices)
            delete f;
        vertices.clear();
        for (float* f : texcoords)
            delete f;
        texcoords.clear();
        for (float* f : normals)
            delete f;
        normals.clear();
        faces.clear();
    }
    void draw() { glCallList(list); }
};

Model model;

void init() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_DEPTH_TEST);
}

void setupLighting() {
    // Основная логика настройки освещения
}

class Camera {
private:
    double x, y, z;
    double ah, av;
    double dx, dz, dy;
    double step = 0.6;
    double dav = 0.04;
public:
    Camera() {
        Setup(0, 1, 4, -M_PI / 4, 0);
    }
    void Setup(float _x, float _y, float _z, float _ah, float _av) {
        x = _x;
        y = _y;
        z = _z;
        ah = -_ah;
        av = _av;
        TurnH(0, 0, 0, 0);
        TurnV(0);
    }
    void LookAt() {
        gluLookAt(x, y, z,
            x + dx, y + dy, z + dz,
            0, 1, 0);
    }
    void TurnH(int a, int b, float minusx, float minusy)
    {
        ah += a * (abs(minusx) / 4);
        dx = sin(ah * M_PI / 180.0);
        dz = -cos(ah * M_PI / 180.0);
        if (abs(av + (-b * (abs(minusy) / 4))) >= M_PI * 16) return;
        av += -b * (abs(minusy) / 4);
        dy = sin(av * M_PI / 180.0);
    }
    void TurnV(float dir) {
        if (abs(av + dir * dav) >= M_PI / 4) return;
        av += dir * dav;
        dy = sin(av);
    }
    void Move(float dir) {
        x += dir * dx;
        y += dir * dy;
        z += dir * dz;
    }
    void StepV(float dir) {
        y += dir * step;
    }
    void StepH(GLfloat dir) {
        x += dir * cos(ah * M_PI / 180.0) * step;
        z += dir * sin(ah * M_PI / 180.0) * step;
    }
    void Print() {
        printf("Camera ->\n");
        printf("   ah=%6.2f       av=%6.2f\n", ah / M_PI * 180, av / M_PI * 180);
        printf("   (x, y, z) = (%7.3f, %7.3f, %7.3f\n", x, y, z);
        printf("   (dx, dy, dz) = (%7.3f, %7.3f, %7.3f\n", dx, dy, dz);
    }
};

class FlatArea {
public:
    int k;
    float size;
    FlatArea(int _k, float _size) {
        k = _k;
        size = _size;
    }
    void Draw() {
        glPushMatrix();
        glNormal3f(0, 10, 0);
        glPopMatrix();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glShadeModel(GL_FLAT);
        float y = -0.005;
        float ss = 2 * size / (float)k;
        for (int i = 0; i < k; i++) {
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j < k; j++) {
                if ((i + j) % 2 == 1)
                    glColor3f(0, 1, 0); else glColor3f(0, 0.2, 0);
                glVertex3f(-size + ss * i, y, -size + ss * j);
                glVertex3f(-size + ss * (i + 1), y, -size + ss * j);
            }
            glEnd();
        }
        glColor3f(1, 1, 1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glShadeModel(GL_FLAT);
        glPushMatrix();
        glTranslatef(0, 10, 20);
        glutWireCube(1);
        glPopMatrix();
    }
};
Camera cam;
FlatArea flatarea(200, 200);

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    cam.LookAt();

    flatarea.Draw();

    glPushMatrix();
    glTranslatef(2.5, 0.5, -2.5);
    model.draw();
    glPopMatrix();

    if (showShadow) {
        glPushMatrix();
        glTranslatef(-3, 3, -2.5);
        glRotatef(angle, 1.0f, 1.0f, 1.0f);
        glutSolidCube(2.0f);
        glPopMatrix();
        if (enRotate) {
            angle += 2.0f;
            if (angle > 360) {
                angle -= 360;
            }
        }
        glDisable(GL_LIGHTING);
        glPushMatrix();
        glTranslatef(-7, 0.1, -5.5);
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        glScalef(1.0f, 0.01f, 1.0f);
        glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
        glutSolidCube(2.0f);
        glPopMatrix();
        glEnable(GL_LIGHTING);
    }
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 0.1f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
}

void processNormalKeys(unsigned char key, int x, int y) {
    if (key == 27)
        exit(0);
    if (key == 32)
        cam.Setup(4, 1, 4, -M_PI / 4, 0);
    if (key == 'w' || key == 'W' || key == 'ц' || key == 'Ц')
        cam.Move(1);
    if (key == 's' || key == 'S' || key == 'ы' || key == 'Ы')
        cam.Move(-1);
    if (key == 'a' || key == 'A' || key == 'ф' || key == 'Ф')
        cam.StepH(-1);
    if (key == 'd' || key == 'D' || key == 'в' || key == 'В')
        cam.StepH(1);

    // Обработка других клавиш для управления освещением

    if (key == 't' || key == 'T') {
        showShadow = !showShadow;
        glutPostRedisplay();
    }
    if (key == 'y' || key == 'Y') {
        enRotate = !enRotate;
        glutPostRedisplay();
    }
    if (key == 'm') { // Reset
        setupLighting();
        cam.Setup(4, 1, 4, -M_PI / 4, 0);
        glutPostRedisplay();
    }
}

void processSpecialKeys(int key, int xx, int yy) {
    float fraction = 0.1f;
    switch (key) {
    case GLUT_KEY_LEFT:
        cam.TurnH(1, 0, 0, 0);
        break;
    case GLUT_KEY_RIGHT:
        cam.TurnH(-1, 0, 0, 0);
        break;
    case GLUT_KEY_UP:
        cam.StepV(1);
        break;
    case GLUT_KEY_DOWN:
        cam.StepV(-1);
        break;
    }
}

void passiveMotion(int x, int y) {
    mouseX = x;
    mouseY = y;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Test");
    init();
    setupLighting();
    model.load("miku.obj");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutPassiveMotionFunc(passiveMotion);
    glutIdleFunc(display);
    glutMainLoop();
    return 0;
}
