#include <iostream>
#include <queue>
#include <vector>
// Huffman encoding in C++
#include <map>
#include <string>

using namespace std;


// Nodo para el árbol de Huffman
struct Nodo {
    char caracter;
    double prob;
    int tag;
    Nodo *left;
    Nodo *right;

    Nodo(char c, double p) : caracter(c), prob(p), tag(-1), left(nullptr), right(nullptr) {}
};

// Clase Arbol para el árbol de Huffman
class Arbol {
public:
    Nodo* raiz;

    Arbol(Nodo* r) : raiz(r) {}

    // // Para priority_queue: compara por probabilidad y caracter
    // bool operator<(const Arbol& other) const {
    //     if (raiz->prob == other.raiz->prob)
    //         return raiz->caracter < other.raiz->caracter;
    //     return raiz->prob < other.raiz->prob;
    // }

    // Mostrar probabilidades en preorder
    void show() const {
        preorder(raiz);
        cout << endl;
    }

    void preorder(Nodo* node) const {
        if (!node) return;
        cout << node->prob << " ";
        preorder(node->left);
        preorder(node->right);
    }

    // Obtener códigos de Huffman
    void get_codigos(Nodo* node, string code, map<char, string>& tabla) const {
        if (!node) return;
        if (!node->left && !node->right && node->caracter != '\0') {
            tabla[node->caracter] = code;
            return;
        }
        if (node->left) get_codigos(node->left, code + "0", tabla);
        if (node->right) get_codigos(node->right, code + "1", tabla);
    }
};

// Comparator para priority_queue de Arboles
struct CompareArbol {
    bool operator()(Arbol* a, Arbol* b) {
        if (a->raiz->prob == b->raiz->prob)
            return a->raiz->caracter > b->raiz->caracter;
        return a->raiz->prob > b->raiz->prob;
    }
};

// Construye el árbol de Huffman usando la clase Arbol
Arbol* crea_huffman_tree(const vector<char>& alfabeto, const vector<double>& probabilidades) {
    priority_queue<Arbol*, vector<Arbol*>, CompareArbol> arboles;
    int n = alfabeto.size();
    for (int i = 0; i < n; ++i) {
        Nodo* nodo = new Nodo(alfabeto[i], probabilidades[i]);
        arboles.push(new Arbol(nodo));
    }
    while (arboles.size() > 1) {
        Arbol* a = arboles.top(); arboles.pop();
        Arbol* b = arboles.top(); arboles.pop();
        // Fusiona los dos árboles
        Nodo* nuevo = new Nodo('\0', a->raiz->prob + b->raiz->prob);
        nuevo->left = a->raiz;
        nuevo->right = b->raiz;
        nuevo->left->tag = 0;
        nuevo->right->tag = 1;
        arboles.push(new Arbol(nuevo));
    }
    return arboles.top();
}

// Codifica una cadena usando la tabla de códigos
string encoding(const string& cadena, const map<char, string>& tabla_codigos) {
    string resultado;
    for (char c : cadena) {
        resultado += tabla_codigos.at(c);
    }
    return resultado;
}

// Decodifica una cadena usando el árbol de Huffman
string decoding(const string& cadena_codificada, Arbol* arbol_huffman) {
    string resultado;
    Nodo* node = arbol_huffman->raiz;
    for (char bit : cadena_codificada) {
        if (bit == '0') node = node->left;
        else node = node->right;
        if (!node->left && !node->right) {
            resultado += node->caracter;
            node = arbol_huffman->raiz;
        }
    }
    return resultado;
}

int main() {
    // Ejemplo igual al notebook
    vector<char> alfabeto = {'A', 'B', 'C', 'D', 'E'};
    vector<double> probabilidades = {0.35, 0.1, 0.2, 0.2, 0.15};

    // Crea el árbol de Huffman
    Arbol* arbol_huffman = crea_huffman_tree(alfabeto, probabilidades);

    // Imprime las probabilidades en preorder
    cout << "Preorder probabilidades: ";
    arbol_huffman->show();

    // Obtén los códigos de Huffman
    map<char, string> tabla_codigos;
    arbol_huffman->get_codigos(arbol_huffman->raiz, "", tabla_codigos);
    cout << "Codigos de Huffman:\n";
    for (auto& p : tabla_codigos) {
        cout << p.first << ": " << p.second << endl;
    }

    // Codifica y decodifica una cadena de prueba
    string prueba = "ADADADA";
    string prueba_codificada = encoding(prueba, tabla_codigos);
    cout << "Codificada: " << prueba_codificada << endl;
    string prueba_decodificada = decoding(prueba_codificada, arbol_huffman);
    cout << "Decodificada: " << prueba_decodificada << endl;

    // Liberar memoria si es necesario
    // ...
    return 0;
}