#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <ctime>
#include <cstdlib>

#define INF INT_MAX // Представление бесконечности

// Функция для реализации алгоритма Прима
void alg_prim(const std::vector<std::vector<int>>& graph) {
    int num_vertices = graph.size(); // Количество вершин в графе

    std::vector<bool> visited(num_vertices, false); // Массив для отслеживания посещённых вершин

    // Инициализация генератора случайных чисел
    std::srand(std::time(nullptr));

    int start_vertex = std::rand() % num_vertices; // Случайный выбор начальной вершины
    visited[start_vertex] = true; // Помечаем начальную вершину как посещённую

    int total_weight = 0; // Общий вес минимального остовного дерева

    std::cout << "Edge : Weight" << std::endl;
    for (int i = 0; i < num_vertices - 1; ++i) {
        int min_weight = INF;
        int current_vertex, next_vertex;

        // Поиск ближайшей непосещенной вершины к посещённым
        for (int j = 0; j < num_vertices; ++j) {
            if (visited[j]) {
                for (int k = 0; k < num_vertices; ++k) {
                    if (!visited[k] && graph[j][k] && graph[j][k] < min_weight) {
                        min_weight = graph[j][k];
                        current_vertex = j;
                        next_vertex = k;
                    }
                }
            }
        }

        // Добавление ребра в минимальное остовное дерево
        std::cout << current_vertex << " - " << next_vertex << " : " << graph[current_vertex][next_vertex] << std::endl;
        total_weight += graph[current_vertex][next_vertex];
        visited[next_vertex] = true;
    }

    // Вывод общего веса минимального остовного дерева
    std::cout << "Total Weight: " << total_weight << std::endl;
}

int main() {
    std::vector<std::vector<int>> graph = {
        {0, 29, 0, 0, 0, 20, 27},
        {29, 0, 0, 0, 0, 0, 13},
        {0, 0, 0, 23, 0, 0, 12},
        {0, 0, 23, 0, 22, 0, 17},
        {0, 0, 0, 22, 0, 21, 0},
        {20, 0, 0, 0, 21, 0, 15},
        {27, 13, 12, 17, 0, 15, 0}
    };

    alg_prim(graph);

    return 0;
}
