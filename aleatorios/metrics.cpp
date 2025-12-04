#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

vector<int> readResults(const string& filename){
    vector<int> results;
    ifstream file(filename);
    string line;

    while(getline(file, line)){
        results.push_back(stoi(line));
    }

    file.close();
    return results;
}

double getRMSE(const vector<int>& results, int bestKnown){
    double sum = 0.0;
    int n = results.size();

    for(int i = 0; i < n; i++){
        double diff = results[i] - bestKnown;
        sum += diff * diff;
    }

    return sqrt(sum / n);
}

double getAvg(const vector<int>& results){
    double sum = 0.0;
    int n = results.size();

    for(int i = 0; i < n; i++){
        sum += results[i];
    }

    return sum / n;
}

int getBest(const vector<int>& results){
    int bestCase = results[0];

    for(int i = 1; i < results.size(); i++){
        if(results[i] < bestCase){
            bestCase = results[i];
        }
    }

    return bestCase;
}

int main() {
    // bcspwr01
    int bcspwr01_best = 4;
    vector<string> bcspwr01_files = {
        "bcspwr01.mtx.rnd_output_first.txt",
        "bcspwr01.mtx.rnd_output_best.txt",
        "bcspwr01.txt_ils_output.txt",
        "bcspwr01.txt_annealing_output.txt"
    };

    // bcspwr02
    int bcspwr02_best = 7;
    vector<string> bcspwr02_files = {
        "bcspwr02.mtx.rnd_output_first.txt",
        "bcspwr02.mtx.rnd_output_best.txt",
        "bcspwr02.txt_ils_output.txt",
        "bcspwr02.txt_annealing_output.txt"
    };
    
    // bcspwr03
    int bcspwr03_best = 10;
    vector<string> bcspwr03_files = {
        "bcspwr03.mtx.rnd_output_first.txt",
        "bcspwr03.mtx.rnd_output_best.txt",
        "bcspwr03.txt_ils_output.txt",
        "bcspwr03.txt_annealing_output.txt"
    };
    
    // ash85
    int ash85_best = 9;
    vector<string> ash85_files = {
        "ash85.mtx.rnd_output_first.txt",
        "ash85.mtx.rnd_output_best.txt",
        "ash85.txt_ils_output.txt",
        "ash85.txt_annealing_output.txt"
    };

    // Procesar cada instancia
    vector<pair<string, vector<string>>> instances = {
        {"bcspwr01", bcspwr01_files},
        {"bcspwr02", bcspwr02_files},
        {"bcspwr03", bcspwr03_files},
        {"ash85", ash85_files}
    };

    vector<string> algorithms = {
        "Hill Climbing - First Improvement",
        "Hill Climbing - Best Improvement",
        "Iterated Local Search (Hill Climbing - First Improvement)",
        "Simulated Annealing"
    };

    vector<int> bests = {bcspwr01_best, bcspwr02_best, bcspwr03_best, ash85_best};

    for(int i = 0; i < instances.size(); i++){
        string rmse_filename = instances[i].first + "_rmse_results.txt";
        ofstream rmse_file(rmse_filename, ios::app);
        
        cout << instances[i].first << " (Best known: " << bests[i] << ")" << endl;
        
        for(const auto& file : instances[i].second){
            vector<int> results = readResults(file);
            
            double avg = getAvg(results);
            double rmse = getRMSE(results, bests[i]);
            int bestCase = getBest(results);
            
            cout << algorithms[&file - &instances[i].second[0]] << endl;
            cout << "  Avg: " << avg << endl;
            cout << "  RMSE: " << rmse << endl;
            cout << "  Best: " << bestCase << endl;

            // Guarda los RMSE en un archivo
            rmse_file << rmse << endl;
        }

        rmse_file.close();

        cout << endl;
    }

    return 0;
}