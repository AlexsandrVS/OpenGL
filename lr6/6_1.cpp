#include "GL/glut.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Глобальные переменные модуля для записи исходных данных изображения
static GLubyte *pImage = NULL;
static GLint iWidth, iHeight, iComponents;
// Глобальные переменные для хранения режима рисования
static GLint iRenderMode = 1;

// Эта функция выполняет необходимую инициализацию в контексте визуализации.
void SetupRC(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    pImage = stbi_load("1.tga", &iWidth, &iHeight, &iComponents, 0);
}

void ShutdownRC(void) {
    // Освобождаем исходные данные изображения
    stbi_image_free(pImage);
}

// Должным образом обновляем флаги в ответ на выбор позиции из меню
void ProcessMenu(int value) {
    if (value == 9) {
        // Записываем изображение
        stbi_write_jpg("1.jpg", iWidth, iHeight, iComponents, pImage, 0);
    } else {
        // Меняем индекс режима визуализации на индекс, соответствующий позиции меню
        iRenderMode = value;
    }
    // Активизируем перерисовывание изображения
    glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////
// Вызывается для рисования сцены
void RenderScene(void) {
    GLint iViewport[4];
    GLbyte* pModifiedBytes = NULL;
    GLfloat invertMap[256];
    GLint i;

    // Очищаем окно текущим цветом очистки
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0, 0);
    glPixelZoom(-1, -1);

    // В зависимости от индекса режима визуализации выполняются необходимые операции с изображением
    switch (iRenderMode) {
        case 2: // Переворачиваем пиксели
            glRasterPos2i(-1, -1);
            glPixelZoom(1, 1);
            break;
        case 3: // Увеличиваем пиксели для заполнения окна
            glGetIntegerv(GL_VIEWPORT, iViewport);
            glPixelZoom(-(GLfloat)iViewport[2] / (GLfloat)iWidth, -(GLfloat)iViewport[3] / (GLfloat)iHeight);
            break;
        case 4: // Только красный
            glPixelTransferf(GL_RED_SCALE, 1.0f);
            glPixelTransferf(GL_GREEN_SCALE, 0.0f);
            glPixelTransferf(GL_BLUE_SCALE, 0.0f);
            break;
        case 5: // Только зеленый
            glPixelTransferf(GL_RED_SCALE, 0.0f);
            glPixelTransferf(GL_GREEN_SCALE, 1.0f);
            glPixelTransferf(GL_BLUE_SCALE, 0.0f);
            break;
        case 6: // Только синий
            glPixelTransferf(GL_RED_SCALE, 0.0f);
            glPixelTransferf(GL_GREEN_SCALE, 0.0f);
            glPixelTransferf(GL_BLUE_SCALE, 1.0f);
            break;
        case 7: // Черно-белый, более сложный режим
            // Вначале рисуем изображение в буфере цвета
            glDrawPixels(iWidth, iHeight, GL_RGB, GL_UNSIGNED_BYTE, pImage);
            // Распределяем память для карты яркости
            pModifiedBytes = (GLbyte*)malloc(iWidth * iHeight);
            // Масштабируем цвета согласно стандарту NSTC
            glPixelTransferf(GL_RED_SCALE, 0.3f);
            glPixelTransferf(GL_GREEN_SCALE, 0.59f);
            glPixelTransferf(GL_BLUE_SCALE, 0.11f);
            // Считываем пиксели в буфер (будем применено увеличение)
            glReadPixels(0, 0, iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pModifiedBytes);
            // Масштабирование цвета возвращается в норму
            glPixelTransferf(GL_RED_SCALE, 1.0f);
            glPixelTransferf(GL_GREEN_SCALE, 1.0f);
            glPixelTransferf(GL_BLUE_SCALE, 1.0f);
            break;
        case 8: // Инверсия цветов
            invertMap[0] = 1.0f;
            for (i = 1; i < 256; i++)
                invertMap[i] = 1.0f - (1.0f / 255.0f * (GLfloat)i);
            glPixelMapfv(GL_PIXEL_MAP_R_TO_R, 255, invertMap);
            glPixelMapfv(GL_PIXEL_MAP_G_TO_G, 255, invertMap);
            glPixelMapfv(GL_PIXEL_MAP_B_TO_B, 255, invertMap);
            glPixelTransferi(GL_MAP_COLOR, GL_TRUE);
            break;
        case 10: { // Поворот на 90 градусов против часовой стрелки и увеличение масштаба на 1.2
            GLubyte* rotatedImage = (GLubyte*)malloc(iWidth * iHeight * iComponents);
            int newWidth = iHeight;
            int newHeight = iWidth;
            for (int y = 0; y < iHeight; ++y) {
                for (int x = 0; x < iWidth; ++x) {
                    for (int c = 0; c < iComponents; ++c) {
                        rotatedImage[((newWidth - 1 - y) + x * newWidth) * iComponents + c] = pImage[(x + y * iWidth) * iComponents + c];
                    }
                }
            }
            free(pImage);
            pImage = rotatedImage;
            iWidth = newWidth;
            iHeight = newHeight;
            glPixelZoom(1.2f, 1.2f);
            break;
        }
        case 9:
            break;
        case 1: // сохраняем текущее изображение
        default:
            // Данная строка специально оставлена пустой
            break;
    }

    // Рисуются пиксели
    glEnable(GL_SCISSOR_TEST);
    if (pModifiedBytes == NULL) {
        glDrawPixels(iWidth, iHeight, GL_RGB, GL_UNSIGNED_BYTE, pImage);
    } else {
        glRasterPos2i(-1, -1);
        glPixelZoom(1.0f, 1.0f);
        glDrawPixels(iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pModifiedBytes);
        free(pModifiedBytes);
    }

    // Обновление всего до настроек по умолчанию
    glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
    glPixelTransferf(GL_RED_SCALE, 1.0f);
    glPixelTransferf(GL_GREEN_SCALE, 1.0f);
    glPixelTransferf(GL_BLUE_SCALE, 1.0f);
    glPixelZoom(1.0f, 1.0f); // Без увеличения пикселей

    // Переключает буферы
    glutSwapBuffers();
}

void ChangeSize(int w, int h) {
    // Предотвращает деление на нуль, когда окно слишком маленькое (нельзя сделать окно нулевой ширины)
    if (h == 0)
        h = 1;
    glViewport(0, 0, w , h);
    // Устанавливается объем отсечения
    gluOrtho2D(0.0f, w, 0.0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/////////////////////////////////////////////////////////////
// Точка входа основной программы
int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GL_DOUBLE);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("OpenGL");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);

    // Создается меню и добавляются опции выбора
    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Draw Pixels", 1);
    glutAddMenuEntry("Flip Pixels", 2);
    glutAddMenuEntry("Zoom Pixels", 3);
    glutAddMenuEntry("Just Red Channel", 4);
    glutAddMenuEntry("Just Green Channel", 5);
    glutAddMenuEntry("Just Blue Channel", 6);
    glutAddMenuEntry("Black and White", 7);
    glutAddMenuEntry("Invert Colors", 8);
    glutAddMenuEntry("Save as JPG", 9);
    glutAddMenuEntry("Rotate 90 CCW and Scale", 10); // Добавлен новый пункт меню
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    SetupRC(); // Настройка
    glutMainLoop(); // Основной программный цикл
    ShutdownRC(); // Выключение
    return 0;
}
