#include <iostream>
#include <GL/glut.h> // Подключаем библиотеку OpenGL

typedef struct {
    unsigned char imageTypeCode;
    short int imageWidth;
    short int imageHeight;
    unsigned char bitCount;
    unsigned char* imageData;
} TGAFILE;

bool LoadTGAFile(char* filename, TGAFILE* tgaFile) {
    FILE* filePtr;
    unsigned char ucharBad;
    short int sintBad;
    long imageSize;
    int colorMode;
    unsigned char colorSwap;

    // Open the TGA file.
    filePtr = fopen(filename, "rb");
    if (filePtr == NULL) {
        return false;
    }

    // Read the two first bytes we don't need.
    fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
    fread(&ucharBad, sizeof(unsigned char), 1, filePtr);

    // Which type of image gets stored in imageTypeCode.
    fread(&tgaFile->imageTypeCode, sizeof(unsigned char), 1, filePtr);

    // For our purposes, the type code should be 2 (uncompressed RGB image)
    // or 3 (uncompressed black-and-white images).
    if (tgaFile->imageTypeCode != 2 && tgaFile->imageTypeCode != 3) {
        fclose(filePtr);
        return false;
    }

    // Read 13 bytes of data we don't need.
    fread(&sintBad, sizeof(short int), 1, filePtr);
    fread(&sintBad, sizeof(short int), 1, filePtr);
    fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
    fread(&sintBad, sizeof(short int), 1, filePtr);
    fread(&sintBad, sizeof(short int), 1, filePtr);

    // Read the image's width and height.
    fread(&tgaFile->imageWidth, sizeof(short int), 1, filePtr);
    fread(&tgaFile->imageHeight, sizeof(short int), 1, filePtr);

    // Read the bit depth.
    fread(&tgaFile->bitCount, sizeof(unsigned char), 1, filePtr);

    // Read one byte of data we don't need.
    fread(&ucharBad, sizeof(unsigned char), 1, filePtr);

    // Color mode -> 3 = BGR, 4 = BGRA.
    colorMode = tgaFile->bitCount / 8;
    imageSize = tgaFile->imageWidth * tgaFile->imageHeight * colorMode;

    // Allocate memory for the image data.
    tgaFile->imageData = (unsigned char*)malloc(sizeof(unsigned char) * imageSize);

    // Read the image data.
    fread(tgaFile->imageData, sizeof(unsigned char), imageSize, filePtr);

    // Change from BGR to RGB so OpenGL can read the image data.
    for (int imageIdx = 0; imageIdx < imageSize; imageIdx += colorMode) {
        colorSwap = tgaFile->imageData[imageIdx];
        tgaFile->imageData[imageIdx] = tgaFile->imageData[imageIdx + 2];
        tgaFile->imageData[imageIdx + 2] = colorSwap;
    }

    fclose(filePtr);
    return true;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Очищаем буфер цвета

    TGAFILE tgaFile;
    if (LoadTGAFile("11.tga", &tgaFile)) {
        // Рисуем изображение на экране
        glDrawPixels(tgaFile.imageWidth, tgaFile.imageHeight, GL_RGB, GL_UNSIGNED_BYTE, tgaFile.imageData);
    } else {
        std::cerr << "Failed to load image." << std::endl;
    }

    TGAFILE tgaFile2;
    if (LoadTGAFile("22.tga", &tgaFile2)) {
        // Рисуем изображение на экране
        glDrawPixels(tgaFile2.imageWidth, tgaFile2.imageHeight, GL_RGB, GL_UNSIGNED_BYTE, tgaFile2.imageData);
    } else {
        std::cerr << "Failed to load image." << std::endl;
    }
    

     glColor3f(0.0f, 0.0f, 1.0f); // Устанавливаем цвет в синий
    // Рисуем квадрат
    glBegin(GL_QUADS);
        glVertex2f(-0.5f, 0.5f); // Левый верхний угол
        glVertex2f(0.5f, 0.5f);  // Правый верхний угол
        glVertex2f(0.5f, -0.5f); // Правый нижний угол
        glVertex2f(-0.5f, -0.5f); // Левый нижний угол
    glEnd();

    // Переключаем буферы и выводим изображение на экран
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    // Инициализация OpenGL и GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("OpenGL");

    // Устанавливаем функцию отображения
    glutDisplayFunc(display);

    // Запускаем главный цикл обработки событий GLUT
    glutMainLoop();

    return 0;
}