#include <iostream>
#include <stdio.h>

using namespace std;

// Decrease and conquer para a^n
// Complejidad: O(log n)
int calculate(int a, int n){   
    if(n == 0){ 
        return 1;
    }

    if(n % 2 == 0){
        int half = calculate(a, n / 2);
        return half * half;
    }
    
    else{
        return a * calculate(a, n - 1);
    }
}

int main(){
    cout << calculate(2, 3) << endl;
    cout << calculate(4, 2) << endl;
    cout << calculate(5, 5) << endl;
    cout << calculate(2, 10) << endl;
}