#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// Edmonds-Karp (BFS) implementation using capacity matrix
// graph is an NxN matrix of capacities (0 if no edge)
vector<int> bfs_find_path(const vector<vector<int>>& rGraph, int s, int t, vector<int>& parent){
    int N = rGraph.size();
    parent.assign(N, -1);
    vector<bool> visited(N, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for(int v = 0; v < N; v++){
            if(!visited[v] && rGraph[u][v] > 0){
                parent[v] = u;
                visited[v] = true;
                if(v == t){
                    // recontruct path
                    vector<int> path;
                    int cur = t;
                    while(cur != -1){
                        path.push_back(cur);
                        cur = parent[cur];
                    }
                    reverse(path.begin(), path.end());
                    return path;
                }
                q.push(v);
            }
        }
    }
    return {};
}

int edmonds_karp(vector<vector<int>> capacity, int s, int t){
    int N = capacity.size();
    vector<vector<int>> rGraph = capacity; // residual capacities
    int maxFlow = 0;
    vector<int> parent;

    while(true){
        vector<int> path = bfs_find_path(rGraph, s, t, parent);
        if(path.empty()) break;

        // find bottleneck
        int bottleneck = INT_MAX;
        for(int i = 0; i + 1 < (int)path.size(); i++){
            int u = path[i];
            int v = path[i+1];
            bottleneck = min(bottleneck, rGraph[u][v]);
        }

        // update residual graph R: R[u][v] -= bottleneck; R[v][u] += bottleneck
        for(int i = 0; i + 1 < (int)path.size(); i++){
            int u = path[i];
            int v = path[i+1];
            rGraph[u][v] -= bottleneck;
            rGraph[v][u] += bottleneck;
        }

        maxFlow += bottleneck;
    }

    // optional: print residual
    cout << "Residual matrix after max-flow:" << endl;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++) cout << rGraph[i][j] << " ";
        cout << endl;
    }

    return maxFlow;
}

int main(){
    // Ejemplo de matriz de capacidades (dirigido)
    // Nodos: 0 = source, 3 = sink
    // Grafo:
    // 0 -> 1 (3), 0 -> 2 (2)
    // 1 -> 2 (1), 1 -> 3 (2)
    // 2 -> 3 (3)
    vector<vector<int>> capacity = {
        {0,3,2,0},
        {0,0,1,2},
        {0,0,0,3},
        {0,0,0,0}
    };

    int source = 0;
    int sink = 3;

    int maxFlow = edmonds_karp(capacity, source, sink);
    cout << "El flujo maximo es: " << maxFlow << endl;

    return 0;
}