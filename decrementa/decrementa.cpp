#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <numeric>

using namespace std;

// Clase para stack
class Stack{
	private:
		int *data;
		int top;
		int maxSize;

	public:
		Stack(int maxSize){	
			this->maxSize = maxSize;
			this->data = (int *) malloc( this->maxSize * sizeof(int));
			this->top = -1;
		}

		~Stack(){	
			//free(this->data);	
			delete[] this->data;
		}

		void push(int Valor);
		int getTop();
		int pop();
		bool isEmpty();  
		bool isFull();
};

//	solo consulta el dato en top
int Stack::getTop(){	
	int value = this->data[top];
	return value;
}

//	extrae el dato en top
int Stack::pop(){	
	int value = this->data[top];
	this->top--;
	return value;
}

// 	agrega elemento
void Stack::push(int value){	
	this->top++;
	this->data[this->top] = value;
}

//	revisa si la pila esta llena
bool Stack::isFull(){	
	return this->top == (this->maxSize - 1);	
}

//	 revisa si la pila esta vacia
bool Stack::isEmpty(){	
	return this->top == -1;	
}

// Clase para queue
class Queue{
	private:
		int *data;
		int front;
		int end;
		int maxSize;

	public:
		Queue(int maxSize){   
			this->maxSize = maxSize;
			this->data = (int *) malloc( this->maxSize * sizeof(int));
			this->front = -1;
			this->end = -1;
		}

		~Queue(){	
			free(this->data);	
			//	delete[] this->data;
		}

	void enqueue(int Valor);
	int dequeue();
	int getFront();
	bool isEmpty();  
	bool isFull();

};

//	muestra el valor al frente
int Queue::getFront(){	
	return this->data[this->front];  
}

//	extrae elemento del frente
int Queue::dequeue(){	
	int value = this->data[this->front];
	
	if (this->front == this->end){	
		this->front = -1;
		this->end = -1 ;	
	}
	
	else{	
		this->front = (this->front + 1) % this->maxSize;	
	}

	return value;
}

//	agrega elemento
void Queue::enqueue(int value){	
	this->end = (this->end + 1) % this->maxSize;
	this->data[end] = value;

	if (this->front == -1 ){	
		this->front++;
	}
}

//	si esta llena
bool Queue::isFull(){	
	return (end+1) % this->maxSize == front;	
}

//	si esta vacia
bool Queue::isEmpty(){	
	return front == -1;	 
}


void dfsTopo(int u, int N, int **matrix, bool *visited, Stack &stack) {
    visited[u] = true;
    
	for (int v = 0; v < N; v++) {
        if (matrix[u][v] == 1 && !visited[v]) {
            dfsTopo(v, N, matrix, visited, stack);
        }
    }

    stack.push(u);
}

// Ordenamiento topologico
// Complejidad O(V + E)
void topoSort(int N, int **matrix) {
    bool visited[N];
    Stack stack(N);

    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            dfsTopo(i, N, matrix, visited, stack);
        }
    }

    cout << "Orden topologico: ";
    while (!stack.isEmpty()) {
        cout << stack.pop() << " ";
    }

    cout << endl;
}


// Fake coin
// Complejidad O(log n)
int fakeCoin(const vector<int> coins, int offset) {
    int n = coins.size();
    if (n == 1){
		return offset;
	} 

    if (n == 2) {
        if(coins[0] < coins[1]) {
			return offset;
		} 
		
		else {
			return offset + 1;
		}
    }

    int third = n / 3;
    int rem = n % 3;

    int left_end = third;
    int middle_end = 2 * third;

    vector<int> left(coins.begin(), coins.begin() + left_end);
    vector<int> middle(coins.begin() + left_end, coins.begin() + middle_end);
    vector<int> right(coins.begin() + middle_end, coins.end());

    int sumLeft = accumulate(left.begin(), left.end(), 0);
    int sumMiddle = accumulate(middle.begin(), middle.end(), 0);

    if (sumLeft == sumMiddle) {
        return fakeCoin(right, offset + middle_end);
    } 
	
	else if (sumLeft < sumMiddle) {
        return fakeCoin(left, offset);
    } 
	
	else {
        return fakeCoin(middle, offset + left_end);
    }
}

// Quick select, algoritmo de Lomuto
// Complejidad O(n) en promedio, O(n^2) en el peor caso
int lomutoPartition(vector<int>& A, int L, int R) {
	int s = L;
	int pivote = A[L];

	for (int i = L + 1; i <= R; i++) {
		if (A[i] < pivote) {
			s = s + 1;
			swap(A[s], A[i]);
		}
	}

	swap(A[s], A[L]);
	
	return s;
}


int main(int argc, char* argv[]){    
	string sline, line, aux;
	int index;
	int a, b, current = -1;

	int **directed;

	int *start; 

	ifstream inFile("grafo.txt");         //  input file stream
	
	//    Verifica que los archivos se hayan abierto correctamente
	int i = 0;
	int j;
	int N;
	int E;

	// Grafo dirigido
	if(inFile.is_open()){    
		//    Lee linea a linea
		while(getline(inFile, sline)){    
			if(i == 1){   
				//cout << sline << endl;
				line = sline;
				index = line.find(" "); 
				aux = line.substr(0, index);
				N = stoi(aux);//lee cuantos nodos son

				line = line.substr(index+1);
				index = line.find(" ");

				line = line.substr(index+1);
				index = line.find(" ");
				aux = line.substr(0, index);
				E = stoi(aux);

				// cout << "N: "<< N << endl;
				// cout << "E: "<< E << endl;

				directed = (int **) malloc ( N * sizeof(int*) );

				for (j = 0; j < N; j++){     
					directed[j] = (int *) malloc(N * sizeof(int));    
				}

				for (int x = 0; x < N; x++) {
					for (int y = 0; y < N; y++) {
						directed[x][y] = 0;
					}
				}
			}

			else if(i > 1){    
				//cout << sline << endl;

				line = sline;
				index = line.find(" "); 
				aux = line.substr(0, index);
				a = stoi(aux);

				line = line.substr(index+1);
				index = line.find(" "); 
				aux = line.substr(0, index);
				b = stoi(aux);

				// cout << "a y b: "<< a << " " << b << endl;

				//    asignar valores a la matriz
				//    indicando que a es adyacente a b
				directed[a][b] = 1;
			}
			
			i++;
		}
	}

	inFile.close();

	cout << "Matriz del grafo dirigido:" << endl;

	for(i = 0; i < N; i++){	
		for(j = 0; j < N; j++){	
			cout << "["<< i <<","<< j <<"] " << directed[i][j] << "\t\t"; 
		}
		
		cout << endl;
	}
	
	cout << endl;

	topoSort(N, directed);

	cout << endl;

	int n_coins = 8; // Número de monedas
	vector<int> coins(n_coins, 2);

	coins[5] = 1;

	int index_falsa = fakeCoin(coins, 0);
	cout << "La moneda falsa esta en: " << index_falsa << endl;
	cout << endl;


	vector<int> A = {3, 6, 8, 10, 1, 2, 20};
	int k = 6; // 3ero mas pequeno
	int left = 0;
	int right = A.size() - 1;
	int pivotIndex = lomutoPartition(A, left, right);

	while (pivotIndex != k - 1) {
		if (pivotIndex < k - 1) {
			left = pivotIndex + 1;
		} 
		
		else {
			right = pivotIndex - 1;
		}

		pivotIndex = lomutoPartition(A, left, right);
	}

	cout << "El " << k << " elemento mas pequeno es: " << A[pivotIndex] << endl;
}