#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

class Punto{
public:
    double x;
    double y;

    Punto(double x, double y){
        this->x = x;
        this->y = y;
    }

    Punto(){
        this->x = 0;
        this->y = 0;
    }
};


bool compareX(const Punto &a, const Punto &b){
    return a.x < b.x;
}


bool compareY(const Punto &a, const Punto &b){
    return a.y < b.y;
}


pair<double, pair<Punto, Punto>> closest3(vector<Punto> puntos){
    double minDist = __DBL_MAX__;
    pair<Punto, Punto> closestPair;

    for(int i = 0; i < puntos.size(); i++){
        for(int j = i + 1; j < puntos.size(); j++){
            double dist = sqrt(pow(puntos[i].x - puntos[j].x, 2) + pow(puntos[i].y - puntos[j].y, 2));
            
            if(dist < minDist){
                minDist = dist;
                closestPair = {puntos[i], puntos[j]};
            }
        }
    }

    return {minDist, closestPair};
}


pair<vector<Punto>, vector<Punto>> readPoints(string filename){
    ifstream puntosFile(filename);

    int n;
    puntosFile >> n;

    // cout << "Numero de puntos a procesar: " << n << endl;

    vector<Punto> puntos;
    double x, y;

    for(int i = 0; i < n; i++){
        puntosFile >> x >> y;
        puntos.push_back(Punto(x, y));
    }

    // Puntos ordenados por X
    vector<Punto> OX = puntos;
    sort(OX.begin(), OX.end(), compareX);

    // Puntos ordenados por Y
    vector<Punto> OY = puntos;
    sort(OY.begin(), OY.end(), compareY);

    return {OX, OY};
}

pair<double, pair<Punto, Punto>> closestPair(vector<Punto> OX, vector<Punto> OY){
    // for(int i = 0; i < n; i++){
    //     cout << "(" << OX[i].x << ", " << OX[i].y << ")" << endl;
    // }

    // for(int i = 0; i < n; i++){
    //     cout << "(" << OY[i].x << ", " << OY[i].y << ")" << endl;
    // }
    int n = OX.size();

    if(n < 2){
        cout << "No hay pares de puntos" << endl;
        return {-1, {{0, 0}, {0, 0}}};
    }

    if(n == 2){
        double dist = sqrt(pow(OX[0].x - OX[1].x, 2) + pow(OX[0].y - OX[1].y, 2));
        return {dist, {OX[0], OX[1]}};
    }

    if(n == 3){
        pair<double, pair<Punto, Punto>> result = closest3(OX);
        double dist = result.first;
        pair<Punto, Punto> pairPoints = result.second;
        return {dist, pairPoints};
    }

    int medio = n / 2;
    pair<double, pair<Punto, Punto>> resultL = closestPair(vector<Punto>(OX.begin(), OX.begin() + medio), OY);
    double minL = resultL.first;
    pair<Punto, Punto> closestL = resultL.second;

    pair<double, pair<Punto, Punto>> resultR = closestPair(vector<Punto>(OX.begin() + medio, OX.end()), OY);
    double minR = resultR.first;
    pair<Punto, Punto> closestR = resultR.second;

    double minA;
    pair<Punto, Punto> closestA;

    if(minL < minR){
        minA = minL;
        closestA = closestL;
    } 
    
    else{
        minA = minR;
        closestA = closestR;
    }

    vector<Punto> S;
    for(int i = 0; i < OY.size(); i++){
        if(abs(OY[i].x - OX[medio].x) < minA){
            S.push_back(OY[i]);
        }
    }

    for(int i = 0; i < S.size(); i++){
        for(int j = i + 1; j < S.size() && (S[j].y - S[i].y) < minA; j++){
            double dist = sqrt(pow(S[i].x - S[j].x, 2) + pow(S[i].y - S[j].y, 2));

            if(dist < minA){
                minA = dist;
                closestA = {S[i], S[j]};
            }
        }
    }

    return {minA, closestA};
}


int main(){
    cout << "Closest pair" << endl;

    string baseName = "puntos-n";
    vector<int> sizes = {8, 10, 11, 15 , 16, 20, 50, 100};

    for(int i = 0; i < sizes.size(); i++){
        string filename = baseName + to_string(sizes[i]) + ".txt";

        pair<vector<Punto>, vector<Punto>> puntos = readPoints(filename);
        vector<Punto> OX = puntos.first;
        vector<Punto> OY = puntos.second;

        pair<double, pair<Punto, Punto>> result = closestPair(OX, OY);
        double minDist = result.first;
        pair<Punto, Punto> closest = result.second;

        cout << filename << ":    " << minDist << "    [[" << closest.first.x << ", " << closest.first.y << "], [" << closest.second.x << ", " << closest.second.y << "]]" << endl;
    }

    return 0;
}
