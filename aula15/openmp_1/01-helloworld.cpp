#include<iostream>
using namespace std;
int main() {
    #pragma omp parallel 
    {
        cout << "Hello, World!" << endl;
    }
    

    return 0;
}