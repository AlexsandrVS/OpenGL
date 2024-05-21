#pragma region includes_and_Variables

#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC
#define _CRT_SECURE_NO_WARNINGS
#include "GL/glew.h"
#include "GL/glut.h" 
#include <GL/gl.h>
#include <math.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <cstdio>

using namespace std;
using namespace this_thread;
using namespace chrono_literals;
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

#pragma endregion




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
			printf("Не удалось открыть модель %s\n", filename);
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
				sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
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
		auto verticesSize = vertices.size();
		auto normalsSize = normals.size();
		auto texcoordsSize = texcoords.size();
		auto facesSize = faces.size();
		for (Face& face : faces) {
			if (face.normal != -1 && face.normal < normalsSize)
				glNormal3fv(normals[face.normal]);
			else
				glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_POLYGON);
			for (int i = 0; i < face.edge; i++) {
				if (i < verticesSize)
					glVertex3fv(vertices[face.vertices[i]]);
				if (face.texcoords != NULL && i < texcoordsSize)
					glTexCoord2fv(texcoords[face.texcoords[i]]);
			}
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
	void draw() { 
		glCallList(list); 
	}
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

	glFogi(GL_FOG_MODE, GL_LINEAR);
	GLfloat fogColor[] = { 0.0f, 0.5f, 0.5f, 1.0f };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.05f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 10.0f);
	glFogf(GL_FOG_END, 50.0f);
}

void setupLighting() {
    // Сохраняем текущую матрицу видовой
    glPushMatrix();
    
    // Загружаем единичную матрицу
    glLoadIdentity();
    
    // Устанавливаем цвет освещения
    GLfloat light_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // Устанавливаем позицию источника света 0
    float position[] = { 0,10,20 };
    GLfloat light0_position[] = { 5.0f, 5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glEnable(GL_LIGHT0);    
    
    // Повторяем для источников света 1, 2 и 3
    GLfloat light1_position[] = { -5.0f, 5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color);
    // Добавляем компонент отраженного света
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glEnable(GL_LIGHT1);

    GLfloat light2_position[] = { 5.0f, -5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_color);
    glEnable(GL_LIGHT2);

    GLfloat light3_position[] = { -5.0f, -5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT3, GL_SPECULAR, specular);
    glEnable(GL_LIGHT3);

    // Включаем освещение и материалы
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    
    // Устанавливаем цвет материала
    GLfloat material_color[] = { r, g, b, 0.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color);
    glEnable(GL_NORMALIZE);
    
    
    GLfloat light4_position[] = { 2.5f, 0.5f, -2.5f, 1.0f };
    GLfloat light4_diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f }; // Синий цвет
    glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, light4_diffuse);
    glEnable(GL_LIGHT4);

    GLfloat light5_position[] = { 0.5f, 0.0f, -2.5f, 1.0f };
    GLfloat light5_diffuse[] = { 0.0f, 1.0f, 0.0f, 1.0f }; // Зеленый цвет
    glLightfv(GL_LIGHT5, GL_POSITION, light5_position);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, light5_diffuse);
    glEnable(GL_LIGHT5);

    GLfloat light6_position[] = { -2.0f, 0.0f, -2.5f, 1.0f };
    GLfloat light6_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // Красный цвет
    glLightfv(GL_LIGHT6, GL_POSITION, light6_position);
    glLightfv(GL_LIGHT6, GL_DIFFUSE, light6_diffuse);
    glEnable(GL_LIGHT6);

    
    // Восстанавливаем предыдущее состояние матрицы
    glPopMatrix();
}

class Camera {
    double x, y, z;     // Позиция камеры в трехмерном пространстве
    double ah, av;      // Углы поворота камеры по горизонтали и вертикали
    double dx, dz, dy;  // Вектор направления камеры
    double step = 0.6;  // Шаг движения и поворота камеры
    double dav = 0.04;  // Шаг изменения угла поворота по вертикали

public:
    // Конструктор класса Camera
    Camera() {
        Setup(0, 1, 4, -M_PI / 4, 0);  // Начальная установка параметров камеры
    }
    
    // Метод для установки начальных параметров камеры
    void Setup(float _x, float _y, float _z, float _ah, float _av) {
        x = _x;
        y = _y;
        z = _z;
        ah = -_ah;  // Поворачиваем камеру в противоположную сторону по горизонтали
        av = _av;
        TurnH(0, 0, 0, 0);  // Поворачиваем камеру по горизонтали
        TurnV(0);           // Поворачиваем камеру по вертикали
    }
    
    // Метод для установки камеры в OpenGL
    void LookAt() {
        gluLookAt(x, y, z, x + dx, y + dy, z + dz, 0, 1, 0);
    }
    
    // Метод для поворота камеры по горизонтали
    void TurnH(int a, int b, float minusx, float minusy) {
        // Изменяем угол поворота по горизонтали
        ah += a * (abs(minusx) / 4);
        // Вычисляем новые компоненты вектора направления камеры
        dx = sin(ah * M_PI / 180.0);
        dz = -cos(ah * M_PI / 180.0);
        // Изменяем угол поворота по вертикали
        if (abs(av + (-b * (abs(minusy) / 4))) >= M_PI * 16) return;
        av += -b * (abs(minusy) / 4);
        // Вычисляем новую компоненту y вектора направления камеры
        dy = sin(av * M_PI / 180.0);
    }
    
    // Метод для поворота камеры по вертикали
    void TurnV(float dir) {
        // Изменяем угол поворота по вертикали
        if (abs(av + dir * dav) >= M_PI / 4) return;
        av += dir * dav;
        // Вычисляем новую компоненту y вектора направления камеры
        dy = sin(av);
    }
    
    // Метод для перемещения камеры вдоль ее направления
    void Move(float dir) {
        x += dir * dx;
        y += dir * dy;
        z += dir * dz;
    }
    
    // Метод для перемещения камеры вверх или вниз
    void StepV(float dir) {
        y += dir * step;
    }
    
    // Метод для перемещения камеры вперед или назад
    void StepH(GLfloat dir) {
        x += dir * cos(ah * M_PI / 180.0) * step;
        z += dir * sin(ah * M_PI / 180.0) * step;
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



#pragma region displays

Camera cam;
FlatArea flatarea(200, 200);

bool showShadow = false;
bool enRotate = false;
bool blik = false;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	cam.LookAt();

	flatarea.Draw();
	
	if (blik) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		GLfloat light_color[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		GLfloat light5_position[] = { 3.0f, 2.0f, -4.8f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, light5_position);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
		glEnable(GL_LIGHT0);
		GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
		GLfloat mat_specular[] = { 0.0, 1.0, 0.0, 1.0 };
		GLfloat mat_shininess[] = { 100.0 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glTranslatef(0, 1, 0);
		glutSolidSphere(1.0, 50, 50);
		glTranslatef(0, -1, 0);
	}

	glPushMatrix();
	glTranslatef(2.5, 0.65, -4.8);
	model.draw();
	glPopMatrix();

	if (showShadow) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
	glPushMatrix();
	glTranslatef(-3, 2, -2.5);
	if (enRotate)
	glRotatef(angle, 1.0f, 1.0f, 1.0f);
	glutSolidCube(2.0f);
	glPopMatrix();
	// Тень куба
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(-4, 0.1, -7.5);
	if (enRotate)
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glScalef(1.0f, 0.01f, 1.0f);
	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	glutSolidCube(2.0f);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	}
	glutSwapBuffers();
}

#pragma endregion

#pragma region other_reshape_keys_mouse

void reshape(int w, int h) {
    // Сохранение новых размеров окна
    wight = w;
    height = h;

    // Установка области просмотра с размерами окна
    glViewport(0, 0, w, h);

    // Установка перспективной проекции
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 0.1f, 500.0f);

    // Возвращение к видовой матрице
    glMatrixMode(GL_MODELVIEW);
}

void processNormalKeys(unsigned char key, int x, int y) {

    /*
    Основные клавиши управления
    Esc (27): Завершает работу программы.
    Space (32): Устанавливает параметры камеры (позиция и направление).

    Управление камерой
    w: Перемещение камеры вперед.
    s: Перемещение камеры назад.
    a: Перемещение камеры влево.
    d: Перемещение камеры вправо.

    Управление освещением
    1: Включение/выключение первого источника света (GL_LIGHT0).
    2: Включение/выключение второго источника света (GL_LIGHT1).
    3: Включение/выключение третьего источника света (GL_LIGHT2).
    4: Включение/выключение четвертого источника света (GL_LIGHT3).
    g: Включение/выключение пятого источника света (GL_LIGHT5).
    b: Включение/выключение шестого источника света (GL_LIGHT4).
    r: Включение/выключение седьмого источника света (GL_LIGHT6).

    Настройки материалов и освещения
    5: Включение/выключение использования материала цвета и установка фонового освещения.
    6: Включение/выключение использования материала цвета и включение двустороннего освещения.
    7: Включение/выключение использования материала цвета и установка коэффициента рассеивания материала.
    8: Установка положения наблюдателя.
    9: Установка распределения освещения в пространстве (SEPARATE_SPECULAR_COLOR).
    0: Установка распределения освещения в пространстве (SINGLE_COLOR).

    Прочие функции
    o: Перезапуск настройки освещения и камеры.
    p: Установка режима отображения полигонов и загрузка модели.

    Управление тенями и вращением
    q: Включение/выключение отображения теней.
    e: Включение/выключение вращения сцены.

    Управление бликом и туманом
    f: Включение/выключение бликов.
    t: Включение/выключение тумана.
    y: Установка квадратичного экспоненциального режима тумана (GL_FOG_MODE, GL_EXP2).
    u: Установка экспоненциального режима тумана (GL_FOG_MODE, GL_EXP).
    i: Установка линейного режима тумана (GL_FOG_MODE, GL_LINEAR). 
    */


	if (key == 27) exit(0);
	if (key == 32) cam.Setup(4, 1, 4, -M_PI / 4, 0);
	if (key == 'w' || key == 'W') cam.Move(0.1);
	if (key == 's' || key == 'S') cam.Move(-0.1);
	if (key == 'a' || key == 'A') cam.StepH(-0.1);
	if (key == 'd' || key == 'D') cam.StepH(0.1);
	if (key == '1') {
		(glIsEnabled(GL_LIGHT0))?glDisable(GL_LIGHT0):glEnable(GL_LIGHT0);
	}
	if (key == '2') {
		(glIsEnabled(GL_LIGHT1))?glDisable(GL_LIGHT1):glEnable(GL_LIGHT1);
	}
	if (key == '3') {
		(glIsEnabled(GL_LIGHT2))?glDisable(GL_LIGHT2):glEnable(GL_LIGHT2);
	}
	if (key == '4') {
		(glIsEnabled(GL_LIGHT3))?glDisable(GL_LIGHT3):glEnable(GL_LIGHT3);
	}
    if (key == 'g') {
		(glIsEnabled(GL_LIGHT5))?glDisable(GL_LIGHT5):glEnable(GL_LIGHT5);
	}
    if (key == 'b') {
		(glIsEnabled(GL_LIGHT4))?glDisable(GL_LIGHT4):glEnable(GL_LIGHT4);
	}
    if (key == 'r') {
		(glIsEnabled(GL_LIGHT6))?glDisable(GL_LIGHT6):glEnable(GL_LIGHT6);
	}
	if (key == '5') {
        if (glIsEnabled(GL_COLOR_MATERIAL))
            glDisable(GL_COLOR_MATERIAL);
		else
			glEnable(GL_COLOR_MATERIAL);
		GLfloat ambient_color[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_color);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    }
	if (key == '6') {
        (glIsEnabled(GL_COLOR_MATERIAL)) ? glDisable(GL_COLOR_MATERIAL) : (glEnable(GL_COLOR_MATERIAL), glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE), glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE));
	}
	if (key == '7') {
		if (glIsEnabled(GL_COLOR_MATERIAL))
			glDisable(GL_COLOR_MATERIAL);
		else
			glEnable(GL_COLOR_MATERIAL);
		// Устанавливаем цвет материала
        GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f};

        // Свойства материалов согласуются с кодами glColor
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

        // С этого момента все материалы имеют максимальный коэффициент зеркального отражения
        glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
        glMateriali(GL_FRONT, GL_SHININESS, 1);
	}
	if (key == '8') {
		GLfloat viewer_pos[] = { 1.0f, 0.0f, 1.0f, 0.0f };
		glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, viewer_pos);
		glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	}
	if (key == '9') {
		glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	}
	if (key == '0') {
		glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	}

	if (key == 'p') {
		glPolygonMode(GL_FRONT_AND_BACK, GL_3D);
		model.load("Tricycle.obj");
		glutPostRedisplay();
	}
	if (key == 'q') {
		showShadow = !showShadow;
		glutPostRedisplay();
	}
	if (key == 'e') {
		enRotate = !enRotate;
		glutPostRedisplay();
	}
	if (key == 'o') {
		setupLighting();
		cam.Setup(4, 1, 4, -M_PI / 4, 0);
		glutPostRedisplay();
	}
    if (key == 'f') {
		blik = !blik;
		glutPostRedisplay();
	}
	if (key == 'i') {
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glutPostRedisplay();
	}
	if (key == 'u') {
		glFogi(GL_FOG_MODE, GL_EXP);
		glutPostRedisplay();
	}
	if (key == 'y') {
		glFogi(GL_FOG_MODE, GL_EXP2);
		glutPostRedisplay();
	}
	if (key == 't') {
		if (glIsEnabled(GL_FOG))
			glDisable(GL_FOG);
		else
			glEnable(GL_FOG);
		glutPostRedisplay();
	}
};

void mouseMove(int x, int y)
{
    int a = 0, b = 0;

    // Определение направления движения по оси X
    if ((x - mouseX) > 0)
        a = 1; // Если мышь движется вправо
    if ((x - mouseX) < 0)
        a = -1; // Если мышь движется влево
    
    // Определение направления движения по оси Y
    if ((y - mouseY) > 0)
        b = 1; // Если мышь движется вверх
    if ((y - mouseY) < 0)
        b = -1; // Если мышь движется вниз

    // Поворот камеры на основе изменения положения мыши
    cam.TurnH(a, b, (x - mouseX), (y - mouseY));

    // Обновление предыдущего положения мыши
    mouseX = x;
    mouseY = y;

    // Проверка выхода мыши за границы окна и корректировка положения мыши при необходимости
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

    // Перерисовка окна для отображения обновленной сцены
    glutPostRedisplay();
}

void update(int value) {
    static bool isIdleMode = false; // Флаг для определения режима работы функции

    /* 
    функция update() используется как единая функция 
    для обновления состояния сцены 
    как во время активного движения (анимации), 
    так и в режиме простоя (когда нет активных действий пользователя). 
    */

    if (!isIdleMode) {
        // angle += i * 2.0f;
        if (angle > 360) {
            angle -= 360;
        }
    } else {
        angle += 0.5f;
        if (angle > 360.0f)
            angle -= 360.0f;
    }

    // Перерисовываем окно
    glutPostRedisplay();

    // Устанавливаем таймер для следующего обновления в зависимости от режима
    if (!isIdleMode)
        glutTimerFunc(10, update, 0);
    else
        glutTimerFunc(25, update, 0);
}

#pragma endregion





int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
	
    // Установка режима отображения с использованием буфера глубины, двойной буферизации и формата цвета RGBA
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("OpenGL");

    // Включение теста глубины для управления отображением объектов в трехмерном пространстве
    glEnable(GL_DEPTH_TEST);

    init();
    glutDisplayFunc(display);
    setupLighting();
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouseMove);
    glutKeyboardFunc(processNormalKeys);
    glutTimerFunc(25, update, 0);

    // Запуск главного цикла GLUT для обработки событий окна
    glutMainLoop();
    
    return 0;
}
