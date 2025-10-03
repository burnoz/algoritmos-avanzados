#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Node{
public:
    char caracter;
    bool es_final;
    Node** hijos;

    Node(char caracter) {
        this->caracter = caracter;
        this->es_final = false;
        this->hijos = new Node*[26]();
    }
};

class Trie {
public:
    Node* raiz;

    Trie() {
        raiz = new Node('\0');
    }

    void insertar(string palabra) {
        Node* actual = raiz;

        for(int i = 0; i < palabra.size(); i++) {
            int idx = palabra[i] - 'a';

            if (actual->hijos[idx] == nullptr) {
                actual->hijos[idx] = new Node(palabra[i]);
            }

            actual = actual->hijos[idx];
        }

        actual->es_final = true;
    }

    // Busca la cadena y devuelve el nodo final si existe parcialmente, nullptr si no existe nada
    Node* buscar(string prefijo) {
        Node* actual = raiz;

        for(int i = 0; i < prefijo.size(); i++) {
            int idx = prefijo[i] - 'a';
            
            if(actual->hijos[idx] == nullptr) {
                return nullptr;
            }
            
            actual = actual->hijos[idx];
        }

        return actual;
    }

    // Recolecta todas las palabras posibles a partir de un nodo
    void recolectar(Node* nodo, string actual, vector<string>& resultados) {
        if(nodo->es_final) {
            resultados.push_back(actual);
        }

        for(int i = 0; i < 26; ++i) {
            if(nodo->hijos[i] != nullptr) {
                recolectar(nodo->hijos[i], actual + nodo->hijos[i]->caracter, resultados);
            }
        }
    }

    void autocompletar(string prefijo) {
        Node* nodo = buscar(prefijo);

        if (nodo == nullptr) {
            cout << "No hay coincidencias para '" << prefijo << "'" << endl;
            return;
        }

        if (nodo->es_final) {
            // Verifica si hay más palabras además de la exacta
            vector<string> opciones;
            recolectar(nodo, prefijo, opciones);

            if(opciones.size() == 1) {
                cout << "La cadena '" << prefijo << "' existe completa" << endl;
                return;
            }
        }

        // Buscar todas las opciones de autocompletado
        vector<string> opciones;
        recolectar(nodo, prefijo, opciones);

        if (opciones.size() == 1 && opciones[0] == prefijo) {
            cout << "La cadena '" << prefijo << "' existe completa" << endl;
        } 
        
        else {
            cout << "Opciones para autocompletar '" << prefijo << "':" << endl;
            
            for(int i = 0; i < opciones.size(); i++) {
                cout << "- " << opciones[i] << endl;
            }
        }
    }
};

int main() {
    Trie trie;
    vector<string> palabras = {"casi", "casa", "cama", "camisa", "camara", "camion", "ave", "alce"};
    
    for(int i = 0; i < palabras.size(); i++) {
        trie.insertar(palabras[i]);
    }

    vector<string> pruebas = {"cam", "casa", "ca", "ave", "al", "boing", "zoom", "ble"};
    
    for(int i = 0; i < pruebas.size(); i++) {
        trie.autocompletar(pruebas[i]);
        cout << endl;
    }

    return 0;
}