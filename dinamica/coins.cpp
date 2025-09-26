#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>

using namespace std;

// Coin collection problem por tabulacion
// Complejidad O(n^2)
// Entrada: archivo con cuadricula
// Salida: cantidad maxima de monedas recolectadas
int coinCollection(string file){
    ifstream grid(file);
    if (!grid) {
        cerr << "Error al intentar abrir: " << file << endl;
        return -1;
    }

    int n;
    grid >> n;

    int coins[n][n];

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            grid >> coins[i][j];
        }
    }

    grid.close();

    // Imprimir cuadricula
    // for(int i = 0; i < n; i++){
    //     for(int j = 0; j < n; j++){
    //         cout << coins[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // Matriz para resultados
    int T[n][n];

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int arriba, izquierda;

            if(i > 0){
                arriba = T[i-1][j];
            }
            
            // Primera fila
            else{
                arriba = 0;
            }

            if(j > 0){
                izquierda = T[i][j-1];
            }
            
            // Primera columna
            else{
                izquierda = 0;
            }

            // Casilla actual + max(arriba, izquierda)
            T[i][j] = max(arriba, izquierda) + coins[i][j];
        }
    }

    // Imprimir matriz
    // for(int i = 0; i < n; i++){
    //     for(int j = 0; j < n; j++){
    //         cout << T[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    
    // cout << T[n-1][n-1] << endl;
    
    // Cantidad maxima
    return T[n-1][n-1];
}

int main(){
    cout << "Monedas maximas para gold-n5.txt: " << coinCollection("gold-n5.txt") << endl;
    cout << "Monedas maximas para gold-n10.txt: " << coinCollection("gold-n10.txt") << endl;
    cout << "Monedas maximas para gold-n20.txt: " << coinCollection("gold-n20.txt") << endl;
    cout << "Monedas maximas para gold-n100.txt: " << coinCollection("gold-n100.txt") << endl;
    
    return 0;
}