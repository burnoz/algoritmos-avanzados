#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

// Funcion que calcule e imprima:
// las coordenadas de los dos puntos mas cercanos y su distancia
// Complejidad: O(n^2)
void closestPair(double coords[][2], int n_points){
    double min_distance = __DBL_MAX__;
    int point1; 
    int point2;

    for (int i = 0; i < n_points; i++) {
        for (int j = i + 1; j < n_points; j++) {
            double dx = coords[i][0] - coords[j][0];
            double dy = coords[i][1] - coords[j][1];
            double distance = sqrt(dx * dx + dy * dy);

            if (distance < min_distance) {
                min_distance = distance;
                point1 = i;
                point2 = j;
            }
        }
    }

    cout << "Par: (" << coords[point1][0] << ", " << coords[point1][1] 
         << ") , (" << coords[point2][0] << ", " << coords[point2][1] 
         << ")" << endl << "Distancia: " <<  min_distance << endl;
}

// Complejidad O(n*m), debe recorrer el largo n del texto y m del patron para comprobar
void stringMAtch(string texto, string patron){
    for(int i = 0; i <= texto.length() - patron.length(); i++) {
        int j;
        
        for (j = 0; j < patron.length(); j++) {
            if (texto[i + j] != patron[j]) {
                break;
            }
        }

        if (j == patron.length()) {
            cout << "Encontrado en: " << i << " hasta " << i + patron.length() << endl;
        }
    }
}


int main(int argc, char* argv[]){	
	// Sintaxis de declaracion de arreglos: 	
	// type arrayName[size];
	// int numbers[5];
	// int arr[5] = {1, 2, 3, 4, 5};	// tamaño explicito
	// int arr2[]  = {10, 20, 30};		// tamaño implicito

	const int n_points = 5; 		// numero de puntos

	// coordenadas de cada punto
	double coords[n_points][2] = 
				{ 	{ -2.423,   -8.469 },
					{  5.721,    9.354 },
					{  6.766,   -3.823 },
					{  4.129,    6.744 },
					{  5.371,   -5.404 } 
				} ;

    cout << "Closest pair" << endl;
    closestPair(coords, n_points);
	
    char texto[] = "Es este un texto algo largo, que contiene multi-multiples caracteres en un arreglo de tipo char";
	char patron[] = "texto";  // los arreglos char terminan en \0

	// int n_texto = sizeof( texto ) / sizeof( texto[0] );
	// int n_patron = sizeof( patron ) / sizeof( patron[0] );

    cout << endl;
    cout << "String matching" << endl;

	cout << texto << endl;
	cout << patron << endl;

	// cout << "len texto:\t" << n_texto << endl;
	// cout << "len patron:\t" << n_patron << patron[n_patron-1] << endl;

    stringMAtch(texto, patron);
}