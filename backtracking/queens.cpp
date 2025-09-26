#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>

using namespace std;

// Clase para representar soluciones parciales
class PartialSolution{
public:
	int n; // Tamaño del tablero
	int* queens; // queens[i] = columna de la reina en la fila i, -1 si no está colocada
	int row; // Siguiente fila a colocar

	// Constructor
	PartialSolution(int size){
		n = size;
		queens = new int[n];
		
		for(int i = 0; i < n; ++i){
			queens[i] = -1;
		} 
		
		row = 0;
	}

	// Constructor de copia
	PartialSolution(const PartialSolution& other) {
		n = other.n;
		queens = new int[n];

		for (int i = 0; i < n; i++){
			queens[i] = other.queens[i];
		}

		row = other.row;
	}

	// Destructor
	~PartialSolution() {
		delete[] queens;
	}
};

// Verifica si es seguro colocar una reina en la fila 'row' y columna 'col'
// Complejidad: O(n)
bool isSafe(const PartialSolution& sol, int row, int col){
	for(int i = 0; i < row; i++){
		// Revisa columna y diagonales
		if(sol.queens[i] == col || abs(sol.queens[i] - col) == row - i){
			return false;
		}
	}

	return true;
}

// Instancia hijos de una solución parcial
// Complejidad: O(n^2), para cada elemento se usa isSafe que es O(n)
void instantiate(const PartialSolution& parent, vector<PartialSolution>& children){
	int n = parent.n;
	int row = parent.row;

	// Para cada columna en la fila actual
	for(int col = 0; col < n; col++){
		// Verifica si es seguro colocar la reina
		if(isSafe(parent, row, col)){
			PartialSolution child = parent;
			// Coloca la reina y avanza a la siguiente fila
			child.queens[row] = col;
			child.row = row + 1;
			children.push_back(child);
		}
	}
}

int main() {
	int n = 8;
	PartialSolution root(n);
	vector<PartialSolution> solutions;

	// Queue de soluciones parciales
	queue<PartialSolution> queue_vec;
	queue_vec.push(root);

	while (!queue_vec.empty()){
		PartialSolution sol = queue_vec.front();
		queue_vec.pop();

		// Si es solución completa, la guarda
		if(sol.row == n){
			solutions.push_back(sol);
			continue;
		}

		vector<PartialSolution> children;
		instantiate(sol, children);
		
		for(int i = 0; i < children.size(); i++){
			queue_vec.push(children[i]);
		}
	}

	// Mostrar soluciones en formato tablero
	cout << "Total soluciones: " << solutions.size() << endl;
	
	for(int i = 0; i < solutions.size(); i++){
		cout << "\nSolucion " << i + 1 << ":\n";
		
		for(int fila = 0; fila < n; fila++){
			for(int col = 0; col < n; col++){
				if(solutions[i].queens[fila] == col){
					cout << "R ";
				}
				
				else{
					cout << "- ";
				}
			}
			
			cout << endl;
		}
	}

	return 0;
}