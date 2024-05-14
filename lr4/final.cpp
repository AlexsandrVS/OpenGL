#pragma region include
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
using namespace this_thread;     // sleep_for, sleep_until
using namespace chrono_literals; // ns, us, ms, s, h, etc.
using chrono::system_clock;
#pragma endregion
#pragma region global perem
// Объявление переменных для хранения позиции мыши, параметров камеры и цвета
float mouseX, mouseY;
float cameraDistanceX;
float cameraDistanceY;
float cameraAngleX;
float cameraAngleY;
float wight, height;
bool W = false;

// Вектор вершин модели
vector<vector<float>> vertices;

// Переменные для управления цветом
float angle = 0.0f;
int i = 0;
float r = 1.0;
float g = 0;
float b = 0;
int d = 0;
#pragma endregion
#pragma region class_model
class Model {
private:
    // Вложенный класс Face для представления граней модели
    class Face {
    public:
        int edge;          // Количество вершин в грани
        int* vertices;     // Индексы вершин грани
        int* texcoords;    // Индексы текстурных координат грани
        int normal;        // Индекс нормали грани

        // Конструктор класса Face для инициализации грани
        Face(int edge, int* vertices, int* texcoords, int normal = -1) {
            this->edge = edge;
            this->vertices = vertices;
            this->texcoords = texcoords;
            this->normal = normal;
        }
    };

    // Векторы для хранения вершин, текстурных координат, нормалей и граней модели
    std::vector<GLfloat*> vertices;
    std::vector<GLfloat*> texcoords;
    std::vector<GLfloat*> normals;
    std::vector<Face> faces;

    // Идентификатор списка отображения OpenGL
    GLuint list;

public:
    void load(const char* filename) {
        // Переменные для временного хранения данных из файла
        std::string line;
        std::vector<std::string> lines;
        std::ifstream in(filename);

        // Проверка открытия файла
        if (!in.is_open()) {
            printf("Cannot load model %s\n", filename);
            return;
        }

        // Чтение содержимого файла и разбиение его на строки
        while (!in.eof()) {
            std::getline(in, line);
            lines.push_back(line);
        }
        in.close();

        // Парсинг содержимого для получения информации о вершинах, текстурах и гранях
        float a, b, c;
        for (std::string& line : lines) {
            if (line[0] == 'v') {
                if (line[1] == ' ') {
                    // Парсинг строки для извлечения координат вершины
                    sscanf(line.c_str(), "v %f %f %f", &a, &b, &c);
                    // Добавление вершины в вектор вершин
                    vertices.push_back(new float[3] {a, b, c});
                }
                else if (line[1] == 't') {
                    // Парсинг строки для извлечения текстурных координат
                    sscanf(line.c_str(), "vt %f %f", &a, &b);
                    // Добавление текстурных координат в вектор
                    texcoords.push_back(new float[2] {a, b});
                }
                else {
                    // Парсинг строки для извлечения нормалей
                    sscanf(line.c_str(), "vn %f %f %f", &a, &b, &c);
                    // Добавление нормалей в вектор
                    normals.push_back(new float[3] {a, b, c});
                }
            }
            else if (line[0] == 'f') {
                // Парсинг строки для извлечения индексов вершин, текстурных координат и нормалей
                int v0, v1, v2, v3, t0, t1, t2, t3, n;
                sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n, &v3, &t3, &n);

                if (v3 != 0) {
                    // Создание грани с четырьмя вершинами
                    int* v = new int[4] {v0 - 1, v1 - 1, v2 - 1, v3 - 1};
                    faces.push_back(Face(4, v, NULL, n - 1));
                }
                else {
                    // Создание грани с тремя вершинами
                    int* v = new int[3] {v0 - 1, v1 - 1, v2 - 1};
                    faces.push_back(Face(3, v, NULL, n - 1));
                }
            }
        }

        // Создание списка отображения для рендеринга модели
        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        for (Face& face : faces) {
            // Установка нормали грани, если она существует
            if (face.normal != -1)
                glNormal3fv(normals[face.normal]);
            else
                glDisable(GL_LIGHTING);

            // Установка режима отображения и рисование грани
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBegin(GL_POLYGON);
            for (int i = 0; i < face.edge; i++)
                glVertex3fv(vertices[face.vertices[i]]);
            glEnd();

            // Включение освещения, если грани не имеют нормали
            if (face.normal == -1)
                glEnable(GL_LIGHTING);
        }
        glEndList();
                                // Устанавливаем цвет материала
                                GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f};

                                // Свойства материалов согласуются с кодами glColor
                                glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

                                // С этого момента все материалы имеют максимальный коэффициент зеркального отражения
                                glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
                                glMateriali(GL_FRONT, GL_SHININESS, 10);

        // Очистка памяти после загрузки модели
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

    // Метод draw для отображения модели
    void draw() { glCallList(list); }
};
Model model;
#pragma endregion
#pragma region init
void init() {
    // Включение освещения и источника света
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Установка цвета фона
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

    // Установка режима проецирования и загрузка единичной матрицы
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Установка текущей матрицы как видовой
    glMatrixMode(GL_MODELVIEW);

    // Включение смешивания цветов и настройка параметров
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Включение сглаживания линий
    glEnable(GL_LINE_SMOOTH);

    // Установка параметров текстуры
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Установка режима отображения полигонов
    glPolygonMode(GL_FRONT_AND_BACK, GL_3D);

    // Включение буфера глубины
    glEnable(GL_DEPTH_TEST);
}
#pragma endregion
#pragma region setupLighting
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
#pragma endregion
#pragma region Camera
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
    
    // Метод для вывода информации о камере
    void Print() {
        printf("Camera ->\n");
        printf("   ah=%6.2f       av=%6.2f\n", ah / M_PI * 180, av / M_PI * 180);
        printf("   (x, y, z) = (%7.3f, %7.3f, %7.3f\n", x, y, z);
        printf("   (dx, dy, dz) = (%7.3f, %7.3f, %7.3f\n", dx, dy, dz);
    }
};
class FlatArea {
public:
    int k;          // Количество ячеек на стороне квадрата
    float size;     // Размер квадрата

    // Конструктор класса FlatArea
    FlatArea(int _k, float _size) {
        k = _k;
        size = _size;
    }

    // Метод для отрисовки плоской области
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
                    glColor3f(0, 1, 0);
                else
                    glColor3f(0, 0.2, 0);
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

Camera cam;             // Экземпляр класса камеры
FlatArea flatarea(500, 150);  // Экземпляр класса FlatArea для отображения плоской области

bool showShadow = true; // Флаг для отображения теней
bool enRotate = false;   // Флаг для включения вращен-ия

#pragma endregion 
#pragma region display
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
#pragma endregion 
#pragma region control
void processNormalKeys(unsigned char key, int x, int y) {

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
    if (key == 'v') {
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
	if (key == 'F' || key == 'f')
		if (glIsEnabled(GL_LIGHT0))
			glDisable(GL_LIGHT0);
		else
			glEnable(GL_LIGHT0);
	if (key == 'r')
		setupLighting();
	    glutPostRedisplay();
	if (key == 'p') {
		glPolygonMode(GL_FRONT_AND_BACK, GL_3D);
		model.load("Tricycle.obj");
		glutPostRedisplay();
	}
	if (key == 't' || key == 'T') {
		showShadow = !showShadow;
		glutPostRedisplay();
	}
	if (key == 'y' || key == 'Y') {
		enRotate = !enRotate;
		glutPostRedisplay();
	}
	if (key == 'm') {
		setupLighting();
		cam.Setup(4, 1, 4, -M_PI / 4, 0);
		glutPostRedisplay();
	}
}
void mouseMove(int x, int y)
{
    // Переменные для хранения направления движения мыши
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
    
    // Поворот камеры по горизонтали в соответствии с движением мыши
    cam.TurnH(a, b, (x - mouseX), (y - mouseY));
    
    // Обновление текущих координат мыши
    mouseX = x;
    mouseY = y;
    
    // "Захват" мыши в центре окна, если она достигла края
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
    
    // Обновление изображения
    glutPostRedisplay();
}
void update(int value) {
    // Увеличение угла вращения куба
	angle += i * 2.0f;
    
    // Проверка на превышение угла 360 градусов
	if (angle > 360) {
		angle -= 360;
	}
    
    // Обновление изображения
	glutPostRedisplay();
    
    // Установка таймера для следующего обновления
	glutTimerFunc(10, update, 0);
}
#pragma endregion

int main(int argc, char* argv[]) {

	// printf("Управление:\n");
    // printf("WASD: Движение камеры вперед, назад, влево и вправо\n");
    // printf("1-4: Включение и выключение источников света\n");
    // printf("5: Включение или выключение цветового материала и установка цвета фонового освещения\n");
    // printf("6: Включение или выключение двустороннего освещения\n");
    // printf("7: Включение отражения\n");
    // printf("8: Установка положения наблюдателя\n");
    // printf("9: Установка распределения освещения в пространстве\n");
    // printf("0: Сброс настроек освещения\n");
    // printf("F: Включение и выключение фонаря\n");
    // printf("R: Перезагрузка настроек освещения\n");
    // printf("Z: Загрузить модель Tricycle.obj\n");
    // printf("X: Включение и выключение дополнительного источника света\n");
    // printf("T: Включение и выключение куба\n");
    // printf("Y: Включение и выключение вращения куба\n");
    // printf("M: Сброс всех настроек\n");
    // printf("ESC: Выход из программы\n");

    // Инициализация GLUT и создание окна
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1200, 800); // Задайте желаемый размер окна
    glutCreateWindow("OpenGL");

    // Включить проверку глубины
    glEnable(GL_DEPTH_TEST);

    // Инициализация OpenGL и настройка освещения
    init();
    setupLighting();

    // Установка функций обратного вызова
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouseMove);
    glutKeyboardFunc(processNormalKeys);
    glutTimerFunc(25, update, 0);

    glutMainLoop();

    return 0;
}