// -*- mode: c++; coding: utf-8; -*-

// EAY Mayo 2023
// Para MADI

// Cuidado con las lineas que tienen comentario !
// Puedes descifrar que estamos haciendo ahi?

#include <iostream>
#include <limits>
#include <new>


float INF = std::numeric_limits<float>::infinity();

/////////////////////////////////////////////////////
// TIPO DE DATO PARA GRAFICAS DIRIGIDAS PONDERADAS //
/////////////////////////////////////////////////////

typedef float graph;

// Crea una grafica de n vertices sin aristas
// 
// La grafica se representa como un arreglo unidimensional de
// flotantes que se interpreta como una matriz de adyacencia
//
// Regresa nullptr si no se pudo reservar suficiente memoria
graph* empty_graph(int n) {
    graph* g;
        
    if (n < 1) return nullptr;
    
    g = new (std::nothrow) graph[n * n + 1];
    
    if (g == nullptr) return nullptr;

    g[0] = *(float*)&n;         // !
    for (int i = 1; i <= n * n; i++)
        g[i] = INF;
    
    return g + 1;               // !
}

// Calcula el indice al arreglo correspondiente al renglon y columna
int index_of(int n, int row, int col) {
    return n * row + col;
}

// Libera la memoria reservada para la grafica
void free_graph(graph* g) {
    delete [] (g - 1);          // !
}

// Obten la cantidad de vertices en la grafica
int graph_vertices(graph* g) {
    float n = *(g - 1);         // !
    return *(int*)&n;           // !
}

// Agrega una arista entre dos vertices u y v con peso weight
void connect(graph* g, int u, int v, float weight) {
    int n = graph_vertices(g);
    //weight = weight < 0 ? 0 : weight; //************************************************
    g[index_of(n, u, v)] = weight;
}

// Agrega una arista bidireccional
void biconnect(graph* g, int u, int v, float weight) {
    connect(g, u, v, weight);
    connect(g, v, u, weight);
}

// Elimina una arista entre dos vertices u y v
void disconnect(graph* g, int u, int v) {
    int n = graph_vertices(g);
    g[index_of(n, u, v)] = INF;
}

// Obten el peso de la arista entre u y v
float weight(graph* g, int u, int v) {
    int n = graph_vertices(g);
    return g[index_of(n, u, v)];
}

// Predicado de adyacencia entre dos vertices u y v
bool adjacent(graph* g, int u, int v) {
    return weight(g, u, v) < INF;
}

//////////////////////////////////////////////////
// PROCEDIMIENTOS PARA ENCONTRAR RUTA MAS CORTA //
//////////////////////////////////////////////////

// Encuentra el vertice de la frontera con minima distancia
//
// Regresa -1 si la frontera esta vacia
int nearest_vertex(int n, bool* frontier, float* distances) {
    float min_d = INF;
    int min_v = -1;
    
    for (int v = 0; v < n; v++) {
        if (!frontier[v]) continue;
        if (min_d <= distances[v]) continue;
        min_d = distances[v];
        min_v = v;
    }
    
    return min_v;
}

// Implementacion del algoritmo de Dijkstra para encontrar la ruta mas
// corta entre un vertice inicial beg y un vertice final end
int* find_path(graph* g, int beg, int end) {
    int n = graph_vertices(g);
    int* backlinks = new (std::nothrow) int[n];
    float* distances = new (std::nothrow) float[n];
    bool* frontier = new (std::nothrow) bool[n];

    if (backlinks == nullptr) return nullptr;
    if (distances == nullptr) return nullptr;
    if (frontier == nullptr) return nullptr;

    for (int v = 0; v < n; v++) {
        backlinks[v] = -1;
        distances[v] = INF;
        frontier[v] = false;
    }

    frontier[beg] = true;
    distances[beg] = 0.0;

    int minv;
    float new_d;
    while ((minv = nearest_vertex(n, frontier, distances)) != -1) {
        frontier[minv] = false;
        if (minv == end) break;
        for (int u = 0; u < n; u++) {
            if (!adjacent(g, minv, u)) continue;
            new_d = distances[minv] + weight(g, minv, u);
            if (distances[u] <= new_d) continue;
            frontier[u] = true;
            distances[u] = new_d;
            backlinks[u] = minv;
        }
    }

    return backlinks;
}

// Imprime en pantalla la ruta desde beg hasta end
void show_route(int* backlinks, int beg, int end) {
    while (end != beg) {
        if (backlinks[end] < 0) {
            std::cout << "! No existe una ruta" << std::endl;
            return;
        }
        std::cout << end << " <- ";
        end = backlinks[end];
    }
    std::cout << beg << std::endl;
}

//////////////////////
// RUTINA PRINCIPAL //
//////////////////////

int main() {
    // Creación de gráfica ejemplo
    graph* g = empty_graph(12);
    if (g == nullptr) {
        std::cout << "Memoria insuficiente" << std::endl;
        return 1;
    }

    connect(g, 0, 1, 1.0);
    connect(g, 0, 2, 2.0);
    connect(g, 0, 3, 3.0);
    connect(g, 1, 4, 6.0);
    connect(g, 2, 1, 4.0);
    connect(g, 2, 3, 5.0);
    connect(g, 2, 4, 7.0);
    connect(g, 3, 4, 8.0);
    connect(g, 4, 6, 9.0);
    connect(g, 5, 8, 12.0);
    connect(g, 5, 10, 16.0);
    connect(g, 6, 5, 10.0);
    connect(g, 7, 6, 11.0);
    connect(g, 7, 8, 13.0);
    connect(g, 8, 9, 14.0);
    connect(g, 8, 11, 17.0);
    connect(g, 9, 7, 15.0);
    connect(g, 10, 11, 19.0);
    connect(g, 11, 8, 18.0);

    // Resolvemos el problema para vertices inicial y final de ejemplo
    int beg = 0;
    int end = 11;

    std::cout << "Calculando ruta desde " << beg << " hasta " << end << std::endl;
    
    int* backlinks = find_path(g, beg, end);
    if (backlinks == nullptr) {
        std::cout << "Memoria insuficiente" << std::endl;
        return 1;
    }

    show_route(backlinks, beg, end);

    // Liberacion de memoria
    delete [] backlinks;
    free_graph(g);

    // Todo salio bien
    return 0;
}

// Local variables:
// compile-command: "g++ -std=c++17 -O0 -g3 -Wall -Wextra -Wpedantic -Werror dijkstra.cpp -o dijkstra"
// End:
