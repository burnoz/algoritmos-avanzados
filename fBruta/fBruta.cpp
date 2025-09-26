#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

// Closest pair leyendo un archivo de puntos
// Complejidad O(n^2)
// Entrada: archivo con numero de puntos y sus coordenadas
// Salida: imprime las coordenadas de los dos puntos mas cercanos y su distancia
void ClosestPair(string file){
    ifstream infile(file);
    if (!infile) {
        cerr << "Error al intentar abrir: " << file << endl;
        return;
    }

    int n;
    infile >> n;

    cout << "Numero de puntos a procesar: " << n << endl;

    double min_distance = __DBL_MAX__;
    double coords[n][2];
    int point1;
    int point2;

    for (int i = 0; i < n; i++) {
        infile >> coords[i][0] >> coords[i][1];
    }

    infile.close();

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dx = coords[i][0] - coords[j][0];
            double dy = coords[i][1] - coords[j][1];
            double distance = sqrt(dx * dx + dy * dy);

            if (distance < min_distance) {
                min_distance = distance;
                point1 = i;
                point2 = j;
            }
        }
    }

    cout << "Par: (" << coords[point1][0] << ", " << coords[point1][1] 
         << ") , (" << coords[point2][0] << ", " << coords[point2][1] 
         << ")" << endl << "Distancia: " <<  min_distance << endl << endl;
}


// String matching leyendo un archivo de patron y texto
// Complejidad O(n*m), debe recorrer el largo n del texto y m del patron para comprobar
// Entrada: archivo de patrones y archivo de texto
// Salida: imprime las posiciones de los patrones encontrados
void StringMatch(string patrones, string texto) {
    ifstream patronesFile(patrones);
    ifstream textoFile(texto);

    if (!patronesFile || !textoFile) {
        cerr << "Error al intentar abrir los archivos" << endl;
        return;
    }

    string textoContent;
    // Guarda todo el contenido del texto en una sola cadena
    getline(textoFile, textoContent, '\0');

    string patron;
    while (getline(patronesFile, patron)) {
        for (int i = 0; i <= textoContent.length() - patron.length(); i++) {
            int j;
            
            for (j = 0; j < patron.length(); j++) {
                if (textoContent[i + j] != patron[j]) {
                    break;
                }
            }

            if (j == patron.length()) {
                cout << "'" << patron << "' en: " 
                     << i << " hasta " << i + patron.length() << endl;
            }
        }
    }
}

// Permutaciones en orden lexicografico
// Complejidad O(n!)
// Entrada: n numero de elementos a permutar
// Salida: imprime todas las permutaciones
vector<vector<int>> permutacionesLex(int n) {
    // Permutacion base
    vector<int> p(n);
    for (int i = 0; i < n; i++) {
        p[i] = i + 1;
    }

    vector<vector<int>> L;
    L.push_back(p);

    while(true){
        int i = n - 2;
        // i mayor tal que p[i] < p[i + 1]
        while(i >= 0 && p[i] >= p[i + 1]){
            i--;
        }
        
        if(i < 0){
            break;
        }

        int j = n - 1;
        // j mayor tal que p[j] > p[i]
        while(p[j] <= p[i]){
            j--;
        }

        int aux = p[i];
        p[i] = p[j];
        p[j] = aux;

        int left = i + 1;
        int right = n - 1;
        while(left < right){
            aux = p[left];
            p[left] = p[right];
            p[right] = aux;
            left++;
            right--;
        }

        L.push_back(p);
    }

    return L;
}

// Generacion de subsets
// Complejidad O(2^n)
// Entrada: n numero de elementos
// Salida: imprime todos los subsets binarios de n elementos
vector<vector<int>> powerSetBin(int n){
    // Caso base
    if(n == 1) {
        return {{0}, {1}};
    }

    // Caso recursivo
    else{
        vector<vector<int>> L1 = powerSetBin(n - 1);
        vector<vector<int>> L2;

        for(int i = 0; i < L1.size(); i++) {
            vector<int> subset = L1[i];
            subset.push_back(0);
            L2.push_back(subset);
        }

        for(int i = 0; i < L1.size(); i++) {
            vector<int> subset = L1[i];
            subset.push_back(1);
            L2.push_back(subset);
        }

        return L2;
    }
}


void generar_combinaciones(int longitud_actual, string caracteres_posibles, string combinacion_actual, string password) {
    if (longitud_actual == 0) {
        // Para visualizar la combinacion actual (llena la terminal :b)
        // cout << combinacion_actual << endl;

        if (combinacion_actual == password) {
            cout << "Password encontrada: " << combinacion_actual << endl;
        }

        return;
    }

    for(int i = 0; i < caracteres_posibles.size(); i++) {
        string nueva_combinacion = combinacion_actual + caracteres_posibles[i];
        generar_combinaciones(longitud_actual - 1, caracteres_posibles, nueva_combinacion, password);
    }
}

// Ataque de fuerza bruta a una password
// Complejidad O(m^n), donde m es el numero de caracteres posibles y n es la longitud de la password
// Entrada: longitud maxima de la password, caracteres posibles, password a encontrar
// Imprime la password encontrada
void fuerza_bruta_password(int longitud_maxima, const string caracteres_posibles, const string password) {
    for(int longitud = 1; longitud <= longitud_maxima; longitud++) {
        generar_combinaciones(longitud, caracteres_posibles, "", password);
    }
}


int main() {
    cout << "Closest pair" << endl;
    ClosestPair("puntos-n10.txt");
    ClosestPair("puntos-n11.txt");
    ClosestPair("puntos-n15.txt");
    ClosestPair("puntos-n20.txt");
    ClosestPair("puntos-n50.txt");
    ClosestPair("puntos-n100.txt");

    cout << "String matching" << endl;
    string patrones = "string-matching-Patrones.txt";
    string texto = "string-matching-Texto.txt";
    StringMatch(patrones, texto);
    cout << endl;

    cout << "Permutaciones en orden lexicografico" << endl;
    int n = 3;
    vector<vector<int>> permutaciones = permutacionesLex(n);
    for(int i = 0; i < permutaciones.size(); i++) {
        cout << "[ ";
        
        for(int j = 0; j < permutaciones[i].size(); j++) {
            cout << permutaciones[i][j] << " ";
        }
        
        cout << "]" << endl;
    }
    
    cout << endl;

    cout << "Subsets" << endl;
    vector<vector<int>> subsets = powerSetBin(n);
    for(int i = 0; i < subsets.size(); i++) {
        cout << "[ ";
        
        for(int j = 0; j < subsets[i].size(); j++) {
            cout << subsets[i][j] << " ";
        }
        
        cout << "]" << endl;
    }

    cout << endl;
    
    cout << "Ataque de fuerza bruta a una password" << endl;
    string password = "ab2c1";
    string caracteres_posibles = "abc12";
    int longitud_maxima = password.length();
    fuerza_bruta_password(longitud_maxima, caracteres_posibles, password);
}
