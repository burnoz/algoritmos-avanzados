#include <iostream>
#include <vector>

using namespace std;

// Algoritmo de Manacher
// Complejidad: O(n)
vector<int> manacher(string palabra){
    // Texto modificado: @t#ex#t#o#$
    string modified = "@";

    for(int i = 0; i < palabra.size(); i++){
        modified += "#" + palabra.substr(i, 1);
    }

    modified += "#$";

    int centro = 0;
    int limite = 0;
    int gap = 0;
    vector<int> P(modified.size(), 0);

    for(int i = 1; i < modified.size() - 1; i++){
        // Si i esta dentro del limite, usa la simetria
        if(i < limite){
            int simetrico = 2 * centro - i;
            P[i] = min(limite - i, P[simetrico]);
        }

        // Intenta expandir el palindromo alrededor de i
        gap = P[i] + 1;

        // Mientras los caracteres a ambos lados coincidan, expande
        while(modified[i + gap] == modified[i - gap]){
            P[i]++;
            gap++;
        }

        // Si el palindromo se extiende mas alla del limite, actualiza centro y limite
        if(i + P[i] > limite){
            limite = i + P[i];
            centro = i;
        }
    }

    return P;
}


int main(){
    string palabra = "aaaa";

    vector<int> P = manacher(palabra);

    int max_index = 0;
    int max_length = 0;

    for(int i = 0; i < P.size(); i++){
        if(P[i] > max_length){
            max_length = P[i];
            max_index = i;
        }
    }

    int inicio = (max_index - max_length) / 2;

    cout << "Palindromo mas largo desde " << inicio << " hasta " << inicio + max_length - 1 << ":\n";
    cout << "Longitud: " << max_length << endl;

    return 0;
}