#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

class Graph{
public:
    int N;
    int **matrix;

    Graph(int N){
        this->N = N;
        this->matrix = (int **) calloc (N, sizeof(int*));

        for(int j = 0; j < N; j++){
            this->matrix[j] = (int *) calloc(N, sizeof(int));
        }
    }
};

Graph matriz_grafo(string filename){
    ifstream file(filename);
    Graph graph(0);
    string line;
	int index;
	int a = -1; 
    int b = -1;
    int i = 0;
	int j;
	int N;

    if(file.is_open()){
        getline(file, line);
        N = stoi(line);
    
        graph = Graph(N);

        // Lee cada linea del archivo
        while(getline(file, line)){
            index = line.find(" ");
            a = stoi(line.substr(0, index));
            line = line.substr(index + 1);
            index = line.find(" ");
            b = stoi(line.substr(0, index));
            line = line.substr(index + 1);
            int w = stoi(line);

            // cout << "a: " << a << ", b: " << b << ", w: " << w << endl;

            graph.matrix[a][b] = w;
        }
    }

    // for(int i = 0; i < graph.N; i++){
    //     for(int j = 0; j < graph.N; j++){
    //         cout << graph.matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    return graph;
}

// Busca si hay un camino del nodo start al nodo end
pair<vector<int>, vector<int>> bfs(Graph graph, int start, int end){
    queue<int> q;
    q.push(start);

    int N = graph.N;

    vector<bool> visited(N, false);
    vector<int> predecesor(N, -1);
    vector<int> pesos(N, 0);
    vector<int> path;

    visited[start] = true;

    while(!q.empty()){
        int u = q.front();
        q.pop();

        // Si se llego al nodo final, termina
        if(u == end){
            break;
        }

        for(int v = 0; v < N; v++){
            // Si hay arista y no ha sido visitado
            if(graph.matrix[u][v] != 0 && !visited[v]){
                visited[v] = true;
                predecesor[v] = u;
                pesos[v] = graph.matrix[u][v];
                q.push(v);
            }
        }
    }

    for(int i = 0; i < N; i++){
        if(i == end){
            int pos = i;

            while(pos != -1){
                path.push_back(pos);
                pos = predecesor[pos];
            }

            break;
        }
    }

    if(path.size() == 1){
        return {{}, {}};
    }

    // for(int i = 0; i < N; i++){
    //     if(i == end){
    //         cout << "Recorrido: [";
    //         for(int j = path.size() - 1; j >= 0; j--){
    //             cout << path[j];
    //             if(j != 0){
    //                 cout << ", ";
    //             }
    //         }
    //         cout << "] ";

    //         cout << "[";
    //         for(int j = path.size() - 1; j >= 0; j--){
    //             cout << pesos[path[j]];
    //             if(j != 0){
    //                 cout << ", ";
    //             }
    //         }
    //         cout << "]" << endl;
    //     }
    // }

    return {path, pesos};
}

// Encuentra el peso minimo en el path
int findMin(vector<int> pesos, vector<int> path){
    int minPeso = __INT_MAX__;

    for(int i = path.size() - 1; i > 0; i--){
        int u = path[i];
        int v = path[i - 1];

        if(pesos[v] < minPeso){
            minPeso = pesos[v];
        }
    }

    return minPeso;
}

// Algoritmo de Edmon-Karp, devuelve max flow
int edmonKarp(Graph graph, int inicio, int fin){
    Graph rGraph = graph; // residual R
    int maxFlow = 0;
    int bottleneck;

    while(true){
        pair<vector<int>, vector<int>> result = bfs(rGraph, inicio, fin);
        vector<int> path = result.first;
        vector<int> pesos = result.second;

        // Si no hay camino, termina
        if(path.size() == 0){
            break;
        }

        bottleneck = findMin(pesos, path);

        for(int i = path.size() - 1; i > 0; i--){
            int u = path[i];
            int v = path[i - 1];
            // Path augmentation
            rGraph.matrix[u][v] -= bottleneck;
            rGraph.matrix[v][u] += bottleneck;
        }

        maxFlow += bottleneck;
    }

    return maxFlow;
}

int main(){
    string baseName = "flow-grafo";
    string filename;

    for(int i = 1; i < 6; i++){
        string filename;
        
        if(i == 1){
            filename = baseName + ".txt";
        }

        else{
            filename = baseName + "-" + to_string(i) + ".txt";
        }

        cout << filename << ": " << endl;

        Graph graph = matriz_grafo(filename);
        int maxFlow = edmonKarp(graph, 0, graph.N - 1);

        cout << "Max Flow -> " << maxFlow << endl;
        cout << endl;
    }

    return 0;
}