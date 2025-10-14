#include <iostream>
#include <vector>

using namespace std;

// Algoritmo greedy basico para colorear un grafo
// Devuelve un vector con el color asignado a cada nodo y el numero de colores usados
pair<vector<int>, int> greedy_coloring(const vector<vector<int>>& graph){
    int n = graph.size();
    vector<int> color(n, -1); // -1: sin color
    int k = 0; // numero de colores usados

    // Para cada nodo
    for(int u = 0; u < n; u++) {
        vector<bool> used(n, false); // colores usados por vecinos

        // Para cada vecino del nodo
        for(int v = 0; v < n; v++){
            // Si hay arista y el vecino ya tiene color, marca ese color como usado
            if(graph[u][v] && color[v] != -1){
                used[color[v]] = true;
            }
        }
        
        // Busca el color mas pequeño no usado
        int c;
        
        for(c = 0; c < n; c++) {
            if(!used[c]){
                break;
            }
        }
        
        color[u] = c;
        // Si es un nuevo color, aumenta k
        if(c == k){
            k++;
        }
    }

    return {color, k};
}

int main() {
    vector<vector<int>> graph = {
        {0,1,1,1,0}, // A
        {1,0,1,0,1}, // B
        {1,1,0,1,1}, // C
        {1,0,1,0,1}, // D
        {0,1,1,1,0}  // E
    };

    vector<char> names = {'A','B','C','D','E'};
    // Colores de catppuccin ᓚᘏᗢ
    vector<string> colores = {"Lavender", "Sapphire", "Mauve", "Flamingo", "Rosewater", "Maroon"};

    pair<vector<int>, int> result = greedy_coloring(graph);
    
    vector<int> color = result.first;
    int k = result.second;
    
    cout << "Colores asignados:" << endl;
    
    for(int i = 0; i < color.size(); i++){
        cout << names[i] << ": " << color[i] << " (Color: " << colores[color[i]] << ")" << endl;
    }

    cout << endl;
    
    cout << "Colores usados: " << k << endl;
    
    return 0;
}