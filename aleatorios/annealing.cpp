#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <cstdlib>

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

// Simulated annealing
int annealing(map<int, int>& nodos, const vector<pair<int, int>>& aristas, int iterations, double initialTemp, double coolingRate){
    map<int, int> bestSolution = newTags(nodos);
    int bestBandwidth = getMaxBandwidth(getBandwidths(bestSolution, aristas));

    for(int i = 0; i < iterations; i++){
        map<int, int> newSolution = newTags(bestSolution);
        int newBandwidth = getMaxBandwidth(getBandwidths(newSolution, aristas));

        if(newBandwidth < bestBandwidth){
            bestSolution = newSolution;
            bestBandwidth = newBandwidth;
        }

        else{
            double acceptanceProb = exp((bestBandwidth - newBandwidth) / initialTemp);
            double randomProb = (double) rand() / RAND_MAX;

            if(randomProb < acceptanceProb){
                bestSolution = newSolution;
                bestBandwidth = newBandwidth;
            }
        }

        initialTemp *= coolingRate;
    }

    return bestBandwidth;
}

// Estima una temperatura inicial adecuada
double estimateInitialTemp(map<int, int>& nodos, const vector<pair<int, int>>& aristas, int samples = 100){
    vector<int> costs;
    
    for(int i = 0; i < samples; i++){
        map<int, int> sol = newTags(nodos);
        int cost = getMaxBandwidth(getBandwidths(sol, aristas));
        costs.push_back(cost);
    }
    
    // Calcular desviación estándar
    double mean = 0;
    for(int c : costs){
        mean += c;
    }
    
    mean /= costs.size();
    
    double variance = 0;
    for(int c : costs){
        variance += (c - mean) * (c - mean);
    }
    
    variance /= costs.size();
    
    double stddev = sqrt(variance);

    return stddev * 1.5;
}

int main(){
    vector<string> filenames = {"bcspwr01.txt", "bcspwr02.txt", "bcspwr03.txt", "ash85.txt"};

    for(const auto& filename : filenames){
        auto [nodos, aristas] = readFile(filename);
        
        // Estimar temperatura inicial óptima
        double temp = estimateInitialTemp(nodos, aristas);
        
        for(int i = 0; i < 50; i++){
            int result = annealing(nodos, aristas, 250000, temp, 0.95);
            
            string filename_out = filename + "_annealing_output.txt";
            ofstream output_file(filename_out, ios::app);

            output_file << result << endl;
        }
    }

    cout << "Ejecucion completada" << endl;

    return 0;
}