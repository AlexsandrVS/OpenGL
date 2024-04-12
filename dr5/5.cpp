#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Structure to represent an edge
struct Edge {
    int source;      // Index of the source vertex of the edge
    int destination; // Index of the destination vertex of the edge
    int weight;      // Weight of the edge
};

// Structure to represent a subset for Kruskal's algorithm
struct Subset {
    int parent; // Index of the representative element of the subset
    int rank;   // Rank of the subset for optimizing subset union
};

// Find the root of a subset
int find_root(Subset subsets[], int index) {
    // This function recursively finds the root of the subset to which an element with the given index belongs.
    // If the current element is not the root, the function continues recursive calls until it finds the root.
    // During each recursive call, each element traversed "moves up" the hierarchy, with its parent becoming the root of the subset.

    // If the current element is the root of the subset, return its index
    if (subsets[index].parent != index)
        subsets[index].parent = find_root(subsets, subsets[index].parent);

    // Return the index of the root of the subset
    return subsets[index].parent;
}

// Union two subsets
void union_subsets(Subset subsets[], int x, int y) {
    // This function merges two subsets to which elements with the given indices x and y belong.
    // For each element, it finds the roots of their subsets, and then determines which subset will
    // be the parent for both. The subset with the lower rank is chosen as the parent. If ranks are equal,
    // one of the subsets is chosen as the parent, and its rank is incremented by one.

    int xroot = find_root(subsets, x); // Find the root of the subset containing element x
    int yroot = find_root(subsets, y); // Find the root of the subset containing element y

    // If the rank of the subset with root xroot is less than the rank of the subset with root yroot
    // Make the subset with root xroot a child of the subset with root yroot
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    // If the rank of the subset with root xroot is greater than the rank of the subset with root yroot
    // Make the subset with root yroot a child of the subset with root xroot
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot; 
    // If the ranks of subsets are equal
    // Make the subset with root yroot a child of the subset with root xroot
    // Increase the rank of the subset with root xroot by one
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Compare edges by weight
bool compare_edges(Edge x, Edge y) {
    // If the weight of edge 'x' is less than the weight of edge 'y', return true,
    // indicating that edge 'x' should come before edge 'y' in sorting. Otherwise, return false.

    return x.weight < y.weight;
}

// Implementation of Prim's algorithm to build Minimum Spanning Tree
void prim_algorithm(const std::vector<std::vector<int>>& graph) {

    // The algorithm works as follows:
    // 1. Starting from an arbitrary vertex, edges with the minimum weight that connect visited and unvisited vertices are chosen.
    // 2. The selected vertex is marked as visited.
    // 3. Steps 1 and 2 are repeated until all vertices are visited or all edges are built.
    // 4. As a result, a minimum spanning tree is obtained.

    int num_vertices = graph.size(); // Number of vertices in the graph
    int total_weight = 0; // Total weight of the minimum spanning tree
    std::vector<bool> visited(num_vertices, false); // Array to track visited vertices
    int start_vertex = std::rand() % num_vertices; // Randomly select a starting vertex

    visited[start_vertex] = true; // Mark the starting vertex as visited

    std::cout << "Minimum Spanning Tree using Prim's algorithm:\nEdge : Weight" << std::endl;
    
    // Start iterations to build the spanning tree
    for (int i = 0; i < num_vertices - 1; ++i) {
        int min_weight = INT_MAX;
        int current_vertex, next_vertex;

        // Find the nearest unvisited vertex to the visited ones
        for (int vertex = 0; vertex < num_vertices; ++vertex) {
            if (visited[vertex]) {
                for (int adjacent_vertex = 0; adjacent_vertex < num_vertices; ++adjacent_vertex) {
                    if (!visited[adjacent_vertex] && graph[vertex][adjacent_vertex] && graph[vertex][adjacent_vertex] < min_weight) {
                        min_weight = graph[vertex][adjacent_vertex];
                        current_vertex = vertex;
                        next_vertex = adjacent_vertex;

                    }
                }
            }
        }

        // Add the edge to the minimum spanning tree
        std::cout << current_vertex << " - " << next_vertex << " : " << graph[current_vertex][next_vertex] << std::endl;
        total_weight += graph[current_vertex][next_vertex];
        visited[next_vertex] = true;
    }

    // Output the total weight of the minimum spanning tree
    std::cout << "Total Weight: " << total_weight << std::endl;
}

// Implementation of Kruskal's algorithm to build Minimum Spanning Tree
void kruskal_algorithm(std::vector<std::vector<int>>& graph) {

    // The algorithm works as follows:
    // 1. All possible edges of the graph are created and sorted by weight.
    // 2. For each edge in increasing order of weight:
    //    - If adding the edge does not create a cycle in the spanning tree, it is added to the tree.
    //    - To detect a cycle, a data structure called "Disjoint Set" is used.
    //    - Edges are added until (graph.size() - 1) edges are added, where graph.size() is the number of vertices in the graph.

    std::vector<Edge> edges; // Vector to store the edges of the graph
    for (int i = 0; i < graph.size(); ++i) {
        for (int j = i + 1; j < graph.size(); ++j) {
            if (graph[i][j] != 0) {
                edges.push_back({i, j, graph[i][j]}); // Add the edge to the vector
            }
        }
    }

    std::vector<Edge> result; // Vector to store the edges of the minimum spanning tree
    int e = 0; // Counter for edges added to the spanning tree
    int i = 0; // Index for iterating through all edges in sorted order

    // Sort edges by weight
    std::sort(edges.begin(), edges.end(), compare_edges);

    // Allocate memory for subsets
    Subset *subsets = new Subset[graph.size()];
    
    // Initialize subsets
    for (int v = 0; v < graph.size(); v++) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    // Until the number of edges in the spanning tree reaches (graph.size() - 1)
    while (e < graph.size() - 1 && i < edges.size()) {
        Edge next_edge = edges[i++]; // Take the next edge with the minimum weight from the sorted array

        int x = find_root(subsets, next_edge.source); // Find the root of the subset containing the source vertex
        int y = find_root(subsets, next_edge.destination); // Find the root of the subset containing the destination vertex

        // If adding the edge does not create a cycle, add it to the result
        if (x != y) {
            result.push_back(next_edge);
            union_subsets(subsets, x, y); // Union two subsets
            e++;
        }
    }

    // Output the edges of the spanning tree and their weight
    int total_weight = 0;
    std::cout << "Minimum Spanning Tree using Kruskal's algorithm:\n";
    std::cout << "Edge \tWeight\n";
    for (i = 0; i < result.size(); i++) {
        std::cout << result[i].source << " - " << result[i].destination << " \t" << result[i].weight << std::endl;
        total_weight += result[i].weight;
    }
    std::cout << "Total Weight: " << total_weight << std::endl;

    delete[] subsets;
}

int main() {

    std::srand(std::time(nullptr));

    std::vector<std::vector<int>> graph = {
        {0, 22, 0, 0, 0, 0, 0},
        {22, 0, 21, 0, 16, 0, 0},
        {0, 21, 0, 29, 15, 0, 0},
        {0, 0, 29, 0, 14, 25, 0},
        {0, 16, 15, 14, 0, 19, 27},
        {0, 0, 0, 25, 19, 0, 24},
        {0, 0, 0, 0, 27, 24, 0}
    };

    prim_algorithm(graph);
    kruskal_algorithm(graph);

    return 0;
}
