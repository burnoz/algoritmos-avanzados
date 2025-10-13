#include <vector>
#include <queue>
#include <iostream>
#include <fstream>

using namespace std;

// Grafo y num de nodos
pair<int**, int> matriz_grafo(string filename){
    ifstream file(filename);
	int **matrix; 
    
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
        // cout << "N: " << N << endl;

        // Crea la matriz de adyacencia
        matrix = (int **) calloc (N, sizeof(int*));

        for(j = 0; j < N; j++){
            matrix[j] = (int *) calloc(N, sizeof(int));
        }

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

            matrix[a][b] = w;
        }
    }

    // for(int i = 0; i < N; i++){
    //     for(int j = 0; j < N; j++){
    //         cout << matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    return {matrix, N};
}

void bfsWeighted(int **graph, int start, int end, int N){
    queue<int> q;
    q.push(start);

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
            if(graph[u][v] != 0 && !visited[v]){
                visited[v] = true;
                predecesor[v] = u;
                pesos[v] = graph[u][v];
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

            cout << "Recorrido: [";
            for(int j = path.size() - 1; j >= 0; j--){
                cout << path[j];
                if(j != 0){
                    cout << ", ";
                }
            }
            cout << "] ";

            cout << "[";
            for(int j = path.size() - 1; j >= 0; j--){
                cout << pesos[path[j]];
                if(j != 0){
                    cout << ", ";
                }
            }
            cout << "]" << endl;
        }
    }
}


int main(){
    pair<int**, int> aux;
    string num_file;
    int start = 0;
    int end = 5;

    cout << "Recorridos desde el nodo " << start << " hasta el nodo " << end << ":" << endl;
    cout << endl;

    for(int i = 1; i <= 5; i++){
        if(i == 1){
            num_file = "";
        }

        else{
            num_file = "-" + to_string(i);
        }

        string filename = "flow-grafo" + num_file + ".txt";
        cout << "Archivo: " << filename << endl;

        aux = matriz_grafo(filename);
        int **graph = aux.first;
        int N = aux.second;

        bfsWeighted(graph, start, end, N);

        cout << endl;
    }
}