#include "GL/glut.h"
#include "math.h"

// Параметры поворота
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
//Вызывается для рисования сцены
void RenderScene(void)
{
  // Очищаем окно текущим цветом очистки
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Записываем состояние матрицы и выполняем поворот glPushMatrix();
  glRotatef(xRot, 1.0f, 0.0f, 0.0f);
  glRotatef(yRot, 0.0f, 1.0f, 0.0f);
  // Носовой конус /////////////////////////////
  // Ярко-зеленый
  glColor3ub(0, 255, 0);
  glBegin(GL_TRIANGLES);
  glVertex3f(0.0f, 0.0f, 60.0f);
  glVertex3f(-15.0f, 0.0f, 30.0f);
  glVertex3f(15.0f, 0.0f, 30.0f);
  glVertex3f(15.0f, 0.0f, 30.0f);
  glVertex3f(0.0f, 15.0f, 30.0f);
  glVertex3f(0.0f, 0.0f, 60.0f);
  glVertex3f(0.0f, 0.0f, 60.0f);
  glVertex3f(0.0f, 15.0f, 30.0f);
  glVertex3f(-15.0f, 0.0f, 30.0f);
    // Тело самолета ////////////////////////
    // Светло-серый
    glColor3ub(192, 192, 192);
  glVertex3f(-15.0f, 0.0f, 30.0f);
  glVertex3f(0.0f, 15.0f, 30.0f);
  glVertex3f(0.0f, 0.0f, -56.0f);
  glVertex3f(0.0f, 0.0f, -56.0f);
  glVertex3f(0.0f, 15.0f, 30.0f);
  glVertex3f(15.0f, 0.0f, 30.0f);
  glVertex3f(15.0f, 0.0f, 30.0f);
  glVertex3f(-15.0f, 0.0f, 30.0f);
  glVertex3f(0.0f, 0.0f, -56.0f);
  ///////////////////////////////////////////////
  // Левое крыло
  // Темно-серый
  glColor3ub(64, 64, 64);
  glVertex3f(0.0f, 2.0f, 27.0f);
  glVertex3f(-60.0f, 2.0f, -8.0f);
  glVertex3f(60.0f, 2.0f, -8.0f);
  glVertex3f(60.0f, 2.0f, -8.0f);
  glVertex3f(0.0f, 7.0f, -8.0f);
  glVertex3f(0.0f, 2.0f, 27.0f);
  glVertex3f(60.0f, 2.0f, -8.0f);
  glVertex3f(-60.0f, 2.0f, -8.0f);
  glVertex3f(0.0f, 7.0f, -8.0f);
  // Другие секции верхнего крыла
    glVertex3f(0.0f, 2.0f, 27.0f);
  glVertex3f(0.0f, 7.0f, -8.0f);
  glVertex3f(-60.0f, 2.0f, -8.0f);
  // Хвостовая часть///////////////////////////////
  // Нижняя часть стабилизатора
  glColor3ub(255, 255, 0);
  glVertex3f(-30.0f, -0.50f, -57.0f);
  glVertex3f(30.0f, -0.50f, -57.0f);
  glVertex3f(0.0f, -0.50f, -40.0f);
  // верхняя часть левой стороны
  glVertex3f(0.0f, -0.5f, -40.0f);
  glVertex3f(30.0f, -0.5f, -57.0f);
  glVertex3f(0.0f, 4.0f, -57.0f);
  // верхняя часть правой стороны
  glVertex3f(0.0f, 4.0f, -57.0f);
  glVertex3f(-30.0f, -0.5f, -57.0f);
  glVertex3f(0.0f, -0.5f, -40.0f);
  // задняя нижняя часть хвоста
  glVertex3f(30.0f, -0.5f, -57.0f);
  glVertex3f(-30.0f, -0.5f, -57.0f);
  glVertex3f(0.0f, 4.0f, -57.0f);
  // Верхняя левая часть хвоста
  glColor3ub(255, 0, 0);
  glVertex3f(0.0f, 0.5f, -40.0f);
  glVertex3f(3.0f, 0.5f, -57.0f);
  glVertex3f(0.0f, 25.0f, -65.0f);
  glVertex3f(0.0f, 25.0f, -65.0f);
  glVertex3f(-3.0f, 0.5f, -57.0f);
  glVertex3f(0.0f, 0.5f, -40.0f);
  // Задняя часть горизонтального участка
  glVertex3f(3.0f, 0.5f, -57.0f);
  glVertex3f(-3.0f, 0.5f, -57.0f);
  glVertex3f(0.0f, 25.0f, -65.0f);
  glEnd();
  glPopMatrix();
  // Отобразить результаты
  glutSwapBuffers();
}
// Функция выполняет необходимую инициализацию
// в контексте визуализации
void SetupRC()
{
  // Параметры света
  // Яркий белый свет
  // GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  glEnable(GL_DEPTH_TEST); // Удаление скрытых час-тей
  glEnable(GL_CULL_FACE); //Расчеты внутри самолета не выполняются
  glFrontFace(GL_CCW);// Многоугольники с обходом про-тив часовой стрелки
  //направлены наружу
  // Освещение материала


    glEnable(GL_LIGHTING); // Активиза-ция освещения
  // В модели освещения задается использование рас-сеянного света, //заданного в функции ambientLight[]
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
  glEnable(GL_COLOR_MATERIAL); // Активизируется согласование цветов
  //Рассеянный и диффузный цвета передней стороны объектов
  //соответствуют тому, что указано в glColor
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  // Светло-синий цвет
  glClearColor(0.0f, 0.0f, 05.f, 1.0f);
}
void SpecialKeys(int key, int x, int y)
{
  if (key == GLUT_KEY_UP)
    xRot -= 5.0f;
  if (key == GLUT_KEY_DOWN)
    xRot += 5.0f;
  if (key == GLUT_KEY_LEFT)
    yRot -= 5.0f;
  if (key == GLUT_KEY_RIGHT)
    yRot += 5.0f;
    if (key > 356.0f)
      xRot = 0.0f;
  if (key < -1.0f)
    xRot = 355.0f;
  if (key > 356.0f)
    yRot = 0.0f;
  if (key < -1.0f)
    yRot = 355.0f;
  // Обновляется окно
  glutPostRedisplay();
}
void ChangeSize(int w, int h)
{
  GLfloat nRange = 80.0f;
  // Предотвращение деления на ноль
  if (h == 0)
    h = 1;
  // Устанавливает поле просмотра по размерам окна
  glViewport(0, 0, w, h);
  // Обновляет стек матрицы проектирования
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // Устанавливает объем отсечения с помощью от-секающих
  // плоскостей (левая, правая, нижняя, верхняя,
  // ближняя, дальняя)
    if (w <= h)
      glOrtho(-nRange, nRange, -nRange * h / w, nRange * h / w, -nRange, nRange);
    else
      glOrtho(-nRange * w / h, nRange * w / h, -nRange, nRange, -nRange, nRange);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Ambient Light Jet");
  glutReshapeFunc(ChangeSize);
  glutSpecialFunc(SpecialKeys);
  glutDisplayFunc(RenderScene);
  SetupRC();
  glutMainLoop();
  return 0;
}