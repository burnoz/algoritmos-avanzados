#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>

using namespace std;

class PartialSolution{
public:
    int x; // coord x
    int y; // coord y
    int total; // suma de monedas recogidas
    vector<string> path; // direcciones tomadas
    vector<int> values; // valores recogidos
    int upperbound; // upperbound

    PartialSolution(){
        x = 0; 
        y = 0;
        total = 0;
        path = vector<string>();
        values = vector<int>();
        upperbound = 0;
    }

    PartialSolution(const PartialSolution& other){
        x = other.x;
        y = other.y;
        total = other.total;
        path = other.path;
        values = other.values;
        upperbound = other.upperbound;
    }
};

// Funcion objeto para comparar upperbounds en la priority_queue
class CompareUpperbound {
public:
    bool operator()(const PartialSolution& a, const PartialSolution& b) const{
        return a.upperbound < b.upperbound; // mayor upperbound tiene mas prioridad
    }
};

// Calcula upperbound desde (x, y) sumando el maximo posible en cada movimiento
int calc_upperbound(const vector<vector<int>>& grid, int x, int y, int total){
    int n = grid.size();
    int ub = total;
    int i = x, j = y;
    
    // Mientras aun no se llegue a la esquina inferior derecha
    while(i < n-1 || j < n-1){
        int down; 
        int right;
        
        // Si se puede mover abajo, toma el valor de esa celda; si no, usa -1
        if(i < n-1){
            down = grid[i+1][j];
        } 
        
        else{
            down = -1;
        }

        // Si se puede mover a la derecha, toma el valor de esa celda; si no, usa -1
        if(j < n-1){
            right = grid[i][j+1];
        } 
        
        else{
            right = -1;
        }
        
        // Suma el mayor valor posible en el siguiente movimiento
        ub += max(down, right);
        
        // Avanza en la dirección que tenga el mayor valor
        if(down > right){
            i++;
        } 
        
        else{
            j++;
        }
    }
    
    // cout << "Upperbound desde (" << x << ", " << y << ") con total " << total << " = " << ub << endl;

    return ub;
}

// Coin collecting por Branch and Bound
void coins(string map){
    // Leer archivo
    ifstream file(map);
    int n;
    file >> n;
    
    vector<vector<int>> grid(n, vector<int>(n));
    
    for (int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            file >> grid[i][j];
        }
    }

    file.close();

    // Fila de prioridad para manejar las soluciones parciales
    priority_queue<PartialSolution, vector<PartialSolution>, CompareUpperbound> pq;

    PartialSolution root;
    pq.push(root);

    int best_total = -1;
    vector<string> best_path;
    vector<int> best_values;

    while(!pq.empty()){
        PartialSolution part_sol = pq.top(); 
        pq.pop();
        
        // Si se llego a la esquina inferior derecha, actualiza la mejor solucion si es necesario
        if(part_sol.x == n-1 && part_sol.y == n-1){
            if(part_sol.total > best_total){
                best_total = part_sol.total;
                best_path = part_sol.path;
                best_values = part_sol.values;
            }

            continue;
        }
        
        // Abajo
        if(part_sol.x < n-1){
            PartialSolution child = part_sol;
            child.x++;
            child.total += grid[child.x][child.y];
            child.path.push_back("abajo");
            child.values.push_back(grid[child.x][child.y]);
            child.upperbound = calc_upperbound(grid, child.x, child.y, child.total);
            
            if(child.upperbound > best_total){
                pq.push(child);
            }
        }

        // Derecha
        if(part_sol.y < n-1){
            PartialSolution child = part_sol;
            child.y++;
            child.total += grid[child.x][child.y];
            child.path.push_back("derecha");
            child.values.push_back(grid[child.x][child.y]);
            child.upperbound = calc_upperbound(grid, child.x, child.y, child.total);
            
            if(child.upperbound > best_total){
                pq.push(child);
            }
        }
    }

    // Mostrar solución optima
    cout << "Solucion optima para " << map << endl;
    
    for(int i = 0; i < best_path.size(); ++i){
        cout << best_path[i] << " (" << best_values[i] << ")";
        
        if(i < best_path.size() - 1){   
            cout << ", ";
        }
    }

    cout << "\nTotal = " << best_total << endl;
    cout << endl;
}

int main(){
    coins("coins-n5.txt");
    coins("coins-n7.txt");
    coins("coins-n8.txt");
    coins("coins-n9.txt");
    coins("coins-n10.txt");
    coins("coins-n12.txt");

    return 0;
}
