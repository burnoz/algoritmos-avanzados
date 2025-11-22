#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>

using namespace std;

// Devuelve el nombre del problema y el mejor valor conocido
pair<string, int> getInfo(const string& filename){
    ifstream grafo(filename);

    string line;
    string name;
    int best = -1;

    if(getline(grafo, line)) {
        int pos = line.find(": ");

        name = line.substr(pos + 2);
    }

    if(getline(grafo, line)){
        int pos1 = line.find(" ");
        int pos2 = line.find(" ", pos1 + 1);

        best = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
    }

    grafo.close();

    return {name, best};
}

// Devuelve la lista de nodos con una etiqueta (iniciada en orden de aparicion) y 
pair<map<int, int>, vector<pair<int, int>>> readFile(const string& filename){
    ifstream grafo(filename);
    string line;

    map<int, int> nodos;
    vector<pair<int, int>> aristas;
    int numNodos = 1; // Iniciado en 1 para que la primera etiqueta sea 1
    int numAristas = 0;

    // Ignora las primeras dos lineas
    getline(grafo, line);
    getline(grafo, line);

    while(getline(grafo, line)){
        int pos1 = line.find(" ");
        int firstNode = stoi(line.substr(0, pos1));
        int secondNode = stoi(line.substr(pos1 + 1));

        if(nodos.find(firstNode) == nodos.end()){
            nodos[firstNode] = numNodos++;
        }

        if(nodos.find(secondNode) == nodos.end()){
            nodos[secondNode] = numNodos++;
        }

        aristas.push_back({firstNode, secondNode});
        numAristas++;
    }

    grafo.close();

    return {nodos, aristas};
}

// Mezcla las etiquetas de los nodos
map<int, int> newTags(map<int, int>& nodos){
    vector<int> etiquetas;
    
    for(const auto& [original, etiqueta] : nodos){
        etiquetas.push_back(etiqueta);
    }

    random_shuffle(etiquetas.begin(), etiquetas.end());

    map<int, int> nuevoMapa;
    int index = 0;
    
    for(const auto& [original, etiqueta] : nodos){
        nuevoMapa[original] = etiquetas[index++];
    }

    return nuevoMapa;
}

// Obtiene los bandwidths de las aristas dado un mapeo de nodos
vector<int> getBandwidths(const map<int, int>& nodos, const vector<pair<int, int>>& aristas){
    vector<int> bandwidths;

    for(const auto& [node1, node2] : aristas){
        int etiqueta1 = nodos.at(node1);
        int etiqueta2 = nodos.at(node2);

        bandwidths.push_back(abs(etiqueta1 - etiqueta2));
    }

    return bandwidths;
}

// Obtiene el bandwidth maximo
int getMaxBandwidth(const vector<int>& bandwidths){
    int maxBandwidth = 0;

    for(int i = 0; i < bandwidths.size(); i++){
        if(bandwidths[i] > maxBandwidth){
            maxBandwidth = bandwidths[i];
        }
    }

    return maxBandwidth;
}

// Obtiene el bandwidth promedio
int getAvgBandwidth(const vector<int>& bandwidths){
    int sum = 0;

    for(int i = 0; i < bandwidths.size(); i++){
        sum += bandwidths[i];
    }

    return sum / bandwidths.size();
}

// Random sampling (Monte Carlo)
vector<int> gamble(map<int, int>& nodos, const vector<pair<int, int>>& aristas, int iterations){
    int bestCase = INT_MAX;
    int WorstCase = 0;
    int avgBandwidth = 0;
    vector<int> bandwidthResults;

    for(int i = 0; i < iterations; i++){
        map<int, int> nuevasEtiquetas = newTags(nodos);
        vector<int> bandwidths = getBandwidths(nuevasEtiquetas, aristas);
        
        bandwidthResults.insert(bandwidthResults.end(), bandwidths.begin(), bandwidths.end());
        int currentMax = getMaxBandwidth(bandwidths);

        if(currentMax < bestCase){
            bestCase = currentMax;
        }

        if(currentMax > WorstCase){
            WorstCase = currentMax;
        }
    }

    avgBandwidth = getAvgBandwidth(bandwidthResults);

    return {bestCase, WorstCase, avgBandwidth};
}

int main() {
    vector<string> filenames = {"bcspwr01.txt", "bcspwr02.txt", "bcspwr03.txt"};
    int iterations = 1000;

    for(int i = 0; i < filenames.size(); i++){
        string filename = filenames[i];
        auto info = getInfo(filename);
        cout << "Nombre del problema: " << info.first << ", Mejor valor: " << info.second << endl;

        map<int, int> nodos;
        vector<pair<int, int>> aristas;
        auto result = readFile(filename);
        nodos = result.first;
        aristas = result.second;

        vector<int> bandwidthResults = gamble(nodos, aristas, iterations);
        cout << "Resultados despues de " << iterations << " iteraciones:" << endl;
        cout << "Mejor caso: " << bandwidthResults[0] << endl;
        cout << "Peor caso: " << bandwidthResults[1] << endl;
        cout << "Caso promedio: " << bandwidthResults[2] << endl;
        cout << endl;
    }

    return 0;
}