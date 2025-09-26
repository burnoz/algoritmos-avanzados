#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// Algoritmo greedy para coin collecting
// Complejidad: O(n), donde n es el tamaño del grid (n x n)
// Entrada: archivo de mapa (n, grid de monedas)
// Salida: Se muestra el camino tomado y el total de monedas recogidas
void coin_collecting(string map){
    ifstream file(map);
    int n;
    file >> n;
    
    vector<vector<int>> grid(n, vector<int>(n));
    
    // Grid de monedas
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            file >> grid[i][j];
        }
    }
    
    file.close();

    int x = 0, y = 0;
    int total = grid[0][0];
    vector<string> path;
    vector<int> values;

    // Mientras no se llegue a la esquina inferior derecha
    while(x < n-1 || y < n-1){
        int down;
        int right;

        //  Si se puede mover abajo, toma el valor de esa celda, si no, usa -1
        if(x < n-1){
            down = grid[x+1][y];
        }

        else{
            down = -1;
        }

        // Si se puede mover a la derecha, toma el valor de esa celda, si no, usa -1
        if(y < n-1){
            right = grid[x][y+1];
        }

        else{
            right = -1;
        }

        // Avanza en la direccion que tenga el mayor valor
        if(down > right){
            x++;
            path.push_back("abajo");
            values.push_back(grid[x][y]);
            total += grid[x][y];
        } 
        
        else{
            y++;
            path.push_back("derecha");
            values.push_back(grid[x][y]);
            total += grid[x][y];
        }
    }

    cout << "Solucion greedy para " << map << ":\n";
    
    for(size_t i = 0; i < path.size(); ++i){
        cout << path[i] << " (" << values[i] << ")";
        
        if(i < path.size() - 1){
            cout << ", ";
        } 
    }
    
    cout << "\nTotal = " << total << endl;
    cout << endl;
}


int main(){
    coin_collecting("coins-n5.txt");
    coin_collecting("coins-n10.txt");
    coin_collecting("coins-n20.txt");
    coin_collecting("coins-n100.txt");
    
    return 0;
}   