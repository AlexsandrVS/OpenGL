#include "GL/glew.h"
#include "GL/glut.h"
#include <math.h>

// Параметры освещения
GLfloat whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };

// Вызывается для рисования сцены
void RenderScene(void)
{
    // Угол поворота системы Земля/Луна
    static float fMoonRot = 0.0f;
    static float fEarthRot = 0.0f;

    // Очищаем окно текущим цветом очистки
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Save the matrix state and do the rotations
    // glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Транслируем всю сцену в поле зрения
    glTranslatef(0.0f, 10.0f, -300.0f);

    // * изменить плоскость вращения Земли и Луны, скорость и направление вращения Земли. 

    // Устанавливаем цвет материала красным
    // Солнце
    glDisable(GL_LIGHTING);
    glColor3ub(255, 255, 0);
    glutSolidSphere(15.0f, 30, 17);
    glEnable(GL_LIGHTING);

    // Движение источника света, после прорисовки солнца!
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Поворот системы координат
    glRotatef(fEarthRot, 0.0f, 0.0f, 1.0f);

    // * изменить плоскость вращения Земли и Луны, скорость и направление вращения Земли. 

    // Прорисовка Земли
    glColor3ub(0, 0, 255);
    glTranslatef(80.0f, 25.0f, 0.0f);
    glutSolidSphere(15.0f, 30, 17);

    // Поворот в системе координат, связанной с Землей
    // и изображение Луны
    glColor3ub(200, 200, 200);
        glRotatef(fMoonRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(30.0f, 0.0f, 0.0f);
    fMoonRot += 15.0f;
    if (fMoonRot > 360.0f)
        fMoonRot = 0.0f;
    glutSolidSphere(6.0f, 30, 17);

    // Восстанавливается состояние матрицы

    // Шаг по орбите Земли равен пяти градусам
    fEarthRot += 5.0f;
    if (fEarthRot > 360.0f)
        fEarthRot = 0.0f;

    glPopMatrix(); // Матрица наблюдения модели
    // Показывается построенное изображение
    glutSwapBuffers();
}

void SetupRC()
{
    // Параметры света и координаты
    glEnable(GL_DEPTH_TEST); // Удаление скрытых поверхностей
    glFrontFace(GL_CCW);     // Многоугольники с обходом против часовой стрелки направлены наружу
    glEnable(GL_CULL_FACE);  // Расчеты внутри самолета не выполняются

    // Активация освещения
    glEnable(GL_LIGHTING);

    // Устанавливается и активизируется источник света 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);

    // Активизирует согласование цветов
    glEnable(GL_COLOR_MATERIAL);
    // Свойства материалов соответствуют кодам glColor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void TimerFunc(int value)
{
    glutPostRedisplay();
    glutTimerFunc(50, TimerFunc, 1);
}

void ChangeSize(int w, int h)
{
    GLfloat fAspect;

    // Предотвращает деление на ноль
    if (h == 0)
        h = 1;

    // Размер поля просмотра устанавливается равным размеру окна
    glViewport(0, 0, w, h);

    // Расчет соотношения сторон окна
    fAspect = (GLfloat)w / (GLfloat)h;

    // Устанавливаем перспективную систему координат
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Поле обзора равно 45 градусов, ближняя и дальняя плоскости
    // проходят через 1 и 425
    gluPerspective(45.0f, fAspect, 1.0, 425.0);

    // Обновляем матрицу наблюдения модели
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(500, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Earth/Moon/Sun System");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutTimerFunc(100, TimerFunc, 1);
    SetupRC();
    glutMainLoop();
    return 0;
}
