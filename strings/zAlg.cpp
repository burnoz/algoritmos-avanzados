#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Algoritmo Z
// Complejidad: O(n + m), donde n es el tamano del texto y m el del patron
vector<int> zAlgorithm(string text, string pattern){
    // Texto combinado: patron$texto
    string c = pattern + "$" + text;
    int x = c.size();
    vector<int> Z(x, -1);

    // Ventana de coincidencias
    int L = 0;
    int R = 0;

    int k;

    for(int i = 0; i < x; i++){
        if(i <= R){
            k = i - L;
            // Si Z[k] cabe en el segmento, lo reutiliza
            if(Z[k] < R - i + 1){
                Z[i] = Z[k];
            }
            // Si no, recalcula desde la posición actual
            else{
                L = i;
                // Extiende el segmento mientras coincidan los caracteres
                while(R < x && c[R - L] == c[R]){
                    R++;
                }
                Z[i] = R - L;
                R--;
            }
        }
        
        else{
            L = R = i;
            // Extiende el segmento mientras coincidan los caracteres
            while(R < x && c[R - L] == c[R]){
                R++;
            }
            Z[i] = R - L;
            R--;
        }
    }

    return Z;
}

int main(){
    string text = "ababcababcabc";
    string pattern = "abc";

    vector<int> Z = zAlgorithm(text, pattern);

    for(int i = 0; i < Z.size(); i++){
        if(Z[i] == pattern.size()){
            cout << "Coindidencia desde " << i - pattern.size() - 1 << " hasta " << i - 1 << endl;
        }
    }

    return 0;
}