#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>

using namespace std;

// Algoritmo greedy para knapsack
// Complejidad: O(n log n) por el uso de la priority queue
// Entrada: archivo de items (numero de items, peso maximo, lista de (valor, peso))
// Salida: Se muestra el valor total, peso total y los items seleccionados
void knap(string items){
    ifstream file(items);
    int n;
    int maxW;
    file >> n >> maxW;

    vector<pair<int, int>> item(n);
    for(int i = 0; i < n; ++i){
        file >> item[i].first >> item[i].second;
    }

    file.close();

    // Vector de ratios valor/peso
    vector<double> ratio(n);
    for(int i = 0; i < n; ++i){
        ratio[i] = (double)item[i].first / item[i].second;
    }

    int current_weight = 0;
    int total_value = 0;

    // Priority queue para ordenar por ratio
    priority_queue<pair<double, int>> pq;
    for(int i = 0; i < n; ++i){
        pq.push({ratio[i], i});
    }

    // Mientras haya items y no se exceda el peso maximo
    while(!pq.empty() && current_weight < maxW){
        // Item de mayor ratio
        pair<double, int> top_item = pq.top();
        pq.pop();

        int idx = top_item.second;

        // Si no se excede el peso, lo agrega
        if(current_weight + item[idx].second <= maxW){
            current_weight += item[idx].second;
            total_value += item[idx].first;
        }
    }

    cout << "Solucion greedy para "<< items << ":\n";
    cout << "Valor total: " << total_value << endl;
    cout << "Peso total: " << current_weight << endl;
    cout << "Items seleccionados (v, w):\n";
    
    for(int i = 0; i < n; i++){
        if(current_weight >= item[i].second){
            cout << "(" << item[i].first << ", " << item[i].second << ")" << endl;
            current_weight -= item[i].second;
        }
    }

    cout << endl;
}


int main(){
    knap("f1_l-d_kp_10_269.txt");
    knap("f2_l-d_kp_20_878.txt");
    knap("f3_l-d_kp_4_20.txt");

    return 0;
}