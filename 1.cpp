#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC
#include "glew.h"
#include "glut.h" 
#include "GL\stb_image.h"
#include <GL/gl.h>
#include <math.h>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include <winuser.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <mmsystem.h>
#include <thread>
#include <chrono>
#include <dos.h>

using namespace std;
using namespace this_thread;     // sleep_for, sleep_until
using namespace chrono_literals; // ns, us, ms, s, h, etc.
using chrono::system_clock;


float mouseX, mouseY;
float cameraDistanceX;
float cameraDistanceY;
float cameraAngleX;
float cameraAngleY;
float wight, height;
bool W = false;
vector<vector<float>> vertices;
float angle = 0.0f;
int i = 0;
float r = 1.0;
float g = 0;
float b = 0;
int d = 0;

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
	std::vector<GLfloat*> vertices;
	std::vector<GLfloat*> texcoords;
	std::vector<GLfloat*> normals;
	std::vector<Face> faces;
	GLuint list;
public:
	void load(const char* filename) {
		std::string line;
		std::vector<std::string> lines;
		std::ifstream in(filename);
		if (!in.is_open()) {
			printf("Cannot load model %s\n", filename);
			return;
		}
		while (!in.eof()) {
			std::getline(in, line);
			lines.push_back(line);
		}
		in.close();
		float a, b, c;
		for (std::string& line : lines) {
			if (line[0] == 'v') {
				if (line[1] == ' ') {
					sscanf_s(line.c_str(), "v %f %f %f", &a, &b, &c);
					vertices.push_back(new float[3] {a, b, c});
				}
				else if (line[1] == 't') {
					sscanf_s(line.c_str(), "vt %f %f", &a, &b);
					texcoords.push_back(new float[2] {a, b});
				}
				else {
					sscanf_s(line.c_str(), "vn %f %f %f", &a, &b, &c);
					normals.push_back(new float[3] {a, b, c});
				}
			}
			else if (line[0] == 'f') {
				int v0, v1, v2, v3, t0, t1, t2, t3, n;
				sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
					&v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n, &v3, &t3, &n);

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
		printf("Model: %s\n", filename);
		printf("Vertices: %d\n", vertices.size());
		printf("Texcoords: %d\n", texcoords.size());
		printf("Normals: %d\n", normals.size());
		printf("Faces: %d\n", faces.size());
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
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPolygonMode(GL_FRONT_AND_BACK, GL_3D);
	glEnable(GL_DEPTH_TEST);
}

void setupLighting() {
	glPushMatrix();
	glLoadIdentity();
	GLfloat light_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float position[] = { 0,10,20 };

	GLfloat light0_position[] = { 5.0f, 5.0f, 5.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glEnable(GL_LIGHT0);

	GLfloat light1_position[] = { -5.0f, 5.0f, 5.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color);
	glEnable(GL_LIGHT1);

	GLfloat light2_position[] = { 5.0f, -5.0f, 5.0f, 1.0f };
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_color);
	glEnable(GL_LIGHT2);

	GLfloat light3_position[] = { -5.0f, -5.0f, 5.0f, 1.0f };
	glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_color);
	glEnable(GL_LIGHT3);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat material_color[] = { r, g, b, 0.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color);
	glEnable(GL_NORMALIZE);

	GLfloat light4_position[] = { 2.5f, 0.5f, -2.5f, 1.0f };
	GLfloat light4_diffuse[] = { 1.0f, 0.0f, -.0f, 1.0f };
	glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light4_diffuse);
	glEnable(GL_LIGHT4);

	glPopMatrix();
}

class Camera {
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
		ah = -ah;
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

bool showShadow = true;
bool enRotate = true;
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
			angle += i * 2.0f;
		}
		// Тень куба
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

	wight = w;
	height = h;
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

	if (key == '1') {
		if (glIsEnabled(GL_LIGHT0))
			glDisable(GL_LIGHT0);
		else
			glEnable(GL_LIGHT0);
	}
	if (key == '2') {
		if (glIsEnabled(GL_LIGHT1))
			glDisable(GL_LIGHT1);
		else
			glEnable(GL_LIGHT1);
	}
	if (key == '3') {
		if (glIsEnabled(GL_LIGHT2))
			glDisable(GL_LIGHT2);
		else
			glEnable(GL_LIGHT2);
	}
	if (key == '4') {
		if (glIsEnabled(GL_LIGHT3))
			glDisable(GL_LIGHT3);
		else
			glEnable(GL_LIGHT3);
	}
	// Установка цвета фонового освещения
	if (key == '5') {
        if (glIsEnabled(GL_COLOR_MATERIAL))
            glDisable(GL_COLOR_MATERIAL);
		else
			glEnable(GL_COLOR_MATERIAL);
		GLfloat ambient_color[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_color);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    }
	// Включение двустороннего освещения
	if (key == '6') {
		if (glIsEnabled(GL_COLOR_MATERIAL))
			glDisable(GL_COLOR_MATERIAL);
		else
			glEnable(GL_COLOR_MATERIAL);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	}
	// Установка коэффициента рассеивания материала
	if (key == '7') {
		if (glIsEnabled(GL_COLOR_MATERIAL))
			glDisable(GL_COLOR_MATERIAL);
		else
			glEnable(GL_COLOR_MATERIAL);
		GLfloat material_diffuse[] = { 0.0f, 1.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
		glColorMaterial(GL_BACK, GL_AMBIENT_AND_DIFFUSE);
	}
	// Установка положения наблюдателя
	if (key == '8') {
		GLfloat viewer_pos[] = { 1.0f, 0.0f, 1.0f, 0.0f };
		glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, viewer_pos);
		glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	}
	// Установка распределения освещения в пространстве
	if (key == '9') {
		glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	}
	// Установка распределения освещения в пространстве
	if (key == '0') {
		glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	}
	if (key == 'F' || key == 'f')
		if (glIsEnabled(GL_LIGHT0))
			glDisable(GL_LIGHT0);
		else
			glEnable(GL_LIGHT0);
	if (key == 'r')
		setupLighting();
	glutPostRedisplay();
	if (key == 'z') {
		glPolygonMode(GL_FRONT_AND_BACK, GL_3D);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
		model.load("Car.obj");
		glutPostRedisplay();
	}
	if (key == 'x') {
		if (glIsEnabled(GL_LIGHT4))
			glDisable(GL_LIGHT4);
		else
			glEnable(GL_LIGHT4);
	}
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

void mouseMove(int x, int y)
{
	int a = 0, b = 0;
	if ((x - mouseX) > 0) a = 1;
	if ((x - mouseX) < 0) a = -1;
	if ((y - mouseY) > 0) b = 1;
	if ((y - mouseY) < 0) b = -1;
	cam.TurnH(a, b, (x - mouseX), (y - mouseY));
	mouseX = x;
	mouseY = y;
	if ((mouseX != wight / 2))
	{
		if (mouseX < 20)
		{
			mouseX += wight / 2;
			glutWarpPointer(mouseX, mouseY);
		}
		if (mouseX > (wight - 20))
		{
			mouseX -= wight / 2;
			glutWarpPointer(mouseX + 1, mouseY);
		}
	}
	glutPostRedisplay();
}

void update(int value) {
	angle += i * 2.0f;
	if (angle > 360) {
		angle -= 360;
	}
	glutPostRedisplay();
	glutTimerFunc(10, update, 0);
}

 int main(int argc, char* argv[])
 {
	 glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	 glutGameModeString("2560x1600:64@240");
	 glutEnterGameMode();
	 glutSetCursor(GLUT_CURSOR_NONE);
	 glEnable(GL_DEPTH_TEST);
	 init();
	 glPolygonMode(GL_FRONT_AND_BACK, GL_3D);
	 glutDisplayFunc(display);
	 setupLighting();
	 glutReshapeFunc(reshape);
	 glutPassiveMotionFunc(mouseMove);
	 glutKeyboardFunc(processNormalKeys);
	 glutTimerFunc(25, update, 0);
	 glutMainLoop();
 }