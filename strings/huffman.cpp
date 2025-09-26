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

    void preorder(Node* node){
        if(!node){
            return;
        }

        cout << node->prob << " ";
        preorder(node->left);
        preorder(node->right);
    }

    void get_codes(Node* node, string code, map<char, string>& table){
        if(!node){
            return;
        }

        if(!node->left && !node->right && node->character != '\0'){
            table[node->character] = code;
            return;
        }

        if(node->left) get_codes(node->left, code + "0", table);
        if(node->right) get_codes(node->right, code + "1", table);
    }
};


class CompareTree{
public:
    bool operator()(Tree* a, Tree* b){
        if(a->root->prob == b->root->prob)
            return a->root->character > b->root->character;
        return a->root->prob > b->root->prob;
    }
};


Tree* create_huffman_tree(const vector<char>& alphabet, const vector<double>& probabilities){
    priority_queue<Tree*, vector<Tree*>, CompareTree> trees;

    int n = alphabet.size();
    
    for(int i = 0; i < n; ++i){
        Node* node = new Node(alphabet[i], probabilities[i]);
        trees.push(new Tree(node));
    }
    
    while(trees.size() > 1){
        Tree* a = trees.top(); trees.pop();
        Tree* b = trees.top(); trees.pop();
        
        Node* new_node = new Node('\0', a->root->prob + b->root->prob);
        new_node->left = a->root;
        new_node->right = b->root;
        new_node->left->tag = 0;
        new_node->right->tag = 1;
        
        trees.push( new Tree(new_node) );
    }

    return trees.top();
}

string encode(const string& text, map<char, string> table){
    string result;
    
    for(char c : text){
        result += table.at(c);
    }
    
    return result;
}

string decode(string encoded, Node* root){
    string result;
    Node* node = root;

    for(int i = 0; i < encoded.size(); ++i){
        if(encoded[i] == '0') node = node->left;
        else node = node->right;

        if(!node->left && !node->right){
            result += node->character;
            node = root;
        }
    }

    return result;
}

// Lee un archivo de texto y calcula las probabilidades de cada letra
string process_file(string text, map<char, double>& freq_map){
    ifstream file(text);

    if (!file.is_open()) {
        cerr << "Error opening file: " << text << endl;
        return "";
    }

    char c;
    int total = 0;
    string file_content;
    
    while(file.get(c)) {
        freq_map[c]++;
        total++;
        file_content += c;
    }

    for(auto& pair : freq_map) {
        pair.second /= total;
    }

    file.close();

    return file_content;
}


int main(){
    map<char, double> freq_map;
    string filename = "escalera.txt";
    string file_content = process_file(filename, freq_map);

    vector<char> alphabet;
    vector<double> probabilities;

    for(const auto& pair : freq_map) {
        alphabet.push_back(pair.first);
        probabilities.push_back(pair.second);
    }

    Tree* huffman_tree = create_huffman_tree(alphabet, probabilities);
    map<char, string> code_table;
    huffman_tree->get_codes(huffman_tree->root, "", code_table);

    for(auto pair : code_table){
        cout << pair.first << ": " << pair.second << endl;
    }

    string encoded = encode(file_content, code_table);

    string bin_file = "encoded.bin";
    ofstream out(bin_file, ios::binary);
    out << encoded;
    out.close();

    string decoded = decode(encoded, huffman_tree->root);
    // cout << "Decoded matches original: " << decoded << endl;

    // Menu    
    // 1. Codificar / decodificar archivos dados. 
    // 2. Mostrar una lista de los caracteres, con sus ocurrencias, probabilidades y códigos
    // 3. Codificar lo que el usuario ingrese.
    // 4. Decodificar lo que el usuario ingrese
    // 5. Mostrar el arbol. Diseña tu propio formato que sea legible. 

    while(true){
        cout << "Menu:\n";
        cout << "1. Codificar archivo\n";
        cout << "2. Decodificar archivo\n";
        cout << "3. Codificar texto\n";
        cout << "4. Decodificar texto\n";
        cout << "5. Mostrar tabla de codigos\n";
        cout << "6. Mostrar arbol\n";
        cout << "7. Salir\n";
        cout << "Elige una opcion: ";
        int option;
        cin >> option;

        if(option == 1){
            string input_file;
            cout << "Ingresa el nombre del archivo a codificar: ";
            cin >> input_file;

            map<char, double> freq_map;
            string file_content = process_file(input_file, freq_map);

            string encoded = encode(file_content, code_table);
            string bin_file = "encoded.bin";
            ofstream out(bin_file, ios::binary);
            out << encoded;
            out.close();

            cout << "Archivo codificado y guardado en " << bin_file << endl;
        }

        else if(option == 2){
            string bin_file;
            cout << "Ingresa el nombre del archivo a decodificar: ";
            cin >> bin_file;

            ifstream in(bin_file, ios::binary);
            string encoded((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
            in.close();

            string decoded = decode(encoded, huffman_tree->root);
            cout << "Archivo decodificado:\n" << decoded << endl;
        }

        else if(option == 3){
            cin.ignore(); // Limpiar el buffer
            string text;
            cout << "Ingresa el texto a codificar: ";
            getline(cin, text);

            string encoded = encode(text, code_table);
            cout << "Texto codificado: " << encoded << endl;
        }

        else if(option == 4){
            cin.ignore(); // Limpiar el buffer
            string encoded;
            cout << "Ingresa el texto a decodificar (secuencia de 0s y 1s): ";
            getline(cin, encoded);

            string decoded = decode(encoded, huffman_tree->root);
            cout << "Texto decodificado: " << decoded << endl;
        }

        else if(option == 5){
            cout << "Tabla de codigos:\n";
            for(auto pair : code_table){
                cout << pair.first << ": " << pair.second << endl;
            }
        }

        else if(option == 6){
            cout << "Arbol de Huffman (preorder probabilities): ";
            huffman_tree->show();
        }

        else if(option == 7){
            break;
        }

        else{
            cout << "Opcion invalida. Intenta de nuevo.\n";
        }
    }
}
