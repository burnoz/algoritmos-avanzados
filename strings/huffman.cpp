#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

class Node{
public:
    char character;
    double prob;
    int tag;
    Node *left;
    Node *right;

    Node(char character, double probability){
        this->character = character;
        this->prob = probability;
        tag = -1;
        left = nullptr; 
        right = nullptr;
    }
};


class Tree{
public:
    Node *root;

    Tree(Node *root){
        this->root = root;
    }

    void show(){
        preorder(root);
        cout << endl;
    }

    // Recorrido preorder
    void preorder(Node *node){
        if(!node){
            return;
        }

        cout << node->prob << " ";
        preorder(node->left);
        preorder(node->right);
    }

    // Muestra el arbol por niveles (BFS)
    void show_tree() {
        if(!root){
            return;
        }

        queue<Node*> q;
        q.push(root);

        int level = 1;
        while(!q.empty()){
            int n = q.size();
            cout << level << ": ";

            vector<string> nodos;

            for(int i = 0; i < n; i++){
                Node *node = q.front();
                q.pop();
                string s;

                if(node->character != '\0') {
                    s = string(1, node->character) + " - " + to_string(node->prob);
                } 
                
                else{
                    s = "(" + to_string(node->prob) + ")";
                }

                nodos.push_back(s);

                if(node->left){
                    q.push(node->left);
                }

                if(node->right){
                    q.push(node->right);
                }
            }

            for(int i = 0; i < nodos.size(); i++) {
                cout << nodos[i];
                if(i + 1 < nodos.size()){
                    cout << " | ";
                }
            }

            cout << endl;
            level++;
        }
    }

    void get_codes(Node *node, string code, map<char, string>& table){
        if(!node){
            return;
        }

        // Si es hoja, guarda el codigo
        if(!node->left && !node->right && node->character != '\0'){
            table[node->character] = code;
            return;
        }

        // Recorre el arbol agregando 0 para izquierda y 1 para derecha
        if(node->left){
            get_codes(node->left, code + "0", table);
        } 

        if(node->right){
            get_codes(node->right, code + "1", table);
        }
    }
};

// Funcion objeto para comparar probabilidades y caracteres de los nodos
class CompareTree{
public:
    bool operator()(Tree *a, Tree *b){
        if(a->root->prob == b->root->prob){
            return a->root->character > b->root->character;
        }

        return a->root->prob > b->root->prob;
    }
};


Tree *create_huffman_tree(vector<char> alphabet, vector<double> probabilities){
    // Fila de prioridad para los arboles
    priority_queue<Tree*, vector<Tree*>, CompareTree> trees;

    int n = alphabet.size();
    
    // Crear un nodo para cada caracter y su probabilidad
    for(int i = 0; i < n; i++){
        Node* node = new Node(alphabet[i], probabilities[i]);
        trees.push(new Tree(node));
    }
    
    while(trees.size() > 1){
        // Sacar los dos arboles con menor probabilidad
        Tree* a = trees.top(); trees.pop();
        Tree* b = trees.top(); trees.pop();
        
        // Crear un nuevo nodo padre
        Node* new_node = new Node('\0', a->root->prob + b->root->prob);
        // Agrega los nodos de menor probabilidad como hijos
        new_node->left = a->root;
        new_node->right = b->root;
        // Asigna etiquetas para codificacion
        new_node->left->tag = 0;
        new_node->right->tag = 1;
        
        // Crear un nuevo arbol con el nodo padre y agregarlo a la fila de prioridad
        trees.push(new Tree(new_node));
    }

    return trees.top();
}

string encode(string text, map<char, string> table){
    string result;
    
    // Obtiene el codigo para cada caracter
    for(char c : text){
        result += table.at(c);
    }
    
    return result;
}

string decode(string encoded, Node *root){
    string result;
    Node *node = root;

    for(int i = 0; i < encoded.size(); i++){
        // Si el tag es 0, va a la izquierda
        if(encoded[i] == '0') {
            node = node->left;
        }

        // Si el tag es 1, va a la derecha
        else{
            node = node->right;
        } 

        // Si es hoja, agrega el caracter al resultado y vuelve a la raiz
        if(!node->left && !node->right){
            result += node->character;
            node = root;
        }
    }

    return result;
}

// Lee un archivo de texto y calcula las probabilidades de cada letra
string process_file(string text, map<char, double>& freq_map, map<char, int>& occurrences){
    ifstream file(text);

    if (!file.is_open()) {
        cerr << "Error opening file: " << text << endl;
        return "";
    }

    char c;
    int total = 0;
    string file_content;
    
    // Cuenta la frecuencia de cada caracter y guarda el contenido del archivo
    while(file.get(c)){
        freq_map[c]++;
        occurrences[c]++;
        total++;
        file_content += c;
    }

    // Convierte las frecuencias a probabilidades
    for(auto& pair : freq_map){
        pair.second /= total;
    }

    file.close();

    return file_content;
}


int main(){
    map<char, double> freq_map;
    map<char, int> occ_map;
    string filename = "escalera.txt";
    string file_content = process_file(filename, freq_map, occ_map);

    vector<char> alphabet;
    vector<double> probabilities;
    vector<int> occurrences;

    // Separa los caracteres y sus probabilidades
    for(auto pair : freq_map) {
        alphabet.push_back(pair.first);
        probabilities.push_back(pair.second);
    }

    // Guarda las ocurrencias de cada caracter
    for(auto pair : occ_map) {
        occurrences.push_back(pair.second);
    }

    // Crea el arbol de Huffman y la tabla de codigos
    Tree *huffman_tree = create_huffman_tree(alphabet, probabilities);
    map<char, string> code_table;
    huffman_tree->get_codes(huffman_tree->root, "", code_table);

    while(true){
        cout << "Menu:" << endl;
        cout << "1. Codificar archivo" << endl;
        cout << "2. Decodificar archivo" << endl;
        cout << "3. Codificar texto" << endl;
        cout << "4. Decodificar texto" << endl;
        cout << "5. Mostrar tabla de codigos" << endl;
        cout << "6. Mostrar arbol" << endl;
        cout << "7. Salir" << endl;
        cout << "Elige una opcion: ";
        int option;
        cin >> option;

        if(option == 1){
            string input_file;
            cout << "Ingresa el nombre del archivo a codificar: ";
            cin >> input_file;

            ifstream file(input_file);
            string content;
            string line;

            while(getline(file, line)){
                content += line + "\n";
            }

            string encoded = encode(content, code_table);
            string bin_file = "encoded.bin";
            ofstream out(bin_file, ios::binary);
            out << encoded;
            out.close();

            cout << "Archivo codificado y guardado en " << bin_file << endl;
            cout << endl;
        }

        else if(option == 2){
            string bin_file;
            cout << "Ingresa el nombre del archivo a decodificar: ";
            cin >> bin_file;

            ifstream in(bin_file, ios::binary);
            string line;
            string encoded;

            while(getline(in, line)){
                encoded += line;
            }

            in.close();

            string decoded = decode(encoded, huffman_tree->root);
            cout << "Archivo decodificado:\n" << decoded << endl;
            cout << endl;
        }

        else if(option == 3){
            cin.ignore();
            string text;
            cout << "Ingresa el texto a codificar: ";
            getline(cin, text);

            string encoded = encode(text, code_table);
            cout << "Texto codificado: " << encoded << endl;
            cout << endl;
        }

        else if(option == 4){
            cin.ignore();
            string encoded;
            cout << "Ingresa el texto a decodificar: ";
            getline(cin, encoded);

            string decoded = decode(encoded, huffman_tree->root);
            cout << "Texto decodificado: " << decoded << endl;
            cout << endl;
        }

        else if(option == 5){
            cout << "Tabla de codigos:\n";
            
            for(auto pair : code_table){
                cout << pair.first << ": " << pair.second << " - Ocurrencias: " << occ_map[pair.first] << " - Probabilidad: " << freq_map[pair.first] << endl;
            }
            
            cout << endl;
        }

        else if(option == 6){
            cout << "Arbol de Huffman:" << endl;
            huffman_tree->show_tree();

            cout << endl;
        }

        else if(option == 7){
            break;
        }

        else{
            cout << "Opcion invalida. Intenta de nuevo.\n";
        }
    }
}
