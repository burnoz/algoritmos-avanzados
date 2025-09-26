#include <iostream>
#include <stdio.h>

using namespace std;

// Coin change memoization
// Complejidad temporal: O(M*N), donde M es la cantidad objetivo y N es el número de monedas
int coins_memo(int C[], int n, int k, int** T){
    if(k == 0){
        T[n][k] = 1;
        return T[n][k];
    }

    if(n <= 0 || k < 0){
        return 0;
    }

    if(T[n][k] != -1){
        return T[n][k];
    }

    else{
        T[n][k] = coins_memo(C, n-1, k, T) + coins_memo(C, n, k-C[n-1], T);
        return T[n][k];
    }
}


// Coin change tabulacion
// Complejidad temporal: O(M*N), donde M es la cantidad objetivo y N es el número de monedas
int coins_tabu(int C[], int n, int k){
    int T[n + 1][k + 1];

    T[0][0] = 1;

    for(int i = 1; i < n + 1; i++){
        T[i][0] = 1;
    }

    for(int j = 1; j < k + 1; j++){
        T[0][j] = 0;
    }

    for(int i = 1; i < n + 1; i++){
        for(int j = 0; j < k + 1; j++){
            T[i][j] = T[i - 1][j];

            if(j - C[i - 1] >= 0){
                T[i][j] += T[i][j - C[i - 1] ];
            }
        }
    }

    // Imprimir tabla
    // for(int i = 0; i < n + 1; i++){
    //     for(int j = 0; j < k + 1; j++){
    //         cout << T[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    return T[n][k];
}


int main(){
    int C[] = {1, 2, 5, 10};
    int k = 5;
    int n = 4;

    // Memoization
    int** T = new int*[n + 1];
    for(int i = 0; i < n + 1; i++){
        T[i] = new int[k + 1];
        for(int j = 0; j < k + 1; j++){
            T[i][j] = -1;
        }
    }

    cout << "Monedas: ";
    for(int i = 0; i < n; i++){
        cout << C[i] << " ";
    }
    cout << endl;

    cout << "Coin change (memoization) para " << k << ": " << endl; 
    cout << coins_memo(C, n, k, T) << endl;

    cout << "Coin change (tabulation) para " << k << ": " << endl;
    cout << coins_tabu(C, n, k) << endl;

    k = 10;
    for(int i = 0; i < n + 1; i++){
        for(int j = 0; j < k + 1; j++){
            T[i][j] = -1;
        }
    }

    cout << "Coin change (memoization) para " << k << ": " << endl; 
    cout << coins_memo(C, n, k, T) << endl;

    cout << "Coin change (tabulation) para " << k << ": " << endl;
    cout << coins_tabu(C, n, k) << endl;
}