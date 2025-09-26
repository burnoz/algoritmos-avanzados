#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Obtiene el prefijo mas largo que es sufijo
// Complejidad: O(n)
string getLPS(string text){
    int n = text.size();

    vector<int> lps(n, 0);

    int len_prefix = 0; // Longitud del prefijo mas largo que es sufijo
    int i = 1;

    while(i < n){
        // Si hay coincidencia, extiende el prefijo
        if(text[i] == text[len_prefix]){
            len_prefix++;
            lps[i] = len_prefix;
            i++;
        }

        else{
            // Si no hay coincidencia y len_prefix no es 0, retrocede
            if(len_prefix != 0){
                len_prefix = lps[len_prefix - 1];
            }

            // Si len_prefix es 0, no hay prefijo, avanza
            else{
                lps[i] = 0;
                i++;
            }
        }
    }

    return text.substr(0, lps[n-1]);
}


// KMP
// Complejidad: O(n + m), donde n es el tamano del texto y m el del patron
vector<int> kmp(string text, string pattern){
    int n = text.size();
    int m = pattern.size();

    vector<int> lps(m, 0);
    vector<int> occurrences;

    int len_prefix = 0;

    int i = 0;
    int j = 0;

    while(i < n){
        // Si hay coincidencia, avanza en ambos
        if(text[i] == pattern[j]){
            i++;
            j++;
        }

        // Si se encontro el patron, guarda la posicion
        if(j == m){
            occurrences.push_back(i - j);
            j = lps[j - 1];
        }

        // Si no hay coincidencia
        else if(i < n && text[i] != pattern[j]){
            // Si j no es 0, retrocede en el patron
            if(j > 0){
                j = lps[j - 1];
            }

            // Si j es 0, avanza en el texto
            else{
                i++;
            }
        }
    }

    return occurrences;
}


int main(){
    // string test_text = "bababababba";

    // string lps = getLPS(test_text);

    // cout << lps << endl;

    string text = "ababcababcabc";
    string pattern = "abc";

    vector<int> occurrences = kmp(text, pattern);

    for(int i = 0; i < occurrences.size(); i++){
        cout << "Coindidencia desde " << occurrences[i] << " hasta " << occurrences[i] + pattern.size() << endl;
    }
}