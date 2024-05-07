#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glut.h>

float angle = 0.0f;
float posX = 0.0f;
float posY = 0.0f;
float posZ = 5.0f;

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            angle -= 0.1f;
            break;
        case GLUT_KEY_RIGHT:
            angle += 0.1f;
            break;
        case GLUT_KEY_UP:
            posZ -= 0.1f;
            break;
        case GLUT_KEY_DOWN:
            posZ += 0.1f;
            break;
    }
    glutPostRedisplay();
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 5.0f,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);
    glTranslatef(posX, posY, -posZ);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    // Устанавливаем белый материал для всех объектов
    GLfloat material_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);

    // Добавляем источник света сверху
    GLfloat light_position[] = {0.0f, 10.0f, 0.0f, 1.0f};
    GLfloat light_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glEnable(GL_LIGHT0);

    // Отображение модели
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile("Tricycle.glb", aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Failed to load model: " << importer.GetErrorString() << std::endl;
        return;
    }

    if (scene->mNumMeshes > 0) {
        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[i];
            for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
                aiFace face = mesh->mFaces[j];
                glBegin(GL_TRIANGLES);
                for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                    unsigned int vertexIndex = face.mIndices[k];
                    if (mesh->mNormals != nullptr) {
                        glNormal3f(mesh->mNormals[vertexIndex].x, mesh->mNormals[vertexIndex].y, mesh->mNormals[vertexIndex].z);
                    }
                    glVertex3f(mesh->mVertices[vertexIndex].x, mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z);
                }
                glEnd();
            }
        }
    }

    glutSwapBuffers();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("GLB Model Viewer");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glutSpecialFunc(specialKeys);
    glutDisplayFunc(renderScene);
    glutMainLoop();
    return 0;
}
