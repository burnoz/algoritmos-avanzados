#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Complejidad: O(n log n) por el sort
vector<int> suffix(string text) {
	int n = text.size();
	vector<pair<string, int>> suffixes;

    // Genera todos los sufijos con su indice
    for(int i = 0; i < n; ++i){
        suffixes.push_back({text.substr(i), i});
    }
    
    // Ordena los sufijos
    sort(suffixes.begin(), suffixes.end());

    vector<int> suffix_array(n);

    // Extrae los indices ordenados
    for(int i = 0; i < n; ++i){
        suffix_array[i] = suffixes[i].second;
    }

    return suffix_array;
}


int main(){
    string text = "anabanana";
    vector<int> suffix_array = suffix(text);

    cout << "Suffix array para " << text << ":\n";

    for(int i = 0; i < suffix_array.size(); ++i){
        cout << suffix_array[i] << " ";
    }
}