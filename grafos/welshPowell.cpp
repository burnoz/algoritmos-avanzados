#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Node{
public:
    char value;
    int grado;
    int color;
    vector<int> vecinos;

    Node(char value){
        this->value = value;
        this->grado = 0;
        this->color = -1; // -1: sin color
        this->vecinos = {};
    }
};

// Funcion objeto para ordenar nodos por grado ascendente
class CompareNode{
public:
    bool operator()(Node const& n1, Node const& n2){
        return n1.grado > n2.grado;
    }
};

// Funcion para verificar si un nodo ya fue procesado
bool find_node(vector<Node> vec, Node value){
    for(int i = 0; i < vec.size(); i++){
        if(vec[i].value == value.value){
            return true;
        }
    }

    return false;
}

pair<vector<int>, int> welsh_powell(const vector<vector<int>>& graph){
    // Crea nodos y calcula grados
    int n = graph.size();
    vector<Node> nodes;
    vector<Node> procesados = {};
    vector<char> names = {'A','B','C','D','E'};

    for(int i = 0; i < n; i++){
        Node nodo(names[i]);
        nodes.push_back(nodo);
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(graph[i][j] == 1){
                nodes[i].grado++;
                nodes[i].vecinos.push_back(j);
            }
        }
    }

    // Ordena nodos por grado ascendente
    priority_queue<Node, vector<Node>, CompareNode> pq;

    for(int i = 0; i < n; i++){
        pq.push(nodes[i]);
    }

    // priority_queue<Node, vector<Node>, CompareNode> temp_pq = pq;

    // while(!temp_pq.empty()){
    //     Node u = temp_pq.top();
    //     temp_pq.pop();
    //     cout << u.value << " (grado: " << u.grado << ")" << endl;
    // }

    vector<int> color(n, -1); // -1: sin color
    int k = 0; // color actual
    int num_colors = 0;

    while(!pq.empty()){
        Node u = pq.top();
        pq.pop();

        if(find_node(procesados, u)){
            continue;
        }

        u.color = k;
        color[u.value - 'A'] = k; // Asigna color al nodo

        // Por cada nodo que no es vecino de u y no ha sido procesado
        for(int i = 0; i < n; i++){
            if(i == (u.value - 'A') || find_node(procesados, nodes[i])){
                continue;
            }

            bool esVecino = false;

            for(int vecino = 0; vecino < u.vecinos.size(); vecino++){
                if(u.vecinos[vecino] == i){
                    esVecino = true;
                    break;
                }
            }

            if(!esVecino){
                nodes[i].color = k;
                color[i] = k; // Asigna color al nodo
                procesados.push_back(nodes[i]);
            }
        }

        procesados.push_back(u);
        k++; // Incrementa el color para el siguiente conjunto
    }

    return {color, k};
}

int main(){
    // Ejemplo: grafo de la imagen
	// A=0, B=1, C=2, D=3, E=4
	vector<vector<int>> graph = {
		{0,1,1,1,0}, // A
		{1,0,1,0,1}, // B
		{1,1,0,1,1}, // C
		{1,0,1,0,1}, // D
		{0,1,1,1,0}  // E
	};

    vector<char> names = {'A','B','C','D','E'};
    // Colores de catppuccin
    vector<string> colores = {"Lavender", "Sapphire", "Mauve", "Flamingo", "Rosewater", "Maroon"};

    pair<vector<int>, int> res = welsh_powell(graph);
    vector<int> color = res.first;
    int k = res.second;

    cout << "Colores asignados:" << endl;
    
    for(int i = 0; i < color.size(); i++){
        cout << names[i] << ": " << color[i] << " (Color: " << colores[color[i]] << ")" << endl;
    }

    cout << "Colores usados: " << k << endl;

    return 0;
}