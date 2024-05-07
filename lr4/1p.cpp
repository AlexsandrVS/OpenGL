#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Параметры для камеры
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraDistance = 5.0f;

// Параметры для модели
aiVector3D modelCenter;
float modelScale = 1.0f;

// Модель
const aiScene* scene;

void loadModel(const std::string& filePath) {
    Assimp::Importer importer;
    scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
        return;
    }
    std::cout << "Model loaded successfully!" << std::endl;
}

void init() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Загрузка модели
    loadModel("Tricycle1.obj");

    // Проверка инициализации GLEW
    glewExperimental = GL_TRUE;
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(glewInitResult) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Проверка поддержки OpenGL 2.0
    if (!glewIsSupported("GL_VERSION_2_0")) {
        std::cerr << "OpenGL 2.0 not supported" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void resize(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void drawModel(const aiNode* node) {
    if (!node || !scene) return;

    glPushMatrix();

    // Draw meshes of this node
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        if (!mesh) continue;

        glBegin(GL_TRIANGLES);
        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            const aiFace& face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                int vertexIndex = face.mIndices[k];
                if (vertexIndex < 0 || vertexIndex >= mesh->mNumVertices) continue;
                aiVector3D vertex = mesh->mVertices[vertexIndex];
                glVertex3f(vertex.x, vertex.y, vertex.z);
            }
        }
        glEnd();
    }

    // Recursively draw children
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        drawModel(node->mChildren[i]);
    }

    glPopMatrix();
}



void renderScene() {
    if (!scene) return; // Проверка на нулевой указатель
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Положение и ориентация камеры
    gluLookAt(cameraDistance * sin(cameraAngleX), cameraDistance * sin(cameraAngleY), cameraDistance * cos(cameraAngleX),
              0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Рисование модели
    glPushMatrix();
    drawModel(scene->mRootNode);
    glPopMatrix();

    glutSwapBuffers();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Model Viewer");

    init();
    glutDisplayFunc(renderScene);
    glutReshapeFunc(resize);
    glutMainLoop();

    return 0;
}
