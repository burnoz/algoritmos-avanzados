#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Amigo{
public:
    string nombre;
    string posicion;
    int pos_vector;
    int velocidad;

    Amigo(string nombre, string posicion, int velocidad){
        this->nombre = nombre;
        this->posicion = posicion;
        this->velocidad = velocidad;
    }
};


int find_in_vector(vector<string> vec, string value){
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] == value){
            return i;
        }
    }

    return -1;
}

void Dijkstra(int **graph, int start, int N, vector<Amigo> amigos, vector<string> city_positions){
    vector<int> dist(N, INT_MAX);
    vector<int> prev(N, -1);
    vector<bool> visited(N, false);
    dist[start] = 0;

    for(int count = 0; count < N - 1; count++){
        int min_dist = INT_MAX;
        int min_index;

        for(int v = 0; v < N; v++){
            if(!visited[v] && dist[v] <= min_dist){
                min_dist = dist[v];
                min_index = v;
            }
        }

        visited[min_index] = true;

        for(int v = 0; v < N; v++){
            if(!visited[v] && graph[min_index][v] && dist[min_index] != INT_MAX && dist[min_index] + graph[min_index][v] < dist[v]){
                dist[v] = dist[min_index] + graph[min_index][v];
                prev[v] = min_index;
            }
        }
    }

    double max_time = 0;

    cout << "Recorridos de los amigos:" << endl;
    for(int i = 0; i < amigos.size(); i++){
        int amigo_pos = amigos[i].pos_vector;
        double tiempo = (double)dist[amigo_pos] / amigos[i].velocidad;
        cout << amigos[i].nombre << ": " << dist[amigo_pos] << " metros recorridos en " << tiempo << " minutos." << endl;
        
        if(tiempo > max_time){
            max_time = tiempo;
        }

        cout << "Recorrido:" << endl;
        vector<int> path;
        vector<int> pesos;
        int pos = amigo_pos;

        while(pos != -1){
            path.push_back(pos);

            if(prev[pos] != -1){
                pesos.push_back(graph[prev[pos]][pos]);
            }

            pos = prev[pos];
        }

        for(int j = 0; j < path.size(); j++){
            cout << city_positions[path[j]];

            if(j != path.size() - 1){
                cout << " --" << pesos[j] << "--> ";
            }
        }

        cout << endl;
        cout << endl;
    }

    int horas = (int)max_time / 60;
    int minutos = (int)max_time % 60;
    cout << "Hora de llegada de todos: " << 17 + horas << ":" << (minutos < 10 ? "0" : "") << minutos << " PM" << endl;
}


int main(){
    ifstream grid("city_2020.txt");

    // Variables para leer los archivos
    string line;
    int k = 0;

    // Lista de posiciones de la ciudad
    vector<string> city_positions;

    // Variables para la matriz de adyacencia de la ciudad
    string coord;
    string aux;
    int i = 0;
    int **city_matrix;
    int j;
    int a;
    int b;
    int N = 800;

    // Matriz de adyacencia para representar la ciudad
    if(grid.is_open()){
        // Crea la matriz de adyacencia
        city_matrix = (int **) calloc (N, sizeof(int*));

        for(j = 0; j < N; j++){
            city_matrix[j] = (int *) calloc(N, sizeof(int));
        }

        // Lee cada linea del archivo de la ciudad
        while(getline(grid, coord)){
            // Obtiene los nodos y el peso
            int index = coord.find(")");
            aux = coord.substr(0, index + 1);
            // cout << "aux: " << aux << endl;
            
            coord = coord.substr(index + 1);
            index = coord.find(")");
            string aux2 = coord.substr(1, index);
            // cout << "aux2: " << aux2 << endl;

            coord = coord.substr(index + 2);
            int w = stoi(coord);
            // cout << "w: " << w << endl;
        
            if(find_in_vector(city_positions, aux) == -1){
                city_positions.push_back(aux);
            }

            if(find_in_vector(city_positions, aux2) == -1){
                city_positions.push_back(aux2);
            }

            a = find_in_vector(city_positions, aux);
            b = find_in_vector(city_positions, aux2);

            // Agrega el peso entre los nodos a la matriz
            city_matrix[a][b] = w;
            // Grafo no dirigido
            city_matrix[b][a] = w;
        }
    }

    grid.close();

    int nodo_inicio = find_in_vector(city_positions, "(2, 3)"); // Casa de Damian

    vector<Amigo> amigos;
    amigos.push_back(Amigo("Carlos", "(7, 4)", 30));
    amigos.push_back(Amigo("Ana", "(0, 19)", 40));
    amigos.push_back(Amigo("Marcela", "(8, 12)", 25));
    amigos.push_back(Amigo("Katia", "(5, 17)", 32));
    amigos.push_back(Amigo("Marcos", "(17, 15)", 20));

    for(int i = 0; i < amigos.size(); i++){
        amigos[i].pos_vector = find_in_vector(city_positions, amigos[i].posicion);
    }

    Dijkstra(city_matrix, nodo_inicio, N, amigos, city_positions);
}