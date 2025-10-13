#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

// Algoritmo de Floyd
// Complejidad: O(n^3)
vector<vector<int>> floyd(vector<vector<int>> graph){
    int n = graph.size();
    vector<vector<int>> dist = graph;

    for(int k = 0; k < n; k++){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                // Verifica que haya un camino de i a k y de k a j
                if(dist[i][k] != INT_MAX && dist[k][j] != INT_MAX){
                    // Si el camino de i a j es más largo que el camino de i a k más el camino de k a j, actualiza
                    if(dist[i][j] > dist[i][k] + dist[k][j]){
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    return dist;
}

vector<vector<int>> floyd_warshall(vector<vector<int>> graph, vector<vector<int>> &dist, vector<vector<int>> &next){
    int n = graph.size();
    dist = graph;
    next = vector<vector<int>>(n, vector<int>(n, -1));

    // Inicializa next buscando los caminos directos
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(graph[i][j] != INT_MAX && i != j){
                next[i][j] = j;
            }
        }
    }

    for(int k = 0; k < n; k++){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                // Verifica que haya un camino de i a k y de k a j
                if(dist[i][k] != INT_MAX && dist[k][j] != INT_MAX){
                    // Si el camino de i a j es más largo que el camino de i a k más el camino de k a j, actualiza
                    if(dist[i][j] > dist[i][k] + dist[k][j]){
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }

    return dist;
}

// Reconstruye el camino de u a v usando la matriz next
vector<int> reconstruir_camino(int inicio, int fin, vector<vector<int>> next){
    // Si no hay camino
    if(next[inicio][fin] == -1){
        return {};
    }

    vector<int> path;
    path.push_back(inicio);

    // Mientras no se haya llegado al nodo final
    while(inicio != fin){
        // Obtiene el siguiente nodo en el camino
        inicio = next[inicio][fin];
        path.push_back(inicio);
    }

    return path;
}

int main(){
    vector<char> letters = {'A', 'B', 'C', 'D'};

    vector<vector<int>> graph = {
        {0, INT_MAX, 3, INT_MAX},
        {2, 0, INT_MAX, INT_MAX},
        {INT_MAX, 6, 0, 1},
        {7, INT_MAX, INT_MAX, 0}
    };

    vector<vector<int>> dist;
    vector<vector<int>> next;

    floyd_warshall(graph, dist, next);

    cout << "Matriz de distancias:" << endl;
    cout << "\t";

    for(int i = 0; i < letters.size(); i++){
        cout << letters[i] << "\t";
    }

    cout << endl;

    for(int i = 0; i < dist.size(); i++){
        cout << letters[i] << "\t";

        for(int j = 0; j < dist[i].size(); j++){    
            cout << dist[i][j] << "\t";
        }
        
        cout << endl;
    }

    cout << endl;

    int inicio = 1; // B
    int fin = 3; // D

    vector<int> camino = reconstruir_camino(inicio, fin, next);
    cout << "Camino mas corto de " << letters[inicio] << " a " << letters[fin] << ": ";
    
    if(camino.empty()){
        cout << "No existe camino.";
    }
    
    else{
        for(int i = 0; i < camino.size(); i++){
            cout << camino[i];

            if(i != camino.size() - 1){
                cout << " -> ";
            }
        }
    }

    cout << endl;
}