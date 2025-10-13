#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

// Algoritmo de Warshall
// Complejidad: O(n^3)
vector<vector<int>> warshall(vector<vector<int>> graph){
    int n = graph.size();
    // Copia del grafo
    vector<vector<int>> reach(n, vector<int>(n, 0));

    for(int k = 0; k < n; k++){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                // Si hay un camino de i a k y de k a j, entonces hay un camino de i a j
                reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j]) || graph[i][j];
            }
        }
    }

    return reach;
}

int main(){
    vector<char> letters = {'A', 'B', 'C', 'D'};

    vector<vector<int>> graph = {
        {0, 1, 0, 0},
        {0, 0, 0, 1},
        {0, 0, 0, 0},
        {1, 0, 1, 0}
    };

    vector<vector<int>> reach = warshall(graph);

    for(int i = 0; i < reach.size(); i++){
        for(int j = 0; j < reach[i].size(); j++){
            cout << "Nodo " << letters[i] << " a nodo " << letters[j] << ": " << (reach[i][j] ? "Si" : "No") << endl;
        }
        
        cout << endl;
    }
}