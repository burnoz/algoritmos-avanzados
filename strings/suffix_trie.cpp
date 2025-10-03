#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Node{
public:
    char caracter;
    vector<int> indices;
    bool es_final;
    int level;
    Node** hijos;

    Node(char caracter, bool es_final){
        this->caracter = caracter;
        this->indices = vector<int>();
        this->es_final = es_final;
        this->level = 0;
        this->hijos = new Node*[26]();
    }
};


class Trie{
public:
    Node* raiz;

    Trie(){
        raiz = new Node('\0', false);
    }

    // Inserta un sufijo en el trie
    void insertar(Node* nodo, string cadena, int i, int og_i){
        int cadena_len = cadena.length();
        if(i < cadena_len){
            // Buscar si ya existe un hijo con el caracter actual
            int idx = cadena[i] - 'a';
            Node* hijo = nodo->hijos[idx];

            if(hijo != nullptr && hijo->caracter == cadena[i]){
                hijo->indices.push_back(og_i + i);
                insertar(hijo, cadena, i + 1, og_i);
                return;
            }

            // Si no existe, crear un nuevo hijo
            Node* nuevo_hijo = new Node(cadena[i], i == cadena_len - 1);
            nuevo_hijo->indices.push_back(og_i + i);
            nuevo_hijo->level = nodo->level + 1;
            nodo->hijos[idx] = nuevo_hijo;
            insertar(nuevo_hijo, cadena, i + 1, og_i);
        }
    }

    void show_preorder(Node* nodo){
        cout << "\nSoy " << nodo->caracter << " existo en: [";

        for(int i = 0; i < nodo->indices.size(); i++){
            cout << nodo->indices[i];
            if(i != nodo->indices.size() - 1) cout << ", ";
        }

        cout << "] final " << nodo->es_final << " lvl: " << nodo->level << endl;
        cout << "\tHijos: ";

        for(int idx = 0; idx < 26; idx++){
            if(nodo->hijos[idx] != nullptr){
                cout << nodo->hijos[idx]->caracter << " ";
            }
        }

        cout << endl;
        for(int idx = 0; idx < 26; idx++){
            if(nodo->hijos[idx] != nullptr){
                show_preorder(nodo->hijos[idx]);
            }
        }
    }

    // Buscar un sufijo en el trie
    // Imprime subcadena buscada y los indices de inicio y fin donde aparece. 
    void buscar(string cadena){
        Node* nodo = raiz;
        int i = 0;
        int cadena_len = cadena.length();

        while(i < cadena_len){
            int idx = cadena[i] - 'a';
            Node* hijo = nodo->hijos[idx];

            if(hijo != nullptr && hijo->caracter == cadena[i]){
                nodo = hijo;
                i++;
            } 
            
            else {
                cout << "La subcadena '" << cadena << "' no se encuentra en el trie" << endl;
                return;
            }
        }
        cout << "La subcadena '" << cadena << "' se encuentra en: [";

        for(int j = 0; j < nodo->indices.size(); j++){
            cout << "(" << nodo->indices[j] - cadena.length() + 1 << " - " << nodo->indices[j] << ")";

            if(j != nodo->indices.size() - 1){
                cout << ", ";
            }
        }

        cout << "]" << endl;
    }
};

vector<string> getSuffixes(string text){
    int n = text.size();
    vector<string> suffixes;

    for(int i = 0; i < n; i++){
        suffixes.push_back(text.substr(i));
    }

    return suffixes;
}   

int main(){
    string text = "anabanana";
    Trie trie;

    vector<string> suffixes = getSuffixes(text);

    for(int i = 0; i < suffixes.size(); i++){
        trie.insertar(trie.raiz, suffixes[i], 0, i);
    }

    // trie.show_preorder(trie.raiz);

    trie.buscar("ble");
    trie.buscar("ana");
    trie.buscar("nan");
    trie.buscar("ban");

    return 0;
}