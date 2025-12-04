#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

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

// Busca la primera mejora posible
map<int, int> firstImprovement(map<int, int>& nodos, const vector<pair<int, int>>& aristas, int &iters_left){
    vector<int> orden_nodos;
    map<int, int> auxiliar = nodos;

    for(const auto& [original, etiqueta] : nodos){
        orden_nodos.push_back(original);
    }

    // Randomiza el orden a cambiar de los nodos
    random_shuffle(orden_nodos.begin(), orden_nodos.end());

    for(int i = 0; i < orden_nodos.size(); i++){
        for(int j = i + 1; j < orden_nodos.size(); j++){
            swap(auxiliar[orden_nodos[i]], auxiliar[orden_nodos[j]]);
            iters_left--;

            if(iters_left < 0) {
                return nodos;
            }

            int currentBandwidth = getMaxBandwidth(getBandwidths(auxiliar, aristas));
            int bestBandwidth = getMaxBandwidth(getBandwidths(nodos, aristas));

            if(currentBandwidth < bestBandwidth){
                return auxiliar; // Retorna la primera mejora encontrada
            }
            
            else{
                swap(auxiliar[orden_nodos[i]], auxiliar[orden_nodos[j]]); // Revertir el cambio
            }
        }
    }

    return nodos; // No se encontro mejora
}

// Busca la mejor mejora posible
map<int, int> bestImprovement(map<int, int>& nodos, const vector<pair<int, int>>& aristas, int &iters_left){
    vector<int> orden_nodos;
    map<int, int> auxiliar = nodos;
    map<int, int> bestMap = nodos;
    int bestBandwidth = getMaxBandwidth(getBandwidths(nodos, aristas));

    for(const auto& [original, etiqueta] : nodos){
        orden_nodos.push_back(original);
    }

    // Randomiza el orden a cambiar de los nodos
    random_shuffle(orden_nodos.begin(), orden_nodos.end());

    for(int i = 0; i < orden_nodos.size(); i++){
        for(int j = i + 1; j < orden_nodos.size(); j++){
            swap(auxiliar[orden_nodos[i]], auxiliar[orden_nodos[j]]);
            iters_left--;

            if(iters_left < 0) {
                return bestMap;
            }
            
            int currentBandwidth = getMaxBandwidth(getBandwidths(auxiliar, aristas));
            
            if(currentBandwidth < bestBandwidth){
                bestBandwidth = currentBandwidth;
                bestMap = auxiliar;
            }
            
            swap(auxiliar[orden_nodos[i]], auxiliar[orden_nodos[j]]); // Revertir el cambio
        }
    }

    return bestMap; // Retorna el mejor mapeo encontrado
}

// Hill climbing
int climb(map<int, int>& nodos, const vector<pair<int, int>>& aristas, int iterations, int option){
    if(option == 1){
        // First Improvement
        map<int, int> nodosSolution = newTags(nodos);
        int bestCase = getMaxBandwidth(getBandwidths(nodosSolution, aristas));
        
        while(iterations > 0){
            map<int, int> newSolution = firstImprovement(nodosSolution, aristas, iterations);
            int newBandwidth = getMaxBandwidth(getBandwidths(newSolution, aristas));
            
            if(newBandwidth < bestCase){
                nodosSolution = newSolution;
                bestCase = newBandwidth;
            }
        }

        return bestCase;
    }

    else{
        // Best Improvement
        map<int, int> nodosSolution = newTags(nodos);
        int bestCase = getMaxBandwidth(getBandwidths(nodosSolution, aristas));
        
        while(iterations > 0){
            map<int, int> newSolution = bestImprovement(nodosSolution, aristas, iterations);
            int newBandwidth = getMaxBandwidth(getBandwidths(newSolution, aristas));
            
            if(newBandwidth < bestCase){
                nodosSolution = newSolution;
                bestCase = newBandwidth;
            }
        }

        return bestCase;
    }
}


// Iterated Local Search
int ils(map<int, int>& nodos, const vector<pair<int, int>>& aristas, int iterations, int gamble, int option){
    map<int, int> bestSolution = newTags(nodos);
    map<int, int> currentSolution = bestSolution;
    int bestBandwidth = getMaxBandwidth(getBandwidths(bestSolution, aristas));
    int currentBandwidth = bestBandwidth;

    for(int i = 0; i < iterations; i++){
        currentBandwidth = climb(currentSolution, aristas, 1000, option);

        if(currentBandwidth < bestBandwidth){
            bestBandwidth = currentBandwidth;
            bestSolution = currentSolution;
        }

        // Reiniciar el proceso aleatorizando
        for(int j = 0; j < gamble; j++){
            currentSolution = newTags(currentSolution);
        }
    }

    return bestBandwidth;
}

int main() {
    vector<string> filenames = {"bcspwr01.txt", "bcspwr02.txt", "bcspwr03.txt", "ash85.txt"};
    int iterations = 250;
    int option = 0;
    int gamble = 10;

    cout << "Climbing con: " << endl;
    cout << "1. First Improvement" << endl;
    cout << "2. Best Improvement" << endl;
    cout << "> ";
    cin >> option;

    for(int i = 0; i < 50; i++){
        for(const string& filename : filenames){
            auto [name, best] = getInfo(filename);
            auto [nodos, aristas] = readFile(filename);

            int result = ils(nodos, aristas, iterations, gamble, option);

            // Agrega el resultado a un archivo de salida
            string filename_out = filename + "_ils_output.txt";
            ofstream output_file(filename_out, ios::app);
            
            output_file << result << endl;
        }
    }
       
    cout << "Ejecucion completada" << endl;

    return 0;
}